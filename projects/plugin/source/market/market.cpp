#include "market.hpp"

namespace solution
{
	namespace plugin
	{
		using Severity = shared::Logger::Severity;

		void Market::Data::load_config(Config & config)
		{
			RUN_LOGGER(logger);

			try
			{
				json_t raw_config;

				load(File::config_json, raw_config);

				config.id      = raw_config[Key::Config::id     ].get < std::string > ();
				config.code    = raw_config[Key::Config::code   ].get < std::string > ();
				config.account = raw_config[Key::Config::account].get < std::string > ();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::Data::load_assets(assets_container_t & assets)
		{
			RUN_LOGGER(logger);

			try
			{
				json_t array;

				load(File::assets_data, array);

				for (const auto & element : array)
				{
					auto class_code = element[Key::Asset::class_code].get < std::string > ();
					auto asset_code = element[Key::Asset::asset_code].get < std::string > ();

					assets.push_back(std::make_pair(std::move(class_code), std::move(asset_code)));
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::Data::load_scales(scales_container_t & scales)
		{
			RUN_LOGGER(logger);

			try
			{
				auto path = File::scales_data;

				std::fstream fin(path.string(), std::ios::in);

				if (!fin)
				{
					throw market_exception("cannot open file " + path.string());
				}

				std::string scale;

				while (std::getline(fin, scale))
				{
					scales.push_back(scale);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::Data::load(const path_t & path, json_t & object)
		{
			RUN_LOGGER(logger);

			try
			{
				std::fstream fin(path.string(), std::ios::in);

				if (!fin)
				{
					throw market_exception("cannot open file " + path.string());
				}

				object = json_t::parse(fin);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		/*
		void Market::Task::operator()() const
		{
			RUN_LOGGER(logger);

			try
			{
				while (m_status.load() == Status::running)
				{
					try
					{
						m_source->update();
					}
					catch (...)
					{
						++m_error_counter;

						std::this_thread::sleep_for(std::chrono::seconds(1));

						if (m_error_counter > critical_error_quantity)
						{
							throw market_exception("critical error quantity");
						}
						else
						{
							continue;
						}
					}

					m_error_counter = 0;

					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler(logger, exception);
			}
		}
		*/

		void Market::initialize()
		{
			RUN_LOGGER(logger);

			try
			{
				load();

				for (const auto & asset : m_assets)
				{
					for (const auto & scale : m_scales)
					{
						m_sources.push_back(std::make_shared < Source > (
							m_state, asset.first, asset.second, scale));
					}
				}

				m_status.store(Status::stopped);

				initialize_shared_memory();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::uninitialize()
		{
			RUN_LOGGER(logger);

			try
			{
				stop();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::load()
		{
			RUN_LOGGER(logger);

			try
			{
				load_config();
				load_assets();
				load_scales();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::load_config()
		{
			RUN_LOGGER(logger);

			try
			{
				Data::load_config(m_config);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::load_assets()
		{
			RUN_LOGGER(logger);

			try
			{
				Data::load_assets(m_assets);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::load_scales()
		{
			RUN_LOGGER(logger);

			try
			{
				Data::load_scales(m_scales);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::initialize_shared_memory()
		{
			RUN_LOGGER(logger);

			try
			{
				const auto shared_memory_name = make_shared_memory_name();

				const auto shared_memory_size = 65536U;

				boost::interprocess::shared_memory_object::remove(shared_memory_name.c_str());

				m_shared_memory = shared_memory_t(boost::interprocess::create_only,
					shared_memory_name.c_str(), shared_memory_size);

				Plugin_Data::holding_allocator_t holding_allocator(m_shared_memory.get_segment_manager());

				m_plugin_data = m_shared_memory.construct < Plugin_Data > (
					boost::interprocess::unique_instance) (holding_allocator);

				m_condition = m_shared_memory.construct < condition_t > (
					boost::interprocess::unique_instance) ();

				m_mutex = m_shared_memory.construct < mutex_t > (
					boost::interprocess::unique_instance) ();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		std::string Market::make_shared_memory_name() const
		{
			RUN_LOGGER(logger);

			try
			{
				return "QUIK";
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::run()
		{
			RUN_LOGGER(logger);

			try
			{
				m_status.store(Status::running);

				std::scoped_lock lock(m_market_mutex);

				while (m_status.load() == Status::running)
				{
					std::unique_lock lock(*m_mutex);

					m_condition->wait(lock); // request from server

					update_shared_memory();

					m_condition->notify_all(); // responce to server

					m_condition->wait(lock); // responce from server

					// TODO
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::stop()
		{
			RUN_LOGGER(logger);

			try
			{
				m_status.store(Status::stopped);

				std::scoped_lock lock(m_market_mutex);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::update_shared_memory() const
		{
			RUN_LOGGER(logger);

			try
			{
				m_plugin_data->available_money = get_available_money();

				auto holdings = get_holdings();

				for (const auto & [asset_code, position] : holdings)
				{
					m_plugin_data->holdings[Plugin_Data::string_t(asset_code.c_str())] = position;
				}

				m_plugin_data->is_updated = true;

				update_sources();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		double Market::get_available_money() const
		{
			RUN_LOGGER(logger);

			try
			{
				m_state.get_global("getPortfolioInfo");

				m_state.push_string(m_config.id);
				m_state.push_string(m_config.code);

				m_state.call(2);

				m_state.push_string("av_lim_all");

				m_state.get_table();

				auto result = std::stod(m_state.to_string());

				m_state.pop(2);

				return result;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		Market::holdings_container_t Market::get_holdings() const
		{
			RUN_LOGGER(logger);

			try
			{
				holdings_container_t holdings;

				m_state.get_global("getNumberOf");

				m_state.push_string("firm_holding");

				m_state.call(1);

				auto size = static_cast < std::size_t > (m_state.to_integer());

				for (auto i = 0U; i < size; ++i)
				{
					m_state.get_global("getItem");

					m_state.push_string("firm_holding");
					m_state.push_number(i);

					m_state.call(2);

					m_state.push_string("sec_code");

					m_state.get_table();

					auto asset = static_cast < std::string > (m_state.to_string());

					m_state.pop(2);

					m_state.get_global("getItem");

					m_state.push_string("firm_holding");
					m_state.push_number(i);

					m_state.call(2);

					m_state.push_string("currentpos");

					m_state.get_table();

					auto position = static_cast < double > (m_state.to_number());

					m_state.pop(2);

					holdings[asset] = position;
				}

				m_state.pop();

				return holdings;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::update_sources() const
		{
			RUN_LOGGER(logger);

			try
			{
				for (const auto & source : m_sources)
				{
					try
					{
						source->update();
					}
					catch (...)
					{
						logger.write(Severity::error, source->asset_code() +
							":" + source->scale_code() + "source update failed");
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		std::size_t Market::get_lot_size(const std::string & class_code, const std::string & asset_code) const
		{
			RUN_LOGGER(logger);

			try
			{
				m_state.get_global("getSecurityInfo");

				m_state.push_string(class_code);
				m_state.push_string(asset_code);

				m_state.call(2);

				m_state.push_string("lot_size");

				m_state.get_table();

				auto result = static_cast <std::size_t> (m_state.to_integer());

				m_state.pop(2);

				return result;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		bool Market::is_connected() const
		{
			RUN_LOGGER(logger);

			try
			{
				m_state.get_global("isConnected");
				m_state.call(0);

				auto result = m_state.to_boolean();

				m_state.pop();

				return result;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::send_message(const std::string & message) const
		{
			RUN_LOGGER(logger);

			try
			{
				m_state.get_global("message");
				m_state.push_string(message);
				m_state.call(1);

				auto result = m_state.to_boolean();

				m_state.pop();

				if (!result)
				{
					throw std::runtime_error("bad message");
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		std::string Market::send_transaction(const transaction_t & transaction) const
		{
			RUN_LOGGER(logger);

			try
			{
				m_state.get_global("sendTransaction");

				m_state.new_table();

				for (const auto & [field, value] : transaction)
				{
					m_state.push_string(field);
					m_state.push_string(value);

					m_state.set_table();
				}

				m_state.call(1);

				auto result = m_state.to_string();

				m_state.pop();

				return result;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

	} // namespace plugin

} // namespace solution