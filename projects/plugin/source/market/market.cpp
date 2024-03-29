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

				config.id       = raw_config[Key::Config::id      ].get < std::string > ();
				config.code     = raw_config[Key::Config::code    ].get < std::string > ();
				config.account  = raw_config[Key::Config::account ].get < std::string > ();
				config.login    = raw_config[Key::Config::login   ].get < std::string > ();
				config.password = raw_config[Key::Config::password].get < std::string > ();
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

		void Market::initialize()
		{
			RUN_LOGGER(logger);

			try
			{
				load();

				login();

				m_status.store(Status::stopped);

				while (!is_session_open())
				{
					std::this_thread::yield();
				}

				initialize_sources();

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

				m_shared_memory.destroy_ptr(m_plugin_data);
				m_shared_memory.destroy_ptr(m_server_data);

				m_shared_memory.destroy_ptr(m_plugin_mutex);
				m_shared_memory.destroy_ptr(m_server_mutex);

				m_shared_memory.destroy_ptr(m_plugin_condition);
				m_shared_memory.destroy_ptr(m_server_condition);

				const auto shared_memory_name = make_shared_memory_name();

				boost::interprocess::shared_memory_object::remove(shared_memory_name.c_str());
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

		std::wstring convert_string_to_wstring(
			const std::string & string,
			const std::locale & locale = std::locale())
		{
			std::vector < wchar_t > buffer(string.size());

			std::use_facet < std::ctype < wchar_t > > (locale).widen(
				string.data(),
				string.data() + string.size(),
				buffer.data());

			return std::wstring(buffer.data(), buffer.size());
		}
 
		void Market::login() const
		{
			RUN_LOGGER(logger);

			try
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));

				auto handler_main_window = FindWindow(L"InfoClass", 0);

				if (handler_main_window)
				{
					PostMessage(handler_main_window, WM_COMMAND, 100, 0);
				}
				else
				{
					send_message("main window not found");
				}

				std::this_thread::sleep_for(std::chrono::seconds(1));

				auto handler_window = FindWindow(L"#32770", L"User identification");

				if (handler_window)
				{
					auto handler_server   = FindWindowEx(handler_window, 0,              L"ComboBox", L"");
					auto handler_login    = FindWindowEx(handler_window, handler_server, L"Edit",     L"");
					auto handler_password = FindWindowEx(handler_window, handler_login,  L"Edit",     L"");

					auto handler_button   = FindWindowEx(handler_window, handler_password, L"Button", L"Enter");

					SendMessage(handler_password, WM_SETTEXT, 0, (LPARAM)
						(convert_string_to_wstring(m_config.password).c_str()));

					std::this_thread::sleep_for(std::chrono::seconds(1));

					SetFocus(handler_button);

					PostMessage(handler_button, BM_CLICK, 0, 0);
				}
				else
				{
					send_message("window not found");
				}

				std::this_thread::sleep_for(std::chrono::seconds(10));
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::initialize_sources()
		{
			RUN_LOGGER(logger);

			try
			{
				for (const auto & [class_code, asset_code] : m_assets)
				{
					for (const auto & scale : m_scales)
					{
						m_sources[asset_code][scale] = std::make_shared < Source > (
							m_state, class_code, asset_code, scale);
					}
				}
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

				m_plugin_data = m_shared_memory.construct < Plugin_Data > ("plugin_data") 
					(Plugin_Data::holding_allocator_t(m_shared_memory.get_segment_manager()));

				m_server_data = m_shared_memory.construct < Server_Data > ("server_data")
					(Server_Data::transaction_allocator_t(m_shared_memory.get_segment_manager()));

				m_plugin_mutex = m_shared_memory.construct < mutex_t > ("plugin_mutex") ();
				m_server_mutex = m_shared_memory.construct < mutex_t > ("server_mutex") ();

				m_plugin_condition = m_shared_memory.construct < condition_t > ("plugin_condition") ();
				m_server_condition = m_shared_memory.construct < condition_t > ("server_condition") ();
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
				run_server();

				m_status.store(Status::running);

				std::scoped_lock market_lock(m_market_mutex);

				while (is_session_open())
				{
					{
						boost::interprocess::scoped_lock plugin_lock(*m_plugin_mutex);

						set_plugin_data();

						m_plugin_condition->notify_one();
					}

					{
						boost::interprocess::scoped_lock server_lock(*m_server_mutex);

						/*
						m_server_condition->wait(server_lock, [this]()
							{ return ((m_status.load() != Status::running) || m_server_data->is_updated); });

						if (m_status.load() != Status::running)
						{
							break;
						}
						*/

						get_server_data();
					}
				}

				while (m_status.load() == Status::running)
				{
					std::this_thread::yield();
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

				m_plugin_condition->notify_all();
				m_server_condition->notify_all();

				std::scoped_lock market_lock(m_market_mutex);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::run_server()
		{
			RUN_LOGGER(logger);

			try
			{
				m_server = boost::process::child("system.exe");
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		bool Market::is_session_open() const
		{
			RUN_LOGGER(logger);

			try
			{
				auto time = std::time(nullptr);

				auto tm = *std::localtime(&time);

				return (
					((tm.tm_hour > 10) || (tm.tm_hour == 10 && tm.tm_min > 0)) && 
					((tm.tm_hour < 23) || (tm.tm_hour == 23 && tm.tm_min < 55)));
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::set_plugin_data() const
		{
			RUN_LOGGER(logger);

			try
			{
				m_plugin_data->available_money = get_available_money();

				auto holdings = get_holdings();

				m_plugin_data->holdings.clear();

				m_plugin_data->holdings.reserve(std::size(holdings));

				for (const auto & [asset_code, position] : holdings)
				{
					m_plugin_data->holdings.emplace_back(Plugin_Data::string_t(asset_code.c_str(), 
						Plugin_Data::char_allocator_t(m_shared_memory.get_segment_manager())), position);
				}

				update_sources();

				m_plugin_data->is_updated = true;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::get_server_data() const
		{
			RUN_LOGGER(logger);

			try
			{
				for (const auto & raw_transaction : m_server_data->transactions)
				{
					try
					{
						transaction_t transaction;

						auto class_code = "TQBR";

						auto asset_code = raw_transaction.asset_code.c_str();
						auto operation  = raw_transaction.operation.c_str();

						auto position   = std::stod(raw_transaction.position.c_str());

						// transaction["FIRM_ID"    ] = m_config.id;

						transaction["CLIENT_CODE"] = m_config.code + '/'; // !
						transaction["ACCOUNT"    ] = m_config.account;
						transaction["TRANS_ID"   ] = std::to_string(generate_transaction_id());
						transaction["ACTION"     ] = "NEW_ORDER";
						transaction["TYPE"       ] = "M";
						transaction["PRICE"      ] = "0";
						transaction["CLASSCODE"  ] = class_code;
						transaction["SECCODE"    ] = asset_code;
						transaction["OPERATION"  ] = operation;
						transaction["QUANTITY"   ] = std::to_string(compute_lot_quantity(asset_code, position));

						auto message = send_transaction(transaction);

						if (!message.empty())
						{
							send_message(message);
						}

						std::this_thread::sleep_for(std::chrono::milliseconds(1));
					}
					catch (const std::exception & exception)
					{
						logger.write(Severity::error, exception.what());
					}
				}

				m_server_data->transactions.clear();

				m_server_data->is_updated = false;
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

				m_state.push_string("assets");

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

				for (const auto & [class_code, asset_code] : m_assets)
				{
					if (class_code == "TQBR")
					{
						m_state.get_global("getDepoEx");

						m_state.push_string(m_config.id);
						m_state.push_string(m_config.code);
						m_state.push_string(asset_code);
						m_state.push_string(m_config.account);
						m_state.push_number(2);

						m_state.call(5);

						if (!m_state.is_nil())
						{
							m_state.push_string("currentbal");

							m_state.get_table();

							auto balance = static_cast < int > (m_state.to_number());

							if (balance != 0)
							{
								holdings[asset_code] = balance * std::begin(m_sources.at(asset_code))->second->last_price();
							}

							m_state.pop();
						}

						m_state.pop();
					}
				}

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
				for (const auto & [asset_code, scales] : m_sources)
				{
					for (const auto & [scale, source] : scales)
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
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		unsigned long Market::generate_transaction_id() const
		{
			RUN_LOGGER(logger);

			try
			{
				std::uniform_int_distribution < unsigned long > distribution;

				return distribution(m_engine);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		std::size_t Market::compute_lot_quantity(const std::string & asset_code, double position) const
		{
			RUN_LOGGER(logger);

			try
			{
				auto source = std::begin(m_sources.at(asset_code))->second;

				return (static_cast < std::size_t > (std::round(position / (source->last_price() *
					get_lot_size(source->class_code(), source->asset_code())))));
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