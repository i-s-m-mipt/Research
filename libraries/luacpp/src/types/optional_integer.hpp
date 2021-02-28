#pragma once

#include <exception>
#include <optional>
#include <stdexcept>
#include <string>

#include "integer.hpp"
#include "optional.hpp"

#include "../declarations.hpp"
#include "../general.hpp"

namespace lua 
{
	template <>
	struct Type_Adapter < std::optional < int > > : public types::Optional < int >
	{};
	
	template <>
	struct Type_Adapter < std::optional < unsigned int > > : public types::Optional < unsigned int >
	{};

	template <>
	struct Type_Adapter < std::optional < short > > : public types::Optional < short >
	{};

	template <>
	struct Type_Adapter < std::optional < unsigned short > > : public types::Optional < unsigned short >
	{};

	template <>
	struct Type_Adapter < std::optional < long > > : public types::Optional < long >
	{};

	template <>
	struct Type_Adapter < std::optional < unsigned long > > : public types::Optional < unsigned long >
	{};

	template <>
	struct Type_Adapter < std::optional < long long > > : public types::Optional < long long >
	{};

	template <>
	struct Type_Adapter < std::optional < unsigned long long > > : public types::Optional < unsigned long long >
	{};

} // namespace lua 