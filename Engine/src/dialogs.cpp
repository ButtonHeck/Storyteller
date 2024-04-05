#include "dialogs.h"
#include "filesystem.h"
#include "string_utils.h"

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

            auto paths = std::vector<std::string>();
            const auto files = opener.result();
            if (files.empty())
            {
                return paths;
            }

            for (const auto& path : files)
            {
                paths.push_back(Filesystem::ToString(Utils::FromStrBytesToWStrBytes(path)));
            }

            return paths;
        }
        //--------------------------------------------------------------------------

        std::string OpenDirectory(const std::string& title)
        {
            pfd::open_file opener(title, Filesystem::ToString(Filesystem::GetCurrentPathCRef()), {}, pfd::opt::force_path);

            const auto directories = opener.result();
            if (directories.empty())
            {
                return std::string();
            }

            return Filesystem::ToString(Utils::FromStrBytesToWStrBytes(directories.front()));
        }
        //--------------------------------------------------------------------------

        std::string SaveFile(const std::string& title, const std::vector<std::string>& filters, bool forceOverwrite)
        {
            pfd::save_file saver(title, Filesystem::ToString(Filesystem::GetCurrentPathCRef()), filters, forceOverwrite ? pfd::opt::force_overwrite : pfd::opt::none);

            return Filesystem::ToString(Utils::FromStrBytesToWStrBytes(saver.result()));
        }
        //--------------------------------------------------------------------------
    }
}
