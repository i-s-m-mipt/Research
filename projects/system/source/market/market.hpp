#ifndef SOLUTION_SYSTEM_MARKET_HPP
#define SOLUTION_SYSTEM_MARKET_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ctime>
#include <exception>
#include <execution>
#include <filesystem>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <mutex>
#include <ostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <boost/asio.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/multi_array.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include <nlohmann/json.hpp>

#include "candle/candle.hpp"
#include "source/source.hpp"

#include "indicators/indicators.hpp"
#include "oscillators/oscillators.hpp"

#include "../config/config.hpp"

#include "../../../shared/source/logger/logger.hpp"
#include "../../../shared/source/python/python.hpp"

namespace solution
{
	namespace system
	{
		class market_exception : public std::exception
		{
		public:

			explicit market_exception(const std::string & message) noexcept :
				std::exception(message.c_str())
			{}

			explicit market_exception(const char * const message) noexcept :
				std::exception(message)
			{}

			~market_exception() noexcept = default;
		};

		class Market
		{
		public:

			using path_t = std::filesystem::path;

		private:

			using Date_Time = market::Date_Time;

			using Level = market::Level;

			using Candle = market::Candle;

			using Source = market::Source;

			using assets_container_t = std::vector < std::string > ;

			using scales_container_t = std::vector < std::string > ;

			using limits_container_t = std::unordered_map < std::string, Candle::raw_date_t > ;

			using levels_container_t = std::vector < Level > ;

			using supports_resistances_container_t = 
				std::unordered_map < std::string, levels_container_t > ;

			using candles_container_t = std::vector < Candle > ;

			using sources_container_t = std::unordered_map < std::string,
				std::unordered_map < std::string, std::shared_ptr < Source > > > ;

			using charts_container_t = std::unordered_map < std::string,
				std::unordered_map < std::string, candles_container_t > > ;

			using self_similarity_matrix_t = boost::multi_array < double, 2U > ;

			using self_similarities_container_t = std::unordered_map < std::string,
				self_similarity_matrix_t > ;

			using distances_matrix_t = boost::multi_array < double, 2U > ;

			using pair_similarity_matrix_t = boost::multi_array < double, 2U > ;

			using pair_similarities_container_t = std::unordered_map < std::string,
				pair_similarity_matrix_t > ;

			using pair_correlation_matrix_t = boost::multi_array < double, 2U > ;

			using pair_correlations_container_t = std::unordered_map < std::string,
				pair_correlation_matrix_t > ;

			using indicators_container_t =
				std::vector < std::function < void(candles_container_t &) > > ;

			using oscillators_container_t =
				std::vector < std::function < void(candles_container_t &) > > ;

		private:

			struct Record
			{
				std::string asset;

				Date_Time date_time;

				std::vector < double > vector;

				int direction;

				double deviation;
			};

		private:

			using environment_t = std::vector < Record > ;

			using thread_pool_t = boost::asio::thread_pool;

		private:

			template < typename Iterator >
			class Candle_Parser : public boost::spirit::qi::grammar <
				Iterator, Candle(), boost::spirit::qi::blank_type >
			{
			private:

				using rule_t = boost::spirit::qi::rule <
					Iterator, Candle(), boost::spirit::qi::blank_type > ;

			public:

				Candle_Parser() : Candle_Parser::base_type(start)
				{
					static const auto separator = ',';

					start %=
						boost::spirit::qi::uint_   >> separator >> // Date
						boost::spirit::qi::uint_   >> separator >> // Time
						boost::spirit::qi::double_ >> separator >> // Open
						boost::spirit::qi::double_ >> separator >> // High
						boost::spirit::qi::double_ >> separator >> // Low
						boost::spirit::qi::double_ >> separator >> // Close
						boost::spirit::qi::ulong_long;             // Volume
				}

				~Candle_Parser() noexcept = default;

			private:

				rule_t start;
			};

		private:

			struct Extension
			{
				using extension_t = std::string;

				static inline const extension_t txt = ".txt";
				static inline const extension_t csv = ".csv";
				static inline const extension_t dat = ".dat";

