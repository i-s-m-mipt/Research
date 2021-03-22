#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../../shared/source/logger/logger.hpp"

#include "system/system.hpp"

using Logger = solution::shared::Logger;
using System = solution::system::System;

int main(int argc, char** argv)
{
	RUN_LOGGER(logger);

	try
	{
		{
			System system;

			system.run();
		}
		
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