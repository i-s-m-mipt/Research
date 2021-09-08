#ifndef SOLUTION_SYSTEM_MARKET_CANDLE_HPP
#define SOLUTION_SYSTEM_MARKET_CANDLE_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
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

			std::ostream & operator<< (std::ostream & stream, const Date_Time & date_time);

			std::time_t duration(const Date_Time & date_time_1, const Date_Time & date_time_2);

			struct Level
			{
			public:

				using date_time_t = Date_Time;

				using points_container_t = std::vector < date_time_t > ;

			public:

				std::size_t strength_to_date(const date_time_t & date) const;
				std::size_t weakness_to_date(const date_time_t & date) const;

			public:

				date_time_t begin;

				double price_low  = 0.0;
				double price_high = 0.0;

				std::size_t locality = 0U;

				points_container_t strength_points;
				points_container_t weakness_points;
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

				enum class Type
				{
					local_min,
					local_max,

					empty
				};

			public:

				void update_date_time() noexcept;

			public:

				static inline const std::size_t prediction_range = 10U;

			public:

				raw_date_t raw_date = 0U;
				raw_time_t raw_time = 0U;

				date_time_t date_time;

				price_t price_open  = 0.0;
				price_t price_high  = 0.0;
				price_t price_low   = 0.0;
				price_t price_close = 0.0;

				volume_t volume = 0ULL;

				double price_deviation      = 0.0;
				double price_deviation_open = 0.0;
				double price_deviation_max  = 0.0;
				double price_deviation_min  = 0.0;

				double volume_deviation = 0.0;

				std::array < double, prediction_range > regression_tags;

				std::string classification_tag;

				int movement_tag = 0;

				std::size_t n_levels = 0U;

				Level level;

				std::vector < double > indicators;
				std::vector < double > oscillators;

				Type type = Type::empty;
			};

		} // namespace market

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_CANDLE_HPP