				static inline const extension_t empty = "";
			};

		private:

			class Data
			{
			private:

				using json_t = nlohmann::json;

			private:

				struct File
				{
					using path_t = std::filesystem::path;

					static inline const path_t assets_data = "market/assets.data";
					static inline const path_t scales_data = "market/scales.data";
					static inline const path_t limits_data = "market/limits.data";

					static inline const path_t self_similarities_data    = "market/output/self_similarities.data";
					static inline const path_t pair_similarities_data    = "market/output/pair_similarities.data";
					static inline const path_t pair_correlations_data    = "market/output/pair_correlations.data";
					static inline const path_t cumulative_distances_data = "market/output/cumulative_distances.data";
					static inline const path_t price_deviations_data     = "market/output/price_deviations.data";
					static inline const path_t tagged_charts_data        = "market/output/tagged_charts.data";
					static inline const path_t environment_data          = "market/output/environment.data";
					static inline const path_t supports_resistances_data = "market/output/supports_resistances.data";
				};

			private:

				using path_t = File::path_t;

			public:

				static void load_assets(assets_container_t & assets);

				static void load_scales(scales_container_t & scales);

				static void load_limits(limits_container_t & limits);

			public:

				static void save_self_similarities(const self_similarities_container_t & self_similarities);

				static void save_pair_similarities(const pair_similarities_container_t & pair_similarities);

				static void save_pair_correlations(const pair_correlations_container_t & pair_correlations);

				static void save_cumulative_distances(const distances_matrix_t & matrix);

				static void save_price_deviations(const charts_container_t & charts);

				static void save_tagged_charts(const charts_container_t & charts, const Config & config); // TODO

				static void save_environment(const charts_container_t & charts, const Config & config);

				static void save_supports_resistances(const supports_resistances_container_t & supports_resistances);

			private:

				static void load(const path_t & path, json_t & object);

				static void save(const path_t & path, const json_t & object);
			};

		private:

			struct State_Tag
			{
				static inline const std::string C = "C";
				static inline const std::string L = "L";
				static inline const std::string S = "S";
			};

		private:

			struct Scale
			{
				static inline const std::string H = "M60";
				static inline const std::string D = "D";
				static inline const std::string W = "W";
				static inline const std::string M = "MN";
			};

		public:

			Market(const Config & config) : 
				m_config(config), m_thread_pool(2U * std::thread::hardware_concurrency())
			{
				initialize();
			}

			~Market() noexcept
			{
				try
				{
					uninitialize();
				}
				catch (...)
				{
					// std::abort();
				}
			}

		private:

			void initialize();

			void uninitialize();

		private:

			void load();

		private:

			void load_assets();

			void load_scales();

			void load_limits();

			void load_indicators();

			void load_oscillators();

			void load_charts();

		private:

			std::pair < path_t, std::size_t > get_all_charts() const;
	
			path_t get_chart(const std::string & asset, const std::string & scale) const;

		private:

			std::string make_file_name(const std::string & asset, const std::string & scale) const;

		private:

			candles_container_t load_candles(const std::string & asset,
				const std::string & scale, const path_t & path) const;

			Candle parse(const std::string & line) const;

			void update_deviations(const std::string & asset, 
				const std::string & scale, candles_container_t & candles) const;

		private:

			void handle_self_similarities();

			void handle_pair_similarities();

			void handle_pair_correlations();

			void handle_price_deviations();

			void handle_tagged_charts();

			void handle_environment();

			void handle_local_environment();

			void handle_quik_initialization();

		private:

			void run_local_environment_test() const;

			void run_fridays_test() const;

			void run_mornings_test() const;

			void run_intraday_test() const;

		private:

			double distance(const Record & record_test, const Record & record) const;

		private:

			void compute_self_similarities();

			void compute_pair_similarities();

			void compute_pair_correlations();

		private:

			double compute_self_similarity(const std::string & asset,
				const std::string & scale_1, const std::string & scale_2) const;

			double compute_pair_similarity(const std::string & scale,
				const std::string & asset_1, const std::string & asset_2) const;

