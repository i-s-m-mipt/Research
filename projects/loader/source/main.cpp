#include <ctime>
#include <exception>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include <boost/process.hpp>

#include <windows.h>

#include "../../shared/source/logger/logger.hpp"

using Logger = solution::shared::Logger;

class loader_exception : public std::exception
{
public:

	explicit loader_exception(const std::string & message) noexcept :
		std::exception(message.c_str())
	{}

	explicit loader_exception(const char * const message) noexcept :
		std::exception(message)
	{}

	~loader_exception() noexcept = default;
};

bool is_session_open()
{
	RUN_LOGGER(logger);

	try
	{
		auto time = std::time(nullptr);

		auto tm = *std::localtime(&time);

		return (
			((tm.tm_hour >  9) || (tm.tm_hour ==  9 && tm.tm_min > 50)) &&
			((tm.tm_hour < 23) || (tm.tm_hour == 23 && tm.tm_min < 59)));
	}
	catch (const std::exception & exception)
	{
		solution::shared::catch_handler < loader_exception > (logger, exception);
	}
}

int main(int argc, char ** argv)
{
	RUN_LOGGER(logger);

	try
	{
		while (!is_session_open())
		{
			std::this_thread::yield();
		}

		boost::process::child quik("info.exe");

		while (is_session_open())
		{
			std::this_thread::yield();
		}

		auto handler_main_window = FindWindow(L"InfoClass", 0);

		if (handler_main_window)
		{
			PostMessage(handler_main_window, WM_CLOSE, 0, 0);
		}
		else
		{
			throw std::runtime_error("main window not found");
		}

		const std::filesystem::path path = "info.log";

		std::filesystem::remove(path);

		system("pause");

		return EXIT_SUCCESS;
	}
	catch (const std::exception & exception)
	{
		logger.write(Logger::Severity::fatal, exception.what());

		system("pause");

		return EXIT_FAILURE;
	}
	catch (...)
	{
		logger.write(Logger::Severity::fatal, "unknown exception");

		system("pause");

		return EXIT_FAILURE;
	}
}