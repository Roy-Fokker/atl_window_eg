#include "atl_window.h"

#include <string>
#include <Windows.h>

auto main() -> int
{
	std::wstring cmd_line(GetCommandLine());

	{
		using namespace atl_window_eg;

		bool exit_program = false;
		atl_window wnd({ 800, 600 }, L"ATL Window");

		wnd.set_message_callback(atl_window::message_type::keypress,
								 [&exit_program](uintptr_t key_code, uintptr_t extension) -> bool
		{
			if (key_code == VK_ESCAPE)
			{
				exit_program = true;
			}

			return true;
		});

		wnd.show();
		while (wnd.handle() and (not exit_program))
		{
			wnd.process_messages();
		}
	}
	return 0;
}