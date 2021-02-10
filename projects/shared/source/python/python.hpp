#ifndef SOLUTION_SHARED_PYTHON_HPP
#define SOLUTION_SHARED_PYTHON_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <exception>
#include <mutex>
#include <stdexcept>
#include <string>

#define BOOST_PYTHON_STATIC_LIB

#include <boost/python.hpp>

#include "../../../shared/source/logger/logger.hpp"

namespace solution
{
	namespace shared
	{
		class python_exception : public std::exception
		{
		public:

			explicit python_exception(const std::string & message) noexcept :
				std::exception(message.c_str())
			{}

			explicit python_exception(const char * const message) noexcept :
				std::exception(message)
			{}

			~python_exception() noexcept = default;
		};

		class Python
		{
		public:

			Python()
			{
				initialize();
			}

			~Python() noexcept
			{
				try
				{
					uninitialize();
				}
				catch (...)
				{
					std::abort();
				}
			}

		private:

			void initialize();

			void uninitialize();

		public:

			auto & global() const noexcept
			{
				return m_global;
			}

		public:

			static std::string exception() noexcept;

		private:

			static inline wchar_t directory[] = L"C:/Libraries/python";

		private:

			static std::once_flag is_initialized_once;

			static std::mutex       mutex;
			static PyGILState_STATE state;

		private:

			boost::python::api::object m_global;
		};

		// ===========================================================================

		class Python_Outer_Interpreter
		{
		public:

			Python_Outer_Interpreter()
			{
				initialize();
			}

			~Python_Outer_Interpreter() noexcept
			{
				try
				{
					uninitialize();
				}
				catch (...)
				{
					std::abort();
				}
			}

		private:

			void initialize();

			void uninitialize();

		private:

			static inline wchar_t directory[] = L"C:/Libraries/python";

		private:

			PyGILState_STATE m_gil_state;
			PyThreadState * m_thread_state;
		};

		// ===========================================================================

		class Python_Inner_Interpreter
		{
		public:

			Python_Inner_Interpreter()
			{
				initialize();
			}

			~Python_Inner_Interpreter() noexcept
			{
				try
				{
					uninitialize();
				}
				catch (...)
				{
					std::abort();
				}
			}

		private:

			void initialize();

			void uninitialize();

		private:

			PyGILState_STATE m_outer_interpreter_gil_state;
			PyGILState_STATE m_inner_interpreter_gil_state;

			PyThreadState * m_thread_state;

			PyThreadState * m_old_thread_state;
			PyThreadState * m_new_thread_state;
		};

	} // namespace shared

} // namespace solution

#endif // #ifndef SOLUTION_SHARED_PYTHON_HPP