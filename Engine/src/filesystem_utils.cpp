#include "filesystem_utils.h"

namespace Storyteller
{
    std::filesystem::path Filesystem::_currentPath;

    void Filesystem::Initialize()
    {
        _currentPath = std::filesystem::current_path();
    }
    //--------------------------------------------------------------------------

    bool Filesystem::PathExists(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path);
    }
    //--------------------------------------------------------------------------

    bool Filesystem::FilePathIsValid(const std::filesystem::path& path)
    {
        return !path.empty() && path.has_filename() && path.has_extension();
    }
    //--------------------------------------------------------------------------

    bool Filesystem::CreatePathTree(const std::filesystem::path& path)
    {
        if (!FilePathIsValid(path))
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

    std::filesystem::path Filesystem::GetCurrentPath()
    {
        return _currentPath;
    }
    //--------------------------------------------------------------------------

    std::string Filesystem::PathUnicode(const std::filesystem::path& path)
    {
        if (!FilePathIsValid(path))
        {
            return std::string();
        }

        return path.generic_u8string();
    }
    //--------------------------------------------------------------------------

    std::string Filesystem::GetFilePathString(const char* dir, const char* filename)
    {
        return std::filesystem::path(dir).append(filename).string();
    }
    //--------------------------------------------------------------------------
}
