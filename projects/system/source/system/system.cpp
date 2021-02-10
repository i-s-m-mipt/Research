#include "system.hpp"

namespace solution
{
	namespace system
	{
		using Severity = shared::Logger::Severity;

		void System::initialize()
		{
			RUN_LOGGER(logger);

			try
			{
				// ...
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::run()
		{
			RUN_LOGGER(logger);

			try
			{
				// ...
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution