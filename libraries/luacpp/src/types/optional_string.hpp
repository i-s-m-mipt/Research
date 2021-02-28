#pragma once

#include <exception>
#include <optional>
#include <stdexcept>
#include <string>

#include "optional.hpp"
#include "string.hpp"

#include "../declarations.hpp"
#include "../general.hpp"

namespace lua 
{
	template <>
	struct Type_Adapter < std::optional < char * > > : public types::Optional < char * > 
	{};

	template <>
	struct Type_Adapter < std::optional < const char * > > : public types::Optional < const char * > 
	{};

	template < std::size_t N >
	struct Type_Adapter < std::optional < char[N] > > : public types::Optional < char * > 
	{};

	template < std::size_t N >
	struct Type_Adapter < std::optional < const char[N] > > : public types::Optional < const char * > 
	{};

	template < std::size_t N >
	struct Type_Adapter < std::optional < const char(&)[N] > > : public types::Optional < const char * > 
	{};

	template <>
	struct Type_Adapter < std::optional < std::string > > : public types::Optional < std::string > 
	{};

} // namespace lua