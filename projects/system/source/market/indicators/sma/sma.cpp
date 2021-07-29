#include "sma.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void SMA::initialize() const
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

				void SMA::operator()(candles_container_t & candles) const
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