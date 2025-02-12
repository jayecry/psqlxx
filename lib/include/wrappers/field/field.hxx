#ifndef _PSQLXX_WRAPPERS_COLUMN_HXX
#define _PSQLXX_WRAPPERS_COLUMN_HXX

namespace psqlxx::wrappers {
    struct field_t {
        PSQLXX_INLINE field_t() = default;

        PSQLXX_INLINE field_t(pq_raw_result_t* result, const row_index_t row, const column_index_t column)
            : m_result(result), m_row(row), m_column(column) {
            if (!m_result) {
                throw internal::base_exception_t("ptr for field is nullptr");
            }
        }

      private:
        pq_raw_result_t* m_result{};

        row_index_t m_row{};
        column_index_t m_column{};
    };
}

#endif // _PSQLXX_WRAPPERS_COLUMN_HXX