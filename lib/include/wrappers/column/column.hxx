#ifndef _PSQLXX_WRAPPERS_FIELD_HXX
#define _PSQLXX_WRAPPERS_FIELD_HXX

#include "field/field.hxx"

namespace psqlxx::wrappers {
    struct column_t {
        PSQLXX_INLINE column_t() = default;

        PSQLXX_INLINE column_t(pq_raw_result_t* result, const row_index_t row, const column_index_t column)
            : m_result(result), m_row(row), m_column(column) {
            if (!m_result) {
                throw internal::base_exception_t("ptr for column is nullptr");
            }
        }

      private:
        pq_raw_result_t* m_result{};

        row_index_t m_row{};
        column_index_t m_column{};
    };
}

#endif // _PSQLXX_WRAPPERS_FIELD_HXX