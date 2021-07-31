#include "macd.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void MACD::initialize() const
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

				void MACD::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						auto k_long = 2.0 / (m_timesteps_long + 1.0);

						std::vector < double > ema_long;

						ema_long.reserve(std::size(candles));

						ema_long.push_back(candles.front().price_close);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							ema_long.push_back(k_long * candles[i].price_close + 
								(1.0 - k_long) * ema_long.back());
						}

						auto k_short = 2.0 / (m_timesteps_short + 1.0);

						std::vector < double > ema_short;

						ema_short.reserve(std::size(candles));

						ema_short.push_back(candles.front().price_close);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							ema_short.push_back(k_short * candles[i].price_close + 
								(1.0 - k_short) * ema_short.back());
						}

						auto k_signal = 2.0 / (m_timesteps_signal + 1.0);

						std::vector < double > ema_signal;

						ema_signal.reserve(std::size(candles));

						ema_signal.push_back(ema_short.front() - ema_long.front());

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							ema_signal.push_back(k_signal * (ema_short[i] - ema_long[i]) +
								(1.0 - k_signal) * ema_signal.back());
						}

						for (auto i = 0U; i < std::size(candles); ++i)
						{
							candles[i].oscillators.push_back((ema_short[i] - ema_long[i]) - ema_signal[i]);
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