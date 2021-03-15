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
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

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

			using sources_container_t = std::vector < std::shared_ptr < Source > > ;

			using holdings_container_t = std::unordered_map < std::string, double > ;

			using transaction_t = std::unordered_map < std::string, std::string > ; // TODO

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

			/*
			class Task
			{
			public:

				Task(std::shared_ptr < Source > source, std::atomic < Status > & status) :
					m_source(source), m_status(status)
				{}

				~Task() noexcept = default;

			public:

				void operator()() const;

			private:

				static const inline std::size_t critical_error_quantity = 60;

			private:

				std::shared_ptr < Source > m_source;

			private:

				std::atomic < Status > & m_status;

			private:

				mutable std::size_t m_error_counter = 0;
			};
			*/

		private:

			using shared_memory_t = boost::interprocess::managed_shared_memory;			

		private:

			struct Plugin_Data
			{
			public:

				using char_allocator_t = boost::interprocess::allocator < char, shared_memory_t::segment_manager > ;

				using string_t = boost::interprocess::basic_string < char, std::char_traits < char > , char_allocator_t > ;

				using holding_t = std::pair < string_t, double > ;

				using holding_allocator_t = boost::interprocess::allocator < holding_t, shared_memory_t::segment_manager > ;

				using holdings_container_t = boost::unordered_map < string_t, double,
					boost::hash < string_t > , std::equal_to < string_t > , holding_allocator_t > ;

			public:

				explicit Plugin_Data(const holding_allocator_t & allocator) : 
					available_money(0.0), holdings(allocator), is_updated(false)
				{}

				~Plugin_Data() noexcept = default;

			public:

				bool is_updated;

				double available_money;

				holdings_container_t holdings;
			};

		private:

			using condition_t = boost::interprocess::interprocess_condition;

			using mutex_t = boost::interprocess::interprocess_mutex;

		//private: // TODO

		//	struct Transaction
		//	{
		//		string_t asset_code; // SBER
		//		string_t operation;  // B
		//		string_t quantity;   // 100
		//	};

		//private: // TODO

		//	using transaction_allocator_t =
		//		boost::interprocess::allocator < Transaction, shared_memory_t::segment_manager > ;

		//	using transactions_container_t = boost::interprocess::vector < Transaction, transaction_allocator_t > ;

		public:

			explicit Market(detail::lua::State state) : m_state(state)
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

			void update_shared_memory() const;

		private:

			double get_available_money() const;

			holdings_container_t get_holdings() const;

			void update_sources() const;

		private:

			std::size_t get_lot_size(const std::string & class_code, const std::string & asset_code) const;

		public:

			bool is_connected() const;

			void send_message(const std::string & message) const;

			std::string send_transaction(const transaction_t & transaction) const; // TODO

		private:

			const detail::lua::State m_state;

		private:

			Config m_config;

			assets_container_t m_assets;

			scales_container_t m_scales;

			sources_container_t m_sources;

		private:

			shared_memory_t m_shared_memory;

			condition_t * m_condition;

			mutex_t * m_mutex;

			Plugin_Data * m_plugin_data;

		private:

			mutable std::mutex m_market_mutex;

			mutable std::atomic < Status > m_status;
		};

	} // namespace plugin

} // namespace solution

#endif // #ifndef SOLUTION_PLUGIN_MARKET_HPP