#ifndef _PSQLXX_INTERNAL_TYPE_TRAITS_BINARY_HXX
#define _PSQLXX_INTERNAL_TYPE_TRAITS_BINARY_HXX

#include "arithmetic/arithmetic.hxx"

namespace psqlxx::internal {
    PSQLXX_INLINE constexpr std::uint8_t char_to_hex(char c) {
        if (c >= '0' && c <= '9') {
            return c - '0';
        }

        if (c >= 'a' && c <= 'f') {
            return c - 'a' + 10;
        }

        if (c >= 'A' && c <= 'F') {
            return c - 'A' + 10;
        }

        throw base_exception_t("invalid hex digit");
    }

    template <>
    struct pq_type_traits_t<binary_t> {
        PSQLXX_INLINE static raw_sql_str_t to_str(const binary_t& data) {
            std::string ret = "\\x";

            for (const auto& byte : data) {
                ret += fmt::format("{:02x}", byte);
            }

            return ret;
        }

        PSQLXX_INLINE static binary_t from_str(const raw_sql_str_t& str) {
            if (str.size() < 2 || str.compare(0, 2, "\\x") != 0) {
                throw base_exception_t("invalid bytea format: expected \\x prefix");
            }

            const std::string_view hex_data(str.data() + 2, str.size() - 2);

            if (hex_data.size() % 2 != 0) {
                throw base_exception_t("invalid bytea format: hex data length must be even");
            }

            binary_t ret;

            ret.reserve(hex_data.size() / 2);

            for (std::size_t i{}; i < hex_data.size(); i += 2) {
                auto hi = hex_data[i];
                auto lo = hex_data[i + 1];

                ret.emplace_back(static_cast<std::uint8_t>((char_to_hex(hi) << 4) | char_to_hex(lo)));
            }

            return ret;
        }

      public:
        PSQLXX_INLINE static constexpr std::string_view type() { return "BYTEA"; }

        PSQLXX_INLINE static constexpr object_id_t obj_id() { return m_obj_id; }

        PSQLXX_INLINE static constexpr bool binary() { return m_binary; }

      private:
        static constexpr bool m_binary = true;

        static constexpr object_id_t m_obj_id = BYTEAOID;
    };
}

#endif // _PSQLXX_INTERNAL_TYPE_TRAITS_BINARY_HXX