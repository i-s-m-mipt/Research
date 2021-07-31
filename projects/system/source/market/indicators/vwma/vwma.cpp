#include "vwma.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void VWMA::initialize() const
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
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void VWMA::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						for (auto i = m_timesteps - 1U; i < std::size(candles); ++i)
						{
							auto value = 0.0;

							auto volume = 0ULL;

							for (auto j = i + 1U - m_timesteps; j <= i; ++j)
							{
								value += candles[j].price_close * candles[j].volume;

								volume += candles[j].volume;
							}

							candles[i].indicators.push_back(value / volume);
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