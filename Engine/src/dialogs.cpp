#include "dialogs.h"
#include "filesystem.h"
#include "utils.h"

#include <portable-file-dialogs.h>

namespace Storyteller
{
	namespace Dialogs
	{
		std::string OpenFile(const std::string& title, const std::vector<std::string>& filters)
		{
			pfd::open_file opener(title, Filesystem::ToString(Filesystem::GetCurrentPathCRef()), filters, pfd::opt::none);

			const auto files = opener.result();
			if (files.empty())
			{
				return std::string();
			}

			return Filesystem::ToString(Utils::FromStrBytesToWStrBytes(files.front()));
		}
		//--------------------------------------------------------------------------

		std::vector<std::string> OpenFiles(const std::string& title, const std::vector<std::string>& filters)
		{
			pfd::open_file opener(title, Filesystem::ToString(Filesystem::GetCurrentPathCRef()), filters, pfd::opt::multiselect);

			const auto files = opener.result();
			if (files.empty())
			{
				return std::vector<std::string>();
			}

			return files;
		}
		//--------------------------------------------------------------------------

		std::string SaveFile(const std::string& title, const std::vector<std::string>& filters)
		{
			pfd::save_file saver(title, Filesystem::ToString(Filesystem::GetCurrentPathCRef()), filters, true);

			return Filesystem::ToString(Utils::FromStrBytesToWStrBytes(saver.result()));
		}
		//--------------------------------------------------------------------------
	}
}