#include "source.hpp"

namespace solution
{
	namespace plugin
	{
		namespace market
		{
			using Severity = shared::Logger::Severity;

			void Source::initialize()
			{
				RUN_LOGGER(logger);

				try
				{
					initialize_source();

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

			void Source::initialize_source()
			{
				RUN_LOGGER(logger);

				try
				{
					m_state.get_global(references);

					if (m_state.is_nil(-1))
					{
						m_state.pop();

						m_state.new_table();

						m_state.set_global(references);
						m_state.get_global(references);
					}

					m_state.call < 2 > ("CreateDataSource", m_class_code, m_asset_code, get_scale_constant());

					m_state.push_value(-2);

					m_reference = luaL_ref(m_state.state(), -4); // !

					m_state.pop(2);
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

					const auto shared_memory_size = 65536U;

					boost::interprocess::shared_memory_object::remove(shared_memory_name.c_str());

					m_shared_memory = shared_memory_t(boost::interprocess::create_only,
						shared_memory_name.c_str(), shared_memory_size);

					record_allocator_t allocator(m_shared_memory.get_segment_manager());

					m_deque = m_shared_memory.construct < deque_t > (boost::interprocess::unique_instance) (allocator);
					m_mutex = m_shared_memory.construct < mutex_t > (boost::interprocess::unique_instance) ();
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::scale_constant_t Source::get_scale_constant() const
			{
				RUN_LOGGER(logger);

				try
				{
					m_state.get_global("INTERVAL_" + m_scale_code);

					auto result = m_state.top < scale_constant_t > ().get();

					m_state.pop();

					return result;
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
					const auto size = max_size();

					if (size >= m_size)
					{
						if (m_deque->size() == 0)
						{
							boost::interprocess::scoped_lock lock(*m_mutex);

							for (index_t index = size - m_size + 1; index <= size; ++index)
							{
								m_deque->push_back(make_record(make_candle(index)));

								m_last_index = index;
							}
						}

						if (m_last_index < size)
						{
							auto delta = std::min(size - m_last_index, m_size);

							boost::interprocess::scoped_lock lock(*m_mutex);

							for (index_t index = std::max(static_cast < unsigned int > ( // ?
								size - m_size + 1U), m_last_index + 1U); index <= size; ++index)
							{
								m_deque->push_back(make_record(make_candle(index)));

								m_last_index = index;
							}

							for (index_t index = 0; index < delta; ++index)
							{
								m_deque->pop_front();
							}
						}
						else
						{
							boost::interprocess::scoped_lock lock(*m_mutex);

							m_deque->pop_back();

							m_deque->push_back(make_record(make_candle(size)));
						}
					}
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			std::size_t Source::max_size() const
			{
				RUN_LOGGER(logger);

				try
				{
					m_state.get_global(references);

					m_state.push_number(m_reference);
					m_state.raw_get(-2);
					m_state.push_string("Size");
					m_state.raw_get(-2);
					m_state.push_value(-2);
					m_state.call(1, 1);

					auto result = m_state.top < std::size_t > ().get();

					m_state.pop(3);

					return result;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::Candle Source::make_candle(index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					return Candle {     index, 
						get_date_time  (index),
						get_price_open (index),
						get_price_high (index),
						get_price_low  (index),
						get_price_close(index),
						get_volume     (index) };
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::record_t Source::make_record(const Candle & candle) const
			{
				RUN_LOGGER(logger);

				try
				{
					std::stringstream date;
					
					date <<
						std::setfill('0') << std::setw(4) << candle.date_time.year  <<
						std::setfill('0') << std::setw(2) << candle.date_time.month <<
						std::setfill('0') << std::setw(2) << candle.date_time.day;

					std::stringstream time;

					time <<
						std::setfill('0') << std::setw(2) << candle.date_time.hour   <<
						std::setfill('0') << std::setw(2) << candle.date_time.minute <<
						std::setfill('0') << std::setw(2) << candle.date_time.second;

					std::stringstream sout;

					static const char delimeter = ',';

					sout << std::setprecision(6) << std::fixed <<
						date.str()         << delimeter <<
						time.str()         << delimeter <<
						candle.price_open  << delimeter <<
						candle.price_high  << delimeter <<
						candle.price_low   << delimeter <<
						candle.price_close << delimeter <<
						candle.volume;

					return record_t(sout.str().c_str(), m_shared_memory.get_segment_manager());
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::Candle::Date_Time Source::get_date_time(index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					return std::get < Candle::Date_Time > (call("T", index));
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::Candle::price_t Source::get_price_open(index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					return std::get < Candle::price_t > (call("O", index));
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::Candle::price_t Source::get_price_high(index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					return std::get < Candle::price_t > (call("H", index));
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::Candle::price_t Source::get_price_low(index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					return std::get < Candle::price_t > (call("L", index));
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::Candle::price_t Source::get_price_close(index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					return std::get < Candle::price_t > (call("C", index));
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::Candle::volume_t Source::get_volume(index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					return std::get < Candle::volume_t > (call("V", index));
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			std::variant < Source::Candle::Date_Time, Source::Candle::price_t, Source::Candle::volume_t >
				Source::call(const std::string & name, index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					m_state.get_global(references);

					m_state.push_number(m_reference);
					m_state.raw_get(-2);
					m_state.push_string(name);
					m_state.raw_get(-2);
					m_state.push_value(-2);
					m_state.push_number(index);

					m_state.call(2, 1);

					if (name == "T")
					{
						//auto table = m_state.top < detail::lua::tables::Chart_Time > ().get();

						//Candle::Date_Time result {
						//	table.year.get(), table.month.get(), table.day.get(),
						//	table.hour.get(), table.min.get(),   table.sec.get() };

						Candle::Date_Time result { 0, 0, 0, 0, 0, 0 }; // TODO

						m_state.pop(3);

						return result;
					}
					else
					{
						if (name == "V")
						{
							auto result = m_state.top < Candle::volume_t > ().get();

							m_state.pop(3);

							return result;
						}
						else
						{
							auto result = m_state.top < Candle::price_t > ().get();

							m_state.pop(3);

							return result;
						}
					}
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

		} // namespace market

	} // namespace plugin

} // namespace solution