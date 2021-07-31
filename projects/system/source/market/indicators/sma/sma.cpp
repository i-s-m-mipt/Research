#include "sma.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void SMA::initialize() const
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
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void SMA::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						candles.at(m_timesteps - 1U).indicators.push_back(std::transform_reduce(
							std::begin(candles), std::next(std::begin(candles), m_timesteps), 0.0, 
								std::plus <> (), [](const auto & candle) { return candle.price_close; }) / m_timesteps);

						for (auto i = m_timesteps; i < std::size(candles); ++i)
						{
							candles[i].indicators.push_back(candles[i - 1U].indicators.back() + 
								(candles[i].price_close - candles[i - m_timesteps].price_close) / m_timesteps);
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