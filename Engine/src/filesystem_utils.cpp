#include "filesystem_utils.h"

namespace Storyteller
{
    bool Filesystem::CheckPathAndTryCreate(const std::filesystem::path& path)
    {
        if (path.empty() || !path.has_filename() || !path.has_extension())
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
}
