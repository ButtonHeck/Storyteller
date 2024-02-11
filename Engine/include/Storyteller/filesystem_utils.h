#pragma once

#include <filesystem>

namespace Storyteller
{
    namespace Filesystem
    {
        bool PathIsValid(const std::filesystem::path& path);
        //--------------------------------------------------------------------------

        bool CreatePathTree(const std::filesystem::path& path);
        //--------------------------------------------------------------------------

        std::string PathUnicode(const std::filesystem::path& path);
        //--------------------------------------------------------------------------
    }
}
