#include "game_document.h"
#include "log.h"

namespace Storyteller
{
    GameDocument::GameDocument(const std::string& pathString)
        : GameDocument(std::filesystem::path(pathString))
    {}
    //--------------------------------------------------------------------------

    GameDocument::GameDocument(const std::filesystem::path& path)
        : _gameName("Untitled")
        , _path(path)
        , _dirty(false)
        , _entryPointUuid(UUID::InvalidUuid)
    {
        STRTLR_CORE_LOG_INFO("GameDocument: create '{}'", _path.generic_string());
    }
    //--------------------------------------------------------------------------

    std::string GameDocument::GetGameName() const
    {
        return _gameName;
    }
    //--------------------------------------------------------------------------

    void GameDocument::SetGameName(const std::string& gameName)
    {
        STRTLR_CORE_LOG_INFO("GameDocument: set name '{}'", gameName);

        if (_gameName != gameName)
        {
            _gameName = gameName;
            SetDirty(true);
        }
    }
    //--------------------------------------------------------------------------

    std::filesystem::path GameDocument::GetPath() const
    {
        return _path;
    }
    //--------------------------------------------------------------------------

    std::string GameDocument::GetPathString() const
    {
        return _path.string();
    }
    //--------------------------------------------------------------------------

    void GameDocument::SetPath(const std::filesystem::path& path)
    {
        STRTLR_CORE_LOG_INFO("GameDocument: set path '{}'", path.generic_string());

        _path = path;
    }
    //--------------------------------------------------------------------------

    void GameDocument::SetPathString(const std::string& path)
    {
        STRTLR_CORE_LOG_INFO("GameDocument: set path '{}'", path);

        _path = std::filesystem::path(path);
    }
    //--------------------------------------------------------------------------

    bool GameDocument::IsDirty() const
    {
        return _dirty;
    }
    //--------------------------------------------------------------------------

    void GameDocument::SetDirty(bool dirty)
    {
        _dirty = dirty;
    }
    //--------------------------------------------------------------------------

    bool GameDocument::AddObject(ObjectType type, const UUID& uuid)
    {
        STRTLR_CORE_LOG_INFO("GameDocument: add object ({}) of type '{}'", uuid, ObjectTypeToString(type));

        if (type == ObjectType::ErrorObjectType || std::find_if(_objects.cbegin(), _objects.cend(), [&](const Ptr<BasicObject> obj) { return obj->GetUuid() == uuid; }) != _objects.cend())
        {
            STRTLR_CORE_LOG_WARN("GameDocument: type is invalid or ({}) is already exist", uuid);
            return false;
        }

        switch (type)
        {
        case ObjectType::QuestObjectType:
            _objects.push_back(CreatePtr<QuestObject>(uuid, [this]() { SetDirty(true); }));
            SetDirty(true);
            return true;

        case ObjectType::ActionObjectType:
            _objects.push_back(CreatePtr<ActionObject>(uuid, [this]() { SetDirty(true); }));
            SetDirty(true);
            return true;

        default:
            break;
        }

        return false;
    }
    //--------------------------------------------------------------------------

    bool GameDocument::AddObject(const Ptr<BasicObject>& object)
    {
        STRTLR_CORE_LOG_INFO("GameDocument: add object ({}) of type '{}'", object->GetUuid(), ObjectTypeToString(object->GetObjectType()));

        if (std::find_if(_objects.cbegin(), _objects.cend(), [&](const Ptr<BasicObject> obj) { return obj->GetUuid() == object->GetUuid(); }) != _objects.cend())
        {
            STRTLR_CORE_LOG_WARN("GameDocument: type is invalid or ({}) is already exist", object->GetUuid());
            return false;
        }

        object->SetChangeCallback([this]() { SetDirty(true); });
        _objects.push_back(object);
        SetDirty(true);
        return true;
    }
    //--------------------------------------------------------------------------

    bool GameDocument::RemoveObject(const UUID& uuid)
    {
        STRTLR_CORE_LOG_INFO("GameDocument: removing object ({})", uuid);

        const auto it = std::find_if(_objects.cbegin(), _objects.cend(), [&](const Ptr<BasicObject> obj) { return obj->GetUuid() == uuid; });
        if (it == _objects.cend())
        {
            STRTLR_CORE_LOG_WARN("GameDocument: object ({}) is not found to remove", uuid);
            return false;
        }

        _objects.erase(it);
        SetDirty(true);
        return true;
    }
    //--------------------------------------------------------------------------

    Ptr<BasicObject> GameDocument::GetBasicObject(const UUID& uuid) const
    {
        const auto it = std::find_if(_objects.cbegin(), _objects.cend(), [&](const Ptr<BasicObject> obj) { return obj->GetUuid() == uuid; });
        if (it != _objects.cend())
        {
            return *it;
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    Ptr<BasicObject> GameDocument::GetBasicObject(const std::string& name) const
    {
        const auto it = std::find_if(_objects.cbegin(), _objects.cend(), [&](const Ptr<BasicObject> obj) { return obj->GetName() == name; });
        if (it != _objects.cend())
        {
            return *it;
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    const std::vector<Ptr<BasicObject>>& GameDocument::GetObjects() const
    {
        return _objects;
    }
    //--------------------------------------------------------------------------

    std::vector<Ptr<BasicObject>> GameDocument::GetObjects(ObjectType type, bool noEmptyName) const
    {
        std::vector<Ptr<BasicObject>> result;
        std::copy_if(_objects.cbegin(), _objects.cend(), std::back_inserter(result), [type, noEmptyName](const Ptr<BasicObject>& object) 
            { 
                if (object->GetObjectType() != type)
                {
                    return false;
                }

                if (noEmptyName)
                {
                    return !object->GetName().empty();
                }
                
                return true;
            });
        return result;
    }
    //--------------------------------------------------------------------------

    void GameDocument::SetEntryPoint(const UUID& uuid)
    {
        STRTLR_CORE_LOG_INFO("GameDocument: set entry point ({})", uuid);

        _entryPointUuid = uuid;
        SetDirty(true);
    }
    //--------------------------------------------------------------------------

    Ptr<BasicObject> GameDocument::GetEntryPoint() const
    {
        return GetBasicObject(_entryPointUuid);
    }
    //--------------------------------------------------------------------------

    bool GameDocument::SetObjectName(const UUID& uuid, const std::string& name) const
    {
        if (std::find_if(_objects.cbegin(), _objects.cend(), [&](const Ptr<BasicObject> ptr) { return !ptr->GetName().empty() && ptr->GetName() == name; }) != _objects.cend())
        {
            STRTLR_CORE_LOG_WARN("GameDocument: object name '{}' already exists", name);
            return false;
        }

        const auto object = GetBasicObject(uuid);
        if (!object)
        {
            STRTLR_CORE_LOG_WARN("GameDocument: cannot find object ({})", uuid);
            return false;
        }

        object->SetName(name);
        return true;
    }
    //--------------------------------------------------------------------------

    bool GameDocument::CheckConsistency() const
    {
        return std::find_if(_objects.cbegin(), _objects.cend(), [&](const Ptr<BasicObject> ptr) { return !ptr->IsConsistent(); }) != _objects.cend()
            && GetBasicObject(_entryPointUuid) != nullptr;
    }
    //--------------------------------------------------------------------------
}