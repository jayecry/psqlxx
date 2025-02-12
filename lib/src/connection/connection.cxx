#include <psqlxx.hxx>

namespace psqlxx {
    void connection_t::continue_polling() {
        if (!m_active) {
            if (m_connection) {
                throw internal::base_exception_t(
                    fmt::format(
                        "connection not established: {}",

                        internal::pq_wrapper_t::connection_t::error_message(m_connection.get())
                    )
                );
            }

            throw internal::base_exception_t("connection not established: connection does not exist");
        }

        auto status = internal::pq_wrapper_t::connection_t::poll(m_connection.get());

        switch (status) {
            case PGRES_POLLING_OK:
                break;

            case PGRES_POLLING_FAILED:
                m_active = false;

                throw internal::base_exception_t(
                    fmt::format(
                        "polling failed: {}",

                        internal::pq_wrapper_t::connection_t::error_message(m_connection.get())
                    )
                );

            case PGRES_POLLING_READING:
                wait_readable([weak_self = weak_from_this()](auto) {
                    if (auto self = weak_self.lock()) {
                        self->continue_polling();
                    }
                });

                break;

            case PGRES_POLLING_WRITING:
                wait_writable([weak_self = weak_from_this()](auto) {
                    if (auto self = weak_self.lock()) {
                        self->continue_polling();
                    }
                });

                break;

            default:
                break;
        }
    }

    void connection_t::polling() {
        if (!m_active) {
            if (m_connection) {
                throw internal::base_exception_t(
                    fmt::format(
                        "connection not established: {}",

                        internal::pq_wrapper_t::connection_t::error_message(m_connection.get())
                    )
                );
            }

            throw internal::base_exception_t("connection not established: system error");
        }

        if (internal::pq_wrapper_t::connection_t::status(m_connection.get()) != CONNECTION_OK) {
            throw internal::base_exception_t(
                fmt::format(
                    "connection status not OK: {}",

                    internal::pq_wrapper_t::connection_t::error_message(m_connection.get())
                )
            );
        }

        if (internal::pq_wrapper_t::connection_t::set_non_blocking(m_connection.get(), 1) != 0) {
            throw internal::base_exception_t("failed to set non-blocking mode");
        }

        const auto socket = internal::pq_wrapper_t::connection_t::socket(m_connection.get());

        if (socket < 0) {
            throw internal::base_exception_t("failed to get socket");
        }

        m_stream.assign(boost::asio::ip::tcp::v4(), socket);

        boost::asio::post(
            m_strand,

            [weak_self = weak_from_this()]() {
                if (auto self = weak_self.lock()) {
                    self->continue_polling();
                }
            }
        );
    }

    void connection_t::wait_readable(wait_callback_t callback) {
        m_stream.async_wait(
            boost::asio::socket_base::wait_read,

            boost::asio::bind_executor(
                m_strand,

                [weak_self = weak_from_this(),
                 cb = std::move(callback)](const boost::system::error_code& err_code) mutable {
                    if (auto self = weak_self.lock()) {
                        auto error = errors::error_t();

                        if (err_code) {
                            error.code() = errors::e_code_t::read_socket_error;

                            error.message() = fmt::format(
                                "failed to wait readable from socket: {}",

                                err_code.message()
                            );
                        }

                        cb(std::move(error));
                    }
                }
            )
        );
    }

    void connection_t::wait_writable(wait_callback_t callback) {
        m_stream.async_wait(
            boost::asio::socket_base::wait_write,

            boost::asio::bind_executor(
                m_strand,

                [weak_self = weak_from_this(),
                 cb = std::move(callback)](const boost::system::error_code& err_code) mutable {
                    if (auto self = weak_self.lock()) {
                        auto error = errors::error_t();

                        if (err_code) {
                            error.code() = errors::e_code_t::write_socket_error;

                            error.message() = fmt::format(
                                "failed to wait writable from socket: {}",

                                err_code.message()
                            );
                        }

                        cb(std::move(error));
                    }
                }
            )
        );
    }

    void connection_t::parse_results(query_callback_t callback) {
        try {
            if (!internal::pq_wrapper_t::connection_t::consume_input(m_connection.get())) {
                auto error = errors::error_t(
                    errors::e_code_t::consume_input_failed,

                    fmt::format(
                        "failed to consume input: {}",

                        internal::pq_wrapper_t::connection_t::error_message(m_connection.get())
                    )
                );

                clear_input();

                return callback(wrappers::results_t(), std::move(error));
            }

            if (internal::pq_wrapper_t::connection_t::is_busy(m_connection.get())) {
                wait_readable([weak_self = weak_from_this(), cb = std::move(callback)](errors::error_t err) mutable {
                    if (err.code() != errors::e_code_t::success) {
                        if (auto self = weak_self.lock()) {
                            self->clear_input();
                        }

                        cb(wrappers::results_t(), std::move(err));

                        return;
                    }

                    if (auto self = weak_self.lock()) {
                        self->parse_results(std::move(cb));
                    }
                });

                return;
            }

            wrappers::results_t results{};

            while (true) {
                auto result = internal::pq_wrapper_t::result_t::get_ptr(m_connection.get());

                if (!result) {
                    break;
                }

                const auto status = internal::pq_wrapper_t::result_t::status(result);

                if (status == PGRES_FATAL_ERROR) {
                    clear_input();

                    auto error = errors::error_t(
                        errors::e_code_t::query_failed,

                        fmt::format(
                            "query failed: {}",

                            internal::pq_wrapper_t::result_t::error_message(result)
                        )
                    );

                    internal::pq_wrapper_t::result_t::clear(result);

                    return callback(wrappers::results_t(), std::move(error));
                }

                results.emplace_back(wrappers::result_t(result));
            }

            callback(std::move(results), errors::error_t());
        }
        catch (const std::exception& e) {
            clear_input();

            callback(wrappers::results_t(), errors::error_t(errors::e_code_t::query_failed, e.what()));
        }
    }

    void connection_t::clear_input() {
        internal::pq_wrapper_t::cancel_t::cancel(m_connection.get());

        while (true) {
            auto result = internal::pq_wrapper_t::result_t::get_ptr(m_connection.get());

            if (!result) {
                break;
            }

            internal::pq_wrapper_t::result_t::clear(result);
        }
    }
}