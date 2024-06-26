#include "game_document_serializer.h"
#include "log.h"
#include "entities.h"
#include "filesystem.h"
#include "json_reader.h"
#include "json_writer.h"

#include <fstream>

namespace Storyteller
{
#define JSON_KEY_GAME_NAME "GameName"
#define JSON_KEY_GAME_DOMAIN_NAME "GameDomainName"
#define JSON_KEY_ENTRY_POINT_UUID "EntryPointUuid"
#define JSON_KEY_OBJECTS "Objects"
#define JSON_KEY_UUID "UUID"
#define JSON_KEY_NAME "Name"
#define JSON_KEY_OBJECT_TYPE "ObjectType"
#define JSON_KEY_TEXT "Text"
#define JSON_KEY_ACTIONS "Actions"
#define JSON_KEY_TARGET "Target"
#define JSON_KEY_FINAL "Final"

    GameDocumentSerializer::GameDocumentSerializer(const Ptr<GameDocument> document)
        : _document(document)
    {}
    //--------------------------------------------------------------------------

    bool GameDocumentSerializer::Load(const std::filesystem::path& path)
    {
        STRTLR_CORE_LOG_INFO("GameDocumentSerializer: load from '{}'", Filesystem::ToU8String(path));

        if (!Filesystem::PathExists(path) || !Filesystem::FilePathIsValid(path))
        {
            STRTLR_CORE_LOG_WARN("GameDocumentSerializer: insufficient path");
            return false;
        }

        const auto ok = Deserialize(path);
        if (!ok)
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
        STRTLR_CORE_LOG_INFO("GameDocumentSerializer: saving to '{}'", Filesystem::ToU8String(path));

        if (!Filesystem::CreatePathTree(path))
        {
            STRTLR_CORE_LOG_WARN("GameDocumentSerializer: insufficient path");
            return false;
        }

        const auto ok = Serialize(path);
        if (!ok)
        {
            STRTLR_CORE_LOG_WARN("GameDocumentSerializer: serialization failed");
            return false;
        }

        _document->SetPath(path);
        _document->SetDirty(false);

        return true;
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSerializer::Serialize(const std::filesystem::path& path) const
    {
        JsonWriter writer(path);
        auto ok = true;

        ok &= writer.Start();

        ok &= writer.SaveString(JSON_KEY_GAME_NAME, _document->GetGameName());

        ok &= writer.SaveString(JSON_KEY_GAME_DOMAIN_NAME, _document->GetDomainName());

        const auto entryPoint = _document->GetEntryPoint();
        ok &= writer.SaveUInt64(JSON_KEY_ENTRY_POINT_UUID, entryPoint ? entryPoint->GetUuid() : UUID::InvalidUuid);

        ok &= writer.StartArray(JSON_KEY_OBJECTS);
        const auto& objects = _document->GetObjects();

        for (auto i = 0; i < objects.size(); i++)
        {
            const auto& object = objects.at(i);
            ok &= writer.StartObject();

            ok &= writer.SaveUInt64(JSON_KEY_UUID, object->GetUuid());
            ok &= writer.SaveString(JSON_KEY_NAME, object->GetName());

            const auto objectType = object->GetObjectType();
            ok &= writer.SaveString(JSON_KEY_OBJECT_TYPE, ObjectTypeToString(objectType));

            const auto textObject = dynamic_cast<const TextObject*>(object.get());
            ok &= writer.SaveString(JSON_KEY_TEXT, textObject->GetText());

            switch (objectType)
            {
            case ObjectType::QuestObjectType:
            {
                const auto questObject = dynamic_cast<const QuestObject*>(textObject);
                const auto& actions = questObject->GetActions();

                ok &= writer.StartArray(JSON_KEY_ACTIONS);
                for (auto action = 0; action < actions.size(); action++)
                {
                    ok &= writer.SaveUInt64(actions.at(action));
                }
                ok &= writer.EndArray();

                ok &= writer.SaveBool(JSON_KEY_FINAL, questObject->IsFinal());
                break;
            }

            case ObjectType::ActionObjectType:
            {
                const auto actionObject = dynamic_cast<const ActionObject*>(textObject);
                ok &= writer.SaveUInt64(JSON_KEY_TARGET, actionObject->GetTargetUuid());
                break;
            }

            default:
                break;
            }

            ok &= writer.EndObject();
        }

        ok &= writer.EndArray();
        ok &= writer.End();

        return ok;
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSerializer::Deserialize(const std::filesystem::path& path)
    {
        JsonReader reader(path);

        if (!reader.Start())
        {
            return false;
        }

        _document->SetGameName(reader.GetString(JSON_KEY_GAME_NAME, "Untitled"));
        _document->SetDomainName(reader.GetString(JSON_KEY_GAME_DOMAIN_NAME, "Untitled"));
        _document->SetEntryPoint(UUID(reader.GetUInt64(JSON_KEY_ENTRY_POINT_UUID, UUID::InvalidUuid)));
        const auto objectsArraySize = reader.StartArray(JSON_KEY_OBJECTS);

        for (auto i = 0; i < objectsArraySize; i++)
        {
            reader.StartArrayObject(i);

            const auto objectUuid = UUID(reader.GetUInt64(JSON_KEY_UUID));
            const auto objectName = reader.GetString(JSON_KEY_NAME);
            const auto objectType = StringToObjectType(reader.GetString(JSON_KEY_OBJECT_TYPE));
            const auto objectText = reader.GetString(JSON_KEY_TEXT);

            switch (objectType)
            {
            case ObjectType::QuestObjectType:
            {
                auto questObject = CreatePtr<QuestObject>(objectUuid, [this]() { _document->SetDirty(true); });
                questObject->SetText(objectText);
                questObject->SetName(objectName);

                const auto questObjectActionsSize = reader.StartArray(JSON_KEY_ACTIONS);
                for (auto a = 0; a < questObjectActionsSize; a++)
                {
                    questObject->AddAction(UUID(reader.GetUInt64(a)));
                }
                reader.EndArray();
                
                questObject->SetFinal(reader.GetBool(JSON_KEY_FINAL));

                _document->AddObject(questObject);

                break;
            }

            case ObjectType::ActionObjectType:
            {
                auto actionObject = CreatePtr<ActionObject>(objectUuid, [this]() { _document->SetDirty(true); });
                actionObject->SetTargetUuid(UUID(reader.GetUInt64(JSON_KEY_TARGET)));
                actionObject->SetText(objectText);
                actionObject->SetName(objectName);

                _document->AddObject(actionObject);

                break;
            }

            default:
                return false;
            }

            reader.EndArrayObject();
        }

        return true;
    }
    //--------------------------------------------------------------------------
}