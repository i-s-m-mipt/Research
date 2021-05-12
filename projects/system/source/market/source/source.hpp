#ifndef SOLUTION_SYSTEM_MARKET_SOURCE_HPP
#define SOLUTION_SYSTEM_MARKET_SOURCE_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <exception>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "../../../../shared/source/logger/logger.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			class source_exception : public std::exception
			{
			public:

				explicit source_exception(const std::string & message) noexcept :
					std::exception(message.c_str())
				{}

				explicit source_exception(const char * const message) noexcept :
					std::exception(message)
				{}

				~source_exception() noexcept = default;
			};

			class Source
			{
			private:

				using shared_memory_t = boost::interprocess::managed_shared_memory;

				using segment_manager_t = shared_memory_t::segment_manager;

				using char_allocator_t = boost::interprocess::allocator < char, segment_manager_t > ;

				using record_t = boost::interprocess::basic_string < char, std::char_traits < char >, char_allocator_t > ;

				using record_allocator_t = boost::interprocess::allocator < record_t, segment_manager_t > ;

				using deque_t = boost::interprocess::deque < record_t, record_allocator_t > ;

				using mutex_t = boost::interprocess::interprocess_mutex;

			public:

				template < typename A, typename S, typename Enable =
					std::enable_if_t <
						std::is_convertible_v < A, std::string > &&
						std::is_convertible_v < S, std::string > > >
				explicit Source(A && asset, S && scale) : 
						m_asset(std::forward < A > (asset)), 
						m_scale(std::forward < S > (scale))
				{
					initialize();
				}

				~Source() noexcept
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

			private:

				void initialize_source();

				void initialize_shared_memory();

			private:

				std::string make_shared_memory_name() const;

			public:

				const auto & asset() const noexcept
				{
					return m_asset;
				}

				const auto & scale() const noexcept
				{
					return m_scale;
				}

			public:

				std::string get(std::size_t size) const;

			private:

				const std::string m_asset;
				const std::string m_scale;

			private:

				shared_memory_t m_shared_memory;

				deque_t * m_deque;

				mutex_t * m_mutex;
			};

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_SOURCE_HPP