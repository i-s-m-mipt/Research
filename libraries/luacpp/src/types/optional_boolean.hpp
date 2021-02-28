#pragma once

#include <exception>
#include <optional>
#include <stdexcept>
#include <string>

#include "boolean.hpp"
#include "optional.hpp"

#include "../declarations.hpp"
#include "../general.hpp"

namespace lua 
{
	template <>
	struct Type_Adapter < std::optional < bool > > : public types::Optional < bool >
	{};

} // namespace lua