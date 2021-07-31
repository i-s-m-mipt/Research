#include "ema.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void EMA::initialize() const
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

				void EMA::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						candles.front().indicators.push_back(candles.front().price_close);

						auto k = 2.0 / (m_timesteps + 1.0);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							candles[i].indicators.push_back(k * candles[i].price_close +
								(1.0 - k) * candles[i - 1U].indicators.back());
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