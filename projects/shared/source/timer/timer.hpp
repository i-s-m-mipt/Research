#ifndef SOLUTION_SHARED_TIMER_HPP
#define SOLUTION_SHARED_TIMER_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <chrono>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "../logger/logger.hpp"

namespace solution
{
	namespace shared
	{
		class timer_exception : public std::exception 
		{
		public:

			explicit timer_exception(const std::string & message) noexcept :
				std::exception(message.c_str())
			{}

			explicit timer_exception(const char * const message) noexcept :
				std::exception(message)
			{}

			~timer_exception() noexcept = default;
		};

		class Timer
		{
		private:
			
			using clock_t = std::chrono::steady_clock;
			using time_point_t = clock_t::time_point;

		public:

			template < typename Name, typename Enable = 
				std::enable_if_t < std::is_convertible_v < Name, std::string > > >
			explicit Timer(Name && name, std::ostream & stream) :
				m_name(std::forward < Name > (name)), m_stream(stream), m_begin(clock_t::now())
			{
				initialize();
			}

			~Timer() noexcept
			{
				try
				{
					uninitialize();
				}
				catch (...)
				{
					// std::abort();
				}
			}

		private:

			void initialize();

			void uninitialize();

		public:

			void elapsed() const;

		private:

			std::string m_name;
			time_point_t m_begin;

		private:

			std::ostream & m_stream;
		};

	} // namespace shared

} // namespace solution 

#endif // #ifndef SOLUTION_SHARED_TIMER_HPP