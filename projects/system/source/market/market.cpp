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
				load_charts();
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

		void Market::load_charts()
		{
			RUN_LOGGER(logger);

			try
			{
				for (const auto & asset : m_assets)
				{
					for (const auto & scale : m_scales)
					{
						auto path = directory; path /= make_file_name(asset, scale);

						if (!std::filesystem::exists(path))
						{
							logger.write(Severity::error, "file " + path.string() + " doesn't exist");

							continue;
						}

						m_charts[asset][scale] = load_bars(path);
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		Market::bars_container_t Market::load_bars(const path_t & path) const
		{
			RUN_LOGGER(logger);

			try
			{
				bars_container_t bars;

				std::fstream fin(path.string(), std::ios::in);

				if (!fin)
				{
					throw market_exception("cannot open file " + path.string());
				}

				std::string s;

				while (std::getline(fin, s))
				{
					bars.push_back(parse(s));
				}

				std::reverse(std::begin(bars), std::end(bars));

				return bars;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		Market::Bar Market::parse(const std::string & s) const
		{
			RUN_LOGGER(logger);

			try
			{
				Bar_Parser < std::string::const_iterator > parser;

				auto first = std::begin(s);
				auto last  = std::end(s);

				Bar bar;

				auto result = boost::spirit::qi::phrase_parse(
					first, last, parser, boost::spirit::qi::blank, bar);

				if (result)
				{
					return bar;
				}
				else
				{
					throw market_exception("cannot parse line " + s);
				}
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
				auto path = directory; path /= make_file_name(asset, scale);

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

		std::string Market::make_file_name(
			const std::string & asset, const std::string & scale) const
		{
			RUN_LOGGER(logger);

			try
			{
				return (asset + "_" + scale + Extension::csv);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution