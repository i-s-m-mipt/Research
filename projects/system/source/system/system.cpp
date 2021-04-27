#include "system.hpp"

namespace solution
{
	namespace system
	{
		using Severity = shared::Logger::Severity;

		void System::Data::load_config(Config & config)
		{
			RUN_LOGGER(logger);

			try
			{
				json_t raw_config;

				load(File::config_json, raw_config);

				config.required_charts               = raw_config[Key::Config::required_charts              ].get < bool > ();
				config.required_self_similarities    = raw_config[Key::Config::required_self_similarities   ].get < bool > ();
				config.required_pair_similarities    = raw_config[Key::Config::required_pair_similarities   ].get < bool > ();
				config.required_pair_correlations    = raw_config[Key::Config::required_pair_correlations   ].get < bool > ();
				config.self_similarity_DTW_delta     = raw_config[Key::Config::self_similarity_DTW_delta    ].get < int > ();
				config.cumulative_distances_asset    = raw_config[Key::Config::cumulative_distances_asset   ].get < std::string > ();
				config.cumulative_distances_scale_1  = raw_config[Key::Config::cumulative_distances_scale_1 ].get < std::string > ();
				config.cumulative_distances_scale_2  = raw_config[Key::Config::cumulative_distances_scale_2 ].get < std::string > ();
				config.required_deviations           = raw_config[Key::Config::required_deviations          ].get < bool > ();
				config.required_tagged_charts        = raw_config[Key::Config::required_tagged_charts       ].get < bool > ();
				config.min_price_change              = raw_config[Key::Config::min_price_change             ].get < double > ();
				config.max_price_rollback            = raw_config[Key::Config::max_price_rollback           ].get < double > ();
				config.level_max_deviation           = raw_config[Key::Config::level_max_deviation          ].get < double > ();
				config.level_max_lifetime            = raw_config[Key::Config::level_max_lifetime           ].get < double > ();
				config.level_max_strength            = raw_config[Key::Config::level_max_strength           ].get < double > ();
				config.level_resolution              = raw_config[Key::Config::level_resolution             ].get < std::string > ();
				config.level_frame                   = raw_config[Key::Config::level_frame                  ].get < std::size_t > ();
				config.required_quik                 = raw_config[Key::Config::required_quik                ].get < bool > ();
				config.required_supports_resistances = raw_config[Key::Config::required_supports_resistances].get < bool > ();
				config.classification_max_deviation  = raw_config[Key::Config::classification_max_deviation ].get < double > ();
				config.run_julia_test                = raw_config[Key::Config::run_julia_test               ].get < bool > ();
				config.prediction_timeframe          = raw_config[Key::Config::prediction_timeframe         ].get < std::string > ();
				config.prediction_timesteps          = raw_config[Key::Config::prediction_timesteps         ].get < std::size_t > ();
				config.transaction_base_value        = raw_config[Key::Config::transaction_base_value       ].get < double > ();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::load(const path_t & path, json_t & object)
		{
			RUN_LOGGER(logger);

			try
			{
				std::fstream fin(path.string(), std::ios::in);

				if (!fin)
				{
					throw system_exception("cannot open file " + path.string());
				}

				object = json_t::parse(fin);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::Data::save(const path_t & path, const json_t & object)
		{
			RUN_LOGGER(logger);

			try
			{
				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw system_exception("cannot open file " + path.string());
				}

				fout << std::setw(4) << object;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::initialize()
		{
			RUN_LOGGER(logger);

			try
			{
				load();

				m_market = std::make_unique < Market > (m_config);

				if (m_config.required_quik)
				{
					initialize_shared_memory();
				}

				if (m_config.run_julia_test)
				{
					run_julia_test();
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::uninitialize()
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

		void System::load()
		{
			RUN_LOGGER(logger);

			try
			{
				load_config();

				load_dividends();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::load_config()
		{
			RUN_LOGGER(logger);

			try
			{
				Data::load_config(m_config);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		std::time_t to_time(const std::string & date)
		{
			RUN_LOGGER(logger);

			try
			{
				std::tm tm = { 0 }; // !

				std::stringstream sin(date);

				sin >> std::get_time(&tm, "%d.%m.%Y");

				return mktime(&tm);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::load_dividends()
		{
			RUN_LOGGER(logger);

			try
			{
				shared::Python python;

				boost::python::exec("from system import get_dividends", python.global(), python.global());

				std::stringstream sin(boost::python::extract < std::string > (python.global()["get_dividends"]()));

				auto array = json_t::parse(sin);

				for (const auto & object : array)
				{
					auto asset    = object[Dividend::Key::asset   ].get < std::string > ();

					auto dividend = object[Dividend::Key::dividend].get < double > ();

					auto buy_date = to_time(object[Dividend::Key::buy_date].get < std::string > ());
					auto gap_date = to_time(object[Dividend::Key::gap_date].get < std::string > ());

					m_dividends[asset] = { dividend, buy_date, gap_date };
				}
			}
			catch (const boost::python::error_already_set &)
			{
				logger.write(Severity::error, shared::Python::exception());
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::initialize_shared_memory()
		{
			RUN_LOGGER(logger);

			try
			{
				const auto shared_memory_name = make_shared_memory_name();

				m_shared_memory = shared_memory_t(boost::interprocess::open_only, shared_memory_name.c_str());

				m_plugin_data = m_shared_memory.find < Plugin_Data > ("plugin_data").first;
				m_server_data = m_shared_memory.find < Server_Data > ("server_data").first;

				m_plugin_mutex = m_shared_memory.find < mutex_t > ("plugin_mutex").first;
				m_server_mutex = m_shared_memory.find < mutex_t > ("server_mutex").first;

				m_plugin_condition = m_shared_memory.find < condition_t > ("plugin_condition").first;
				m_server_condition = m_shared_memory.find < condition_t > ("server_condition").first;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		std::string System::make_shared_memory_name() const
		{
			RUN_LOGGER(logger);

			try
			{
				return "QUIK";
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::run_julia_test() const
		{
			RUN_LOGGER(logger);

			try
			{
				std::cout << "Running julia test ... \n" << std::endl;

				jl_init();

				auto result = jl_eval_string("sqrt(2.0)");

				if (jl_typeis(result, jl_float64_type))
				{
					std::cout << std::setprecision(6) << std::fixed << jl_unbox_float64(result) << std::endl;
				}
				else 
				{
					throw std::runtime_error("unexpected return type from sqrt(::Float64)");
				}

				auto a = jl_box_float64(3.0);
				auto b = jl_box_float32(3.0f);
				auto c = jl_box_int32(3);

				auto function = jl_get_function(jl_base_module, "sqrt");
				auto argument = jl_box_float64(2.0);

				result = jl_call1(function, argument);

				auto array_type = jl_apply_array_type((jl_value_t * )jl_float64_type, 1);

				auto array = new double[10];

				auto x = jl_ptr_to_array_1d(array_type, array, 10, 0);

				auto x_data = (double *)(jl_array_data(x));

				for (auto i = 0U; i < jl_array_len(x); ++i)
				{
					x_data[i] = 0.42;
				}

				function = jl_get_function(jl_base_module, "reverse!");

				jl_call1(function, (jl_value_t *)x);

				function  = jl_get_function(jl_base_module, "reverse");

				jl_array_t * y = (jl_array_t *)jl_call1(function, (jl_value_t *)x);

				jl_atexit_hook(0);

				jl_eval_string("this_function_does_not_exist()");

				if (jl_exception_occurred())
				{
					std::cout << jl_typeof_str(jl_exception_occurred()) << std::endl;
				}

				std::cout << std::endl << "julia test passed\n" << std::endl;
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
				shared::Python python;

				boost::python::exec("from system import predict", python.global(), python.global());

				boost::python::object function = python.global()["predict"];

				if (m_config.required_quik)
				{
					{
						boost::interprocess::scoped_lock plugin_lock(*m_plugin_mutex);

						m_plugin_condition->wait(plugin_lock, [this]() { return m_plugin_data->is_updated; });

						get_plugin_data();
					}

					handle_data(function);

					{
						boost::interprocess::scoped_lock server_lock(*m_server_mutex);

						set_server_data();

						m_server_condition->notify_one();
					}
				}
			}
			catch (const boost::python::error_already_set &)
			{
				logger.write(Severity::error, shared::Python::exception());
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::get_plugin_data()
		{
			RUN_LOGGER(logger);

			try
			{
				m_available_money = m_plugin_data->available_money;

				m_holdings.clear();

				std::cout << "Current positions: " << std::endl << std::endl;

				if (m_plugin_data->holdings.empty())
				{
					std::cout << "-" << std::endl;
				}
				else
				{
					for (const auto & [asset, position] : m_plugin_data->holdings)
					{
						m_holdings.emplace(asset.c_str(), position);

						std::cout <<
							std::setw(5) << std::left << std::setfill(' ') << asset << " " <<
							std::setw(10) << std::right << std::setprecision(2) << std::fixed << std::showpos <<
							position << std::endl;
					}
				}				

				std::cout << std::endl;

				m_plugin_data->is_updated = false;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::handle_data(const boost::python::object & function)
		{
			RUN_LOGGER(logger);

			try
			{
				const auto scale = m_config.prediction_timeframe;

				m_transactions.clear();

				for (const auto & asset : m_market->assets())
				{
					handle_state(asset, boost::python::extract < std::string > (
						function(asset.c_str(), scale.c_str(), m_market->get_current_data(
							asset, scale, m_config.prediction_timesteps).c_str())));
				}
			}
			catch (const boost::python::error_already_set &)
			{
				logger.write(Severity::error, shared::Python::exception());
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::handle_state(const std::string & asset, const std::string & state)
		{
			RUN_LOGGER(logger);

			try
			{
				if (m_handled_assets.find(asset) != std::end(m_handled_assets)) // ?
				{
					return;
				}

				auto current_position = 0.0;

				if (auto iterator = m_holdings.find(asset); iterator != std::end(m_holdings))
				{
					current_position = iterator->second;
				}

				auto has_dividends_flag = has_dividends(asset);

				if ((state == State::C || has_dividends_flag) && current_position < 0.0)
				{
					insert_transaction(asset, "B", std::abs(current_position));
				}

				if (state == State::C && current_position > 0.0)
				{
					insert_transaction(asset, "S", std::abs(current_position));
				}

				if (state == State::L && current_position <= 0.0)
				{
					insert_transaction(asset, "B", std::abs(current_position) + m_config.transaction_base_value);
				}

				if (state == State::S && current_position >= 0.0 && !has_dividends_flag)
				{
					insert_transaction(asset, "S", std::abs(current_position) + m_config.transaction_base_value);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		bool System::has_dividends(const std::string & asset) const
		{
			RUN_LOGGER(logger);

			try
			{
				if (auto iterator = m_dividends.find(asset); iterator != std::end(m_dividends))
				{
					auto time = iterator->second.gap_date - 
						std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

					if (time > 0LL && time < seconds_in_day * 10LL)
					{
						std::cout << std::setw(5) << std::left << std::setfill(' ') <<
							asset << " has dividends soon.\n";

						return true;
					}
				}
					
				return false;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::insert_transaction(const std::string & asset, const std::string & operation, double position)
		{
			RUN_LOGGER(logger);

			try
			{
				m_transactions.push_back({ asset, operation, position });

				m_handled_assets[asset] = clock_t::now();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::set_server_data() const
		{
			RUN_LOGGER(logger);

			try
			{
				m_server_data->transactions.clear();

				std::cout << std::endl << "Required operations: " << std::endl << std::endl;

				for (const auto & transaction : m_transactions)
				{
					m_server_data->transactions.push_back({
						Server_Data::string_t(transaction.asset.c_str(),      
							Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())),
						Server_Data::string_t(transaction.operation.c_str(),  
							Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())),
						Server_Data::string_t(std::to_string(transaction.position).c_str(),   
							Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())) });

					std::cout << std::setw(5) << std::left << std::setfill(' ') << transaction.asset << " " << transaction.operation << " " << 
						std::setw(9) << std::right << std::setprecision(2) << std::fixed << std::noshowpos << transaction.position << std::endl;
				}

				std::cout << std::endl << "Accept? (y/n) ";

				char c;

				std::cin >> c;

				if (c != 'y')
				{
					m_server_data->transactions.clear();
				}
				
				m_server_data->is_updated = true;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution