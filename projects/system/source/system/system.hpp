#ifndef SOLUTION_SYSTEM_SYSTEM_HPP
#define SOLUTION_SYSTEM_SYSTEM_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <array>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <exception>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
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
						static inline const std::string required_price_deviations     = "required_price_deviations";
						static inline const std::string required_tagged_charts        = "required_tagged_charts";
						static inline const std::string required_environment          = "required_environment";
						static inline const std::string required_local_environment    = "required_local_environment";
						static inline const std::string min_price_change              = "min_price_change";
						static inline const std::string max_price_rollback            = "max_price_rollback";
						static inline const std::string level_max_deviation           = "level_max_deviation";
						static inline const std::string level_max_lifetime            = "level_max_lifetime";
						static inline const std::string level_max_strength            = "level_max_strength";
						static inline const std::string level_resolution              = "level_resolution";
						static inline const std::string level_min_bias                = "level_min_bias";
						static inline const std::string level_max_bias                = "level_max_bias";
						static inline const std::string critical_deviation            = "critical_deviation";
						static inline const std::string required_level_reduction      = "required_level_reduction";
						static inline const std::string required_quik                 = "required_quik";
						static inline const std::string required_supports_resistances = "required_supports_resistances";
						static inline const std::string required_consultation_mode    = "required_consultation_mode";
						static inline const std::string classification_max_deviation  = "classification_max_deviation";
						static inline const std::string run_julia_test                = "run_julia_test";
						static inline const std::string prediction_timeframe          = "prediction_timeframe";
						static inline const std::string prediction_timesteps          = "prediction_timesteps";
						static inline const std::string transaction_base_value        = "transaction_base_value";
						static inline const std::string days_for_dividends            = "days_for_dividends";
						static inline const std::string deviation_threshold           = "deviation_threshold";
						static inline const std::string run_model_sensibility_test    = "run_model_sensibility_test";
						static inline const std::string model_stabilization_time      = "model_stabilization_time";
						static inline const std::string run_fridays_test              = "run_fridays_test";
						static inline const std::string required_sentiment_service    = "required_sentiment_service";
						static inline const std::string telegram_username             = "telegram_username";
						static inline const std::string telegram_api_id               = "telegram_api_id";
						static inline const std::string telegram_api_hash             = "telegram_api_hash";
						static inline const std::string volume_timesteps              = "volume_timesteps";
						static inline const std::string run_mornings_test             = "run_mornings_test";
						static inline const std::string mornings_test_asset           = "mornings_test_asset";
						static inline const std::string mornings_test_scale           = "mornings_test_scale";
						static inline const std::string mornings_test_delta           = "mornings_test_delta";
						static inline const std::string mornings_test_year            = "mornings_test_year";
						static inline const std::string mornings_test_confidence      = "mornings_test_confidence";
						static inline const std::string run_intraday_test             = "run_intraday_test";
						static inline const std::string intraday_test_deviation       = "intraday_test_deviation";
						static inline const std::string intraday_test_asset           = "intraday_test_asset";
						static inline const std::string intraday_test_scale           = "intraday_test_scale";
						static inline const std::string intraday_test_year            = "intraday_test_year";
						static inline const std::string local_environment_test_asset  = "local_environment_test_asset";
						static inline const std::string local_environment_test_scale  = "local_environment_test_scale";
						static inline const std::string run_local_environment_test    = "run_local_environment_test";
						static inline const std::string knn_method_parameter          = "knn_method_parameter";
						static inline const std::string geometric_progression_q       = "geometric_progression_q";
						static inline const std::string movement_timesteps            = "movement_timesteps";
						static inline const std::string knn_method_timesteps          = "knn_method_timesteps";
						static inline const std::string max_waves_sequence            = "max_waves_sequence";
						static inline const std::string local_environment_test_start  = "local_environment_test_start";
						static inline const std::string max_wave_length               = "max_wave_length";
						static inline const std::string required_test_data            = "required_test_data";
						static inline const std::string test_data_start               = "test_data_start";
						static inline const std::string skipped_timesteps             = "skipped_timesteps";
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

		private:

			struct State
			{
				static inline const std::string C = "C";
				static inline const std::string L = "L";
				static inline const std::string S = "S";
			};

		private:

			struct Transaction
			{
				std::string asset;
				std::string operation;

				double position;
			};

		private:

			using transactions_container_t = std::vector < Transaction > ;

			using clock_t = std::chrono::steady_clock;

			using time_point_t = clock_t::time_point;

			using states_container_t = std::unordered_map < std::string,
				std::pair < std::string, time_point_t > > ;

		private:

			struct Dividend
			{
			public:

				struct Key
				{
					static inline const std::string asset    = "asset";
					static inline const std::string dividend = "dividend";
					static inline const std::string buy_date = "buy_date";
					static inline const std::string gap_date = "gap_date";
				};

			public:

				double dividend;
				
				std::time_t buy_date;
				std::time_t gap_date;
			};

		private:

			using dividends_container_t = std::unordered_map < std::string, Dividend > ;

			using json_t = nlohmann::json;

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

			void load_dividends();

		private:

			void initialize_shared_memory();

		private:

			std::string make_shared_memory_name() const;

		private:

			void run_julia_test() const;

		public:

			void run();

		private:

			void run_autonomous();

			void run_consulting();

		private:

			void run_model_sensibility_test(const boost::python::object & function) const;

			void run_sentiment_service() const;

			bool is_session_open() const;

			void get_plugin_data();

			void handle_data(const boost::python::object & function);

			bool is_state_stable(const std::string & asset, const std::string & state);

			void handle_state(const std::string & asset, const std::string & state);

			bool has_dividends(const std::string & asset) const;

			void insert_transaction(const std::string & asset, const std::string & operation, double position);

			void set_server_data() const;

		private:

			static inline const std::time_t seconds_in_day = 86400LL;

			static inline const std::time_t initialization_delay = 10LL;

		private:

			Config m_config;

			dividends_container_t m_dividends;

			std::unique_ptr < Market > m_market;

			double m_available_money = 0.0;

			holdings_container_t m_holdings;

			transactions_container_t m_transactions;

			states_container_t m_states;

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