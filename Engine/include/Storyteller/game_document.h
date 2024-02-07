#pragma once

#include "pointers.h"
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
        explicit GameDocument(const std::string& pathString = "");
        explicit GameDocument(const std::filesystem::path& path = "");

        std::string GetGameName() const;
        void SetGameName(const std::string& gameName);

        std::filesystem::path GetPath() const;
        std::string GetPathString() const;
        void SetPath(const std::filesystem::path& path);
        void SetPathString(const std::string& path);

        bool IsDirty() const;
        void SetDirty(bool dirty);

        bool AddObject(ObjectType type, const UUID& uuid = UUID());
        bool AddObject(const Ptr<BasicObject>& object);
        bool RemoveObject(const UUID& uuid);
        Ptr<BasicObject> GetBasicObject(const UUID& uuid) const;
        Ptr<BasicObject> GetBasicObject(const std::string& name) const;
        const std::vector<Ptr<BasicObject>>& GetObjects() const;
        std::vector<Ptr<BasicObject>> GetObjects(ObjectType type, bool noEmptyName) const;
        void SetEntryPoint(const UUID& uuid);
        Ptr<BasicObject> GetEntryPoint() const;
        bool SetObjectName(const UUID& uuid, const std::string& name) const;

        bool CheckConsistency() const;

    private:
        std::string _gameName;
        std::filesystem::path _path;
        bool _dirty;
        std::vector<Ptr<BasicObject>> _objects;
        UUID _entryPointUuid;
    };
    //--------------------------------------------------------------------------
}