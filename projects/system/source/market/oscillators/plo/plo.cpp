#include "plo.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void PLO::initialize() const
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

				void PLO::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						for (auto i = m_timesteps; i < std::size(candles); ++i)
						{
							auto n = 0U;

							for (auto j = i - m_timesteps; j < i; ++j)
							{
								if (candles[j + 1U].price_close - candles[j].price_close > epsilon)
								{
									++n;
								}
							}

							candles[i].oscillators.push_back(100.0 * n / m_timesteps);
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