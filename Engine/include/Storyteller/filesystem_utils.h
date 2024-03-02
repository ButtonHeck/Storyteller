#pragma once

#include <filesystem>

namespace Storyteller
{
    class Filesystem
    {
    public:
        static void Initialize();

        static bool PathExists(const std::filesystem::path& path);
        static bool FilePathIsValid(const std::filesystem::path& path);
        static bool CreatePathTree(const std::filesystem::path& path);
        static std::filesystem::path GetCurrentPath();
        static std::string PathUnicode(const std::filesystem::path& path);
        static std::string GetFilePathString(const char* dir, const char* filename);

    private:
        static std::filesystem::path _currentPath;
    };
    //--------------------------------------------------------------------------
}
