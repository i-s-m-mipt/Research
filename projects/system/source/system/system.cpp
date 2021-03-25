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

				config.run_julia_test = raw_config[Key::Config::run_julia_test].get < bool > ();
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

				initialize_shared_memory();

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
				while (true)
				{
					{
						boost::interprocess::scoped_lock plugin_lock(*m_plugin_mutex);

						m_plugin_condition->wait(plugin_lock, [this]() { return m_plugin_data->is_updated; });

						get_plugin_data();
					}

					{
						boost::interprocess::scoped_lock server_lock(*m_server_mutex);

						set_server_data();

						m_server_condition->notify_one();
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

		void System::set_server_data() const
		{
			RUN_LOGGER(logger);

			try
			{
				m_server_data->transactions.clear(); // TODO

				std::string asset_code;
				std::string operation;
				std::string position;

				std::cin >> asset_code >> operation >> position;

				m_server_data->transactions.push_back({
					Server_Data::string_t(asset_code.c_str(),
						Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())),
					Server_Data::string_t(operation.c_str(),
						Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())),
					Server_Data::string_t(position.c_str(),
						Server_Data::char_allocator_t(m_shared_memory.get_segment_manager())) });

				m_server_data->is_updated = true;

				std::string asset;
				std::string scale;

				std::cin >> asset >> scale;

				std::cout << m_market.get_current_chart(asset, scale, 10U) << std::endl;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution