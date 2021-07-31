#include "template.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			namespace indicators
			{
				void TEMPLATE::initialize() const
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

				void TEMPLATE::operator()(candles_container_t & candles) const
				{
					RUN_LOGGER(logger);

					try
					{
						// ...
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