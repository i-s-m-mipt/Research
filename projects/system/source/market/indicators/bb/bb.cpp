#include "bb.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void BB::initialize() const
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

				void BB::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						std::vector < double > sma;

						sma.reserve(std::size(candles) - m_timesteps + 1U);

						sma.push_back(std::transform_reduce(std::begin(candles),
							std::next(std::begin(candles), m_timesteps), 0.0, std::plus <> (),
								[](const auto & candle) { return candle.price_close; }) / m_timesteps);

						for (auto i = m_timesteps; i < std::size(candles); ++i)
						{
							sma.push_back(sma.back() + (candles[i].price_close -
								candles[i - m_timesteps].price_close) / m_timesteps);
						}

						for (auto i = m_timesteps; i <= std::size(candles); ++i)
						{
							auto s = 0.0;

							for (auto j = i - m_timesteps; j < i; ++j)
							{
								s += std::pow(candles[j].price_close - sma[i - m_timesteps], 2.0);
							}

							auto standard_deviation = std::sqrt(s / m_timesteps);

							auto upper_band = sma[i - m_timesteps] + m_deviations * standard_deviation;
							auto lower_band = sma[i - m_timesteps] - m_deviations * standard_deviation;

							auto bollinger_bands_width = (4.0 * standard_deviation) / sma[i - m_timesteps];

							candles[i - 1U].indicators.push_back(upper_band);
							candles[i - 1U].indicators.push_back(lower_band);

							candles[i - 1U].oscillators.push_back(bollinger_bands_width);
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