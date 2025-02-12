#ifndef _PSQLXX_ERRORS_HXX
#define _PSQLXX_ERRORS_HXX

namespace psqlxx::errors {
    enum struct e_code_t {
        success = 0,

        not_established,

        poll_failed,

        connection_not_ok,

        query_failed,

        read_socket_error,
        write_socket_error,

        consume_input_failed,

        query_in_progress,

        system_error,

        unknown = -1
    };

    struct error_t {
        PSQLXX_INLINE error_t()
            : m_code(e_code_t::success), m_message() {
        }

        PSQLXX_INLINE error_t(const e_code_t& code, const std::string& message = "")
            : m_code(code), m_message(message) {
        }

      public:
        PSQLXX_INLINE auto& code() noexcept { return m_code; }

        PSQLXX_INLINE const auto& code() const noexcept { return m_code; }

        PSQLXX_INLINE auto& message() noexcept { return m_message; }

        PSQLXX_INLINE const auto& message() const noexcept { return m_message; }

      private:
        e_code_t m_code{};

        std::string m_message{};
    };
}

#endif // _PSQLXX_ERRORS_HXX