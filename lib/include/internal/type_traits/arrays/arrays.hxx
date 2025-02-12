#ifndef _PSQLXX_INTERNAL_TYPE_TRAITS_ARRAYS_HXX
#define _PSQLXX_INTERNAL_TYPE_TRAITS_ARRAYS_HXX

#include "arithmetic/arithmetic.hxx"

namespace psqlxx::internal {
    template <typename _type_t>
    PSQLXX_INLINE static array_t<_type_t> parse_array(const raw_sql_str_t& str) {
        std::string_view str_conc(str.c_str());

        if (str_conc.size() < 2 || str_conc.front() != '{' || str_conc.back() != '}') {
            throw base_exception_t("invalid array format");
        }

        str_conc.remove_prefix(1);
        str_conc.remove_suffix(1);

        array_t<_type_t> ret{};

        while (!str_conc.empty()) {
            auto pos = str_conc.find(',');

            std::string_view item{};

            if (pos == std::string_view::npos) {
                item = str_conc;

                str_conc = {};
            }
            else {
                item = str_conc.substr(0, pos);

                str_conc.remove_prefix(pos + 1);
            }

            while (!item.empty() && (item.front() == ' ')) {
                item.remove_prefix(1);
            }

            while (!item.empty() && (item.back() == ' ')) {
                item.remove_suffix(1);
            }

            ret.emplace_back(pq_type_traits_t<_type_t>::from_str(item.data()));
        }

        return ret;
    }

    PSQLXX_INLINE std::vector<std::string> parse_bytes_array(const raw_sql_str_t& str) {
        if (str.size() < 2 || str.front() != '{' || str.back() != '}') {
            throw base_exception_t("invalid array format");
        }

        std::vector<std::string> ret;

        ret.reserve(str.size() / 2);

        bool in_quotes{};

        std::string current{};

        for (std::size_t i{1}; i < str.size() - 1; ++i) {
            char c = str[i];

            if (c == '"') {
                in_quotes = !in_quotes;
            }
            else if (c == ',' && !in_quotes) {
                ret.push_back(current);

                current.clear();
            }
            else {
                current += c;
            }
        }

        if (!current.empty()) {
            ret.push_back(current);
        }

        return ret;
    };

    template <>
    struct pq_type_traits_t<array_t<std::int16_t>> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const array_t<std::int16_t>& value) { return fmt::format("{{{}}}", fmt::join(value, ",")); }

        PSQLXX_INLINE static array_t<std::int16_t> from_str(const raw_sql_str_t& str) { return parse_array<std::int16_t>(str); }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "SMALLINT[]"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = INT2ARRAYOID;
    };

    template <>
    struct pq_type_traits_t<array_t<std::int32_t>> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const array_t<std::int32_t>& value) { return fmt::format("{{{}}}", fmt::join(value, ",")); }

        PSQLXX_INLINE static array_t<std::int32_t> from_str(const raw_sql_str_t& str) { return parse_array<std::int32_t>(str); }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "INTEGER[]"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = INT4ARRAYOID;
    };

    template <>
    struct pq_type_traits_t<array_t<std::int64_t>> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const array_t<std::int64_t>& value) { return fmt::format("{{{}}}", fmt::join(value, ",")); }

        PSQLXX_INLINE static array_t<std::int64_t> from_str(const raw_sql_str_t& str) { return parse_array<std::int64_t>(str); }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "BIGINT[]"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = INT8ARRAYOID;
    };

    template <>
    struct pq_type_traits_t<array_t<float>> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const array_t<float>& value) { return fmt::format("{{{}}}", fmt::join(value, ",")); }

        PSQLXX_INLINE static array_t<float> from_str(const raw_sql_str_t& str) { return parse_array<float>(str); }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "REAL[]"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = FLOAT4ARRAYOID;
    };

    template <>
    struct pq_type_traits_t<array_t<double>> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const array_t<double>& value) { return fmt::format("{{{}}}", fmt::join(value, ",")); }

        PSQLXX_INLINE static array_t<double> from_str(const raw_sql_str_t& str) { return parse_array<double>(str); }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "DOUBLE PRECISION[]"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = FLOAT8ARRAYOID;
    };

    template <>
    struct pq_type_traits_t<array_t<binary_t>> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const array_t<binary_t>& value) {
            std::vector<std::string> serialized_elements;

            serialized_elements.reserve(value.size());

            for (const auto& elem : value) {
                serialized_elements.push_back(pq_type_traits_t<binary_t>::to_str(elem));
            }

            return fmt::format("{{{}}}", fmt::join(serialized_elements, ","));
        }

        PSQLXX_INLINE static array_t<binary_t> from_str(const raw_sql_str_t& str) {
            auto elements = parse_bytes_array(str);

            array_t<binary_t> ret;

            ret.reserve(elements.size());

            for (const auto& elem : elements) {
                ret.emplace_back(pq_type_traits_t<binary_t>::from_str(elem));
            }

            return ret;
        }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "BYTEA[]"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = BYTEAOID;
    };

    template <>
    struct pq_type_traits_t<array_t<bool>> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const array_t<bool>& value) { return fmt::format("{{{}}}", fmt::join(value, ",")); }

        PSQLXX_INLINE static array_t<bool> from_str(const raw_sql_str_t& str) { return parse_array<bool>(str); }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "BOOLEAN[]"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = BOOLOID;
    };

    template <>
    struct pq_type_traits_t<array_t<std::string>> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const array_t<std::string>& value) { return fmt::format("{{{}}}", fmt::join(value, ",")); }

        PSQLXX_INLINE static array_t<std::string> from_str(const raw_sql_str_t& str) { return parse_array<std::string>(str); }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "TEXT[]"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = TEXTARRAYOID;
    };

    // TODO: JSONBARRAY
}

#endif // _PSQLXX_INTERNAL_TYPE_TRAITS_ARRAYS_HXX