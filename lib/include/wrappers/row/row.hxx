#ifndef _PSQLXX_WRAPPERS_ROW_HXX
#define _PSQLXX_WRAPPERS_ROW_HXX

namespace psqlxx::wrappers {
    struct row_t {
        PSQLXX_INLINE row_t() = default;

        PSQLXX_INLINE row_t(pq_raw_result_t* result, const row_index_t row)
            : m_result(result), m_row(row) {
            if (!m_result) {
                throw internal::base_exception_t("ptr for row is nullptr");
            }
        }

      private:
        pq_raw_result_t* m_result{};

        row_index_t m_row{};
    };
}

#endif // _PSQLXX_WRAPPERS_ROW_HXX