#ifndef _PSQLXX_INTERNAL_MAPPING_HXX
#define _PSQLXX_INTERNAL_MAPPING_HXX

namespace psqlxx::internal {
    template <typename... _args_t>
    struct mapping_t {
        constexpr static auto m_size = sizeof...(_args_t);

        PSQLXX_INLINE mapping_t(const _args_t&... args) {
            m_types.reserve(m_size);
            m_values.reserve(m_size);

            m_lengths.reserve(m_size);
            m_formats.reserve(m_size);

            m_storage.reserve(m_size);

            (parse(args), ...);
        }

      public:
        std::vector<object_id_t> m_types{};
        std::vector<unsafe_sql_str_t> m_values{};

        std::vector<length_t> m_lengths{};
        std::vector<length_t> m_formats{};

        std::vector<raw_sql_str_t> m_storage{};

      private:
        template <typename _type_t>
        PSQLXX_INLINE void parse(const _type_t& value) {
            using traits = pq_type_traits_t<_type_t>;

            m_types.push_back(traits::obj_id());

            m_storage.push_back(traits::to_str(value));
            m_values.push_back(m_storage.back().c_str());

            const auto binary = traits::binary();

            if (binary) {
                m_formats.push_back(1);

                m_lengths.push_back(m_storage.back().length());
            }
            else {
                m_formats.push_back(0);

                m_lengths.push_back(0);
            }
        }
    };
}

#endif // _PSQLXX_INTERNAL_MAPPING_HXX