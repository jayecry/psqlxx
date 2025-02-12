#ifndef _PSQLXX_INTERNAL_TYPE_TRAITS_STRINGS_HXX
#define _PSQLXX_INTERNAL_TYPE_TRAITS_STRINGS_HXX

namespace psqlxx::internal {
    template <std::size_t _size>
    struct pq_type_traits_t<char[_size]> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const char (&value)[_size]) { return value; }

        PSQLXX_INLINE static std::string from_str(const raw_sql_str_t& str) { return str; }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "TEXT"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = TEXTOID;
    };

    template <>
    struct pq_type_traits_t<std::string> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const std::string& value) { return value; }

        PSQLXX_INLINE static std::string from_str(const raw_sql_str_t& str) { return str; }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "TEXT"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = TEXTOID;
    };

    template <>
    struct pq_type_traits_t<base_json_str_t> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const base_json_str_t& value) { return value; }

        PSQLXX_INLINE static base_json_str_t from_str(const raw_sql_str_t& str) { return str; }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "JSONB"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = JSONBOID;
    };
}

#endif // _PSQLXX_INTERNAL_TYPE_TRAITS_STRINGS_HXX