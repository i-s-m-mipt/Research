#include "eri.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void ERI::initialize() const
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

				void ERI::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						auto k = 2.0 / (m_timesteps + 1.0);

						std::vector < double > ema;

						ema.reserve(std::size(candles));

						ema.push_back(candles.front().price_close);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							ema.push_back(k * candles[i].price_close + (1.0 - k) * ema.back());
						}

						for (auto i = 0U; i < std::size(candles); ++i)
						{
							candles[i].oscillators.push_back(
								(candles[i].price_high + candles[i].price_low) / 2.0 - ema[i]);
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