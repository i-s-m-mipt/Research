#include "market.hpp"

namespace solution
{
	namespace system
	{
		using Severity = shared::Logger::Severity;

		void Market::Data::load_config(Config & config)
		{
			RUN_LOGGER(logger);

			try
			{
				json_t raw_config;

				load(File::config_json, raw_config);

				config.required_charts = 
					raw_config[Key::Config::required_charts].get < bool > ();
				config.required_self_similarities = 
					raw_config[Key::Config::required_self_similarities].get < bool > ();
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
				auto path = File::assets_data;

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

		void Market::Data::save(const path_t & path, const json_t & object)
		{
			RUN_LOGGER(logger);

			try
			{
				std::fstream fout(path.string(), std::ios::out);

				if (!fout)
				{
					throw market_exception("cannot open file " + path.string());
				}

				fout << std::setw(4) << object;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::Candle::update_date_time() noexcept
		{
			RUN_LOGGER(logger);

			try
			{
				date_time.year   = (date / 100) / 100;
				date_time.month  = (date / 100) % 100;
				date_time.day    = (date % 100);

				date_time.hour   = (time / 100) / 100;
				date_time.minute = (time / 100) % 100;
				date_time.second = (time % 100);
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
				std::filesystem::create_directory(directory);

				load();

				if (m_config.required_self_similarities)
				{
					compute_self_similarities();
				}
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
				load_charts();
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

		void Market::load_charts()
		{
			RUN_LOGGER(logger);

			try
			{
				if (m_config.required_charts)
				{
					get_all_charts();
				}

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

						m_charts[asset][scale] = load_candles(path);
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		Market::candles_container_t Market::load_candles(const path_t & path) const
		{
			RUN_LOGGER(logger);

			try
			{
				candles_container_t candles;

				std::fstream fin(path.string(), std::ios::in);

				if (!fin)
				{
					throw market_exception("cannot open file " + path.string());
				}

				std::string line;

				while (std::getline(fin, line))
				{
					candles.push_back(parse(line));
				}

				std::reverse(std::begin(candles), std::end(candles));

				return candles;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		Market::Candle Market::parse(const std::string & s) const
		{
			RUN_LOGGER(logger);

			try
			{
				Candle_Parser < std::string::const_iterator > parser;

				auto first = std::begin(s);
				auto last  = std::end(s);

				Candle candle;

				auto result = boost::spirit::qi::phrase_parse(
					first, last, parser, boost::spirit::qi::blank, candle);

				if (result)
				{
					candle.update_date_time();

					return candle;
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

		Market::path_t Market::get_chart(const std::string & asset, const std::string & scale) const
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
		
		std::pair < Market::path_t, std::size_t > Market::get_all_charts() const
		{
			RUN_LOGGER(logger);

			try
			{
				auto counter = 0U;

				for (const auto & asset : m_assets)
				{
					for (const auto & scale : m_scales)
					{
						get_chart(asset, scale);

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

		void Market::compute_self_similarities()
		{
			RUN_LOGGER(logger);

			try
			{
				for (const auto & asset : m_assets)
				{
					m_self_similarities.insert(std::make_pair(asset, self_similarity_matrix_t(
						boost::extents[std::size(m_scales)][std::size(m_scales)])));

					for (auto i = 0U; i < std::size(m_scales); ++i)
					{
						for (auto j = 0U; j < std::size(m_scales); ++j)
						{
							m_self_similarities[asset][i][j] = ((i == j) ? 1.0 : // ?
								compute_self_similarity(asset, m_scales[i], m_scales[j]));
						}
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		double Market::compute_self_similarity(const std::string & asset,
			const std::string & scale_1, const std::string & scale_2) const
		{
			RUN_LOGGER(logger);

			try
			{
				auto self_similarity = 0.0;

				// TODO
				// USE 2 std::vector < Candle > : 
				// m_charts[asset][scale_1].something
				// m_charts[asset][scale_2].something

				return self_similarity;
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