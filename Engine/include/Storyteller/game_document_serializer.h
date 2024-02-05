#pragma once

#include "pointers.h"
#include "game_document.h"

#include <filesystem>

namespace Storyteller
{
    class GameDocumentSerializer
    {
    public:
        explicit GameDocumentSerializer(Ptr<GameDocument> document);

        bool Load(const std::filesystem::path& path);
        bool Save();
        bool Save(const std::filesystem::path& path);

    private:
        bool Serialize(const std::filesystem::path& path);
        bool Deserialize(const std::filesystem::path& path);

    private:
        Ptr<GameDocument> _document;
    };
    //--------------------------------------------------------------------------
}
