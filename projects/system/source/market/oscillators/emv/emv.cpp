#include "emv.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void EMV::initialize() const
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

				void EMV::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						const auto epsilon = std::numeric_limits < double > ::epsilon();

						std::vector < double > emv(std::size(candles) - 1U, 0.0);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							auto h_0 = candles[i].price_high;
							auto l_0 = candles[i].price_low;

							auto h_1 = candles[i - 1U].price_high;
							auto l_1 = candles[i - 1U].price_low;

							emv[i - 1U] = ((h_0 + l_0) / 2.0 - (h_1 + l_1) / 2.0) / 
								std::max((candles[i].volume / std::max((h_0 - l_0), epsilon)), epsilon);
						}

						candles.at(m_timesteps).oscillators.push_back(std::accumulate(
							std::begin(emv), std::next(std::begin(emv), m_timesteps), 0.0) / m_timesteps);

						for (auto i = m_timesteps + 1U; i < std::size(candles); ++i)
						{
							candles[i].oscillators.push_back(candles[i - 1U].oscillators.back() +
								(emv[i - 1U] - emv[i - m_timesteps - 1U]) / m_timesteps);
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