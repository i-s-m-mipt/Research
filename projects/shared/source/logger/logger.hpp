#ifndef SOLUTION_SHARED_LOGGER_HPP
#define SOLUTION_SHARED_LOGGER_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#ifndef _HAS_AUTO_PTR_ETC
#define _HAS_AUTO_PTR_ETC 1
#endif // #ifndef _HAS_AUTO_PTR_ETC

#if defined(_MSC_VER) || defined(__GNUC__)
#  define FUNCTION __FUNCTION__ /*__FUNCSIG__*/
#else
#  define FUNCTION __func__
#endif // #if defined(_MSC_VER) || defined(__GNUC__)

#ifndef NDEBUG
#  define ENABLE_LOGGER_RAII_TRACE true
#else
#  define ENABLE_LOGGER_RAII_TRACE false
#endif

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>

#include <boost/config/user.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/exceptions.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/support/exception.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/utility/value_ref.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/make_shared.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace solution
{
	namespace shared
	{
		class logger_exception : public std::exception 
		{
		public:

			explicit logger_exception(const std::string & message) noexcept :
				std::exception(message.c_str())
			{}

			explicit logger_exception(const char * const message) noexcept :
				std::exception(message)
			{}

			~logger_exception() noexcept = default;
		};

		class Logger : private boost::noncopyable
		{			
		private:

			enum class Attribute_Index
			{
				line,
				time,
				severity,
				thread,
				process				
			};

		private:

			using attributes_container_t = std::unordered_map < Attribute_Index,
				std::pair < std::string, std::shared_ptr < boost::log::attribute > > > ;

		public:

			enum class Severity
			{
				empty,
				debug,
				trace,
				error,
				fatal
			};

		private:

			using severities_container_t = std::unordered_map < Severity, std::string > ;

		private:

			using logger_t = boost::log::sources::severity_logger_mt < Severity > ;

			using status_t = bool;

		private:

			using sink_t = boost::log::sinks::synchronous_sink < boost::log::sinks::text_file_backend > ;

		public:

			template < typename S, typename Enable =
				std::enable_if_t < std::is_convertible_v < S, std::string > > >
			explicit Logger(S && scope) : m_scope(std::forward < S > (scope))
			{
				try
				{
					std::call_once(is_initialized_once, &Logger::initialize);

					if (ENABLE_LOGGER_RAII_TRACE)
					{
						write(Severity::trace, "EXECUTION ... ");
					}
				}
				catch (const std::exception & exception)
				{
					catch_handler(FUNCTION, exception);
				}
			}

			~Logger() noexcept
			{
				try
				{
					if (ENABLE_LOGGER_RAII_TRACE)
					{
						write(Severity::trace, "EXECUTION COMPLETE");
					}
				}
				catch (...)
				{
					// std::abort();
				}
			}

		private:

			static void initialize();

			static void uninitialize();

		private:

			static void add_attributes();

			static void add_sinks();

			static void remove_attributes();

			static void remove_sinks();

		private:

			static boost::shared_ptr < sink_t > make_fout_sink();

		private:

			static void fout_formatter(const boost::log::record_view & record_view, boost::log::formatting_ostream & stream);

			static bool fout_filter(const boost::log::attribute_value_set & attribute_value_set);

		public:

			const auto & scope() const noexcept
			{
				return m_scope;
			}

		public:

			void write(Severity severity, const std::string & message) const noexcept;

		private:

			static void catch_handler(const std::string & scope, const std::exception & exception) noexcept
			{
				try
				{
					std::cerr << scope << delimeter << exception.what() << '\n';
				}
				catch (...)
				{
					// std::abort();
				}
			}

			template < typename E >
			static void	catch_handler(const std::string & scope, const std::exception & exception)
			{
				static_assert(std::is_same_v < logger_exception, E > , "bad exception type");

				try
				{
					std::cerr << scope << delimeter << exception.what() << '\n';
				}
				catch (...)
				{
					// std::abort();
				}

				throw E(scope + " " + "exception");
			}

		private:

			static inline const std::string delimeter = " : ";

		private:

			static const attributes_container_t attributes;

			static const severities_container_t severities;

		private:

			static std::once_flag is_initialized_once;

			static logger_t logger;

			static status_t status;

		private:

			const std::string m_scope;
		};

		template < typename E >
		void catch_handler(Logger & logger, const std::exception & exception)
		{
			static_assert(std::is_base_of_v < std::exception, E >, "bad exception type");

			logger.write(Logger::Severity::error, exception.what());

			throw E(logger.scope() + " " + "exception");
		}

		template < typename E >
		void catch_handler(Logger & logger)
		{
			static_assert(std::is_base_of_v < std::exception, E >, "bad exception type");

			logger.write(Logger::Severity::error, "unknown exception");

			throw E(logger.scope() + " " + "exception");
		}

		void catch_handler(Logger & logger, const std::exception & exception) noexcept;

	} // namespace shared

} // namespace solution 

#define RUN_LOGGER(logger_name) \
 solution::shared::Logger logger_name(FUNCTION);

#if defined(_DEBUG)
#  define LOGGER_WRITE_DEBUG(logger_name, message) \
   logger_name.write(solution::shared::Logger::Severity::debug, message);
#else
#  define LOGGER_WRITE_DEBUG(logger_name, message) ;
#endif // #if defined(_DEBUG)

#define RUN_NAMED_SCOPE_LOGGER(logger_name, scope_name) \
 solution::shared::Logger logger_name(scope_name);

#define LOGGER_WRITE(logger_name, message) \
 logger_name.write(solution::shared::Logger::Severity::trace, message);

#endif // #ifndef SOLUTION_SHARED_LOGGER_HPP