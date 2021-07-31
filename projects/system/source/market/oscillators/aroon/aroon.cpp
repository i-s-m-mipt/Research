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
							throw std::domain_error("required: (timesteps > 0)");
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

							candle->oscillators.push_back((std::distance(min_max.first, candle) - 
								std::distance(min_max.second, candle)) * 100.0 / m_timesteps);
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