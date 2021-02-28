#pragma once

#include <exception>
#include <stdexcept>
#include <string>

namespace lua
{
	class State;

	template < typename T >
	class Entity;

	template < typename T >
	struct Type_Adapter;

	template < typename T >
	class Element;

	template < typename T >
	class Vector;

} // namespace lua