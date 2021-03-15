#include "market.hpp"

namespace solution
{
	namespace system
	{
		using Severity = shared::Logger::Severity;

		void Market::Candle::update_date_time() noexcept
		{
			RUN_LOGGER(logger);

			try
			{
				date_time.year   = (raw_date / 100U) / 100U;
				date_time.month  = (raw_date / 100U) % 100U;
				date_time.day    = (raw_date % 100U);

				date_time.hour   = (raw_time / 100U) / 100U;
				date_time.minute = (raw_time / 100U) % 100U;
				date_time.second = (raw_time % 100U);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

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
				config.cumulative_distances_asset =
					raw_config[Key::Config::cumulative_distances_asset].get < std::string > ();
				config.cumulative_distances_scale_1 =
					raw_config[Key::Config::cumulative_distances_scale_1].get < std::string > ();
				config.cumulative_distances_scale_2 =
					raw_config[Key::Config::cumulative_distances_scale_2].get < std::string > ();
				config.required_deviations =
					raw_config[Key::Config::required_deviations].get < bool > ();
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

		void Market::Data::save_self_similarities(const self_similarities_container_t & self_similarities)
		{
			RUN_LOGGER(logger);

			try
			{
				auto path = File::self_similarities_data;

				std::fstream fout(path.string(), std::ios::out | std::ios::trunc);

				if (!fout)
				{
					throw market_exception("cannot open file " + path.string());
				}

				for (const auto & [asset, matrix] : self_similarities)
				{
					auto size = matrix.size();

					fout << asset << " " << size << std::endl << std::endl;

					for (auto i = 0U; i < size; ++i)
					{
						for (auto j = 0U; j < size; ++j)
						{
							fout << std::setw(3 + 1 + 3) << std::right << std::setprecision(3) << std::fixed << matrix[i][j] << " ";
						}

						fout << std::endl;
					}

					fout << std::endl;
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::Data::save_cumulative_distances(const distances_matrix_t & matrix)
		{
			RUN_LOGGER(logger);

			try
			{
				auto path = File::cumulative_distances_data;

				std::fstream fout(path.string(), std::ios::out | std::ios::trunc);

				if (!fout)
				{
					throw market_exception("cannot open file " + path.string());
				}

				auto size_1 = matrix.size();

				auto size_2 = matrix.begin()->size();

				fout << size_1 << " " << size_2 << std::endl << std::endl;

				for (auto i = 0U; i < size_1; ++i)
				{
					for (auto j = 0U; j < size_2; ++j)
					{
						fout << std::setw(3 + 1 + 3) << std::right << std::setprecision(3) << std::fixed << matrix[i][j] << " ";
					}

					fout << std::endl;
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::Data::save_deviations(const charts_container_t & charts)
		{
			RUN_LOGGER(logger);

			try
			{
				auto path = File::deviations_data;

				std::fstream fout(path.string(), std::ios::out | std::ios::trunc);

				if (!fout)
				{
					throw market_exception("cannot open file " + path.string());
				}

				for (const auto & [asset, scales] : charts)
				{
					for (const auto & [scale, candles] : scales)
					{
						fout << asset << " " << scale << " " << std::size(candles) << std::endl << std::endl;

						std::for_each(std::begin(candles), std::end(candles), [&fout](const auto & candle) 
							{ fout << std::setprecision(3) << std::fixed << candle.deviation << " "; });

						fout << std::endl << std::endl;
					}
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

		void Market::initialize()
		{
			RUN_LOGGER(logger);

			try
			{
				std::filesystem::create_directory(charts_directory);
				std::filesystem::create_directory(output_directory);

				load();

				if (m_config.required_self_similarities)
				{
					compute_self_similarities();

					save_self_similarities();
				}

				if (m_config.required_deviations)
				{
					save_deviations();
				}
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
				m_thread_pool.join();
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
						auto path = charts_directory; path /= make_file_name(asset, scale);

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

				update_deviations(candles);

				return candles;
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		Market::Candle Market::parse(const std::string & line) const
		{
			RUN_LOGGER(logger);

			try
			{
				Candle_Parser < std::string::const_iterator > parser;

				auto first = std::begin(line);
				auto last  = std::end(line);

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
					throw market_exception("cannot parse line " + line);
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::update_deviations(candles_container_t & candles) const
		{
			RUN_LOGGER(logger);

			try
			{
				for (auto i = 1U; i < std::size(candles); ++i)
				{
					if (candles[i - 1].price_close <= std::numeric_limits < double > ::epsilon())
					{
						throw std::domain_error("division by zero");
					}

					candles[i].deviation = (candles[i].price_close - candles[i - 1].price_close) / candles[i - 1].price_close;
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
				auto path = charts_directory; path /= make_file_name(asset, scale);

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

				return std::make_pair(charts_directory, counter);
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
				const auto size = std::size(m_scales);

				for (const auto & asset : m_assets)
				{
					std::vector < std::future < double > > futures(size * (size - 1U) / 2U);

					for (auto i = 0U, index = 0U; i < size; ++i)
					{
						for (auto j = i + 1; j < size; ++j)
						{
							std::packaged_task < double() > task([this, asset, i, j]()
								{ return compute_self_similarity(asset, m_scales[i], m_scales[j]); });

							futures[index++] = boost::asio::post(m_thread_pool, std::move(task));
						}
					}

					m_self_similarities.insert(std::make_pair(asset, self_similarity_matrix_t(boost::extents[size][size])));

					for (auto i = 0U, index = 0U; i < size; ++i)
					{
						for (auto j = 0U; j < size; ++j)
						{
							if (i == j)
							{
								m_self_similarities[asset][i][j] = 0.0;
							}
							else
							{
								if (i > j)
								{
									m_self_similarities[asset][i][j] = m_self_similarities[asset][j][i];
								}
								else
								{
									m_self_similarities[asset][i][j] = futures[index++].get();
								}
							}
						}
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		template < typename T >
		auto min(T lhs, T rhs)
		{
			return std::min(lhs, rhs);
		}

		template < typename T, typename ... Types >
		auto min(T lhs, Types ... args)
		{
			return std::min(lhs, min(args...));
		}

		double Market::compute_self_similarity(const std::string & asset,
			const std::string & scale_1, const std::string & scale_2) const
		{
			RUN_LOGGER(logger);

			try
			{
				auto size_1 = std::size(m_charts.at(asset).at(scale_1));
				auto size_2 = std::size(m_charts.at(asset).at(scale_2));

				distances_matrix_t distances(boost::extents[size_1][size_2]);

				for (auto i = 0U; i < size_1; ++i)
				{
					for (auto j = 0U; j < size_2; ++j)
					{
						distances[i][j] = std::abs(
							m_charts.at(asset).at(scale_1)[i].deviation -
							m_charts.at(asset).at(scale_2)[j].deviation);
					}
				}

				distances_matrix_t cumulative_distances(boost::extents[size_1][size_2]);

				cumulative_distances[0][0] = distances[0][0];

				for (auto i = 1U; i < size_1; ++i)
				{
					cumulative_distances[i][0] = distances[i][0] + cumulative_distances[i - 1][0];
				}

				for (auto j = 1U; j < size_2; ++j)
				{
					cumulative_distances[0][j] = distances[0][j] + cumulative_distances[0][j - 1];
				}

				constexpr auto delta = std::numeric_limits < int > ::max(); // ?

				for (auto i = 1; i < size_1; ++i)
				{
					for (auto j = 1; j < size_2; ++j)
					{
						if (std::abs(i - j) < delta)
						{
							cumulative_distances[i][j] = distances[i][j] + min(cumulative_distances[i - 1][j - 1],
								cumulative_distances[i - 1][j], cumulative_distances[i][j - 1]);
						}
						else
						{
							cumulative_distances[i][j] = std::numeric_limits < double > ::infinity();
						}
					}
				}

				if (asset   == m_config.cumulative_distances_asset   &&
					scale_1 == m_config.cumulative_distances_scale_1 &&
					scale_2 == m_config.cumulative_distances_scale_2) // TODO
				{
					save_cumulative_distances(cumulative_distances);
				}

				return cumulative_distances[size_1 - 1][size_2 - 1];
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::save_self_similarities() const
		{
			RUN_LOGGER(logger);

			try
			{
				Data::save_self_similarities(m_self_similarities);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::save_cumulative_distances(const distances_matrix_t & matrix) const
		{
			RUN_LOGGER(logger);

			try
			{
				Data::save_cumulative_distances(matrix);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::save_deviations() const
		{
			RUN_LOGGER(logger);

			try
			{
				Data::save_deviations(m_charts);
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