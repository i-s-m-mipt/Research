#include "bollinger_bands.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void BOLLINGER_BANDS::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps == 0U)
						{
							throw std::domain_error("invalid timesteps value");
						}

						if (m_deviations < std::numeric_limits < double > ::epsilon())
						{
							throw std::domain_error("invalid deviations value");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void BOLLINGER_BANDS::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						auto value = 0.0;

						for (auto i = 0U; i < m_timesteps; ++i)
						{
							value += candles[i].price_close;
						}

						value /= m_timesteps;

						candles[m_timesteps - 1U].indicators.push_back(value);

						for (auto i = m_timesteps; i < std::size(candles); ++i)
						{
							candles[i].indicators.push_back(candles[i - 1U].indicators.back() -
								candles[i - m_timesteps].price_close / m_timesteps + candles[i].price_close / m_timesteps);
						}

						for (auto i = m_timesteps; i < std::size(candles); ++i)
						{
							auto s = 0.0;

							for (auto j = i - m_timesteps; j < i; ++j)
							{
								s += std::pow(candles[j].price_close - candles[i - 1U].indicators.back(), 2.0);
							}

							auto standard_deviation = std::sqrt(s / m_timesteps);

							auto upper_band = candles[i - 1U].indicators.back() + m_deviations * standard_deviation;
							auto lower_band = candles[i - 1U].indicators.back() - m_deviations * standard_deviation;

							candles[i - 1U].indicators.push_back(upper_band);
							candles[i - 1U].indicators.push_back(lower_band);
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