#ifndef _PSQLXX_CONNECTION_HXX
#define _PSQLXX_CONNECTION_HXX

namespace psqlxx {
    using wait_callback_t = std::function<void(errors::error_t)>;

    using query_callback_t = std::function<void(wrappers::results_t, errors::error_t)>;

    struct connection_t : std::enable_shared_from_this<connection_t> {
        PSQLXX_INLINE connection_t(boost::asio::io_context& io_ctx, const std::string_view& dsn)
            : m_active(true),
              m_dsn(dsn.data()),
              m_io_ctx(io_ctx),
              m_strand(io_ctx),
              m_stream(io_ctx),
              m_connection(internal::pq_wrapper_t::connection_t::connect(dsn.data()), &PQfinish) {
            if (!m_connection) {
                m_active = false;

                throw internal::base_exception_t("failed to connect to database");
            }
        }

        PSQLXX_INLINE ~connection_t() {
            clear_input();

            m_active = false;

            m_stream.close();
        }

      public:
        void polling();

      private:
        void next_poll();

        void continue_polling();

      public:
        template <typename... _args_t>
        PSQLXX_NOINLINE void send_query_async(const std::string_view& str, query_callback_t callback, const _args_t&... args) {
            if (!m_active) {
                auto error = errors::error_t(errors::e_code_t::not_established);

                if (m_connection.get()) {
                    error.message() = fmt::format(
                        "connection is not established: {}",

                        internal::pq_wrapper_t::connection_t::error_message(m_connection.get())
                    );
                }
                else {
                    error.message() = "connection is not established: system error";
                }

                return callback(wrappers::results_t(), std::move(error));
            }

            if (!internal::pq_wrapper_t::query_t::send_query_params(m_connection.get(), str.data(), args...)) {
                auto error = errors::error_t(
                    errors::e_code_t::query_failed,

                    fmt::format(
                        "failed to send query: {}",

                        internal::pq_wrapper_t::connection_t::error_message(m_connection.get())
                    )
                );

                return callback(wrappers::results_t(), std::move(error));
            }

            try {
                wait_readable([weak_self = weak_from_this(), cb = std::move(callback)](errors::error_t err) mutable {
                    if (err.code() != errors::e_code_t::success) {
                        cb(wrappers::results_t(), std::move(err));

                        return;
                    }

                    if (auto self = weak_self.lock()) {
                        self->parse_results(std::move(cb));
                    }
                });
            }
            catch (const std::exception& e) {
                auto error = errors::error_t(
                    errors::e_code_t::query_failed,

                    fmt::format(
                        "failed to send query: {} <-> {}",

                        e.what(), internal::pq_wrapper_t::connection_t::error_message(m_connection.get())
                    )
                );

                return callback(wrappers::results_t(), std::move(error));
            }
        }

        template <typename... _args_t>
        PSQLXX_NOINLINE void send_query(const std::string_view& str, query_callback_t callback, const _args_t&... args) {
            std::promise<void> promise{};

            auto future = promise.get_future();

            try {
                send_query_async(
                    str,

                    [&promise, cb = std::move(callback)](wrappers::results_t results, errors::error_t error) mutable {
                        cb(std::move(results), std::move(error));

                        promise.set_value();
                    },

                    args...
                );
            }
            catch (const std::exception& e) {
                auto error = errors::error_t(
                    errors::e_code_t::query_failed,

                    fmt::format(
                        "failed to send query: {} <-> {}",

                        e.what(), internal::pq_wrapper_t::connection_t::error_message(m_connection.get())
                    )
                );

                return callback(wrappers::results_t(), std::move(error));
            }

            future.wait();
        }

      private:
        void wait_readable(wait_callback_t callback);

        void wait_writable(wait_callback_t callback);

      private:
        void parse_results(query_callback_t callback);

      private:
        void clear_input();

      private:
        std::atomic<bool> m_active;

        std::string m_dsn;

      private:
        boost::asio::io_context& m_io_ctx;

        boost::asio::io_context::strand m_strand;
        boost::asio::generic::stream_protocol::socket m_stream;

        std::unique_ptr<pq_connection_t, decltype(&PQfinish)> m_connection;
    };
}

#endif // _PSQLXX_CONNECTION_HXX