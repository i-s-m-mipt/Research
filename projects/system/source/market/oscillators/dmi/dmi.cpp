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
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						const auto k = 2.0 / (m_timesteps + 1.0);

						auto pdi = 0.0, ndi = 0.0;

						std::vector < double > dx(std::size(candles) - 1U, 0.0);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							auto tr = std::max(epsilon,
								std::max(candles[i].price_high, candles[i - 1U].price_close) -
								std::min(candles[i].price_low,  candles[i - 1U].price_close));

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

							auto pdm_div_tr = pdm / tr;
							auto ndm_div_tr = ndm / tr;

							if (i == 1U)
							{
								pdi = pdm_div_tr;
								ndi = ndm_div_tr;
							}
							else
							{
								pdi = k * pdm_div_tr + (1.0 - k) * pdi;
								ndi = k * ndm_div_tr + (1.0 - k) * ndi;
							}

							candles[i].oscillators.push_back(std::min(std::max(pdi / max_value, +0.0), +1.0));
							candles[i].oscillators.push_back(std::min(std::max(ndi / max_value, +0.0), +1.0));
							
							dx[i - 1U] = 100.0 * std::abs(pdi - ndi) / std::max((pdi + ndi), epsilon);
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
							candles[i].oscillators.push_back(std::min(std::max(
								 dx [i - 1U] / max_value, +0.0), +1.0));
							candles[i].oscillators.push_back(std::min(std::max(
								adx [i - 1U] / max_value, +0.0), +1.0));
							candles[i].oscillators.push_back(std::min(std::max(
								adxr[i - 1U - m_timesteps] / max_value, +0.0), +1.0));
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