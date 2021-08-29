#include "vhf.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void VHF::initialize() const
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

				void VHF::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						for (auto i = m_timesteps + 1U; i <= std::size(candles); ++i)
						{
							auto min_max = std::minmax_element(
								std::next(std::begin(candles), i - m_timesteps - 1U),
								std::next(std::begin(candles), i),
								[](const auto & lhs, const auto & rhs)
									{ return (lhs.price_close < rhs.price_close); });

							auto s = 0.0;

							for (auto j = i - m_timesteps; j < i; ++j)
							{
								s += std::abs(candles[j].price_close - candles[j - 1U].price_close);
							}

							auto value = (min_max.second - min_max.first) / std::max(s, epsilon);

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