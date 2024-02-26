#include "dialogs.h"

#include <Windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Storyteller
{
	namespace Dialogs
	{
		std::string OpenFile(const char* filter, Ptr<Window> window)
		{
			auto implWindow = reinterpret_cast<GLFWwindow*>(window->GetImplPointer());
			if (!implWindow)
			{
				return std::string();
			}

			window->BeginBlock();

			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window(implWindow);
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
			{
				ofn.lpstrInitialDir = currentDir;
			}

			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

			if (GetOpenFileNameA(&ofn) == TRUE)
			{
				window->EndBlock();

				return ofn.lpstrFile;
			}

			window->EndBlock();

			return std::string();
		}
		//--------------------------------------------------------------------------

		std::string SaveFile(const char* filter, Ptr<Window> window, const char* defaultName)
		{
			auto implWindow = reinterpret_cast<GLFWwindow*>(window->GetImplPointer());
			if (!implWindow)
			{
				return std::string();
			}

			window->BeginBlock();

			OPENFILENAMEA ofn;
			CHAR szFile[260] = { 0 };
			std::strcpy(szFile, defaultName);
			CHAR currentDir[256] = { 0 };
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = glfwGetWin32Window(implWindow);
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			if (GetCurrentDirectoryA(256, currentDir))
			{
				ofn.lpstrInitialDir = currentDir;
			}

			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
			ofn.lpstrDefExt = strchr(filter, '\0') + 1;

			if (GetSaveFileNameA(&ofn) == TRUE)
			{
				window->EndBlock();

				return ofn.lpstrFile;
			}

			window->EndBlock();

			return std::string();
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