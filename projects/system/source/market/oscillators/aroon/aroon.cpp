#include "aroon.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void AROON::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps == 0U)
						{
							throw std::domain_error("invalid timesteps value");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < oscillator_exception > (logger, exception);
					}
				}

				void AROON::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						for (auto candle = std::next(std::begin(candles), m_timesteps); candle != std::end(candles); ++candle)
						{
							auto min_max = std::minmax_element(std::prev(candle, m_timesteps), std::next(candle),
								[](const auto & lhs, const auto & rhs) { return (lhs.price_close < rhs.price_close); });

							auto min_distance = std::distance(min_max.first,  candle);
							auto max_distance = std::distance(min_max.second, candle);

							auto aroon_down = 100.0 * (m_timesteps - min_distance) / m_timesteps;
							auto aroon_up   = 100.0 * (m_timesteps - max_distance) / m_timesteps;
							
							candle->oscillators.push_back(aroon_up - aroon_down);
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < oscillator_exception > (logger, exception);
					}
				}

			} // namespace oscillators

		} // namespace market

	} // namespace system

} // namespace solution