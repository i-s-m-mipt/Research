#include "timer.hpp"

namespace solution
{
	namespace shared
	{
		void Timer::initialize()
		{
			RUN_LOGGER(logger);

			try
			{
				m_stream << "Timer \"" << m_name << "\" launched ... " << std::endl << std::endl;
			}
			catch (const std::exception & exception)
			{
				uninitialize();

				shared::catch_handler < timer_exception > (logger, exception);
			}
		}

		void Timer::uninitialize()
		{
			RUN_LOGGER(logger);

			try
			{
				elapsed();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < timer_exception > (logger, exception);
			}
		}

		void Timer::elapsed() const
		{
			RUN_LOGGER(logger);

			try
			{
				auto end = clock_t::now();

				m_stream << "Timer \"" << m_name << "\" elapsed " << std::setprecision(6) << std::fixed <<
					static_cast < double > (std::chrono::duration_cast < std::chrono::microseconds > 
						(end - m_begin).count()) / 1'000'000.0 << " (seconds)" << std::endl << std::endl;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < timer_exception > (logger, exception);
			}
		}

	} // namespace shared

} // namespace solution 