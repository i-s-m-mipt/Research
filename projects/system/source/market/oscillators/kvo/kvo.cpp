#include "kvo.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void KVO::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps_long == 0U)
						{
							throw std::domain_error("required: (timesteps_long > 0)");
						}

						if (m_timesteps_short == 0U)
						{
							throw std::domain_error("required: (timesteps_short > 0)");
						}

						if (m_timesteps_signal == 0U)
						{
							throw std::domain_error("required: (timesteps_signal > 0)");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < oscillator_exception > (logger, exception);
					}
				}

				void KVO::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto k_long   = 2.0 / (m_timesteps_long   + 1.0);
						const auto k_short  = 2.0 / (m_timesteps_short  + 1.0);
						const auto k_signal = 2.0 / (m_timesteps_signal + 1.0);

						auto ema_long   = 0.0;
						auto ema_short  = 0.0;
						auto ema_signal = 0.0;

						auto previous_typical_price = (candles.front().price_low +
							candles.front().price_high + candles.front().price_close) / 3.0;

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							auto typical_price = (candles[i].price_low +
								candles[i].price_high + candles[i].price_close) / 3.0;

							auto volume = (typical_price > previous_typical_price ? +1.0 : -1.0) *
								static_cast < double > (candles[i].volume);

							if (i == 1U)
							{
								ema_long = ema_short = volume;

								ema_signal = ema_long - ema_short;
							}
							else
							{
								ema_long  = k_long  * volume + (1.0 - k_long ) * ema_long;
								ema_short = k_short * volume + (1.0 - k_short) * ema_short;

								ema_signal = k_signal * (ema_long - ema_short) + 
									(1.0 - k_signal) * ema_signal;
							}

							candles[i].oscillators.push_back(ema_signal);
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