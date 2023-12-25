#pragma once

#include "uuid.h"
#include "entities.h"

#include <string>
#include <vector>
#include <filesystem>

namespace Storyteller
{
    class GameDocument
    {
    public:
        typedef std::shared_ptr<GameDocument> Ptr;

        explicit GameDocument(const std::string& pathString = "");
        explicit GameDocument(const std::filesystem::path& path = "");

        bool Load(const std::filesystem::path& path);
        bool Save();
        bool Save(const std::filesystem::path& path);

        std::string GetGameName() const;
        void SetGameName(const std::string& gameName);

        std::filesystem::path GetPath() const;
        std::string GetPathString() const;
        void SetPath(const std::filesystem::path& path);
        void SetPathString(const std::string& path);

        bool IsDirty() const;
        void SetDirty(bool dirty);

        bool AddObject(ObjectType type, const UUID& uuid = UUID());
        bool AddObject(const BasicObject::Ptr& object);
        bool RemoveObject(const UUID& uuid);
        BasicObject::Ptr GetObject(const UUID& uuid) const;
        BasicObject::Ptr GetObject(const std::string& name) const;
        const std::vector<BasicObject::Ptr>& GetObjects() const;
        std::vector<BasicObject::Ptr> GetObjects(ObjectType type, bool noEmptyName) const;
        void SetEntryPoint(const UUID& uuid);
        BasicObject::Ptr GetEntryPoint() const;

        bool CheckConsistency() const;

    private:
        bool Serialize(const std::filesystem::path& path);
        bool Deserialize(const std::filesystem::path& path);

    private:
        std::string _gameName;
        std::filesystem::path _path;
        bool _dirty;
        std::vector<BasicObject::Ptr> _objects;
        UUID _entryPointUuid;
    };
    //--------------------------------------------------------------------------
}
