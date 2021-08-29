#include "dpc.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void DPC::initialize() const
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

				void DPC::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						for (auto i = m_timesteps - 1U; i < std::size(candles); ++i)
						{
							auto max = std::max_element(
								std::next(std::begin(candles), i + 1U - m_timesteps),
								std::next(std::begin(candles), i + 1U),
								[](const auto & lhs, const auto & rhs)
									{ return (lhs.price_high < rhs.price_high); })->price_high;

							auto min = std::min_element(
								std::next(std::begin(candles), i + 1U - m_timesteps),
								std::next(std::begin(candles), i + 1U),
								[](const auto & lhs, const auto & rhs)
									{ return (lhs.price_low < rhs.price_low); })->price_low;

							candles[i].indicators.push_back(max);
							candles[i].indicators.push_back(min);

							candles[i].indicators.push_back((max + min) / 2.0);
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