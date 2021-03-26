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

#define BOOST_PYTHON_STATIC_LIB

#include <boost/asio.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/multi_array.hpp>
#include <boost/python.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>

#include <nlohmann/json.hpp>

#include "source/source.hpp"

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

			struct Config
			{
				bool required_charts            = false;
				bool required_self_similarities = false;
				bool required_pair_similarities = false;
				bool required_pair_correlations = false;

				int self_similarity_DTW_delta;

				std::string cumulative_distances_asset;
				std::string cumulative_distances_scale_1;
				std::string cumulative_distances_scale_2;

				bool required_deviations = false;

				bool required_tagged_charts = false;

				double min_price_change   = 0.020;
				double max_price_rollback = 0.333;

				double level_max_deviation = 0.005;

				std::string level_resolution = "D";

				std::size_t level_frame = 5U;

				bool required_quik = false;

				bool required_supports_resistances = false;

				double classification_max_deviation = 0.005;
			};

		private:

			using assets_container_t = std::vector < std::string > ;

			using scales_container_t = std::vector < std::string > ;

			using Source = market::Source;

			using sources_container_t = std::unordered_map < std::string,
				std::unordered_map < std::string, std::shared_ptr < Source > > > ;

		public:

			struct Date_Time
			{
			public:

				std::time_t to_time_t() const;

			public:

				unsigned int year   = 0U;
				unsigned int month  = 0U;
				unsigned int day    = 0U;

				unsigned int hour   = 0U;
				unsigned int minute = 0U;
				unsigned int second = 0U;
			};

		public:

			friend bool operator== (const Date_Time & lhs, const Date_Time & rhs);
			friend bool operator!= (const Date_Time & lhs, const Date_Time & rhs);
			friend bool operator<  (const Date_Time & lhs, const Date_Time & rhs);
			friend bool operator<= (const Date_Time & lhs, const Date_Time & rhs);
			friend bool operator>  (const Date_Time & lhs, const Date_Time & rhs);
			friend bool operator>= (const Date_Time & lhs, const Date_Time & rhs);

		private:

			struct Level
			{
			public:

				using date_time_t = Date_Time;

			public:

				date_time_t begin;

				double price = 0.0;

				std::size_t strength = 0U;
			};

		private:

			friend std::ostream & operator<< (std::ostream & stream, const Level & level);

		private:

			using levels_container_t = std::vector < Level > ;

			using supports_resistances_container_t = 
				std::unordered_map < std::string, levels_container_t > ;

		public:

			struct Candle
			{
			public:

				using raw_date_t = unsigned int;
				using raw_time_t = unsigned int;

			public:

				using date_time_t = Date_Time;

				using price_t = double;

				using volume_t = unsigned long long;

			public:

				void update_date_time() noexcept;

			public:

				static inline const std::size_t prediction_range = 5U;

			public:

				raw_date_t raw_date = 0U;
				raw_time_t raw_time = 0U;

			public:

				date_time_t date_time;

				price_t price_open  = 0.0;
				price_t price_high  = 0.0;
				price_t price_low   = 0.0;
				price_t price_close = 0.0;

				volume_t volume = 0ULL;

				double deviation = 0.0;

				std::array < double, prediction_range > regression_tags;

				std::string classification_tag;

				Level support;
				Level resistance;
			};

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

			using candles_container_t = std::vector < Candle > ;

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

					static inline const path_t config_json = "market/config.json";
					static inline const path_t assets_data = "market/assets.data";
					static inline const path_t scales_data = "market/scales.data";

					static inline const path_t self_similarities_data    = "market/output/self_similarities.data";
					static inline const path_t pair_similarities_data    = "market/output/pair_similarities.data";
					static inline const path_t pair_correlations_data    = "market/output/pair_correlations.data";
					static inline const path_t cumulative_distances_data = "market/output/cumulative_distances.data";
					static inline const path_t deviations_data           = "market/output/deviations.data";
					static inline const path_t tagged_charts_data        = "market/output/tagged_charts.data";
					static inline const path_t supports_resistances_data = "market/output/supports_resistances.data";
				};

			private:

				using path_t = File::path_t;

			private:

				struct Key
				{
					struct Config
					{
						static inline const std::string required_charts               = "required_charts";
						static inline const std::string required_self_similarities    = "required_self_similarities";
						static inline const std::string required_pair_similarities    = "required_pair_similarities";
						static inline const std::string required_pair_correlations    = "required_pair_correlations";
						static inline const std::string self_similarity_DTW_delta     = "self_similarity_DTW_delta";
						static inline const std::string cumulative_distances_asset    = "cumulative_distances_asset";
						static inline const std::string cumulative_distances_scale_1  = "cumulative_distances_scale_1";
						static inline const std::string cumulative_distances_scale_2  = "cumulative_distances_scale_2";
						static inline const std::string required_deviations           = "required_deviations";
						static inline const std::string required_tagged_charts        = "required_tagged_charts";
						static inline const std::string min_price_change              = "min_price_change";
						static inline const std::string max_price_rollback            = "max_price_rollback";
						static inline const std::string level_max_deviation           = "level_max_deviation";
						static inline const std::string level_resolution              = "level_resolution";
						static inline const std::string level_frame                   = "level_frame";
						static inline const std::string required_quik                 = "required_quik";
						static inline const std::string required_supports_resistances = "required_supports_resistances";
						static inline const std::string classification_max_deviation  = "classification_max_deviation";
					};
				};

			public:

				static void load_config(Config & config);

				static void load_assets(assets_container_t & assets);

				static void load_scales(scales_container_t & scales);

			public:

				static void save_self_similarities(const self_similarities_container_t & self_similarities);

				static void save_pair_similarities(const pair_similarities_container_t & pair_similarities);

				static void save_pair_correlations(const pair_correlations_container_t & pair_correlations);

				static void save_cumulative_distances(const distances_matrix_t & matrix);

				static void save_deviations(const charts_container_t & charts);

				static void save_tagged_charts(const charts_container_t & charts);

				static void save_supports_resistances(const supports_resistances_container_t & supports_resistances);

			private:

				static void load(const path_t & path, json_t & object);

				static void save(const path_t & path, const json_t & object);
			};

		private:

			using thread_pool_t = boost::asio::thread_pool;

		public:

			Market() : m_thread_pool(2U * std::thread::hardware_concurrency())
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

			void load_config();

			void load_assets();

			void load_scales();

			void load_charts();

		private:

			std::pair < path_t, std::size_t > get_all_charts() const;
	
			path_t get_chart(const std::string & asset, const std::string & scale) const;

		private:

			std::string make_file_name(const std::string & asset, const std::string & scale) const;

		private:

			candles_container_t load_candles(const path_t & path) const;

			Candle parse(const std::string & line) const;

			void update_deviations(candles_container_t & candles) const;

		private:

			void initialize_sources();

		private:

			void handle_self_similarities();

			void handle_pair_similarities();

			void handle_pair_correlations();

			void handle_deviations();

			void handle_tagged_charts();

			void handle_quik_initialization();

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

			double get_deviation_multiplier(const std::string & scale) const;

		private:

			void save_self_similarities() const;

			void save_pair_similarities() const;

			void save_pair_correlations() const;

			void save_cumulative_distances(const distances_matrix_t & matrix) const;

			void save_deviations() const;

		private:

			void make_supports_resistances();

		private:

			levels_container_t make_levels(const candles_container_t & candles) const;

			levels_container_t reduce_levels(levels_container_t && levels) const;

		private:

			void save_supports_resistances() const;

		private:

			void make_tagged_charts();

		private:

			void update_regression_tags(candles_container_t & candles) const;

			void update_classification_tags(candles_container_t & candles) const;

			void sample_classification_tags(candles_container_t & candles, 
				candles_container_t::iterator position, std::string tag) const;

			void update_supports_resistances(candles_container_t & candles, const levels_container_t & levels) const;

		private:

			void save_tagged_charts() const;

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

			void update_supports_resistances();

		private:

			path_t get_chart_for_levels(const std::string & asset, const std::string & scale) const;

		public:

			std::string get_current_chart(const std::string & asset, const std::string & scale, std::size_t size) const;
			
		private:

			std::string serialize_candles(const candles_container_t & candles) const;

		private:

			static inline const path_t charts_directory = "market/charts";
			static inline const path_t levels_directory = "market/levels";
			static inline const path_t output_directory = "market/output";

		private:

			Config m_config;

			assets_container_t m_assets;

			scales_container_t m_scales;

			sources_container_t m_sources;

			charts_container_t m_charts;

			self_similarities_container_t m_self_similarities;

			pair_similarities_container_t m_pair_similarities;

			pair_correlations_container_t m_pair_correlations;

			supports_resistances_container_t m_supports_resistances;

			thread_pool_t m_thread_pool;
		};

	} // namespace system

} // namespace solution

BOOST_FUSION_ADAPT_STRUCT
(
	 solution::system::Market::Candle,
	(solution::system::Market::Candle::raw_date_t, raw_date)
	(solution::system::Market::Candle::raw_time_t, raw_time)
	(solution::system::Market::Candle::price_t,    price_open)
	(solution::system::Market::Candle::price_t,    price_high)
	(solution::system::Market::Candle::price_t,    price_low)
	(solution::system::Market::Candle::price_t,    price_close)
	(solution::system::Market::Candle::volume_t,   volume)
)

#endif // #ifndef SOLUTION_SYSTEM_MARKET_HPP