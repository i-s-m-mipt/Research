#ifndef SOLUTION_SYSTEM_MARKET_CANDLE_HPP
#define SOLUTION_SYSTEM_MARKET_CANDLE_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <array>
#include <ctime>
#include <iomanip>
#include <ostream>
#include <string>
#include <vector>

namespace solution
{
	namespace system
	{
		namespace market
		{
			struct Date_Time
			{
			public:

				std::time_t to_time_t() const noexcept;

			public:

				unsigned int year   = 0U;
				unsigned int month  = 0U;
				unsigned int day    = 0U;

				unsigned int hour   = 0U;
				unsigned int minute = 0U;
				unsigned int second = 0U;
			};

			bool operator== (const Date_Time & lhs, const Date_Time & rhs);
			bool operator!= (const Date_Time & lhs, const Date_Time & rhs);
			bool operator<  (const Date_Time & lhs, const Date_Time & rhs);
			bool operator<= (const Date_Time & lhs, const Date_Time & rhs);
			bool operator>  (const Date_Time & lhs, const Date_Time & rhs);
			bool operator>= (const Date_Time & lhs, const Date_Time & rhs);

			struct Level
			{
			public:

				using date_time_t = Date_Time;

			public:

				date_time_t begin;

				double price = 0.0;

				std::size_t strength = 0U;
			};

			std::ostream & operator<< (std::ostream & stream, const Level & level);

			struct Candle
			{
			public:

				using raw_date_t = unsigned int;
				using raw_time_t = unsigned int;

				using date_time_t = Date_Time;

				using price_t = double;

				using volume_t = unsigned long long;

			public:

				void update_date_time() noexcept;

			public:

				static inline const std::size_t prediction_range = 5U;

			public:

				raw_date_t raw_date = 0U;
				raw_time_t raw_time = 0U;

				date_time_t date_time;

				price_t price_open  = 0.0;
				price_t price_high  = 0.0;
				price_t price_low   = 0.0;
				price_t price_close = 0.0;

				volume_t volume = 0ULL;

				double price_deviation = 0.0;

				double price_deviation_open = 0.0;

				double price_deviation_max = 0.0;
				double price_deviation_min = 0.0;

				double volume_deviation = 0.0;

				std::array < double, prediction_range > regression_tags;

				std::string classification_tag;

				int movement_tag = 0;

				Level support;
				Level resistance;

				std::vector < double > indicators;
				std::vector < double > oscillators;
			};

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_CANDLE_HPP