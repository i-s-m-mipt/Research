#ifndef SOLUTION_PLUGIN_MARKET_SOURCE_HPP
#define SOLUTION_PLUGIN_MARKET_SOURCE_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <cmath>
#include <exception>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/deque.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "../../detail/lua/state.hpp"

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
			private:

				using reference_t = detail::lua::State::reference_t;

				using shared_memory_t = boost::interprocess::managed_shared_memory;

				using segment_manager_t = shared_memory_t::segment_manager;

				using char_allocator_t = boost::interprocess::allocator < char, segment_manager_t > ;

				using record_t = boost::interprocess::basic_string < char, std::char_traits < char >, char_allocator_t > ;

				using record_allocator_t = boost::interprocess::allocator < record_t, segment_manager_t > ;

				using deque_t = boost::interprocess::deque < record_t, record_allocator_t > ;

			private:

				struct Candle
				{
				public:

					using index_t = unsigned int;

				public:

					struct Date_Time
					{
						unsigned int year   = 0U;
						unsigned int month  = 0U;
						unsigned int day    = 0U;

						unsigned int hour   = 0U;
						unsigned int minute = 0U;
						unsigned int second = 0U;
					};

				public:

					using date_time_t = Date_Time;

					using price_t = double;

					using volume_t = unsigned long long;

					using variant_t = std::variant < date_time_t, price_t, volume_t > ;

				public:

					index_t index = 0U;

					date_time_t date_time;

					price_t price_open  = 0.0;
					price_t price_high  = 0.0;
					price_t price_low   = 0.0;
					price_t price_close = 0.0;

					volume_t volume = 0ULL;
				};

			private:

				using index_t = Candle::index_t;

			public:

				template < typename C, typename A, typename S, typename Enable =
					std::enable_if_t <
						std::is_convertible_v < C, std::string > &&
						std::is_convertible_v < A, std::string > &&
						std::is_convertible_v < S, std::string > > >
				explicit Source(detail::lua::State state, C && class_code, A && asset_code, S && scale_code, 
					std::size_t size = default_size) : m_state(state),
						m_class_code(std::forward < C > (class_code)),
						m_asset_code(std::forward < A > (asset_code)), 
						m_scale_code(std::forward < S > (scale_code)), m_size(size), m_reference(LUA_NOREF)
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

				const auto state() const noexcept
				{
					return m_state;
				}

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

				const auto size() const noexcept
				{
					return m_size;
				}

			public:

				void update() const;

			private:

				std::size_t max_size() const;

			private:

				Candle make_candle(index_t index) const;

			private:

				Candle::Date_Time get_date_time(index_t index) const;

				Candle::price_t get_price_open(index_t index) const;

				Candle::price_t get_price_high(index_t index) const;

				Candle::price_t get_price_low(index_t index) const;

				Candle::price_t get_price_close(index_t index) const;

				Candle::volume_t get_volume(index_t index) const;

			private:

				Candle::variant_t call(const std::string & name, index_t index) const;

			private:

				unsigned int get_date_time_field(const std::string & key) const;

			private:

				record_t make_record(const Candle & candle) const;

			public:

				Candle::price_t last_price() const;

			private:

				static inline const std::string references = "references";

				static inline const std::size_t default_size = 100U;

			private:

				const detail::lua::State m_state;

				const std::string m_class_code;
				const std::string m_asset_code;
				const std::string m_scale_code;

				const std::size_t m_size;

			private:

				reference_t m_reference;

				shared_memory_t m_shared_memory;

				deque_t * m_deque;

			private:

				mutable index_t m_last_index = 0U;
			};

		} // namespace market

	} // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_MARKET_SOURCE_HPP