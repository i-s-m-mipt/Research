#include "ama.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void AMA::initialize() const
				{
					RUN_LOGGER(logger);

					try
					{
						if (m_timesteps == 0U)
						{
							throw std::domain_error("required: (timesteps > 0)");
						}

						if (m_f == 0U)
						{
							throw std::domain_error("required: (f > 0)");
						}

						if (m_s == 0U)
						{
							throw std::domain_error("required: (s > 0)");
						}
					}
					catch (const std::exception & exception)
					{
						shared::catch_handler < indicator_exception > (logger, exception);
					}
				}

				void AMA::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						auto fastest = 2.0 / (m_f + 1.0);
						auto slowest = 2.0 / (m_s + 1.0);

						auto delta = fastest - slowest;

						candles.at(m_timesteps - 1U).indicators.push_back(candles.at(m_timesteps - 1U).price_close);

						for (auto i = m_timesteps; i < std::size(candles); ++i)
						{
							auto direction = std::abs(candles[i].price_close - candles[i - m_timesteps].price_close);

							auto volatility = 0.0;

							for (auto j = i - m_timesteps; j < i; ++j)
							{
								volatility += std::abs(candles[j].price_close - candles[j + 1U].price_close);
							}

							auto c = std::pow(delta * direction / std::max(volatility, epsilon) + slowest, 2.0);

							candles[i].indicators.push_back(c * candles[i].price_close +
								(1.0 - c) * candles[i - 1U].indicators.back());
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