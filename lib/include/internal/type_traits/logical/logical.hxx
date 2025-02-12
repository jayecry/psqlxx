#ifndef _PSQLXX_INTERNAL_TYPE_TRAITS_LOGICAL_HXX
#define _PSQLXX_INTERNAL_TYPE_TRAITS_LOGICAL_HXX

namespace psqlxx::internal {
    template <>
    struct pq_type_traits_t<bool> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const bool& value) { return value ? "true" : "false"; }

        PSQLXX_INLINE static bool from_str(const raw_sql_str_t& str) { return str.compare("t") == 0 ? true : false; }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "BOOLEAN"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = BOOLOID;
    };
}

#endif // _PSQLXX_INTERNAL_TYPE_TRAITS_LOGICAL_HXX