#pragma once

#include "pointers.h"
#include "window.h"

#include <string>

struct GLFWwindow;

namespace Storyteller
{
	namespace Dialogs
	{
		std::string OpenFile(const char* filter, Ptr<Window> window);
		std::string SaveFile(const char* filter, Ptr<Window> window, const char* defaultName = "");

		/* Use ImGui messages instead
		enum MessageButtons
		{
			YesNoButtons = 0x1,
			OkButtons = 0x10
		};

		bool Message(const char* text, const char* caption, Ptr<Window> window, MessageButtons buttons = YesNoButtons);
		*/
	};
}