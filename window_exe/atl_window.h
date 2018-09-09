#pragma once

#include <Windows.h>
#include <string_view>
#include <memory>
#include <cstdint>
#include <functional>

namespace atl_window_eg
{
	class atl_window
	{
	public:
		struct size
		{
			uint16_t width;
			uint16_t height;
		};

		enum class message_type
		{
			resize,
			activate,
			keypress
		};
		static constexpr uint8_t max_message_types = 3;

		//using callback_method = auto (*) (uintptr_t, uintptr_t) -> bool;
		using callback_method = std::function<bool(uintptr_t, uintptr_t)>;

	public:
		atl_window(const size &window_size, std::wstring_view title, uint16_t res_icon_id = 0);
		~atl_window();

		void set_message_callback(message_type msg, const callback_method &callback);
		void show();
		void process_messages();

		HWND handle() const;

	private:
		struct atl_window_implementation;
		std::unique_ptr<atl_window_implementation> window_impl;
	};
}