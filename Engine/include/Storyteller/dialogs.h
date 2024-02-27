#pragma once

#include <string>
#include <vector>

namespace Storyteller
{
	namespace Dialogs
	{
		std::string OpenFile(const std::string& title, const std::vector<std::string>& filters);
		//--------------------------------------------------------------------------

		std::vector<std::string> OpenFiles(const std::string& title, const std::vector<std::string>& filters);
		//--------------------------------------------------------------------------

		std::string SaveFile(const std::string& title, const std::vector<std::string>& filters);
		//--------------------------------------------------------------------------

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