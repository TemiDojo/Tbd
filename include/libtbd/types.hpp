#ifndef TBD_TYPES_HPP
#define TBD_TYPES_HPP

#include <array>
#include <cstddef>
#include <variant>

namespace tbd {
	using byte64 = std::array<std::byte, 8>;
	using byte128 = std::array<std::byte, 16>;
}

#endif
