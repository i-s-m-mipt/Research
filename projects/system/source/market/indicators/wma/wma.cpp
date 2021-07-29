#include "wma.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void WMA::initialize() const
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

				void WMA::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						for (auto i = m_timesteps - 1U; i < std::size(candles); ++i)
						{
							auto value = 0.0;

							for (std::size_t j = i + 1U - m_timesteps, k = 1U; j <= i; ++j, ++k)
							{
								value += candles[j].price_close * k;
							}

							value /= (m_timesteps * std::ceil(m_timesteps / 2.0));

							candles[i].indicators.push_back(value);
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