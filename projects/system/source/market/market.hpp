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
#include <vector>

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "../../../../shared/source/logger/logger.hpp"
#include "../../../../shared/source/python/python.hpp"

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

			using clock_t = std::chrono::system_clock;

			using time_point_t = std::chrono::time_point < clock_t > ;

			using date_t = std::string;

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

				struct File
				{
					using path_t = std::filesystem::path;

					static inline const path_t assets = "market/data/assets.data";
					static inline const path_t scales = "market/data/scales.data";
				};

			private:

				using path_t = File::path_t;

			public:

				static void load_assets(assets_container_t & assets);

				static void load_scales(scales_container_t & scales);
			};

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

			void load_assets();

			void load_scales();

		public:

			const auto & assets() const noexcept
			{
				return m_assets;
			}

			const auto & scales() const noexcept
			{
				return m_scales;
			}

		public:

			path_t get(const std::string & asset, const std::string & scale, 
				time_point_t first, time_point_t last) const;
						
			path_t get(const std::string & asset, const std::string & scale) const;

			std::pair < path_t, std::size_t > get_all() const;

		private:

			date_t make_date(time_point_t time_point) const;
			
		private:

			static inline const path_t directory = "market/bars";

		private:

			assets_container_t m_assets;

			scales_container_t m_scales;
		};

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_MARKET_HPP
