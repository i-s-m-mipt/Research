#ifndef SOLUTION_PLUGIN_WINDOW_HPP
#define SOLUTION_PLUGIN_WINDOW_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#  pragma once
#endif // #ifdef BOOST_HAS_PRAGMA_ONCE

#include <algorithm>
#include <atomic>
#include <cmath>
#include <exception>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "windows.h"

#include "../../../shared/source/logger/logger.hpp"

namespace solution
{
	namespace plugin
	{
		struct gui 
		{
			static gui & instance();

			void create(const std::wstring& title);
			void terminate();

			std::wstring title_;
			std::wstring wnd_class_{ L"qluacpp_draw_candles_rt" };

			void* hwnd_{ nullptr };
			int wnd_height_{ 400 };
			int wnd_width_{ 900 };
			std::atomic<bool> done_{ false };

		private:
			gui() {};
		};

	} // namespace plugin

} // namespace solution

#endif // SOLUTION_PLUGIN_WINDOW_HPP