			double compute_pair_correlation(const std::string & scale,
				const std::string & asset_1, const std::string & asset_2) const;

		private:

			void save_self_similarities() const;

			void save_pair_similarities() const;

			void save_pair_correlations() const;

			void save_cumulative_distances(const distances_matrix_t & matrix) const;

			void save_price_deviations() const;

		private:

			void make_supports_resistances();

		private:

			levels_container_t make_levels(candles_container_t & candles) const;

			void make_level(Candle & extremum, levels_container_t & levels, 
				std::size_t locality, Candle::Type type) const;

			void update_levels_strength(levels_container_t & levels, 
				const candles_container_t & candles) const;

			void update_levels_weakness(levels_container_t & levels,
				const candles_container_t & candles) const;

		private:

			void save_supports_resistances() const;

		private:

			void make_tagged_charts();

		private:

			void update_regression_tags(candles_container_t & candles) const;

			void update_classification_tags(candles_container_t & candles) const;

			void sample_classification_tags(candles_container_t & candles, 
				candles_container_t::iterator position, std::string tag) const;

			void concat_classification_tags(std::string & target, const std::string & tag) const;

			void update_movement_tags(candles_container_t & candles) const;

			void update_supports_resistances(candles_container_t & candles, const levels_container_t & levels) const;

			void update_indicators(candles_container_t & candles) const;

			void update_oscillators(candles_container_t & candles) const;

		private:

			void save_tagged_charts() const;

		private:

			void prepare_environment_data();

			void save_environment() const;

			void make_local_environment();

		private:

			void initialize_sources();

			void update_supports_resistances();

		private:

			path_t get_chart_for_levels(const std::string & asset, const std::string & scale) const;

		public:

			const auto & assets() const noexcept
			{
				return m_assets;
			}

			const auto & scales() const noexcept
			{
				return m_scales;
			}

			const auto & charts() const noexcept
			{
				return m_charts;
			}

		public:

			std::string get_current_data(const std::string & asset, const std::string & scale, std::size_t size);

			std::vector < std::string > get_current_data_variation(
				const std::string & asset, const std::string & scale, std::size_t size);

		private:

			void print_last_candle(const std::string & asset, const candles_container_t & candles) const;
			
		private:

			std::string serialize_candles(const candles_container_t & candles) const;

		private:

			static double get_price_deviation_multiplier(const std::string & scale);

			static double get_volume_deviation_multiplier(const std::string & scale);

		private:

			static inline const path_t charts_directory = "market/charts";
			static inline const path_t levels_directory = "market/levels";
			static inline const path_t output_directory = "market/output";

		private:

			static inline const std::time_t seconds_in_day = 86400LL;

			static inline const double months_in_year = 12.0; // normalization
			static inline const double days_in_month  = 31.0; // normalization

			static inline const std::size_t days_in_year = 250U;

		private:

			Config m_config;

			assets_container_t m_assets;

			scales_container_t m_scales;

			limits_container_t m_limits;

			sources_container_t m_sources;

			charts_container_t m_charts;

			self_similarities_container_t m_self_similarities;

			pair_similarities_container_t m_pair_similarities;

			pair_correlations_container_t m_pair_correlations;

			supports_resistances_container_t m_supports_resistances;

			indicators_container_t m_indicators;

			oscillators_container_t m_oscillators;

			environment_t m_environment;

			environment_t m_environment_test;

			thread_pool_t m_thread_pool;
		};

	} // namespace system

} // namespace solution

BOOST_FUSION_ADAPT_STRUCT
(
	 solution::system::market::Candle,
	(solution::system::market::Candle::raw_date_t, raw_date)
	(solution::system::market::Candle::raw_time_t, raw_time)
	(solution::system::market::Candle::price_t,    price_open)
	(solution::system::market::Candle::price_t,    price_high)
	(solution::system::market::Candle::price_t,    price_low)
	(solution::system::market::Candle::price_t,    price_close)
	(solution::system::market::Candle::volume_t,   volume)
)

#endif // #ifndef SOLUTION_SYSTEM_MARKET_HPP