#ifndef SOLUTION_PLUGIN_MARKET_SOURCE_HPP
#define SOLUTION_PLUGIN_MARKET_SOURCE_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iomanip>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/managed_windows_shared_memory.hpp>

#include <qluacpp/qlua>

#include "../../config/config.hpp"

#include "../../../../shared/source/logger/logger.hpp"

namespace solution
{
	namespace plugin
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
			public:

				using api_t = Config::api_t;

			private:

				using source_t = qlua::data_source;

				using shared_memory_t = boost::interprocess::managed_shared_memory;

				using record_t = 
				    boost::interprocess::basic_string < char, std::char_traits < char > ,
					    boost::interprocess::allocator < char, shared_memory_t::segment_manager > > ;

				using record_allocator_t = 
					boost::interprocess::allocator < record_t, shared_memory_t::segment_manager > ;

				using deque_t = boost::interprocess::deque < record_t, record_allocator_t > ;

				using mutex_t = boost::interprocess::interprocess_mutex;

			private:

				struct Bar
				{
					using index_t = std::size_t;

					using date_t = std::string;
					using time_t = std::string;

					using price_t = double;

					using volume_t = std::uint64_t;

					index_t index = 0U;

					date_t date;
					time_t time;

					price_t price_open  = 0.0;
					price_t price_high  = 0.0;
					price_t price_low   = 0.0;
					price_t price_close = 0.0;

					volume_t volume = 0U;
				};

			private:

				using price_t = Bar::price_t;
				using index_t = Bar::index_t;

			public:

				template < typename C, typename A, typename S, typename Enable =
					std::enable_if_t <
						std::is_convertible_v < C, std::string > &&
						std::is_convertible_v < A, std::string > &&
						std::is_convertible_v < S, std::string > > >
				explicit Source(const api_t & api, C && class_code, A && asset_code, S && scale_code, 
					std::size_t size = default_size) :
						m_class_code(std::forward < C > (class_code)),
						m_asset_code(std::forward < A > (asset_code)), 
						m_scale_code(std::forward < S > (scale_code)), m_size(size)
				{
					initialize(api);
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

				void initialize(const api_t & api);

				void uninitialize();

			private:

				std::string make_shared_memory_name() const;

			public:

				const auto & class_code() const noexcept
				{
					return m_class_code;
				}

				const auto & asset_code() const noexcept
				{
					return m_asset_code;
				}

				const auto & scale_code() const noexcept
				{
					return m_scale_code;
				}

			public:

				void update() const;

			private:

				Bar make_bar(index_t index) const;

				record_t make_record(const Bar & bar) const;

			private:

				static inline const std::size_t default_size = 250U;

			private:

				const std::string m_class_code;
				const std::string m_asset_code;
				const std::string m_scale_code;

				const std::size_t m_size = default_size;

			private:

				std::unique_ptr < source_t > m_source;

				shared_memory_t m_shared_memory;

				deque_t * m_deque;
				price_t * m_price;
				mutex_t * m_mutex;

			private:

				mutable index_t m_last_index = 0;
			};

		} // namespace market

	} // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_MARKET_SOURCE_HPP