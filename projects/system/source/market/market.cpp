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
				config.required_pair_similarities =
					raw_config[Key::Config::required_pair_similarities].get < bool > ();
				config.self_similarity_DTW_delta =
					raw_config[Key::Config::self_similarity_DTW_delta].get < int > ();
				config.cumulative_distances_asset =
					raw_config[Key::Config::cumulative_distances_asset].get < std::string > ();
				config.cumulative_distances_scale_1 =
					raw_config[Key::Config::cumulative_distances_scale_1].get < std::string > ();
				config.cumulative_distances_scale_2 =
					raw_config[Key::Config::cumulative_distances_scale_2].get < std::string > ();
				config.required_deviations =
					raw_config[Key::Config::required_deviations].get < bool > ();
				config.required_tagged_charts =
					raw_config[Key::Config::required_tagged_charts].get < bool > ();
				config.min_price_change =
					raw_config[Key::Config::min_price_change].get < double > ();
				config.max_price_rollback =
					raw_config[Key::Config::max_price_rollback].get < double > ();
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

		void Market::Data::save_pair_similarities(const pair_similarities_container_t & pair_similarities)
		{
			RUN_LOGGER(logger);

			try
			{
				auto path = File::pair_similarities_data;

				std::fstream fout(path.string(), std::ios::out | std::ios::trunc);

				if (!fout)
				{
					throw market_exception("cannot open file " + path.string());
				}

				for (const auto & [scale, matrix] : pair_similarities)
				{
					auto size = matrix.size();

					fout << scale << " " << size << std::endl << std::endl;

					for (auto i = 0U; i < size; ++i)
					{
						for (auto j = 0U; j < size; ++j)
						{
							fout << std::setw(1 + 1 + 6) << std::right << std::setprecision(6) << std::fixed << matrix[i][j] << " ";
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
							{ fout << std::setprecision(6) << std::fixed << candle.deviation << " "; });

						fout << std::endl << std::endl;
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::Data::save_tagged_charts(const charts_container_t & charts)
		{
			RUN_LOGGER(logger);

			try
			{
				auto path = File::tagged_charts_data;

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
							{ 
								static const char delimeter = ',';

								fout <<
									candle.date_time.year   << delimeter << std::setfill('0') << std::setw(2) <<
									candle.date_time.month  << delimeter << std::setfill('0') << std::setw(2) <<
									candle.date_time.day    << delimeter << std::setfill('0') << std::setw(2) <<
									candle.date_time.hour   << delimeter << std::setfill('0') << std::setw(2) <<
									candle.date_time.minute << delimeter << std::setfill('0') << std::setw(2) <<
									candle.date_time.second << delimeter;
								
								fout << std::setprecision(6) << std::fixed << std::showpos <<
									candle.deviation << delimeter;

								for (auto regression_tag : candle.regression_tags)
								{
									fout << std::setprecision(6) << std::fixed << std::showpos <<
										regression_tag << delimeter;
								}
								
								fout << candle.classification_tag << std::endl;
							});

						fout << std::endl;
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
					handle_self_similarities();
				}

				if (m_config.required_pair_similarities)
				{
					handle_pair_similarities();
				}

				if (m_config.required_deviations)
				{
					handle_deviations();
				}

				if (m_config.required_tagged_charts)
				{
					handle_tagged_charts();
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

		std::string Market::make_file_name(const std::string & asset, const std::string & scale) const
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

		void Market::handle_self_similarities()
		{
			RUN_LOGGER(logger);

			try
			{
				compute_self_similarities();

				save_self_similarities();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::handle_pair_similarities()
		{
			RUN_LOGGER(logger);

			try
			{
				compute_pair_similarities();

				save_pair_similarities();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::handle_deviations()
		{
			RUN_LOGGER(logger);

			try
			{
				save_deviations();
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::handle_tagged_charts()
		{
			RUN_LOGGER(logger);

			try
			{
				make_tagged_charts();

				save_tagged_charts();
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

		void Market::compute_pair_similarities()
		{
			RUN_LOGGER(logger);

			try
			{
				const auto size = std::size(m_assets);

				for (const auto & scale : m_scales)
				{
					std::vector < std::future < double > > futures(size * (size - 1U) / 2U);

					for (auto i = 0U, index = 0U; i < size; ++i)
					{
						for (auto j = i + 1; j < size; ++j)
						{
							std::packaged_task < double() > task([this, scale, i, j]()
								{ return compute_pair_similarity(scale, m_assets[i], m_assets[j]); });

							futures[index++] = boost::asio::post(m_thread_pool, std::move(task));
						}
					}

					m_pair_similarities.insert(std::make_pair(scale, pair_similarity_matrix_t(boost::extents[size][size])));

					for (auto i = 0U, index = 0U; i < size; ++i)
					{
						for (auto j = 0U; j < size; ++j)
						{
							if (i == j)
							{
								m_pair_similarities[scale][i][j] = 0.0;
							}
							else
							{
								if (i > j)
								{
									m_pair_similarities[scale][i][j] = m_pair_similarities[scale][j][i];
								}
								else
								{
									m_pair_similarities[scale][i][j] = futures[index++].get();
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
				const auto & candles_1 = m_charts.at(asset).at(scale_1);
				const auto & candles_2 = m_charts.at(asset).at(scale_2);

				auto size_1 = std::size(candles_1);
				auto size_2 = std::size(candles_2);

				distances_matrix_t distances(boost::extents[size_1][size_2]);

				const auto multiplier_1 = get_deviation_multiplier(scale_1);
				const auto multiplier_2 = get_deviation_multiplier(scale_2);

				for (auto i = 0U; i < size_1; ++i)
				{
					for (auto j = 0U; j < size_2; ++j)
					{
						distances[i][j] = std::abs(
							candles_1[i].deviation * multiplier_1 - candles_2[j].deviation * multiplier_2);
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

				for (auto i = 1; i < size_1; ++i)
				{
					for (auto j = 1; j < size_2; ++j)
					{
						if (std::abs(i - j) < m_config.self_similarity_DTW_delta)
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

				if ((asset == m_config.cumulative_distances_asset) && (
					(scale_1 == m_config.cumulative_distances_scale_1 && scale_2 == m_config.cumulative_distances_scale_2) ||
					(scale_2 == m_config.cumulative_distances_scale_1 && scale_1 == m_config.cumulative_distances_scale_2)))
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

		double Market::compute_pair_similarity(const std::string & scale,
			const std::string & asset_1, const std::string & asset_2) const
		{
			RUN_LOGGER(logger);

			try
			{
				const auto & candles_1 = m_charts.at(asset_1).at(scale);
				const auto & candles_2 = m_charts.at(asset_2).at(scale);

				auto size = std::min(std::size(candles_1), std::size(candles_2));

				std::vector < double > deviations_1(size, 0.0);
				std::vector < double > deviations_2(size, 0.0);

				std::transform(std::crbegin(candles_1), std::next(std::crbegin(candles_1), size),
					std::begin(deviations_1), [](const auto & candle) { return candle.deviation; });

				std::transform(std::crbegin(candles_2), std::next(std::crbegin(candles_2), size),
					std::begin(deviations_2), [](const auto & candle) { return candle.deviation; });

				std::sort(std::begin(deviations_1), std::end(deviations_1));
				std::sort(std::begin(deviations_2), std::end(deviations_2));

				return (std::transform_reduce(std::begin(deviations_1), std::end(deviations_1), std::begin(deviations_2),
					0.0, std::plus(), [](const auto lhs, const auto rhs) { return std::abs(lhs - rhs); }) / size);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		double Market::get_deviation_multiplier(const std::string & scale) const
		{
			RUN_LOGGER(logger);

			try
			{
				return 1.0;
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

		void Market::save_pair_similarities() const
		{
			RUN_LOGGER(logger);

			try
			{
				Data::save_pair_similarities(m_pair_similarities);
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

		void Market::make_tagged_charts()
		{
			RUN_LOGGER(logger);

			try
			{
				for (auto & [asset, scales] : m_charts)
				{
					for (auto & [scale, candles] : scales)
					{
						update_regression_tags(candles);

						update_classification_tags(candles);
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::update_regression_tags(candles_container_t & candles)
		{
			RUN_LOGGER(logger);

			try
			{
				for (auto i = 0U; i < std::size(candles); ++i)
				{
					for (auto j = 0U; j < Candle::prediction_range; ++j)
					{
						if (i + j + 1 < std::size(candles))
						{
							if (candles[i].price_close <= std::numeric_limits < double > ::epsilon())
							{
								throw std::domain_error("division by zero");
							}

							candles[i].regression_tags[j] = (candles[i + j + 1].price_close -
								candles[i].price_close) / candles[i].price_close;
						}
						else
						{
							candles[i].regression_tags[j] = 0.0;
						}
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::update_classification_tags(candles_container_t & candles)
		{
			RUN_LOGGER(logger);

			try
			{
				const auto min_price_change   = m_config.min_price_change;
				const auto max_price_rollback = m_config.max_price_rollback;

				for (auto first = std::begin(candles); first != std::end(candles);)
				{
					auto flag = false;

					for (auto last = std::next(first); last != std::end(candles); ++last)
					{
						auto result = std::minmax_element(first, std::next(last), [](const auto & lhs, const auto & rhs)
							{
								return lhs.price_close < rhs.price_close;
							});

						auto min_price = result.first ->price_close;
						auto max_price = result.second->price_close;

						auto first_extremum = result.first;
						auto last_extremum  = result.second;

						if (std::distance(first, result.first) > std::distance(first, result.second))
						{
							first_extremum = result.second;
							last_extremum  = result.first;
						}

						if ((((max_price - min_price) / min_price < min_price_change) ||
							(result.first  == last) || 
							(result.second == last) || ((last_extremum != last) &&
								((max_price - min_price) / min_price > min_price_change) &&
								(std::abs(last_extremum->price_close - last->price_close) < max_price_rollback *
									std::abs(first_extremum->price_close - last_extremum->price_close)))) &&
							(std::abs(last_extremum->price_close - last->price_close) /
								std::min(last_extremum->price_close, last->price_close) < min_price_change))
						{
							continue;
						}

						if (first_extremum->price_close < last_extremum->price_close)
						{
							first_extremum->classification_tag += "OL";
							last_extremum ->classification_tag += "CL";
						}
						else
						{
							first_extremum->classification_tag += "OS";
							last_extremum ->classification_tag += "CS";
						}

						first = last_extremum;

						flag = true;

						break;
					}

					if (!flag)
					{
						break;
					}
				}

				for (auto & candle : candles)
				{
					if (candle.classification_tag.empty())
					{
						candle.classification_tag += "WW";
					}
				}
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

		void Market::save_tagged_charts() const
		{
			RUN_LOGGER(logger);

			try
			{
				Data::save_tagged_charts(m_charts);
			}
			catch (const std::exception & exception)
			{
				shared::catch_handler < market_exception > (logger, exception);
			}
		}

	} // namespace system

} // namespace solution