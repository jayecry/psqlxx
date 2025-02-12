#ifndef _PSQLXX_WRAPPERS_RESULTS_HXX
#define _PSQLXX_WRAPPERS_RESULTS_HXX

namespace psqlxx::wrappers {
    struct result_t {
        PSQLXX_INLINE result_t(pq_raw_result_t* result)
            : m_result(result, &PQclear) {
            if (!m_result) {
                throw internal::base_exception_t("ptr for result is nullptr");
            }
        }

      public:
        PSQLXX_INLINE pq_raw_result_t* raw() const { return m_result.get(); }

      private:
        std::unique_ptr<pq_raw_result_t, decltype(&PQclear)> m_result;
    };

    struct results_t {
        PSQLXX_INLINE results_t() { m_results.reserve(3u); }

      public:
        PSQLXX_INLINE void emplace_back(result_t result) { m_results.emplace_back(std::move(result)); }

      public:
        PSQLXX_INLINE bool empty() const { return m_results.empty(); }

      public:
        PSQLXX_INLINE const result_t& first() const { return m_results.front(); }

        PSQLXX_INLINE const result_t& last() const { return m_results.back(); }

      private:
        std::vector<result_t> m_results{};
    };
}

#endif // _PSQLXX_WRAPPERS_RESULTS_HXX