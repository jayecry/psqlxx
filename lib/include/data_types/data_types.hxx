#ifndef _PSQLXX_DATA_TYPES_HXX
#define _PSQLXX_DATA_TYPES_HXX

namespace psqlxx {
    // ----
    using length_t = std::int32_t;

    using object_id_t = Oid;
    // ----

    // ----
    using row_size_t = std::int32_t;
    using row_index_t = std::int32_t;

    using column_size_t = std::int32_t;
    using column_index_t = std::int32_t;
    // ----

    // ----
    using raw_sql_str_t = std::string;

    using unsafe_sql_str_t = const char*;
    // ----

    // ----
    using base_json_str_t = std::string;

    using binary_t = std::vector<std::uint8_t>;
    // ----

    // ----
    template <typename _type_t>
    using array_t = std::vector<_type_t>;
    // ----

    // ----
    using pq_connection_t = PGconn;

    using pq_raw_result_t = PGresult;

    using pq_exec_status_t = ExecStatusType;

    using pq_poll_status_t = PostgresPollingStatusType;
    // ----
}

#endif // _PSQLXX_DATA_TYPES_HXX