#include "trix.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void TRIX::initialize() const
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

				void TRIX::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto k = 2.0 / (m_timesteps + 1.0);

						auto ema = candles.front().price_close;

						auto dma = ema;
						auto tma = dma;

						auto previous_tma = tma;

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							ema = k * candles[i].price_close + (1.0 - k) * ema;

							dma = k * ema + (1.0 - k) * dma;
							tma = k * dma + (1.0 - k) * tma;

							candles[i].oscillators.push_back(100.0 * (tma - previous_tma) / previous_tma);

							previous_tma = tma;
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