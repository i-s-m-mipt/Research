#include "bbs.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void BBS::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps == 0U)
						{
							throw std::domain_error("required: (timesteps > 0)");
						}

						if (m_deviations < std::numeric_limits < double > ::epsilon())
						{
							throw std::domain_error("required: (deviations > 0.0)");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void BBS::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

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
							auto m = sma[i - m_timesteps];

							auto standard_deviation = 0.0;

							for (auto j = i - m_timesteps; j < i; ++j)
							{
								standard_deviation += std::pow(candles[j].price_close - m, 2.0);
							}

							standard_deviation = std::sqrt(standard_deviation / m_timesteps);

							auto upper_band = m + m_deviations * standard_deviation;
							auto lower_band = m - m_deviations * standard_deviation;

							candles[i - 1U].indicators.push_back(upper_band);
							candles[i - 1U].indicators.push_back(lower_band);

							/*
							candles[i - 1U].oscillators.push_back(
								2.0 * m_deviations * standard_deviation / m);
							candles[i - 1U].oscillators.push_back((candles[i - 1U].price_close - 
								lower_band) / std::max((upper_band - lower_band), epsilon));
							*/
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