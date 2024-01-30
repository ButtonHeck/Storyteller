#include "game_document_serializer.h"
#include "log.h"
#include "entities.h"
#include "filesystem_utils.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>

#include <fstream>

namespace Storyteller
{
#define JSON_KEY_GAME_NAME "GameName"
#define JSON_KEY_ENTRY_POINT_UUID "EntryPointUuid"
#define JSON_KEY_OBJECTS "Objects"
#define JSON_KEY_UUID "UUID"
#define JSON_KEY_NAME "Name"
#define JSON_KEY_OBJECT_TYPE "ObjectType"
#define JSON_KEY_TEXT "Text"
#define JSON_KEY_ACTIONS "Actions"
#define JSON_KEY_TARGET "Target"

    GameDocumentSerializer::GameDocumentSerializer(GameDocument::Ptr document)
        : _document(document)
    {}
    //--------------------------------------------------------------------------

    bool GameDocumentSerializer::Load(const std::filesystem::path& path)
    {
        STRTLR_CORE_LOG_INFO("GameDocumentSerializer: load from '{}'", path.generic_string());

        if (path.empty() || !std::filesystem::exists(path)
            || !path.has_filename() || !path.has_extension())
        {
            STRTLR_CORE_LOG_WARN("GameDocumentSerializer: insufficient path");
            return false;
        }

        const auto success = Deserialize(path);
        if (!success)
        {
            STRTLR_CORE_LOG_WARN("GameDocumentSerializer: deserialization failed");
            return false;
        }

        _document->SetPath(path);
        _document->SetDirty(false);

        return true;
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSerializer::Save()
    {
        return Save(_document->GetPath());
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSerializer::Save(const std::filesystem::path& path)
    {
        STRTLR_CORE_LOG_INFO("GameDocumentSerializer: saving to '{}'", path.generic_string());

        if (!Filesystem::CheckPathAndTryCreate(path))
        {
            STRTLR_CORE_LOG_WARN("GameDocumentSerializer: insufficient path");
            return false;
        }

        const auto success = Serialize(path);
        if (!success)
        {
            STRTLR_CORE_LOG_WARN("GameDocumentSerializer: serialization failed");
            return false;
        }

        _document->SetPath(path);
        _document->SetDirty(false);

        return true;
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSerializer::Serialize(const std::filesystem::path& path)
    {
        rapidjson::StringBuffer stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
        auto success = true;

        success &= writer.StartObject();

        success &= writer.Key(JSON_KEY_GAME_NAME);
        success &= writer.String(_document->GetGameName().c_str());

        success &= writer.Key(JSON_KEY_ENTRY_POINT_UUID);
        const auto entryPoint = _document->GetEntryPoint();
        success &= writer.Uint64(entryPoint ? entryPoint->GetUuid() : UUID::InvalidUuid);

        success &= writer.Key(JSON_KEY_OBJECTS);
        success &= writer.StartArray();

        const auto objects = _document->GetObjects();
        for (auto i = 0; i < objects.size(); i++)
        {
            const auto object = objects.at(i);
            success &= writer.StartObject();

            success &= writer.Key(JSON_KEY_UUID);
            success &= writer.Uint64(object->GetUuid());

            success &= writer.Key(JSON_KEY_NAME);
            success &= writer.String(object->GetName().c_str());

            const auto objectType = object->GetObjectType();
            success &= writer.Key(JSON_KEY_OBJECT_TYPE);
            success &= writer.String(ObjectTypeToString(objectType).c_str());

            const auto textObject = dynamic_cast<const TextObject*>(object.get());
            success &= writer.Key(JSON_KEY_TEXT);
            success &= writer.String(textObject->GetText().c_str());

            switch (objectType)
            {
            case ObjectType::QuestObjectType:
            {
                const auto questObject = dynamic_cast<const QuestObject*>(textObject);
                const auto actions = questObject->GetActions();
                success &= writer.Key(JSON_KEY_ACTIONS);
                success &= writer.StartArray();
                for (auto action = 0; action < actions.size(); action++)
                {
                    success &= writer.Uint64(actions.at(action));
                }

                success &= writer.EndArray();
                break;
            }

            case ObjectType::ActionObjectType:
            {
                const auto actionObject = dynamic_cast<const ActionObject*>(textObject);
                success &= writer.Key(JSON_KEY_TARGET);
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

    bool GameDocumentSerializer::Deserialize(const std::filesystem::path& path)
    {
        std::ifstream inputStream(path.string());
        if (!inputStream.is_open() || !inputStream.good())
        {
            return false;
        }

        rapidjson::IStreamWrapper jsonStream(inputStream);
        rapidjson::Document jsonDocument;
        jsonDocument.ParseStream(jsonStream);
        inputStream.close();

        if (jsonDocument.HasParseError())
        {
            STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON parsing error '{}'", jsonDocument.GetParseError());
            return false;
        }

        if (!jsonDocument.HasMember(JSON_KEY_GAME_NAME) || !jsonDocument[JSON_KEY_GAME_NAME].IsString())
        {
            STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading string '{}'", JSON_KEY_GAME_NAME);
            return false;
        }
        _document->SetGameName(jsonDocument[JSON_KEY_GAME_NAME].GetString());

        if (!jsonDocument.HasMember(JSON_KEY_ENTRY_POINT_UUID) || !jsonDocument[JSON_KEY_ENTRY_POINT_UUID].IsUint64())
        {
            STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading UUID '{}'", JSON_KEY_ENTRY_POINT_UUID);
            return false;
        }
        _document->SetEntryPoint(jsonDocument[JSON_KEY_ENTRY_POINT_UUID].GetUint64());

        if (!jsonDocument.HasMember(JSON_KEY_OBJECTS) || !jsonDocument[JSON_KEY_OBJECTS].IsArray())
        {
            STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading array '{}'", JSON_KEY_OBJECTS);
            return false;
        }
        const auto& objectsArray = jsonDocument[JSON_KEY_OBJECTS].GetArray();

        for (rapidjson::SizeType i = 0; i < objectsArray.Size(); i++)
        {
            const auto jsonObject = objectsArray[i].GetObject();

            if (!jsonObject.HasMember(JSON_KEY_UUID) || !jsonObject[JSON_KEY_UUID].IsUint64())
            {
                STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading UUID '{}'", JSON_KEY_UUID);
                return false;
            }
            const auto objectUuid = UUID(jsonObject[JSON_KEY_UUID].GetUint64());

            if (!jsonObject.HasMember(JSON_KEY_NAME) || !jsonObject[JSON_KEY_NAME].IsString())
            {
                STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading string '{}'", JSON_KEY_NAME);
                return false;
            }
            const auto objectName = std::string(jsonObject[JSON_KEY_NAME].GetString());

            if (!jsonObject.HasMember(JSON_KEY_OBJECT_TYPE) || !jsonObject[JSON_KEY_OBJECT_TYPE].IsString())
            {
                STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading string '{}'", JSON_KEY_OBJECT_TYPE);
                return false;
            }
            const auto objectType = StringToObjectType(jsonObject[JSON_KEY_OBJECT_TYPE].GetString());

            if (!jsonObject.HasMember(JSON_KEY_TEXT) || !jsonObject[JSON_KEY_TEXT].IsString())
            {
                STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading string '{}'", JSON_KEY_TEXT);
                return false;
            }
            const auto objectText = std::string(jsonObject[JSON_KEY_TEXT].GetString());

            switch (objectType)
            {
            case ObjectType::QuestObjectType:
            {
                if (!jsonObject.HasMember(JSON_KEY_ACTIONS) || !jsonObject[JSON_KEY_ACTIONS].IsArray())
                {
                    STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading array '{}'", JSON_KEY_ACTIONS);
                    return false;
                }

                auto questObject = std::make_shared<QuestObject>(objectUuid, [this]() { _document->SetDirty(true); });
                questObject->SetText(objectText);
                questObject->SetName(objectName);

                const auto questObjectActions = jsonObject[JSON_KEY_ACTIONS].GetArray();
                for (rapidjson::SizeType i = 0; i < questObjectActions.Size(); i++)
                {
                    if (!questObjectActions[i].IsUint64())
                    {
                        STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading UUID '{}'", JSON_KEY_ACTIONS);
                        return false;
                    }

                    questObject->AddAction(UUID(questObjectActions[i].GetUint64()));
                }

                _document->AddObject(questObject);

                break;
            }
            case ObjectType::ActionObjectType:
            {
                if (!jsonObject.HasMember(JSON_KEY_TARGET) || !jsonObject[JSON_KEY_TARGET].IsUint64())
                {
                    STRTLR_CORE_LOG_ERROR("GameDocumentSerializer: JSON error reading UUID '{}'", JSON_KEY_TARGET);
                    return false;
                }

                auto actionObject = std::make_shared<ActionObject>(objectUuid, [this]() { _document->SetDirty(true); });
                actionObject->SetTargetUuid(UUID(jsonObject[JSON_KEY_TARGET].GetUint64()));
                actionObject->SetText(objectText);
                actionObject->SetName(objectName);

                _document->AddObject(actionObject);

                break;
            }
            default:
                return false;
            }
        }

        return true;
    }
    //--------------------------------------------------------------------------
}