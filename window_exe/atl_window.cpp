#include "atl_window.h"

#include <atlbase.h>
#include <atlwin.h>
#include <array>

using namespace atl_window_eg;

struct atl_window::atl_window_implementation : public CWindowImpl<atl_window::atl_window_implementation>
{
	atl_window_implementation() 
	{}

	~atl_window_implementation()
	{
		if (m_hWnd)
		{
			DestroyWindow();
		}
	}

	BEGIN_MSG_MAP(atl_window)
		MESSAGE_HANDLER(WM_DESTROY, on_wnd_destroy)
		MESSAGE_HANDLER(WM_ACTIVATEAPP, on_wnd_activate)
		MESSAGE_HANDLER(WM_SIZE, on_wnd_resize)
		MESSAGE_HANDLER(WM_KEYUP, on_wnd_keypress)
	END_MSG_MAP()

	LRESULT on_wnd_destroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		PostQuitMessage(NULL);
		bHandled = TRUE;
		return 0;
	}

	LRESULT on_wnd_activate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		if (invoke_callback(message_type::activate, wParam, lParam))
		{
			bHandled = TRUE;
			return 0;
		}

		return DefWindowProc(msg, wParam, lParam);
	}

	LRESULT on_wnd_resize(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		if (invoke_callback(message_type::resize, wParam, lParam))
		{
			bHandled = TRUE;
			return 0;
		}

		return DefWindowProc(msg, wParam, lParam);
	}

	LRESULT on_wnd_keypress(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		if (invoke_callback(message_type::keypress, wParam, lParam))
		{
			bHandled = TRUE;
			return 0;
		}

		return DefWindowProc(msg, wParam, lParam);
	}

	bool invoke_callback(message_type msg, WPARAM wParam, LPARAM lParam)
	{
		uint16_t idx = static_cast<uint16_t>(msg);
		auto call = callback_methods.at(idx);
		if (call)
		{
			return call(wParam, lParam);
		}

		return false;
	}

	std::array<callback_method, max_message_types> callback_methods{ nullptr };
};


atl_window::atl_window(const size &window_size, std::wstring_view title, uint16_t res_icon_id)
{
	window_impl = std::make_unique<atl_window_implementation>();

	DWORD window_style = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
	      window_style_ex = WS_EX_OVERLAPPEDWINDOW;

	RECT window_rectangle { 0, 
	                        0, 
	                        window_size.width, 
	                        window_size.height };

	AdjustWindowRectEx(&window_rectangle, window_style, NULL, window_style_ex);

	window_impl->Create(nullptr,
	                    window_rectangle,
	                    title.data(),
	                    window_style,
	                    window_style_ex);

	if (res_icon_id)
	{
		auto icon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(res_icon_id));
		window_impl->SetIcon(icon);
	}

	window_impl->CenterWindow();
}

atl_window::~atl_window()
{}

void atl_window::set_message_callback(message_type msg, const callback_method &callback)
{
	uint16_t message_index = static_cast<uint16_t>(msg);
	window_impl->callback_methods[message_index] = callback;
}

void atl_window::show()
{
	window_impl->ShowWindow(SW_SHOWNORMAL);
	window_impl->SetFocus();
}

void atl_window::process_messages()
{
	BOOL has_more_messages = TRUE;
	while (has_more_messages)
	{
		MSG msg{};

		// Parameter two here has to be nullptr, putting hWnd here will
		// not retrive WM_QUIT messages, as those are posted to the thread
		// and not the window
		has_more_messages = PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE);
		if (msg.message == WM_QUIT)
		{
			return;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

HWND atl_window::handle() const
{
	return window_impl->m_hWnd;
}

