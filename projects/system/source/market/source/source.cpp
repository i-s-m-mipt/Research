#include "source.hpp"

namespace solution
{
	namespace system
	{
		namespace market
		{
			using Severity = shared::Logger::Severity;

			void Source::initialize()
			{
				RUN_LOGGER(logger);

				try
				{
					initialize_shared_memory();
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			void Source::uninitialize()
			{
				RUN_LOGGER(logger);

				try
				{
					// ...
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			void Source::initialize_shared_memory()
			{
				RUN_LOGGER(logger);

				try
				{
					const auto shared_memory_name = make_shared_memory_name();

					m_shared_memory = shared_memory_t(boost::interprocess::open_only, shared_memory_name.c_str());

					m_deque = m_shared_memory.find < deque_t > (boost::interprocess::unique_instance).first;

					m_mutex = m_shared_memory.find < mutex_t > (boost::interprocess::unique_instance).first;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			std::string Source::make_shared_memory_name() const
			{
				RUN_LOGGER(logger);

				try
				{
					std::string shared_memory_name = "QUIK_" + m_asset + "_";

					if (m_scale == "M60")
					{
						shared_memory_name += "H";
					}
					else
					{
						shared_memory_name += m_scale;
					}

					shared_memory_name += "1";

					return shared_memory_name;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			std::string Source::get(std::size_t size) const
			{
				RUN_LOGGER(logger);

				try
				{
					std::string result;

					{
						boost::interprocess::scoped_lock lock(*m_mutex);

						const auto current_size = m_deque->size();

						if (current_size >= size)
						{
							for (auto i = 0U; i < size; ++i)
							{
								result += std::string((*m_deque)[current_size - size + i].c_str()) + '\n';
							}
						}
					}

					return result;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

		} // namespace market

	} // namespace system

} // namespace solution