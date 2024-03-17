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

		std::string OpenDirectory(const std::string& title);
		//--------------------------------------------------------------------------

		std::string SaveFile(const std::string& title, const std::vector<std::string>& filters, bool forceOverwrite = false);
		//--------------------------------------------------------------------------
	};
}