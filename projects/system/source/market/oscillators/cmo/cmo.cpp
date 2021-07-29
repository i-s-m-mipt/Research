#include "cmo.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void CMO::initialize() const
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
						shared::catch_handler < oscillator_exception > (logger, exception);
					}
				}

				void CMO::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						for (auto i = m_timesteps - 1U; i < std::size(candles); ++i)
						{
							auto positive_s = 0.0;
							auto negative_s = 0.0;

							for (auto j = i + 1U - m_timesteps; j < i; ++j)
							{
								auto delta = candles[j + 1U].price_close - candles[j].price_close;

								if (delta > 0.0)
								{
									positive_s += std::abs(delta);
								}
								else
								{
									negative_s += std::abs(delta);
								}
							}

							candles[i].oscillators.push_back(100.0 * (positive_s - negative_s) / (positive_s + negative_s));
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