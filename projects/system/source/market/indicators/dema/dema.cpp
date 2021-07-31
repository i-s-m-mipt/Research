#include "dema.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void DEMA::initialize() const
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

				void DEMA::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						auto k = 2.0 / (m_timesteps + 1.0);

						std::vector < double > ema_1;

						ema_1.reserve(std::size(candles));

						ema_1.push_back(candles.front().price_close);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							ema_1.push_back(k * candles[i].price_close + (1.0 - k) * ema_1.back());
						}

						std::vector < double > ema_2;

						ema_2.reserve(std::size(ema_1));

						ema_2.push_back(ema_1.front());

						for (auto i = 1U; i < std::size(ema_1); ++i)
						{
							ema_2.push_back(k * ema_1[i] + (1.0 - k) * ema_2.back());
						}

						candles.front().indicators.push_back(candles.front().price_close);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							candles[i].indicators.push_back(2.0 * ema_1[i] - ema_2[i]);
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