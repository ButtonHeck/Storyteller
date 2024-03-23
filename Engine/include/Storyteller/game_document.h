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
        explicit GameDocument(const std::filesystem::path& path = "");

        std::string GetGameName() const;
        void SetGameName(const std::string& gameName);

        std::string GetDomainName() const;
        void SetDomainName(const std::string& domainName);

        std::filesystem::path GetPath() const;
        std::filesystem::path GetTranslationsPath() const;
        void SetPath(const std::filesystem::path& path);

        bool IsDirty() const;
        void SetDirty(bool dirty);

        bool AddObject(ObjectType type, const UUID& uuid = UUID());
        bool AddObject(const Ptr<BasicObject>& object);
        bool RemoveObject(const UUID& uuid);

        Ptr<BasicObject> GetObject(const UUID& uuid) const;
        Ptr<BasicObject> GetObject(const std::string& name) const;

        const std::vector<Ptr<BasicObject>>& GetObjects() const;
        std::vector<Ptr<BasicObject>> GetObjects(ObjectType type) const;
        template<typename T>
        std::vector<Ptr<T>> GetObjects() const;

        void SetEntryPoint(const UUID& uuid);
        Ptr<BasicObject> GetEntryPoint() const;
        bool SetObjectName(const UUID& uuid, const std::string& name) const;

        bool CheckConsistency() const;

    private:
        std::string _gameName;
        std::string _domainName;
        std::filesystem::path _path;
        bool _dirty;
        std::vector<Ptr<BasicObject>> _objects;
        UUID _entryPointUuid;
    };
    //--------------------------------------------------------------------------

    template<>
    inline std::vector<Ptr<QuestObject>> GameDocument::GetObjects() const
    {
        const auto basicObjects = GetObjects(ObjectType::QuestObjectType);
        std::vector<Ptr<QuestObject>> result;
        std::for_each(basicObjects.cbegin(), basicObjects.cend(), [&](const Ptr<BasicObject>& object) {
            const auto typedObject = std::dynamic_pointer_cast<QuestObject>(object);
            if (typedObject)
            {
                result.push_back(typedObject);
            }
            }
        );

        return result;
    }
    //--------------------------------------------------------------------------

    template<>
    inline std::vector<Ptr<ActionObject>> GameDocument::GetObjects() const
    {
        const auto basicObjects = GetObjects(ObjectType::ActionObjectType);
        std::vector<Ptr<ActionObject>> result;
        std::for_each(basicObjects.cbegin(), basicObjects.cend(), [&](const Ptr<BasicObject>& object) {
            const auto typedObject = std::dynamic_pointer_cast<ActionObject>(object);
            if (typedObject)
            {
                result.push_back(typedObject);
            }
            }
        );

        return result;
    }
    //--------------------------------------------------------------------------

    template<>
    inline std::vector<Ptr<TextObject>> GameDocument::GetObjects() const
    {
        const auto& basicObjects = GetObjects();
        std::vector<Ptr<TextObject>> result;
        std::for_each(basicObjects.cbegin(), basicObjects.cend(), [&](const Ptr<BasicObject>& object) {
            const auto textObject = std::dynamic_pointer_cast<TextObject>(object);
            if (textObject)
            {
                result.push_back(textObject);
            }
            }
        );

        return result;
    }
    //--------------------------------------------------------------------------
}
