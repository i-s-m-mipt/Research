#include "chv.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void CHV::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps_ema == 0U)
						{
							throw std::domain_error("required: (timesteps_ema > 0)");
						}

						if (m_timesteps_roc == 0U)
						{
							throw std::domain_error("required: (timesteps_roc > 0)");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < oscillator_exception > (logger, exception);
					}
				}

				void CHV::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						const auto k = 2.0 / (m_timesteps_ema + 1.0);

						std::vector < double > ema(std::size(candles), 0.0);

						ema.front() = candles.front().price_high - candles.front().price_low;

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							ema[i] = k * (candles[i].price_high - candles[i].price_low) + (1.0 - k) * ema[i - 1U];
						}

						for (auto i = m_timesteps_roc; i < std::size(candles); ++i)
						{
							candles[i].oscillators.push_back(100.0 * (ema[i] / 
								std::max(ema[i - m_timesteps_roc], epsilon) - 1.0));
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