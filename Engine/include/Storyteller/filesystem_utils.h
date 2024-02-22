#pragma once

#include <filesystem>

namespace Storyteller
{
    namespace Filesystem
    {
        bool PathExists(const std::filesystem::path& path);
        //--------------------------------------------------------------------------

        bool PathIsValid(const std::filesystem::path& path);
        //--------------------------------------------------------------------------

        bool CreatePathTree(const std::filesystem::path& path);
        //--------------------------------------------------------------------------

        std::string PathUnicode(const std::filesystem::path& path);
        //--------------------------------------------------------------------------

        std::string GetFilePathString(const char* dir, const char* filename);
        //--------------------------------------------------------------------------
    }
}
