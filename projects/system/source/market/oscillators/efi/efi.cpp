#include "efi.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace oscillators
			{
				void EFI::initialize() const
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

				void EFI::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						auto k = 2.0 / (m_timesteps + 1.0);

						for (auto i = 1U; i < std::size(candles); ++i)
						{
							auto raw_force_index = (candles[i].price_close -
								candles[i - 1U].price_close) * candles[i].volume;

							if (i == 1U)
							{
								candles[i].oscillators.push_back(raw_force_index);
							}
							else
							{
								candles[i].oscillators.push_back(k * raw_force_index + 
									(1.0 - k) * candles[i - 1U].oscillators.back());
							}
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