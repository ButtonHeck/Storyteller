#pragma once

#include <filesystem>

namespace Storyteller
{
    namespace Filesystem
    {
        bool CheckPathAndTryCreate(const std::filesystem::path& path);
        //--------------------------------------------------------------------------
    }
}
