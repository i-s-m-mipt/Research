#pragma once

#include <exception>
#include <optional>
#include <stdexcept>
#include <string>

#include "optional.hpp"
#include "real.hpp"

#include "../declarations.hpp"
#include "../general.hpp"

namespace lua 
{
	template <>
	struct Type_Adapter < std::optional < float > > : public types::Optional < float >
	{};

	template <>
	struct Type_Adapter < std::optional < double > > : public types::Optional < double > 
	{};

} // namespace lua