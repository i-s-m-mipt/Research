#include "rsi.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void RSI::initialize() const
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

				void RSI::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						const auto k = 2.0 / (m_timesteps + 1.0);

						auto ema_U = 0.0;
						auto ema_D = 0.0;

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							if (auto delta = candles[i].price_close - 
								candles[i - 1U].price_close; delta > epsilon)
							{
								if (i == 1U)
								{
									ema_U = delta;
								}
								else
								{
									ema_U = k * delta + (1.0 - k) * ema_U;

									ema_D *= (1.0 - k);
								}
							}
							else
							{
								if (i == 1U)
								{
									ema_D = -delta;
								}
								else
								{
									ema_U *= (1.0 - k);

									ema_D = k * -delta + (1.0 - k) * ema_D;
								}
							}

							auto value = 0.0;

							if (ema_U + ema_D < epsilon)
							{
								value = 100.0;
							}
							else
							{
								value = 100.0 * ema_U / (ema_U + ema_D);
							}

							candles[i].oscillators.push_back(
								std::min(std::max(value / max_value, +0.0), +1.0));
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