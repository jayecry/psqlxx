#ifndef _PSQLXX_INTERNAL_WRAPPER_HXX
#define _PSQLXX_INTERNAL_WRAPPER_HXX

namespace psqlxx::internal {
    struct pq_wrapper_t {
        struct connection_t {
            PSQLXX_INLINE static pq_connection_t* connect(const std::string_view& connection_string) { return PQconnectdb(connection_string.data()); }

            PSQLXX_INLINE static void finish(pq_connection_t* connection) { PQfinish(connection); }

          public:
            PSQLXX_INLINE static std::int32_t non_blocking(pq_connection_t* connection) { return PQisnonblocking(connection); }

            PSQLXX_INLINE static std::int32_t set_non_blocking(pq_connection_t* connection, std::int32_t non_blocking) { return PQsetnonblocking(connection, non_blocking); }

          public:
            PSQLXX_INLINE static std::int32_t set_encoding(pq_connection_t* connection, const std::string_view& encoding) { return PQsetClientEncoding(connection, encoding.data()); }

          public:
            PSQLXX_INLINE static std::int32_t consume_input(pq_connection_t* connection) { return PQconsumeInput(connection); }

            PSQLXX_INLINE static std::int32_t is_busy(pq_connection_t* connection) { return PQisBusy(connection); }

            PSQLXX_INLINE static pq_poll_status_t poll(pq_connection_t* connection) { return PQconnectPoll(connection); }

          public:
            PSQLXX_INLINE static std::int32_t socket(pq_connection_t* connection) { return PQsocket(connection); }

          public:
            PSQLXX_INLINE static std::int32_t status(pq_connection_t* connection) { return PQstatus(connection); }

            PSQLXX_INLINE static std::string error_message(pq_connection_t* connection) { return PQerrorMessage(connection); }
        };

        struct cancel_t {
            PSQLXX_INLINE static void cancel(pq_connection_t* connection) {
                if (auto cancel_ptr = PQgetCancel(connection)) {
                    char errbuf[256] = {0};

                    PQcancel(cancel_ptr, errbuf, sizeof(errbuf));

                    PQfreeCancel(cancel_ptr);
                }
            }
        };

        struct query_t {
            template <typename... _args_t>
            PSQLXX_NOINLINE static std::int32_t send_query_params(pq_connection_t* connection, const std::string_view& str, const _args_t&... args) {
                std::int32_t ret{};

                if constexpr (sizeof...(args) <= 0) {
                    ret = PQsendQuery(connection, str.data());
                }
                else {
                    auto mapped = internal::mapping_t(args...);

                    ret = PQsendQueryParams(
                        connection,

                        str.data(),

                        mapped.m_size,

                        mapped.m_types.data(),
                        mapped.m_values.data(),
                        mapped.m_lengths.data(),
                        mapped.m_formats.data(),

                        0
                    );
                }

                return ret;
            }

            PSQLXX_INLINE static std::int32_t send_query(pq_connection_t* connection, const std::string_view& str) { return PQsendQuery(connection, str.data()); }
        };

        struct execute_t {
            template <typename... _args_t>
            PSQLXX_NOINLINE static pq_raw_result_t* execute_params(pq_connection_t* connection, const std::string_view& str, const _args_t&... args) {
                if constexpr (sizeof...(args) <= 0) {
                    auto result = PQexec(connection, str.data());

                    const auto status = PQresultStatus(result);

                    if (status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK) {
                        throw base_exception_t(
                            fmt::format(
                                "failed to exec query: {}",

                                PQresultErrorMessage(result)
                            )
                        );
                    }

                    return result;
                }

                auto mapped = mapping_t(args...);

                if (mapped.m_values.empty()) {
                    throw base_exception_t("failed to exec query: no params values founded");
                }

                auto result = PQexecParams(
                    connection,

                    str.data(),

                    mapped.m_size,

                    mapped.m_types.data(),
                    mapped.m_values.data(),
                    mapped.m_lengths.data(),
                    mapped.m_formats.data(),

                    0
                );

                const auto status = PQresultStatus(result);

                if (status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK) {
                    throw base_exception_t(
                        fmt::format(
                            "failed to exec query: {}",

                            PQresultErrorMessage(result)
                        )
                    );
                }

                return result;
            }

            PSQLXX_INLINE static pq_raw_result_t* execute(pq_connection_t* connection, const std::string_view& str) { return PQexec(connection, str.data()); }
        };

        struct result_t {
            PSQLXX_INLINE static pq_raw_result_t* get_ptr(pq_connection_t* connection) { return PQgetResult(connection); }

            PSQLXX_INLINE static void clear(pq_raw_result_t* result) { PQclear(result); }

          public:
            PSQLXX_INLINE static pq_exec_status_t status(pq_raw_result_t* result) { return PQresultStatus(result); }

            PSQLXX_INLINE static std::string error_message(pq_raw_result_t* result) { return PQresultErrorMessage(result); }

          public:
            PSQLXX_INLINE static column_size_t columns(pq_raw_result_t* result) { return PQnfields(result); }

            PSQLXX_INLINE static row_size_t rows(pq_raw_result_t* result) { return PQntuples(result); }
        };

        struct field_t {
            PSQLXX_INLINE static std::int32_t length(pq_raw_result_t* result, const column_index_t column) { return PQfsize(result, column); }

            PSQLXX_INLINE static std::int32_t format(pq_raw_result_t* result, const column_index_t column) { return PQfformat(result, column); }

          public:
            PSQLXX_INLINE static std::string name(pq_raw_result_t* result, const column_index_t column) { return PQfname(result, column); }

            PSQLXX_INLINE static std::string value(pq_raw_result_t* result, const row_index_t row, const column_index_t column) { return PQgetvalue(result, row, column); }
        };
    };
}

#endif // _PSQLXX_INTERNAL_WRAPPER_HXX