#include "sar.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void SAR::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						if (m_initial_candles <= 2U)
						{
							throw std::domain_error("required: (initial_candles > 2)");
						}

						if (m_min_acceleration < 0.0)
						{
							throw std::domain_error("required: (min_acceleration >= 0.0)");
						}

						if (m_max_acceleration < 0.0)
						{
							throw std::domain_error("required: (max_acceleration >= 0.0)");
						}

						if (m_acceleration_step <= 0.0)
						{
							throw std::domain_error("required: (acceleration_step > 0.0)");
						}

						if (m_max_acceleration - m_min_acceleration < epsilon)
						{
							throw std::domain_error("required: (min_acceleration < max_acceleration)");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void SAR::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						auto is_growing = candles.at(m_initial_candles - 1U).price_close > candles.front().price_close;

						auto acceleration = m_min_acceleration;

						auto highest_high = std::max_element(std::begin(candles), 
							std::next(std::begin(candles), m_initial_candles),
							[](const auto & lhs, const auto & rhs)
								{ return (lhs.price_high < rhs.price_high); })->price_high;

						auto lowest_low = std::min_element(std::begin(candles), 
							std::next(std::begin(candles), m_initial_candles),
							[](const auto & lhs, const auto & rhs)
								{ return (lhs.price_low < rhs.price_low); })->price_low;

						auto sar = (is_growing ? lowest_low : highest_high);

						candles.at(m_initial_candles - 1U).indicators.push_back(sar);

						auto extreme_point = (is_growing ? highest_high : lowest_low);

						for (auto i = m_initial_candles; i < std::size(candles); ++i)
						{
							if (is_growing)
							{
								sar = std::min({ sar + m_acceleration_step * (extreme_point - sar),
									candles[i - 2U].price_low,
									candles[i - 1U].price_low });

								if (extreme_point < candles[i].price_high)
								{
									extreme_point = candles[i].price_high;

									acceleration = std::min(m_max_acceleration, acceleration + m_acceleration_step);
								}

								if (sar >= candles[i].price_low)
								{
									is_growing = false;

									acceleration = m_min_acceleration;

									sar = extreme_point;

									extreme_point = candles[i].price_low;
								}
							}
							else
							{
								sar = std::max({sar - acceleration * (sar - extreme_point),
									candles[i - 2U].price_high,
									candles[i - 1U].price_high });

								if (extreme_point > candles[i].price_low)
								{
									extreme_point = candles[i].price_low;

									acceleration = std::min(m_max_acceleration, acceleration + m_acceleration_step);
								}

								if (sar <= candles[i].price_high)
								{
									is_growing = true;

									acceleration = m_min_acceleration;

									sar = extreme_point;

									extreme_point = candles[i].price_high;
								}
							}

							candles[i].indicators.push_back(sar);
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

			} // namespace indicators

		} // namespace market

	} // namespace system

} // namespace solution