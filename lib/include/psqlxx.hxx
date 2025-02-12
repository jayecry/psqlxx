#ifndef _PSQLXX_HXX
#define _PSQLXX_HXX

// ...
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <thread>
#include <type_traits>
#include <vector>

#include <boost/asio.hpp>

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <libpq-fe.h>

#include <server/catalog/pg_type_d.h>
// ...

#include "overload/overload.hxx"

#include "errors/errors.hxx"

#include "data_types/data_types.hxx"

#include "internal/internal.hxx"

#include "wrappers/wrappers.hxx"

#include "connection/connection.hxx"

namespace psqlxx {
    // ...
}

#endif // _PSQLXX_HXX