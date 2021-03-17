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
				// ...
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < system_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution