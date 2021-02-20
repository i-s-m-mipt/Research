#include "market.hpp"

namespace solution
{
	namespace system
	{
		using Severity = shared::Logger::Severity;

		void Market::Data::load_assets(assets_container_t & assets)
		{
			RUN_LOGGER(logger);

			try
			{
				auto path = File::assets;

				std::fstream fin(path.string(), std::ios::in);

				if (!fin)
				{
					throw market_exception("cannot open file " + path.string());
				}

				std::string asset;

				while (std::getline(fin, asset))
				{
					assets.push_back(asset);
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
				auto path = File::scales;

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

		void Market::initialize()
		{
			RUN_LOGGER(logger);

			try
			{
				load();

				std::filesystem::create_directory(directory);
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
				load_assets();
				load_scales();
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

		Market::path_t Market::get(const std::string & asset, const std::string & scale) const
		{
			RUN_LOGGER(logger);

			try
			{
				path_t path = asset + "_" + scale + Extension::txt;

				shared::Python python;

				try
				{
					boost::python::exec("from market import get", python.global(), python.global());

					python.global()["get"](asset.c_str(), scale.c_str(), path.string().c_str());
				}
				catch (const boost::python::error_already_set &)
				{
					logger.write(Severity::error, shared::Python::exception());
				}
				catch (const std::exception & exception)
				{
					shared::catch_handler < market_exception > (logger, exception);
				}

				return path;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}
		/*
		Market::path_t Market::get(const std::string & asset, const std::string & scale) const
		{
			RUN_LOGGER(logger);

			try
			{
				using days = std::chrono::duration < int, std::ratio < 3600 * 24 > > ;

				const std::size_t total = 365 * 20;
				const std::size_t batch = 250;

				auto first = clock_t::now() - days(batch);
				auto last  = clock_t::now();

				auto path = directory; path /= asset + "_" + scale + Extension::csv;

				std::fstream fout(path.string(), std::ios::out | std::ios::trunc);

				if (!fout)
				{
					throw market_exception("cannot open file " + path.string());
				}

				for (std::size_t size = 0; size < total; size += batch)
				{
					auto temporary_file = get(asset, scale, first, last);

					std::fstream fin(temporary_file.string(), std::ios::in);

					if (!fin)
					{
						throw market_exception("cannot open file " + temporary_file.string());
					}

					std::string line;

					std::vector < std::string > lines;

					while (std::getline(fin, line))
					{
						lines.push_back(line);
					}

					fin.close();

					std::remove(temporary_file.string().c_str());

					std::for_each(std::rbegin(lines), std::rend(lines), [&fout](const auto & line)
					{
						fout << line << std::endl;
					});

					last  = first - days(1);
					first = last  - days(batch);

					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}

				fout.close();

				return path;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}
		*/
		std::pair < Market::path_t, std::size_t > Market::get_all() const
		{
			RUN_LOGGER(logger);

			try
			{
				auto counter = 0U;

				for (const auto & asset : m_assets)
				{
					for (const auto & scale : m_scales)
					{
						get(asset, scale);

						++counter;
					}
				}

				return std::make_pair(directory, counter);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		Market::date_t Market::make_date(time_point_t time_point) const
		{
			RUN_LOGGER(logger);

			try
			{
				auto time = clock_t::to_time_t(time_point);

				std::stringstream sout;

				sout << std::put_time(std::localtime(&time), "%y%m%d");

				return sout.str();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution