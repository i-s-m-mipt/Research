#pragma once

#include <exception>
#include <optional>
#include <stdexcept>
#include <string>

#include "optional.hpp"
#include "pointer.hpp"

#include "../declarations.hpp"
#include "../general.hpp"

namespace lua 
{
	template <>
	struct Type_Adapter < std::optional < void * > > : public types::Optional < void * >
	{};

	template <>
	struct Type_Adapter < std::optional < const void * > > : public types::Optional < const void * >
	{};

} // namespace lua