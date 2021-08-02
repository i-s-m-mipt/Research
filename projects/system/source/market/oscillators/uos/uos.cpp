#include "uos.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void UOS::initialize() const
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

				void UOS::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						auto total_buying_pressure = [&candles, epsilon](std::size_t first, std::size_t last)
						{
							auto buying_pressure = 0.0;

							auto true_range = 0.0;

							for (auto j = first; j < last; ++j)
							{
								buying_pressure += candles[j].price_close -
									std::min(candles[j].price_low,  candles[j - 1U].price_close);

								true_range +=
									(std::max(candles[j].price_high, candles[j - 1U].price_close) -
									 std::min(candles[j].price_low,  candles[j - 1U].price_close));
							}

							return (buying_pressure / std::max(true_range, epsilon));
						};

						for (auto i = 4U * m_timesteps + 1U; i <= std::size(candles); ++i)
						{
							candles[i - 1U].oscillators.push_back(100.0 / (4.0 + 2.0 + 1.0) * (
								4.0 * total_buying_pressure(i - 1U * m_timesteps, i) + 
								2.0 * total_buying_pressure(i - 2U * m_timesteps, i) + 
								1.0 * total_buying_pressure(i - 4U * m_timesteps, i)));
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