#include "candle.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			std::time_t Date_Time::to_time_t() const noexcept
			{
				std::tm time = {
					static_cast < int > (second),
					static_cast < int > (minute),
					static_cast < int > (hour),
					static_cast < int > (day),
					static_cast < int > (month) - 1,
					static_cast < int > (year)  - 1900 };

				return std::mktime(&time);
			}

			bool operator== (const Date_Time & lhs, const Date_Time & rhs)
			{
				return (
					(lhs.year   == rhs.year) &&
					(lhs.month  == rhs.month) &&
					(lhs.day    == rhs.day) &&
					(lhs.hour   == rhs.hour) &&
					(lhs.minute == rhs.minute) &&
					(lhs.second == rhs.second));
			}

			bool operator!= (const Date_Time & lhs, const Date_Time & rhs)
			{
				return !(lhs == rhs);
			}

			bool operator< (const Date_Time & lhs, const Date_Time & rhs)
			{
				return (lhs.to_time_t() < rhs.to_time_t());
			}

			bool operator<= (const Date_Time & lhs, const Date_Time & rhs)
			{
				return !(lhs > rhs);
			}

			bool operator> (const Date_Time & lhs, const Date_Time & rhs)
			{
				return (lhs.to_time_t() > rhs.to_time_t());
			}

			bool operator>= (const Date_Time & lhs, const Date_Time & rhs)
			{
				return !(lhs < rhs);
			}

			std::ostream & operator<< (std::ostream & stream, const Date_Time & date_time)
			{
				const char delimeter = '/';

				stream <<
					std::setw(4) << std::right << std::setfill('0') << std::noshowpos << date_time.year  << delimeter <<
					std::setw(2) << std::right << std::setfill('0') << std::noshowpos << date_time.month << delimeter <<
					std::setw(2) << std::right << std::setfill('0') << std::noshowpos << date_time.day;

				return stream;
			}

			std::time_t duration(const Date_Time & date_time_1, const Date_Time & date_time_2)
			{
				return (date_time_1.to_time_t() - date_time_2.to_time_t());
			}

			std::size_t Level::strength_to_date(const date_time_t & date) const
			{
				return std::count_if(std::begin(strength_points), std::end(strength_points),
					[&date](const auto & strength_point) { return (strength_point < date); });
			}

			std::size_t Level::weakness_to_date(const date_time_t & date) const
			{
				return std::count_if(std::begin(weakness_points), std::end(weakness_points),
					[&date](const auto & weakness_point) { return (weakness_point < date); });
			}

			std::ostream & operator<< (std::ostream & stream, const Level & level)
			{
				const char delimeter = ',';

				stream << level.begin << delimeter;

				stream <<
					std::setprecision(6) << std::fixed << level.price_low  << delimeter <<
					std::setprecision(6) << std::fixed << level.price_high << delimeter;

				stream << std::setw(3) << std::right << std::setfill('0') <<
					std::noshowpos << level.locality << delimeter;

				stream <<
					std::noshowpos << std::size(level.strength_points) << delimeter << 
					std::noshowpos << std::size(level.weakness_points);

				return stream;
			}

			void Candle::update_date_time() noexcept
			{
				date_time.year   = (raw_date / 100U) / 100U;
				date_time.month  = (raw_date / 100U) % 100U;
				date_time.day    = (raw_date % 100U);

				date_time.hour   = (raw_time / 100U) / 100U;
				date_time.minute = (raw_time / 100U) % 100U;
				date_time.second = (raw_time % 100U);
			}

		} // namespace market

	} // namespace system

} // namespace solution