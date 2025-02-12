#ifndef _PSQLXX_INTERNAL_TYPE_TRAITS_ARITHMETIC_HXX
#define _PSQLXX_INTERNAL_TYPE_TRAITS_ARITHMETIC_HXX

namespace psqlxx::internal {
    template <>
    struct pq_type_traits_t<std::int16_t> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const std::int16_t& value) { return std::to_string(value); }

        PSQLXX_INLINE static std::int16_t from_str(const raw_sql_str_t& str) {
            std::int16_t ret{};

            auto [_unused, err] = std::from_chars(str.data(), str.data() + str.size(), ret);

            if (err != std::errc()) {
                const auto err_code = std::make_error_code(err);

                throw base_exception_t(
                    fmt::format(
                        "invalid string for convert: {}",

                        err_code.message()
                    )
                );
            }

            return ret;
        }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "SMALLINT"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = INT2OID;
    };

    template <>
    struct pq_type_traits_t<std::int32_t> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const std::int32_t& value) { return std::to_string(value); }

        PSQLXX_INLINE static std::int32_t from_str(const raw_sql_str_t& str) {
            std::int32_t ret{};

            auto [_unused, err] = std::from_chars(str.data(), str.data() + str.size(), ret);

            if (err != std::errc()) {
                const auto err_code = std::make_error_code(err);

                throw base_exception_t(
                    fmt::format(
                        "invalid string for convert: {}",

                        err_code.message()
                    )
                );
            }

            return ret;
        }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "INTEGER"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = INT4OID;
    };

    template <>
    struct pq_type_traits_t<std::int64_t> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const std::int64_t& value) { return std::to_string(value); }

        PSQLXX_INLINE static std::int64_t from_str(const raw_sql_str_t& str) {
            std::int64_t ret{};

            auto [_unused, err] = std::from_chars(str.data(), str.data() + str.size(), ret);

            if (err != std::errc()) {
                const auto err_code = std::make_error_code(err);

                throw base_exception_t(
                    fmt::format(
                        "invalid string for convert: {}",

                        err_code.message()
                    )
                );
            }

            return ret;
        }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "BIGINT"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = INT8OID;
    };

    template <>
    struct pq_type_traits_t<float> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const float& value) { return std::to_string(value); }

        PSQLXX_INLINE static float from_str(const raw_sql_str_t& str) {
            float ret{};

            auto [_unused, err] = std::from_chars(str.data(), str.data() + str.size(), ret);

            if (err != std::errc()) {
                const auto err_code = std::make_error_code(err);

                throw base_exception_t(
                    fmt::format(
                        "invalid string for convert: {}",

                        err_code.message()
                    )
                );
            }

            return ret;
        }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "REAL"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = FLOAT4OID;
    };

    template <>
    struct pq_type_traits_t<double> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const double& value) { return std::to_string(value); }

        PSQLXX_INLINE static double from_str(const raw_sql_str_t& str) {
            double ret{};

            auto [_unused, err] = std::from_chars(str.data(), str.data() + str.size(), ret);

            if (err != std::errc()) {
                const auto err_code = std::make_error_code(err);

                throw base_exception_t(
                    fmt::format(
                        "invalid string for convert: {}",

                        err_code.message()
                    )
                );
            }

            return ret;
        }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "DOUBLE PRECISION"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = FLOAT8OID;
    };
}

#endif // _PSQLXX_INTERNAL_TYPE_TRAITS_ARITHMETIC_HXX