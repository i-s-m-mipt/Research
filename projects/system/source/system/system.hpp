#ifndef SOLUTION_SYSTEM_SYSTEM_HPP
#define SOLUTION_SYSTEM_SYSTEM_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../config/config.hpp" // !

#include <julia.h>

#if defined(snprintf)
#  undef snprintf
#endif // #if defined(snprintf)

#if defined(strtoull)
#  undef strtoull
#endif // #if defined(strtoull)

#if defined(strtoll)
#  undef strtoll
#endif // #if defined(strtoll)

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <nlohmann/json.hpp>

#include "../config/config.hpp"
#include "../market/market.hpp"

#include "../../../shared/source/logger/logger.hpp"
#include "../../../shared/source/python/python.hpp"

namespace solution 
{
	namespace system
	{
		class system_exception : public std::exception
		{
		public:

			explicit system_exception(const std::string & message) noexcept : 
				std::exception(message.c_str()) 
			{}

			explicit system_exception(const char * const message) noexcept :
				std::exception(message)
			{}

			~system_exception() noexcept = default;
		};

		class System
		{
		private:

			class Data
			{
			private:

				using json_t = nlohmann::json;

			private:

				struct File
				{
					using path_t = std::filesystem::path;

					static inline const path_t config_json = "system/config.json";
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
						static inline const std::string level_max_lifetime            = "level_max_lifetime";
						static inline const std::string level_max_strength            = "level_max_strength";
						static inline const std::string level_resolution              = "level_resolution";
						static inline const std::string level_frame                   = "level_frame";
						static inline const std::string required_quik                 = "required_quik";
						static inline const std::string required_supports_resistances = "required_supports_resistances";
						static inline const std::string classification_max_deviation  = "classification_max_deviation";
						static inline const std::string run_julia_test                = "run_julia_test";
						static inline const std::string prediction_timeframe          = "prediction_timeframe";
						static inline const std::string prediction_timesteps          = "prediction_timesteps";
						static inline const std::string transaction_base_value        = "transaction_base_value";
					};
				};

			public:

				static void load_config(Config & config);

			private:

				static void load(const path_t & path, json_t & object);

				static void save(const path_t & path, const json_t & object);
			};

		private:

			using shared_memory_t = boost::interprocess::managed_shared_memory;

			using segment_manager_t = shared_memory_t::segment_manager;

		private:

			struct Plugin_Data
			{
			public:

				using char_allocator_t = boost::interprocess::allocator < char, segment_manager_t > ;

				using string_t = boost::interprocess::basic_string < char, std::char_traits < char >, char_allocator_t > ;

				using holding_t = std::pair < string_t, double > ;

				using holding_allocator_t = boost::interprocess::allocator < holding_t, segment_manager_t > ;

				using holdings_container_t = boost::interprocess::vector < holding_t, holding_allocator_t > ;

			public:

				explicit Plugin_Data(const holding_allocator_t & allocator) :
					is_updated(false), available_money(0.0), holdings(allocator)
				{}

				~Plugin_Data() noexcept = default;

			public:

				bool is_updated;

				double available_money;

				holdings_container_t holdings;
			};

		private:

			struct Server_Data
			{
			public:

				using char_allocator_t = boost::interprocess::allocator < char, segment_manager_t > ;

				using string_t = boost::interprocess::basic_string < char, std::char_traits < char >, char_allocator_t > ;

			public:

				struct Transaction
				{
					string_t asset_code;
					string_t operation;
					string_t position;
				};

			public:

				using transaction_allocator_t = boost::interprocess::allocator < Transaction, segment_manager_t > ;

				using transactions_container_t = boost::interprocess::vector < Transaction, transaction_allocator_t > ;

			public:

				explicit Server_Data(const transaction_allocator_t & allocator) :
					is_updated(false), transactions(allocator)
				{}

				~Server_Data() noexcept = default;

			public:

				bool is_updated;

				transactions_container_t transactions;
			};

		private:

			using condition_t = boost::interprocess::interprocess_condition;

			using mutex_t = boost::interprocess::interprocess_mutex;

			using holdings_container_t = std::unordered_map < std::string, double > ;

			using transaction_t = Server_Data::Transaction;

			using transactions_container_t = std::vector < transaction_t > ;

		private:

			struct Signal
			{
				static inline const std::string C = "C";
				static inline const std::string L = "L";
				static inline const std::string S = "S";
			};

		public:

			System()
			{
				initialize();
			}

			~System() noexcept
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

		private:

			void initialize_shared_memory();

		private:

			std::string make_shared_memory_name() const;

		private:

			void run_julia_test() const;

		public:

			void run();

		private:

			void get_plugin_data();

			void handle_data(const boost::python::object & function) const;

			void handle_signal(const std::string & signal) const;

			void set_server_data() const;

		private:

			Config m_config;

			Market m_market;

			double m_available_money = 0.0;

			holdings_container_t m_holdings;

		private:

			shared_memory_t m_shared_memory;

			Plugin_Data * m_plugin_data;
			Server_Data * m_server_data;

			mutex_t * m_plugin_mutex;
			mutex_t * m_server_mutex;

			condition_t * m_plugin_condition;
			condition_t * m_server_condition;
		};

	} // namespace system

} // namespace solution

#endif // #ifndef SOLUTION_SYSTEM_SYSTEM_HPP