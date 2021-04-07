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

				m_market.set_config(m_config);

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
					while (true)
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

						std::cout << "Continue ? (y/n) "; // TODO

						char c;

						std::cin >> c;

						if (c != 'y')
						{
							break;
						}
					}
				}
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

		void System::handle_data(const boost::python::object & function) const
		{
			RUN_LOGGER(logger);

			try
			{
				const auto scale = m_config.prediction_timeframe;

				for (const auto & asset : m_market.assets())
				{
					handle_signal(boost::python::extract < std::string > (
						function(asset.c_str(), scale.c_str(), m_market.get_current_data(
							asset, scale, m_config.prediction_timesteps).c_str())));
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

		void System::handle_signal(const std::string & signal) const
		{
			RUN_LOGGER(logger);

			try
			{
				
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
				std::string asset;
				std::string scale;

				std::size_t size;

				std::cin >> asset >> scale >> size;

				std::cout << m_market.get_current_data(asset, scale, size);

				m_server_data->transactions.clear();

				std::string asset_code;
				std::string operation;
				std::string position;

				std::cin >> asset_code >> operation >> position;

				m_server_data->transactions.push_back({
					Server_Data::string_t(asset_code.c_str(), Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())),
					Server_Data::string_t(operation.c_str(),  Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())),
					Server_Data::string_t(position.c_str(),   Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())) });

				m_server_data->is_updated = true;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution