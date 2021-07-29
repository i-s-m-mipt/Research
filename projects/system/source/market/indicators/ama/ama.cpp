#include "ama.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void AMA::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps == 0U)
						{
							throw std::domain_error("invalid timesteps value");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void AMA::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						candles[m_timesteps].indicators.push_back(candles[m_timesteps].price_close);

						for (auto i = m_timesteps + 1U; i < std::size(candles); ++i)
						{
							auto direction = std::abs(candles[i].price_close - candles[i - m_timesteps - 1U].price_close);

							auto volatility = 0.0;

							for (auto j = i - m_timesteps - 1U; j < i; ++j)
							{
								volatility += std::abs(candles[j].price_close - candles[j + 1U].price_close);
							}

							if (volatility < std::numeric_limits < double > ::epsilon())
							{
								throw std::runtime_error("division by zero volatility");
							}

							auto efficiency_ratio = direction / volatility;

							auto c = std::pow(efficiency_ratio * (m_fastest - m_slowest) + m_slowest, 2.0);

							candles[i].indicators.push_back(c * candles[i].price_close +
								(1.0 - c) * candles[i - 1U].indicators.back());
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