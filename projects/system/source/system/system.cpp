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
				config.required_price_deviations     = raw_config[Key::Config::required_price_deviations    ].get < bool > ();
				config.required_tagged_charts        = raw_config[Key::Config::required_tagged_charts       ].get < bool > ();
				config.required_environment          = raw_config[Key::Config::required_environment         ].get < bool > ();
				config.required_local_environment    = raw_config[Key::Config::required_local_environment   ].get < bool > ();
				config.min_price_change              = raw_config[Key::Config::min_price_change             ].get < double > ();
				config.max_price_rollback            = raw_config[Key::Config::max_price_rollback           ].get < double > ();
				config.level_max_deviation           = raw_config[Key::Config::level_max_deviation          ].get < double > ();
				config.level_max_lifetime            = raw_config[Key::Config::level_max_lifetime           ].get < double > ();
				config.level_max_strength            = raw_config[Key::Config::level_max_strength           ].get < double > ();
				config.level_resolution              = raw_config[Key::Config::level_resolution             ].get < std::string > ();
				config.level_min_bias                = raw_config[Key::Config::level_min_bias               ].get < std::size_t > ();
				config.level_max_bias                = raw_config[Key::Config::level_max_bias               ].get < std::size_t > ();
				config.critical_deviation            = raw_config[Key::Config::critical_deviation           ].get < double > ();
				config.required_level_reduction      = raw_config[Key::Config::required_level_reduction     ].get < bool > ();
				config.required_quik                 = raw_config[Key::Config::required_quik                ].get < bool > ();
				config.required_supports_resistances = raw_config[Key::Config::required_supports_resistances].get < bool > ();
				config.required_consultation_mode    = raw_config[Key::Config::required_consultation_mode   ].get < bool > ();
				config.classification_max_deviation  = raw_config[Key::Config::classification_max_deviation ].get < double > ();
				config.run_julia_test                = raw_config[Key::Config::run_julia_test               ].get < bool > ();
				config.prediction_timeframe          = raw_config[Key::Config::prediction_timeframe         ].get < std::string > ();
				config.prediction_timesteps          = raw_config[Key::Config::prediction_timesteps         ].get < std::size_t > ();
				config.transaction_base_value        = raw_config[Key::Config::transaction_base_value       ].get < double > ();
				config.days_for_dividends            = raw_config[Key::Config::days_for_dividends           ].get < std::time_t > ();
				config.deviation_threshold           = raw_config[Key::Config::deviation_threshold          ].get < double > ();
				config.run_model_sensibility_test    = raw_config[Key::Config::run_model_sensibility_test   ].get < bool > ();
				config.model_stabilization_time      = raw_config[Key::Config::model_stabilization_time     ].get < std::time_t > ();
				config.run_fridays_test              = raw_config[Key::Config::run_fridays_test             ].get < bool > ();
				config.required_sentiment_service    = raw_config[Key::Config::required_sentiment_service   ].get < bool > ();
				config.telegram_username             = raw_config[Key::Config::telegram_username            ].get < std::string > ();
				config.telegram_api_id               = raw_config[Key::Config::telegram_api_id              ].get < std::string > ();
				config.telegram_api_hash             = raw_config[Key::Config::telegram_api_hash            ].get < std::string > ();
				config.volume_timesteps              = raw_config[Key::Config::volume_timesteps             ].get < std::size_t > ();
				config.run_mornings_test             = raw_config[Key::Config::run_mornings_test            ].get < bool > ();
				config.mornings_test_asset           = raw_config[Key::Config::mornings_test_asset          ].get < std::string > ();
				config.mornings_test_scale           = raw_config[Key::Config::mornings_test_scale          ].get < std::string > ();
				config.mornings_test_delta           = raw_config[Key::Config::mornings_test_delta          ].get < double > ();
				config.mornings_test_year            = raw_config[Key::Config::mornings_test_year           ].get < unsigned int > ();
				config.mornings_test_confidence      = raw_config[Key::Config::mornings_test_confidence     ].get < double > ();
				config.run_intraday_test             = raw_config[Key::Config::run_intraday_test            ].get < bool > ();
				config.intraday_test_deviation       = raw_config[Key::Config::intraday_test_deviation      ].get < double > ();
				config.intraday_test_asset           = raw_config[Key::Config::intraday_test_asset          ].get < std::string > ();
				config.intraday_test_scale           = raw_config[Key::Config::intraday_test_scale          ].get < std::string > ();
				config.intraday_test_year            = raw_config[Key::Config::intraday_test_year           ].get < unsigned int > ();
				config.local_environment_test_asset  = raw_config[Key::Config::local_environment_test_asset ].get < std::string > ();
				config.local_environment_test_scale  = raw_config[Key::Config::local_environment_test_scale ].get < std::string > ();
				config.run_local_environment_test    = raw_config[Key::Config::run_local_environment_test   ].get < bool > ();
				config.knn_method_parameter          = raw_config[Key::Config::knn_method_parameter         ].get < std::size_t > ();
				config.geometric_progression_q       = raw_config[Key::Config::geometric_progression_q      ].get < double > ();
				config.movement_timesteps            = raw_config[Key::Config::movement_timesteps           ].get < std::time_t > ();
				config.knn_method_timesteps          = raw_config[Key::Config::knn_method_timesteps         ].get < std::size_t > ();
				config.max_waves_sequence            = raw_config[Key::Config::max_waves_sequence           ].get < std::size_t > ();
				config.local_environment_test_start  = raw_config[Key::Config::local_environment_test_start ].get < std::size_t > ();
				config.max_wave_length               = raw_config[Key::Config::max_wave_length              ].get < std::time_t > ();
				config.required_test_data            = raw_config[Key::Config::required_test_data           ].get < bool > ();
				config.test_data_start               = raw_config[Key::Config::test_data_start              ].get < std::size_t > ();
				config.skipped_timesteps             = raw_config[Key::Config::skipped_timesteps            ].get < std::size_t > ();
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

				std::this_thread::sleep_for(std::chrono::seconds(initialization_delay));

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
				if (m_config.required_quik)
				{
					if (m_config.required_consultation_mode)
					{
						run_consulting();
					}
					else
					{
						run_autonomous();
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::run_autonomous()
		{
			RUN_LOGGER(logger);

			try
			{
				shared::Python python;

				boost::python::exec("from system import predict", python.global(), python.global());

				boost::python::object function = python.global()["predict"];

				if (m_config.run_model_sensibility_test)
				{
					run_model_sensibility_test(function);
				}

				while (is_session_open())
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

					std::this_thread::sleep_for(std::chrono::seconds(60));
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

		void System::run_consulting()
		{
			RUN_LOGGER(logger);

			try
			{
				if (m_config.required_sentiment_service)
				{
					run_sentiment_service();
				}

				shared::Python python;

				boost::python::exec("from system import predict", python.global(), python.global());

				boost::python::object function = python.global()["predict"];

				if (m_config.run_model_sensibility_test)
				{
					run_model_sensibility_test(function);
				}

				const auto scale = m_config.prediction_timeframe;

				char c = 'y';

				while (c == 'y')
				{
					{
						boost::interprocess::scoped_lock plugin_lock(*m_plugin_mutex);

						m_plugin_condition->wait(plugin_lock, [this]() { return m_plugin_data->is_updated; });

						get_plugin_data();
					}

					for (const auto & asset : m_market->assets())
					{
						auto data = m_market->get_current_data(
							asset, scale, m_config.prediction_timesteps);

						auto state = boost::python::extract < std::string > (
							function(asset.c_str(), scale.c_str(), data.c_str()))();

						auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

						std::cout << std::put_time(std::localtime(&time), "%y.%m.%d %H:%M:%S") << " : ";

						std::cout << std::setw(4) << std::left << std::setfill(' ') << 
							asset << " prediction: " << state << std::endl;
					}

					std::this_thread::sleep_for(std::chrono::seconds(1));

					std::cout << std::endl << "Continue? (y/n) ";

					std::cin >> c;

					std::cout << std::endl;
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

		void System::run_model_sensibility_test(const boost::python::object & function) const
		{
			RUN_LOGGER(logger);

			try
			{
				const auto scale = m_config.prediction_timeframe;

				std::cout << "Model sensibility test: " << std::endl << std::endl;

				std::array < std::size_t, 12U > delimeters = 
					{ 0U, 10U, 20U, 30U, 40U, 50U, 51U, 61U, 71U, 81U, 91U, 101U };

				for (const auto & asset : m_market->assets())
				{
					auto data = m_market->get_current_data_variation(
						asset, scale, m_config.prediction_timesteps);

					std::cout << std::setw(5) << std::left << std::setfill(' ') << asset << " : ";

					auto i = 0U;

					for (const auto & variant : data)
					{
						if (std::find(std::begin(delimeters), std::end(delimeters), i++) != std::end(delimeters))
						{
							std::cout << ' ';
						}

						std::cout << boost::python::extract < std::string > (
							function(asset.c_str(), scale.c_str(), variant.c_str()))()[0];
					}

					std::cout << std::endl;
				}

				std::cout << std::endl;
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

		void System::run_sentiment_service() const
		{
			RUN_LOGGER(logger);

			try
			{
				shared::Python python;

				boost::python::exec("from system import estimate_sentiments", python.global(), python.global());

				boost::python::object function = python.global()["estimate_sentiments"];

				const std::vector < std::vector < std::string > > asset_classes = { 
					{ "BR" }, 
					{ "SBER", "GAZP", "GMKN", "LKOH", "ROSN", "SIBN", "SNGS", "TATN" } };

				for (const auto & asset_class : asset_classes)
				{
					for (const auto & asset : asset_class)
					{
						auto sentiments = boost::python::extract < std::string > (function(asset.c_str(),
							m_config.telegram_username.c_str(), m_config.telegram_api_id.c_str(),
							m_config.telegram_api_hash.c_str()))();

						auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

						std::cout << std::put_time(std::localtime(&time), "%y.%m.%d %H:%M:%S") << " : ";

						std::cout << std::setw(4) << std::right << std::setfill(' ') <<
							asset << " sentiments: " << sentiments << std::endl;
					}

					std::cout << std::endl;
				}

				const std::filesystem::path path = "messages.json";

				std::filesystem::remove(path);
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

		bool System::is_session_open() const
		{
			RUN_LOGGER(logger);

			try
			{
				auto time = std::time(nullptr);

				auto tm = *std::localtime(&time);

				return (
					((tm.tm_hour > 10) || (tm.tm_hour == 10 && tm.tm_min > 0)) &&
					((tm.tm_hour < 23) || (tm.tm_hour == 23 && tm.tm_min < 50)));
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

				for (const auto & [asset, position] : m_plugin_data->holdings)
				{
					m_holdings.emplace(asset.c_str(), position);
				}

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
					auto data = m_market->get_current_data(
						asset, scale, m_config.prediction_timesteps);

					auto state = boost::python::extract < std::string > (
						function(asset.c_str(), scale.c_str(), data.c_str()))();

					if (is_state_stable(asset, state))
					{
						handle_state(asset, state);
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

		bool System::is_state_stable(const std::string & asset, const std::string & state)
		{
			RUN_LOGGER(logger);

			try
			{
				auto current_state = std::make_pair(state, clock_t::now());

				if (m_states.find(asset) == std::end(m_states) || m_states[asset].first != state)
				{
					m_states[asset] = std::move(current_state);

					return false;
				}
				else
				{
					auto delta = std::chrono::duration_cast < std::chrono::seconds > (
						current_state.second - m_states[asset].second).count();

					if (delta >= m_config.model_stabilization_time)
					{
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

		void System::handle_state(const std::string & asset, const std::string & state)
		{
			RUN_LOGGER(logger);

			try
			{
				auto current_position = 0.0;

				if (auto iterator = m_holdings.find(asset); iterator != std::end(m_holdings))
				{
					current_position = iterator->second;
				}

				auto has_dividends_flag = has_dividends(asset);

				if (state == State::C && current_position < 0.0)
				{
					insert_transaction(asset, "B", std::abs(current_position));
				}
				else if (state == State::C && current_position > 0.0)
				{
					insert_transaction(asset, "S", std::abs(current_position));
				}
				else if (state == State::L && current_position <= 0.0)
				{
					if (current_position != 0.0)
					{
						insert_transaction(asset, "B", std::abs(current_position));
					}

					insert_transaction(asset, "B", m_config.transaction_base_value);
				}
				else if (state == State::S && current_position >= 0.0 && !has_dividends_flag)
				{
					if (current_position != 0.0)
					{
						insert_transaction(asset, "S", std::abs(current_position));
					}

					insert_transaction(asset, "S", m_config.transaction_base_value);
				}
				else if (has_dividends_flag && current_position < 0.0)
				{
					insert_transaction(asset, "B", std::abs(current_position));
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

					if (time > 0LL && time < seconds_in_day * m_config.days_for_dividends)
					{
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

				for (const auto & transaction : m_transactions)
				{
					auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

					std::cout << std::put_time(std::localtime(&time), "%y.%m.%d %H:%M:%S") << " : ";

					std::cout << std::setw(5) << std::left << std::setfill(' ') << transaction.asset << " " << transaction.operation << " " <<
						std::setw(9) << std::right << std::setprecision(2) << std::fixed << std::noshowpos << transaction.position << std::endl;

					m_server_data->transactions.push_back({
						Server_Data::string_t(transaction.asset.c_str(),
							Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())),
						Server_Data::string_t(transaction.operation.c_str(),
							Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())),
						Server_Data::string_t(std::to_string(transaction.position).c_str(),
							Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())) });
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