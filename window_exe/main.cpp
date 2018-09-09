#include "atl_window.h"

#include <string>
#include <Windows.h>

auto main() -> int
{
	std::wstring cmd_line(GetCommandLine());

	{
		using namespace atl_window_eg;

		atl_window wnd({ 800, 600 }, L"ATL Window");

		wnd.show();
		while (wnd.handle())
		{
			wnd.process_messages();
		}
	}
	return 0;
}