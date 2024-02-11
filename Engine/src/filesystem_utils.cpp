#include "filesystem_utils.h"

namespace Storyteller
{
    namespace Filesystem
    {
        bool PathIsValid(const std::filesystem::path& path)
        {
            return !path.empty() && path.has_filename() && path.has_extension();
        }
        //--------------------------------------------------------------------------

        bool CreatePathTree(const std::filesystem::path& path)
        {
            if (!PathIsValid(path))
            {
                return false;
            }

            auto tempPath = path;
            while (!std::filesystem::exists(tempPath.parent_path()))
            {
                if (!std::filesystem::create_directory(tempPath.parent_path()))
                {
                    return false;
                }

                tempPath = tempPath.parent_path();
            }

            return true;
        }
        //--------------------------------------------------------------------------

        std::string PathUnicode(const std::filesystem::path& path)
        {
            if (!PathIsValid(path))
            {
                return std::string();
            }

            return path.generic_u8string();
        }
        //--------------------------------------------------------------------------
    }
}
