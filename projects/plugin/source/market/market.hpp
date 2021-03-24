#ifndef SOLUTION_PLUGIN_MARKET_HPP
#define SOLUTION_PLUGIN_MARKET_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <chrono>
#include <exception>
#include <filesystem>
#include <future>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <boost/asio.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <nlohmann/json.hpp>

#include "source/source.hpp"

#include "../../../shared/source/logger/logger.hpp"

namespace solution
{
	namespace plugin
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
		private:

			struct Config
			{
				std::string id;
				std::string code;
				std::string account;
			};

		private:

			using scales_container_t = std::vector < std::string > ;

			using assets_container_t = 
				std::vector < std::pair < std::string, std::string > > ;

			using Source = market::Source;

			using sources_container_t = std::unordered_map < std::string, 
				std::unordered_map < std::string, std::shared_ptr < Source > > > ;

			using holdings_container_t = std::unordered_map < std::string, double > ;

			using transaction_t = std::unordered_map < std::string, std::string > ;

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

				struct Key
				{
					struct Asset
					{
						static inline const std::string class_code = "class_code";
						static inline const std::string asset_code = "asset_code";
					};

					struct Config
					{
						static inline const std::string id      = "id";
						static inline const std::string code    = "code";
						static inline const std::string account = "account";
					};
				};

			private:

				using path_t = File::path_t;

			public:

				static void load_config(Config & config);

				static void load_assets(assets_container_t & assets);

				static void load_scales(scales_container_t & scales);

			private:

				static void load(const path_t & path, json_t & object);				
			};

		public:

			enum class Status
			{
				running,
				stopped
			};

		private:

			using shared_memory_t = boost::interprocess::managed_shared_memory;		

			using segment_manager_t = shared_memory_t::segment_manager;

		private:

			struct Plugin_Data
			{
			public:

				using char_allocator_t = boost::interprocess::allocator < char, segment_manager_t > ;

				using string_t = boost::interprocess::basic_string < char, std::char_traits < char > , char_allocator_t > ;

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

			using thread_pool_t = boost::asio::thread_pool;

		public:

			explicit Market(detail::lua::State state) : 
				m_state(state), m_engine(static_cast < unsigned int > (
					std::chrono::system_clock::now().time_since_epoch().count())),
				m_thread_pool(std::thread::hardware_concurrency())
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

		private:

			void initialize_shared_memory();

		private:

			std::string make_shared_memory_name() const;

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

			void run();

			void stop();

		private:

			void set_plugin_data() const;

			void get_server_data() const;

		private:

			double get_available_money() const;

			holdings_container_t get_holdings() const;

			void update_sources() const;

		private:

			unsigned long generate_transaction_id() const;

			std::size_t compute_lot_quantity(const std::string & asset_code, double position) const;

			std::size_t get_lot_size(const std::string & class_code, const std::string & asset_code) const;

		public:

			bool is_connected() const;

			void send_message(const std::string & message) const;

			std::string send_transaction(const transaction_t & transaction) const;

		private:

			const detail::lua::State m_state;

		private:

			Config m_config;

			assets_container_t m_assets;

			scales_container_t m_scales;

			sources_container_t m_sources;

		private:

			shared_memory_t m_shared_memory;

			Plugin_Data * m_plugin_data;
			Server_Data * m_server_data;

			mutex_t * m_plugin_mutex;
			mutex_t * m_server_mutex;

			condition_t * m_plugin_condition;
			condition_t * m_server_condition;

			thread_pool_t m_thread_pool;

		private:

			mutable std::default_random_engine m_engine;

			mutable std::mutex m_market_mutex;

			mutable std::atomic < Status > m_status;
		};

	} // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_MARKET_HPP