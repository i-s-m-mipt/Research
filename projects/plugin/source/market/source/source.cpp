#include "source.hpp"

namespace solution
{
	namespace plugin
	{
		namespace market
		{
			void Source::initialize(const api_t & api)
			{
				RUN_LOGGER(logger);

				try
				{
					const auto timeframe = api.constant("INTERVAL_" + m_scale_code);

					m_source = std::make_unique < source_t > (
						api.create_source(m_class_code, m_asset_code, timeframe));

					const auto shared_memory_name = make_shared_memory_name();

					const auto shared_memory_size = 65536U;

					boost::interprocess::shared_memory_object::remove(shared_memory_name.c_str());

					m_shared_memory = shared_memory_t(boost::interprocess::create_only, 
						shared_memory_name.c_str(), shared_memory_size);

					record_allocator_t allocator(m_shared_memory.get_segment_manager());

					m_deque = m_shared_memory.construct < deque_t > (boost::interprocess::unique_instance) (allocator);
					m_price = m_shared_memory.construct < price_t > (boost::interprocess::unique_instance) ();
					m_mutex = m_shared_memory.construct < mutex_t > (boost::interprocess::unique_instance) ();
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
					m_shared_memory.destroy_ptr(m_deque);
					m_shared_memory.destroy_ptr(m_mutex);

					const auto shared_memory_name = make_shared_memory_name();

					boost::interprocess::shared_memory_object::remove(shared_memory_name.c_str());
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
					return "QUIK_" + m_asset_code + "_" + m_scale_code;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			void Source::update() const
			{
				RUN_LOGGER(logger);

				try
				{
					const auto size = m_source->size();

					{
						boost::interprocess::scoped_lock < mutex_t > lock(*m_mutex);

						*m_price = make_bar(size).price_close;
					}

					if (size >= m_size)
					{
						if (m_deque->size() == 0)
						{
							boost::interprocess::scoped_lock < mutex_t > lock(*m_mutex);

							for (index_t index = size - m_size + 1; index <= size; ++index)
							{
								m_deque->push_back(make_record(make_bar(index)));

								m_last_index = index;
							}
						}

						if (m_last_index < size)
						{
							auto delta = std::min(size - m_last_index, m_size);

							boost::interprocess::scoped_lock < mutex_t > lock(*m_mutex);

							for (index_t index = std::max(size - m_size + 1, m_last_index + 1); index <= size; ++index)
							{
								m_deque->push_back(make_record(make_bar(index)));

								m_last_index = index;
							}

							for (index_t index = 0; index < delta; ++index)
							{
								m_deque->pop_front();
							}
						}
						else
						{
							boost::interprocess::scoped_lock < mutex_t > lock(*m_mutex);

							m_deque->pop_back();

							m_deque->push_back(make_record(make_bar(size)));
						}
					}
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::Bar Source::make_bar(index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					Bar bar;

					bar.index = index;

					std::stringstream stream;

					auto time = m_source->time(index);

					stream <<
						std::setfill('0') << std::setw(4) << time.year  <<
						std::setfill('0') << std::setw(2) << time.month <<
						std::setfill('0') << std::setw(2) << time.day;
						
					stream >> bar.date;

					stream.clear();

					stream <<
						std::setfill('0') << std::setw(2) << time.hour   <<
						std::setfill('0') << std::setw(2) << time.minute <<
						std::setfill('0') << std::setw(2) << time.second;

					stream >> bar.time;

					bar.price_open  = m_source->price_open(index);
					bar.price_high  = m_source->price_high(index);
					bar.price_low   = m_source->price_low(index);
					bar.price_close = m_source->price_close(index);

					bar.volume = static_cast < Bar::volume_t > (m_source->volume(index));

					return bar;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::record_t Source::make_record(const Bar & bar) const
			{
				RUN_LOGGER(logger);

				try
				{
					static const char delimeter = ',';

					std::stringstream sout;

					sout << std::setprecision(6) << std::fixed <<
						bar.date        << delimeter <<
						bar.time        << delimeter <<
						bar.price_open  << delimeter <<
						bar.price_high  << delimeter <<
						bar.price_low   << delimeter <<
						bar.price_close << delimeter <<
						bar.volume;

					return record_t(sout.str().c_str(), m_shared_memory.get_segment_manager());
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

		} // namespace market

	} // namespace plugin

} // namespace solution