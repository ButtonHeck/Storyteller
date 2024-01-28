#pragma once

#include "game_document.h"

#include <filesystem>

namespace Storyteller
{
    class GameDocumentSerializer
    {
    public:
        explicit GameDocumentSerializer(GameDocument::Ptr document);

        bool Load(const std::filesystem::path& path);
        bool Save();
        bool Save(const std::filesystem::path& path);

    private:
        bool Serialize(const std::filesystem::path& path);
        bool Deserialize(const std::filesystem::path& path);

    private:
        GameDocument::Ptr _document;
    };
    //--------------------------------------------------------------------------
}
