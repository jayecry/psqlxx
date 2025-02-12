#ifndef _PSQLXX_INTERNAL_EXCEPTIONS_HXX
#define _PSQLXX_INTERNAL_EXCEPTIONS_HXX

namespace psqlxx::internal {
    struct base_exception_t : public std::runtime_error {
        PSQLXX_INLINE explicit base_exception_t(const std::string_view& str)
            : std::runtime_error(str.data()) {
        }
    };
}

#endif // _PSQLXX_INTERNAL_EXCEPTIONS_HXX