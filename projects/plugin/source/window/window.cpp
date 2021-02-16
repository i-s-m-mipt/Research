#include "window.hpp"

namespace solution
{
	namespace plugin
	{
		LRESULT CALLBACK wnd_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
		{
			auto& g = gui::instance();

			switch (uMsg) 
			{
			case WM_CREATE:
				return 0;
			case WM_PAINT: 
			{
				return 0;
			}
			case WM_SIZE:
				return 0;
			case WM_DESTROY:
				g.done_ = true;
				return 0;
			default:
				return DefWindowProc(hwnd, uMsg, wParam, lParam);
			}
			return 0;
		}

		static std::thread message_thread;

		void thread_proc() 
		{
			auto hinstance = GetModuleHandle(NULL);
			auto& hwnd = gui::instance().hwnd_;
			hwnd = CreateWindowA("qluacpp_draw_candles_rt",        // name of window class 
				"title",            // title-bar string 
				WS_SYSMENU | WS_CAPTION, // top-level window 
				CW_USEDEFAULT,       // default horizontal position 
				CW_USEDEFAULT,       // default vertical position 
				gui::instance().wnd_width_,       // default width 
				gui::instance().wnd_height_,       // default height 
				(HWND)NULL,         // no owner window 
				(HMENU)NULL,        // use class menu 
				hinstance,           // handle to application instance 
				(LPVOID)NULL);      // no window-creation data
			if (!hwnd)
				throw std::runtime_error("Could not create window");
			ShowWindow((HWND)hwnd, SW_SHOWNORMAL);
			UpdateWindow((HWND)hwnd);

			MSG msg;
			while (!gui::instance().done_) {
				auto rslt = GetMessage(&msg, (HWND)hwnd, 0, 0);
				switch (rslt) {
				case -1:
					return;
				case 0:
					return;
				default:
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			hwnd = nullptr;
		}

		gui& gui::instance() {
			static gui g;
			return g;
		}

		void gui::create(const std::wstring& title) 
		{
			if (hwnd_ != nullptr) {
				PostMessage((HWND)hwnd_, WM_DESTROY, 0, 0);
			}
			hwnd_ = nullptr;
			done_ = false;
			title_ = title;
			auto hinstance = GetModuleHandle(NULL);
			auto const chart_height = std::lround(wnd_height_ * 0.4);
			auto const bottom_height = std::lround(wnd_height_ * 0.6);

			WNDCLASS wc;
			if (!GetClassInfo(hinstance, wnd_class_.c_str(), &wc)) {
				// Register the main window class. 
				wc.style = CS_HREDRAW | CS_VREDRAW;
				wc.cbClsExtra = 0;
				wc.cbWndExtra = 0;
				wc.hInstance = hinstance;
				wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
				wc.hCursor = LoadCursor(NULL, IDC_ARROW);
				wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
				wc.lpszMenuName = NULL;
				wc.lpszClassName = wnd_class_.c_str();
				wc.lpfnWndProc = wnd_proc;

				if (!RegisterClass(&wc))
					throw std::runtime_error("Could not register window class");
			}
			message_thread = std::move(std::thread(thread_proc));
			message_thread.detach();
		}

		void gui::terminate() {
			if (hwnd_ != nullptr) {
				PostMessage((HWND)hwnd_, WM_DESTROY, 0, 0);
			}
		}

	} // namespace plugin

} // namespace solution