#include "window.h"

#include <string>
#include <Windows.h>

// Standard main function. don't know if i like this format of writing 'main'
auto main() -> int
{
	// Get Command Line parameters as wstring, instead of typical argc**
	std::wstring cmd_line(GetCommandLine());

	// Scoped for focus
	{
		// use only within this scope
		using namespace atl_window_eg;

		bool exit_program = false;	// parameter to let loop below know to stop

		// create window object, with size and title
		window wnd({ 800, 600 }, L"ATL/Win32 Window Example");

		// set callback, with lambda here, such that pressing escape will exit the loop below
		wnd.set_message_callback(window::message_type::keypress,
		                         [&exit_program](uintptr_t key_code, uintptr_t extension) -> bool
		{
			if (key_code == VK_ESCAPE)
			{
				exit_program = true;
			}

			return true;
		});

		// show the window
		wnd.show();

		// loop while windows exists and exit_program is false
		while (wnd.handle() and (not exit_program))
		{
			// check for and process window messages
			wnd.process_messages();
		}
	}
	
	return 0;
}