#include "dmi.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void DMI::initialize() const
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

				void DMI::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto k = 2.0 / (m_timesteps + 1.0);

						const auto epsilon = std::numeric_limits < double > ::epsilon();

						auto tr_ema = 0.0, pdm_ema = 0.0, ndm_ema = 0.0;

						std::vector < double > dx(std::size(candles) - 1U, 0.0);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							auto tr = 
								std::max(candles[i].price_high, candles[i - 1U].price_close) -
								std::min(candles[i].price_low,  candles[i - 1U].price_close);

							auto pm = candles[i     ].price_high - candles[i - 1U].price_high;
							auto nm = candles[i - 1U].price_low  - candles[i     ].price_low;

							auto pdm = 0.0;
							auto ndm = 0.0;

							if (pm - nm > epsilon && pm > epsilon)
							{
								pdm = pm;
							}

							if (nm - pm > epsilon && nm > epsilon)
							{
								ndm = nm;
							}

							if (i == 1U)
							{
								tr_ema = tr, pdm_ema = pdm, ndm_ema = ndm;
							}
							else
							{
								tr_ema  = k * tr  + (1.0 - k) * tr_ema;

								pdm_ema = k * pdm + (1.0 - k) * pdm_ema;
								ndm_ema = k * ndm + (1.0 - k) * ndm_ema;
							}

							auto pdi = pdm_ema / tr_ema;
							auto ndi = ndm_ema / tr_ema;

							dx[i - 1U] = 100.0 * std::abs(pdi - ndi) / (pdi + ndi);
						}

						std::vector < double > adx(std::size(dx), 0.0);

						adx.front() = dx.front();

						for (auto i = 1U; i < std::size(adx); ++i)
						{
							adx[i] = k * dx[i] + (1.0 - k) * adx[i - 1U];
						}

						std::vector < double > adxr(std::size(adx) - m_timesteps, 0.0);

						for (auto i = 0U; i < std::size(adxr); ++i)
						{
							adxr[i] = (adx[i + m_timesteps] - adx[i]) / 2.0;
						}

						for (auto i = 1U + m_timesteps; i < std::size(candles); ++i)
						{
							candles[i].oscillators.push_back(dx  [i - 1U]);
							candles[i].oscillators.push_back(adx [i - 1U]);
							candles[i].oscillators.push_back(adxr[i - 1U - m_timesteps]);
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