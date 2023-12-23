#include "game_document.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>

#include <fstream>

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
        Load(path);
    }
    //--------------------------------------------------------------------------

    bool GameDocument::Load(const std::filesystem::path& path)
    {
        if (path.empty() || !std::filesystem::exists(path)
            || !path.has_filename() || !path.has_extension())
        {
            return false;
        }

        const auto success = Deserialize(path);
        if (!success)
        {
            return false;
        }

        SetPath(path);
        SetDirty(false);
        return true;
    }
    //--------------------------------------------------------------------------

    bool GameDocument::Save()
    {
        return Save(_path);
    }
    //--------------------------------------------------------------------------

    bool GameDocument::Save(const std::filesystem::path& path)
    {
        if (path.empty() || !path.has_filename() || !path.has_extension())
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

        const auto success = Serialize(path);
        if (!success)
        {
            return false;
        }

        SetPath(path);
        SetDirty(false);
        return true;
    }
    //--------------------------------------------------------------------------

    std::string GameDocument::GetGameName() const
    {
        return _gameName;
    }
    //--------------------------------------------------------------------------

    void GameDocument::SetGameName(const std::string& gameName)
    {
        _gameName = gameName;
        SetDirty(true);
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
        _path = path;
    }
    //--------------------------------------------------------------------------

    void GameDocument::SetPathString(const std::string& path)
    {
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
        if (type == ObjectType::ErrorObjectType || std::find_if(_objects.cbegin(), _objects.cend(), [&](const BasicObject::Ptr obj) { return obj->GetUuid() == uuid; }) != _objects.cend())
        {
            return false;
        }

        switch (type)
        {
        case ObjectType::QuestObjectType:
            _objects.push_back(std::make_shared<QuestObject>(uuid, [this]() { SetDirty(true); }));
            SetDirty(true);
            return true;

        case ObjectType::ActionObjectType:
            _objects.push_back(std::make_shared<ActionObject>(uuid, [this]() { SetDirty(true); }));
            SetDirty(true);
            return true;

        default:
            break;
        }

        return false;
    }
    //--------------------------------------------------------------------------

    bool GameDocument::AddObject(const BasicObject::Ptr& object)
    {
        if (std::find_if(_objects.cbegin(), _objects.cend(), [&](const BasicObject::Ptr obj) { return obj->GetUuid() == object->GetUuid(); }) != _objects.cend())
        {
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
        const auto it = std::find_if(_objects.cbegin(), _objects.cend(), [&](const BasicObject::Ptr obj) { return obj->GetUuid() == uuid; });
        if (it == _objects.cend())
        {
            return false;
        }

        _objects.erase(it);
        SetDirty(true);
        return true;
    }
    //--------------------------------------------------------------------------

    BasicObject::Ptr GameDocument::GetObject(const UUID& uuid) const
    {
        const auto it = std::find_if(_objects.cbegin(), _objects.cend(), [&](const BasicObject::Ptr obj) { return obj->GetUuid() == uuid; });
        if (it != _objects.cend())
        {
            return *it;
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    BasicObject::Ptr GameDocument::GetObject(const std::string& name) const
    {
        const auto it = std::find_if(_objects.cbegin(), _objects.cend(), [&](const BasicObject::Ptr obj) { return obj->GetName() == name; });
        if (it != _objects.cend())
        {
            return *it;
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    const std::vector<BasicObject::Ptr>& GameDocument::GetObjects() const
    {
        return _objects;
    }
    //--------------------------------------------------------------------------

    void GameDocument::SetEntryPoint(const UUID& uuid)
    {
        _entryPointUuid = uuid;
        SetDirty(true);
    }
    //--------------------------------------------------------------------------

    BasicObject::Ptr GameDocument::GetEntryPoint() const
    {
        return GetObject(_entryPointUuid);
    }
    //--------------------------------------------------------------------------

    bool GameDocument::CheckConsistency() const
    {
        return std::find_if(_objects.cbegin(), _objects.cend(), [&](const BasicObject::Ptr ptr) { return !ptr->IsConsistent(); }) != _objects.cend()
            && GetObject(_entryPointUuid) != nullptr;
    }
    //--------------------------------------------------------------------------

    bool GameDocument::Serialize(const std::filesystem::path& path)
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
        auto success = true;

        success &= writer.StartObject();

        success &= writer.Key("GameName");
        success &= writer.String(GetGameName().c_str());

        success &= writer.Key("EntryPointUuid");
        success &= writer.Uint64(_entryPointUuid);

        success &= writer.Key("Objects");
        success &= writer.StartArray();
        for (size_t i = 0; i < _objects.size(); i++)
        {
            const auto object = _objects.at(i);
            success &= writer.StartObject();

            success &= writer.Key("UUID");
            success &= writer.Uint64(object->GetUuid());

            success &= writer.Key("Name");
            success &= writer.String(object->GetName().c_str());

            const auto objectType = object->GetObjectType();
            success &= writer.Key("ObjectType");
            success &= writer.String(ObjectTypeToString(objectType).c_str());

            const auto textObject = dynamic_cast<const TextObject*>(object.get());
            success &= writer.Key("Text");
            success &= writer.String(textObject->GetText().c_str());

            switch (objectType)
            {
            case ObjectType::QuestObjectType:
            {
                const auto questObject = dynamic_cast<const QuestObject*>(textObject);
                const auto actions = questObject->GetActions();
                success &= writer.Key("Actions");
                success &= writer.StartArray();
                for (size_t action = 0; action < actions.size(); action++)
                {
                    success &= writer.Uint64(actions.at(action));
                }

                success &= writer.EndArray();
                break;
            }

            case ObjectType::ActionObjectType:
            {
                const auto actionObject = dynamic_cast<const ActionObject*>(textObject);
                success &= writer.Key("Target");
                success &= writer.Uint64(actionObject->GetTargetUuid());
                break;
            }

            default:
                break;
            }

            success &= writer.EndObject();
        }

        success &= writer.EndArray();
        success &= writer.EndObject();

        if (!success)
        {
            return false;
        }

        std::ofstream outputStream(path.string(), std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            return false;
        }

        outputStream << stringBuffer.GetString();
        outputStream.close();

        return true;
    }
    //--------------------------------------------------------------------------

    bool GameDocument::Deserialize(const std::filesystem::path& path)
    {
        std::ifstream inputStream(path.string());
        if (!inputStream.is_open() || !inputStream.good())
        {
            return false;
        }

        rapidjson::IStreamWrapper jsonStream(inputStream);
        rapidjson::Document jsonDocument;
        jsonDocument.ParseStream(jsonStream);
        if (jsonDocument.HasParseError())
        {
            return false;
        }

        if (!jsonDocument.HasMember("GameName") || !jsonDocument["GameName"].IsString())
        {
            return false;
        }
        SetGameName(jsonDocument["GameName"].GetString());

        if (!jsonDocument.HasMember("EntryPointUuid") || !jsonDocument["EntryPointUuid"].IsUint64())
        {
            return false;
        }
        SetEntryPoint(jsonDocument["EntryPointUuid"].GetUint64());

        if (!jsonDocument.HasMember("Objects") || !jsonDocument["Objects"].IsArray())
        {
            return false;
        }
        const auto& objectsArray = jsonDocument["Objects"].GetArray();

        for (rapidjson::SizeType i = 0; i < objectsArray.Size(); i++)
        {
            const auto jsonObject = objectsArray[i].GetObject();

            if (!jsonObject.HasMember("UUID") || !jsonObject["UUID"].IsUint64())
            {
                return false;
            }
            const auto objectUuid = UUID(jsonObject["UUID"].GetUint64());

            if (!jsonObject.HasMember("Name") || !jsonObject["Name"].IsString())
            {
                return false;
            }
            const auto objectName = std::string(jsonObject["Name"].GetString());

            if (!jsonObject.HasMember("ObjectType") || !jsonObject["ObjectType"].IsString())
            {
                return false;
            }
            const auto objectType = StringToObjectType(jsonObject["ObjectType"].GetString());

            if (!jsonObject.HasMember("Text") || !jsonObject["Text"].IsString())
            {
                return false;
            }
            const auto objectText = std::string(jsonObject["Text"].GetString());

            switch (objectType)
            {
            case ObjectType::QuestObjectType:
            {
                if (!jsonObject.HasMember("Actions") || !jsonObject["Actions"].IsArray())
                {
                    return false;
                }

                auto questObject = std::make_shared<QuestObject>(objectUuid, [this]() { SetDirty(true); });
                questObject->SetText(objectText);
                questObject->SetName(objectName);

                const auto questObjectActions = jsonObject["Actions"].GetArray();
                for (rapidjson::SizeType i = 0; i < questObjectActions.Size(); i++)
                {
                    if (!questObjectActions[i].IsUint64())
                    {
                        return false;
                    }

                    questObject->AddAction(UUID(questObjectActions[i].GetUint64()));
                }

                AddObject(questObject);

                break;
            }
            case ObjectType::ActionObjectType:
            {
                if (!jsonObject.HasMember("Target") || !jsonObject["Target"].IsUint64())
                {
                    return false;
                }

                auto actionObject = std::make_shared<ActionObject>(objectUuid, [this]() { SetDirty(true); });
                actionObject->SetTargetUuid(UUID(jsonObject["Target"].GetUint64()));
                actionObject->SetText(objectText);
                actionObject->SetName(objectName);

                AddObject(actionObject);

                break;
            }
            default:
                return false;
            }
        }

        return true;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}