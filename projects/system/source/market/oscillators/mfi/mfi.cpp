#include "mfi.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void MFI::initialize() const
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

				void MFI::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						std::vector < double > typical_prices(std::size(candles), 0.0);

						std::transform(std::begin(candles), std::end(candles), std::begin(typical_prices),
							[](const auto & candle)
							{
								return ((candle.price_high + candle.price_low + candle.price_close) / 3.0);
							});

						for (auto i = m_timesteps + 1U; i <= std::size(candles); ++i)
						{
							auto positive_money_flow = 0.0;
							auto negative_money_flow = 0.0;

							for (auto j = i - m_timesteps; j < i; ++j)
							{
								auto money_flow = typical_prices[j] * candles[j].volume;

								if (typical_prices[j] - typical_prices[j - 1U] > epsilon)
								{
									positive_money_flow += money_flow;
								}
								else
								{
									negative_money_flow += money_flow;
								}
							}

							auto value = 100.0 - 100.0 / (1.0 + (positive_money_flow /
								std::max(negative_money_flow, epsilon)));

							candles[i - 1U].oscillators.push_back(
								std::min(std::max(value / max_value, +0.0), +1.0));
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