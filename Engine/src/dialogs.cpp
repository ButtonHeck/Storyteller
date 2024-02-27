#include "dialogs.h"

#include <portable-file-dialogs.h>
#include <filesystem>

namespace Storyteller
{
	namespace Dialogs
	{
		std::string OpenFile(const std::string& title, const std::vector<std::string>& filters)
		{
			pfd::open_file opener(title, std::filesystem::current_path().generic_u8string(), filters, pfd::opt::none);

			const auto files = opener.result();
			if (files.empty())
			{
				return std::string();
			}

			return files.front();
		}
		//--------------------------------------------------------------------------

		std::string SaveFile(const std::string& title, const std::vector<std::string>& filters, const std::string& defaultFilename)
		{
			pfd::save_file saver(title, std::filesystem::current_path().generic_u8string(), filters, true);

			return saver.result();
		}
		//--------------------------------------------------------------------------

		/*
		bool Message(const char* text, const char* caption, Ptr<Window> window, MessageButtons buttons)
		{
			auto implWindow = reinterpret_cast<GLFWwindow*>(window->GetImplPointer());
			if (!implWindow)
			{
				return false;
			}

			window->BeginBlock();

			const auto nativeButtons = (buttons == YesNoButtons)
				? MB_YESNO
				: (buttons == OkButtons ? MB_OK : MB_OKCANCEL);
			const auto result = MessageBox(glfwGetWin32Window(implWindow), text, caption, nativeButtons);

			window->EndBlock();

			return result == IDYES || result == IDOK;
		}
		//--------------------------------------------------------------------------
		*/
	}
}