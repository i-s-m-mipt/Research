#include "lso.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void LSO::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps_fast == 0U)
						{
							throw std::domain_error("required: (timesteps_fast > 0)");
						}

						if (m_timesteps_slow == 0U)
						{
							throw std::domain_error("required: (timesteps_slow > 0)");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < oscillator_exception > (logger, exception);
					}
				}

				void LSO::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						const auto k = 2.0 / (m_timesteps_slow + 1.0);

						auto ema_slow = 0.0;

						for (auto i = m_timesteps_fast - 1U; i < std::size(candles); ++i)
						{
							auto min = std::min_element(
								std::next(std::begin(candles), i + 1U - m_timesteps_fast),
								std::next(std::begin(candles), i + 1U),
								[](const auto & lhs, const auto & rhs) 
									{ return (lhs.price_low < rhs.price_low); })->price_low;

							auto max = std::max_element(
								std::next(std::begin(candles), i + 1U - m_timesteps_fast),
								std::next(std::begin(candles), i + 1U),
								[](const auto & lhs, const auto & rhs)
									{ return (lhs.price_high < rhs.price_high); })->price_high;

							auto fast = 100.0 * (candles[i].price_close - min) / 
								std::max((max - min), epsilon);

							if (i == m_timesteps_fast - 1U)
							{
								ema_slow = fast;
							}
							else
							{
								ema_slow = k * fast + (1.0 - k) * ema_slow;
							}

							candles[i].oscillators.push_back(
								std::min(std::max(ema_slow / max_value, +0.0), +1.0));
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