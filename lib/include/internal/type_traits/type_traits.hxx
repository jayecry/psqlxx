#ifndef _PSQLXX_INTERNAL_TYPE_TRAITS_HXX
#define _PSQLXX_INTERNAL_TYPE_TRAITS_HXX

namespace psqlxx::internal {
    template <typename _type_t>
    struct pq_type_traits_t {
        PSQLXX_INLINE static raw_sql_str_t to_str(const _type_t&) { static_assert(false, "type not supported"); }

        PSQLXX_INLINE static _type_t from_str(const raw_sql_str_t&) { static_assert(false, "type not supported"); }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { static_assert(false, "type not supported"); }

        PSQLXX_INLINE static constexpr object_id_t object_id() { static_assert(false, "type not supported"); }

        PSQLXX_INLINE static constexpr bool binary() { static_assert(false, "type not supported"); }

      private:
        static constexpr bool m_binary = false;

        static constexpr object_id_t m_obj_id = 0;
    };
}

#include "arithmetic/arithmetic.hxx"

#include "logical/logical.hxx"

#include "strings/strings.hxx"

#include "binary/binary.hxx"

#include "arrays/arrays.hxx"

#endif // _PSQLXX_INTERNAL_TYPE_TRAITS_HXX