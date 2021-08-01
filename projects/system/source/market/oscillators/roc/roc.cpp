#include "roc.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void ROC::initialize() const
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

				void ROC::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						for (auto i = m_timesteps; i < std::size(candles); ++i)
						{
							candles[i].oscillators.push_back((candles[i].price_close /
								candles[i - m_timesteps].price_close - 1.0) * 100.0);
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