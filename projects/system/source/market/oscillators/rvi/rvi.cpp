#include "rvi.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void RVI::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps_sma == 0U)
						{
							throw std::domain_error("required: (timesteps_sma > 0)");
						}

						if (m_timesteps_wma == 0U)
						{
							throw std::domain_error("required: (timesteps_wma > 0)");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < oscillator_exception > (logger, exception);
					}
				}

				void RVI::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						std::vector < double > rvi(std::size(candles), 0.0);

						for (auto i = 0U; i < std::size(candles); ++i)
						{
							rvi[i] = (candles[i].price_close - candles[i].price_open) /
								std::max((candles[i].price_high - candles[i].price_low), epsilon);
						}

						std::vector < double > sma;

						sma.reserve(std::size(candles) - m_timesteps_sma + 1U);

						sma.push_back(std::accumulate(std::begin(rvi), 
							std::next(std::begin(rvi), m_timesteps_sma), 0.0) / m_timesteps_sma);

						for (auto i = m_timesteps_sma; i < std::size(rvi); ++i)
						{
							sma.push_back(sma.back() + (rvi[i] - rvi[i - m_timesteps_sma]) / m_timesteps_sma);
						}

						for (auto i = m_timesteps_wma; i <= std::size(sma); ++i)
						{
							auto value = 0.0;

							for (std::size_t j = i - m_timesteps_wma, k = 1U; j < i; ++j, ++k)
							{
								value += sma[j] * k;
							}

							value /= (m_timesteps_wma * (m_timesteps_wma + 1.0) / 2.0);

							candles[i + m_timesteps_sma - 2U].oscillators.push_back(
								(std::min(std::max(value / max_value, -1.0), +1.0) + 1.0) / 2.0);
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