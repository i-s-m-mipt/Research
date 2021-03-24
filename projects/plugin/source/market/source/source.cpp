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

					if (m_state.is_nil())
					{
						m_state.pop();

						m_state.new_table();

						m_state.set_global(references);
						m_state.get_global(references);
					}

					m_state.get_global("CreateDataSource");

					m_state.push_string(m_class_code);
					m_state.push_string(m_asset_code);

					m_state.get_global("INTERVAL_" + m_scale_code);

					m_state.call(3, 2);

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

					const auto shared_memory_size = 1024U * 1024U;

					boost::interprocess::shared_memory_object::remove(shared_memory_name.c_str());

					m_shared_memory = shared_memory_t(boost::interprocess::create_only,
						shared_memory_name.c_str(), shared_memory_size);

					record_allocator_t allocator(m_shared_memory.get_segment_manager());

					m_deque = m_shared_memory.construct < deque_t > (
						boost::interprocess::unique_instance) (allocator);
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
							for (index_t index = size - m_size + 1; index <= size; ++index)
							{
								m_deque->push_back(make_record(make_candle(index)));

								m_last_index = index;
							}
						}

						if (m_last_index < size)
						{
							auto delta = std::min(size - m_last_index, m_size);

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
					m_state.raw_get();
					m_state.push_string("Size");
					m_state.raw_get();
					m_state.push_value(-2);
					m_state.call(1);

					auto result = static_cast < std::size_t > (m_state.to_integer());

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

			Source::Candle::variant_t Source::call(const std::string & name, index_t index) const
			{
				RUN_LOGGER(logger);

				try
				{
					m_state.get_global(references);

					m_state.push_number(m_reference);
					m_state.raw_get();
					m_state.push_string(name);
					m_state.raw_get();
					m_state.push_value(-2);
					m_state.push_number(index);

					m_state.call(2);

					Candle::variant_t result;

					if (name == "T")
					{
						Candle::Date_Time date_time;

						date_time.year   = get_date_time_field("year");
						date_time.month  = get_date_time_field("month");
						date_time.day    = get_date_time_field("day");

						date_time.hour   = get_date_time_field("hour");
						date_time.minute = get_date_time_field("min");
						date_time.second = get_date_time_field("sec");

						result = date_time;
					}
					else
					{
						if (name != "V")
						{
							result = static_cast < Candle::price_t > (m_state.to_number());
						}
						else
						{
							result = static_cast < Candle::volume_t > (m_state.to_integer());
						}
					}

					m_state.pop(3);

					return result;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			unsigned int Source::get_date_time_field(const std::string & key) const
			{
				RUN_LOGGER(logger);

				try
				{
					m_state.push_string(key);

					m_state.get_table();

					auto value = static_cast < unsigned int > (m_state.to_integer());

					m_state.pop();

					return value;
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

					return record_t(sout.str().c_str(), char_allocator_t(m_shared_memory.get_segment_manager()));
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

			Source::Candle::price_t Source::last_price() const
			{
				RUN_LOGGER(logger);

				try
				{
					return make_candle(max_size()).price_close;
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < source_exception > (logger, exception);
				}
			}

		} // namespace market

	} // namespace plugin

} // namespace solution