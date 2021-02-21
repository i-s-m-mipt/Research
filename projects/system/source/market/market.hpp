#ifndef SOLUTION_SYSTEM_MARKET_HPP
#define SOLUTION_SYSTEM_MARKET_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <vector>

#define BOOST_PYTHON_STATIC_LIB

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
#include <nlohmann/json_fwd.hpp>

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
			};

		private:

			using assets_container_t = std::vector < std::string > ;

			using scales_container_t = std::vector < std::string > ;

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
				};

			private:

				using path_t = File::path_t;

			private:

				struct Key
				{
					struct Config
					{
						static inline const std::string required_charts            = "required_charts";
						static inline const std::string required_self_similarities = "required_self_similarities";
					};
				};

			public:

				static void load_config(Config & config);

				static void load_assets(assets_container_t & assets);

				static void load_scales(scales_container_t & scales);

			private:

				static void load(const path_t & path, json_t & object);

				static void save(const path_t & path, const json_t & object);
			};

		public:

			struct Bar
			{
				using date_t = std::uint32_t;
				using time_t = std::uint32_t;

				using price_t = double;

				using volume_t = std::uint64_t;

				date_t date = 0U;
				time_t time = 0U;

				price_t price_open  = 0.0;
				price_t price_high  = 0.0;
				price_t price_low   = 0.0;
				price_t price_close = 0.0;

				volume_t volume = 0ULL;
			};

		private:

			template < typename Iterator >
			class Bar_Parser : public boost::spirit::qi::grammar < 
				Iterator, Bar(), boost::spirit::qi::blank_type >
			{
			private:

				using rule_t = boost::spirit::qi::rule < 
					Iterator, Bar(), boost::spirit::qi::blank_type >;

			public:

				Bar_Parser() : Bar_Parser::base_type(start)
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

				~Bar_Parser() noexcept = default;

			private:

				rule_t start;
			};

		private:

			using bars_container_t = std::vector < Bar > ;

			using charts_container_t = std::unordered_map < std::string,
				std::unordered_map < std::string, bars_container_t > > ;

			using self_similarity_matrix_t = boost::multi_array < double, 2U > ;

			using self_similarities_container_t = std::unordered_map < std::string,
				self_similarity_matrix_t > ;

		public:

			Market()
			{
				initialize();
			}

			~Market() noexcept = default;

		private:

			void initialize();

		private:

			void load();

		private:

			void load_config();

			void load_assets();

			void load_scales();

			void load_charts();

			bars_container_t load_bars(const path_t & path) const;

			Bar parse(const std::string & s) const;

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

			const auto & self_similarities() const noexcept
			{
				return m_self_similarities;
			}

		public:
	
			path_t get_chart(const std::string & asset, const std::string & scale) const;

			std::pair < path_t, std::size_t > get_all_charts() const;

			void compute_self_similarities();

		private:

			double compute_self_similarity(const std::string & asset,
				const std::string & scale_1, const std::string & scale_2) const;

			std::string make_file_name(
				const std::string & asset, const std::string & scale) const;
			
		private:

			static inline const path_t directory = "market/charts";

		private:

			Config m_config;

			assets_container_t m_assets;

			scales_container_t m_scales;

			charts_container_t m_charts;

			self_similarities_container_t m_self_similarities;
		};

	} // namespace system

} // namespace solution

BOOST_FUSION_ADAPT_STRUCT
(
	 solution::system::Market::Bar,
	(solution::system::Market::Bar::date_t,   date)
	(solution::system::Market::Bar::time_t,   time)
	(solution::system::Market::Bar::price_t,  price_open)
	(solution::system::Market::Bar::price_t,  price_high)
	(solution::system::Market::Bar::price_t,  price_low)
	(solution::system::Market::Bar::price_t,  price_close)
	(solution::system::Market::Bar::volume_t, volume)
)

#endif // #ifndef SOLUTION_SYSTEM_MARKET_HPP