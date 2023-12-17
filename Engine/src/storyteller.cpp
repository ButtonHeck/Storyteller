#include "storyteller.h"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/istreamwrapper.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace Storyteller
{
    std::string ObjectTypeToString(ObjectType type)
    {
        switch (type)
        {
        case ObjectType::QuestObjectType:
            return "Quest object";

        case ObjectType::ActionObjectType:
            return "Action object";

        default:
            break;
        }

        return std::string("Error");
    }
    //--------------------------------------------------------------------------

    ObjectType StringToObjectType(const std::string& string)
    {
        if (string == "Quest object")
        {
            return ObjectType::QuestObjectType;
        }
        else if (string == "Action object")
        {
            return ObjectType::ActionObjectType;
        }

        return ObjectType::ErrorObjectType;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    BasicObject::BasicObject(const UUID& uuid, const std::function<void()>& changeCallback)
        : _uuid(uuid)
        , _name("")
        , _changeCallback(changeCallback)
    {}
    //--------------------------------------------------------------------------

    UUID BasicObject::GetUuid() const
    {
        return _uuid;
    }
    //--------------------------------------------------------------------------

    std::string BasicObject::GetName() const
    {
        return _name;
    }
    //--------------------------------------------------------------------------

    void BasicObject::SetName(const std::string& name)
    {
        _name = name;
        if (_changeCallback)
        {
            _changeCallback();
        }
    }
    //--------------------------------------------------------------------------

    void BasicObject::SetChangeCallback(std::function<void()> changeCallback)
    {
        _changeCallback = changeCallback;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    TextObject::TextObject(const UUID& uuid, const std::function<void()>& changeCallback)
        : BasicObject(uuid, changeCallback)
        , _text("")
    {}
    //--------------------------------------------------------------------------

    std::string TextObject::GetText() const
    {
        return _text;
    }
    //--------------------------------------------------------------------------

    void TextObject::SetText(const std::string& text)
    {
        _text = text;
        if (_changeCallback)
        {
            _changeCallback();
        }
    }
    //--------------------------------------------------------------------------

    bool TextObject::IsConsistent() const
    {
        return !_text.empty();
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    QuestObject::QuestObject(const UUID& uuid, const std::function<void()>& changeCallback)
        : TextObject(uuid, changeCallback)
        , _final(false)
    {}
    //--------------------------------------------------------------------------

    ObjectType QuestObject::GetStaticObjectType()
    {
        return ObjectType::QuestObjectType;
    }
    //--------------------------------------------------------------------------

    ObjectType QuestObject::GetObjectType() const
    {
        return GetStaticObjectType();
    }
    //--------------------------------------------------------------------------

    bool QuestObject::IsConsistent() const
    {
        if (_actions.empty())
        {
            return !_final
                ? false
                : TextObject::IsConsistent();
        }

        return _final
            ? false 
            : TextObject::IsConsistent();
    }
    //--------------------------------------------------------------------------

    const std::vector<UUID>& QuestObject::GetActions() const
    {
        return _actions;
    }
    //--------------------------------------------------------------------------

    bool QuestObject::AddAction(const UUID& actionUuid)
    {
        if (ContainsAction(actionUuid))
        {
            return false;
        }

        _actions.push_back(actionUuid);
        if (_changeCallback)
        {
            _changeCallback();
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool QuestObject::RemoveAction(const UUID& actionUuid)
    {
        const auto it = std::find(_actions.cbegin(), _actions.cend(), actionUuid);
        if (it == _actions.cend())
        {
            return false;
        }

        _actions.erase(it);
        if (_changeCallback)
        {
            _changeCallback();
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool QuestObject::ContainsAction(const UUID& actionUuid) const
    {
        return std::find(_actions.cbegin(), _actions.cend(), actionUuid) != _actions.cend();
    }
    //--------------------------------------------------------------------------

    void QuestObject::SetFinal(bool isFinal)
    {
        _final = isFinal;
    }
    //--------------------------------------------------------------------------

    bool QuestObject::IsFinal() const
    {
        return _final;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    ActionObject::ActionObject(const UUID& uuid, const std::function<void()>& changeCallback)
        : TextObject(uuid, changeCallback)
        , _targetUuid(UUID::InvalidUuid)
    {}
    //--------------------------------------------------------------------------

    ObjectType ActionObject::GetStaticObjectType()
    {
        return ObjectType::ActionObjectType;
    }
    //--------------------------------------------------------------------------

    ObjectType ActionObject::GetObjectType() const
    {
        return GetStaticObjectType();
    }
    //--------------------------------------------------------------------------

    bool ActionObject::IsConsistent() const
    {
        return _targetUuid == UUID::InvalidUuid
            ? false 
            : TextObject::IsConsistent();
    }
    //--------------------------------------------------------------------------

    UUID ActionObject::GetTargetUuid() const
    {
        return _targetUuid;
    }
    //--------------------------------------------------------------------------

    void ActionObject::SetTargetUuid(const UUID& targetUuid)
    {
        _targetUuid = targetUuid;
        if (_changeCallback)
        {
            _changeCallback();
        }
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


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

        if (!std::filesystem::exists(path))
        {
            if (!std::filesystem::create_directory(path.parent_path()))
            {
                return false;
            }
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


    GameDocumentSortFilterProxyView::Sorter::Sorter(bool ascending, SortValue sortValue)
        : ascending(ascending)
        , sortValue(sortValue)
        , active(false)
    {}
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::Sorter::operator()(BasicObject::Ptr a, BasicObject::Ptr b) const
    {
        switch (sortValue)
        {
        case Type:
            return ascending 
                ? (a->GetObjectType() < b->GetObjectType()) 
                : (a->GetObjectType() > b->GetObjectType());

        case Uuid:
            return ascending 
                ? (a->GetUuid() < b->GetUuid()) 
                : (a->GetUuid() > b->GetUuid());

        case Name:
            return ascending 
                ? (a->GetName() < b->GetName()) 
                : (a->GetName() > b->GetName());

        default:
            return true;
        }
    }
    //--------------------------------------------------------------------------

    GameDocumentSortFilterProxyView::Filter::Filter()
        : filterTypes({ObjectType::QuestObjectType, ObjectType::ActionObjectType})
        , active(false)
    {}
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::Filter::Accept(ObjectType type) const
    {
        return filterTypes.contains(type);
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::Filter::NeedFilter() const
    {
        return !filterTypes.contains(ObjectType::QuestObjectType) ||
            !filterTypes.contains(ObjectType::ActionObjectType);
    }
    //--------------------------------------------------------------------------

    GameDocumentSortFilterProxyView::GameDocumentSortFilterProxyView(GameDocument::Ptr document)
        : _document(document)
        , _cache(document->GetObjects())
        , _selectedUuid(UUID::InvalidUuid)
        , _sorter()
    {}
    //--------------------------------------------------------------------------

    GameDocument::Ptr GameDocumentSortFilterProxyView::GetSourceDocument() const
    {
        return _document;
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::AddObject(ObjectType type, const UUID& uuid)
    {
        if (!_document->AddObject(type, uuid))
        {
            return false;
        }

        UpdateView();

        Select(uuid);
        return true;
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::AddObject(const BasicObject::Ptr& object)
    {
        if (!_document->AddObject(object))
        {
            return false;
        }

        UpdateView();

        Select(object->GetUuid());
        return true;
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::RemoveObject(const UUID& uuid)
    {
        if (!_document->RemoveObject(uuid))
        {
            return false;
        }

        UpdateView();

        if (IsSelected(uuid))
        {
            Select(UUID::InvalidUuid);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    BasicObject::Ptr GameDocumentSortFilterProxyView::GetObject(const UUID& uuid) const
    {
        return _document->GetObject(uuid);
    }
    //--------------------------------------------------------------------------

    BasicObject::Ptr GameDocumentSortFilterProxyView::GetObject(const std::string& name) const
    {
        return _document->GetObject(name);
    }
    //--------------------------------------------------------------------------

    const std::vector<BasicObject::Ptr>& GameDocumentSortFilterProxyView::GetObjects() const
    {
        return _cache;
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::SetEntryPoint(const UUID& uuid)
    {
        _document->SetEntryPoint(uuid);
    }
    //--------------------------------------------------------------------------

    BasicObject::Ptr GameDocumentSortFilterProxyView::GetEntryPoint() const
    {
        return _document->GetEntryPoint();
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::Select(const UUID& uuid)
    {
        _selectedUuid = uuid;
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::IsSelected(const UUID& uuid) const
    {
        return _selectedUuid == uuid;
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::RemoveSelected()
    {
        const auto removeIt = GetSelectedObject();
        if (removeIt)
        {
            return RemoveObject(removeIt->GetUuid());
        }

        return false;
    }
    //--------------------------------------------------------------------------

    BasicObject::Ptr GameDocumentSortFilterProxyView::GetSelectedObject() const
    {
        if (_selectedUuid == UUID::InvalidUuid)
        {
            return nullptr;
        }

        const auto it = std::find_if(_cache.cbegin(), _cache.cend(), [&](const BasicObject::Ptr obj) { return obj->GetUuid() == _selectedUuid; });
        if (it != _cache.cend())
        {
            return *it;
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::DoSort(bool ascending, Sorter::SortValue sortValue)
    {
        _sorter.active = true;
        _sorter.ascending = ascending;
        _sorter.sortValue = sortValue;
        DoSort();
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::DoFilter(ObjectType type, bool accept)
    {
        _filter.active = true;
        if (accept)
        {
            _filter.filterTypes.insert(type);
        }
        else
        {
            _filter.filterTypes.erase(type);
        }

        DoFilter();
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::UpdateView()
    {
        _cache = _document->GetObjects();

        if (_sorter.active)
        {
            DoSort();
        }

        if (_filter.active && _filter.NeedFilter())
        {
            DoFilter();
        }
    }

    void GameDocumentSortFilterProxyView::DoSort()
    {
        std::sort(_cache.begin(), _cache.end(), _sorter);
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::DoFilter()
    {
        std::vector<BasicObject::Ptr> temp;
        std::copy_if(_cache.cbegin(), _cache.cend(), std::back_inserter(temp), [&](const BasicObject::Ptr obj) { return _filter.Accept(obj->GetObjectType()); });
        std::swap(_cache, temp);
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    LocalizationManager::LocalizationManager(const std::string& defaultPath)
        : _localeGenetator()
        , _localeString("")
    {
        if (!defaultPath.empty())
        {
            _localeGenetator.add_messages_path(defaultPath);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::Build(const std::string& localeString)
    {
        _localeString = localeString;
        std::locale::global(_localeGenetator(_localeString));
        std::cout.imbue(std::locale());
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesPath(const std::string& path)
    {
        _localeGenetator.add_messages_path(path);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesDomain(const std::string& domain)
    {
        _localeGenetator.add_messages_domain(domain);
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::Translate(const std::string& domain, const std::string& message)
    {
        if (_messages.contains(domain))
        {
            const auto& domainTranslations = _messages[domain];
            if (domainTranslations.contains(message))
            {
                return domainTranslations.at(message);
            }
        }

        const auto translation = boost::locale::translate(message).str(domain);
        if (translation == message && !_localeString.empty())
        {
            return "No translation";
        }

        _messages[domain][message] = translation;

        return translation;
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::CreateTranslations(const GameDocument::Ptr document, const std::filesystem::path& path) const
    {
        if (path.empty() || !path.has_filename() || !path.has_extension())
        {
            return false;
        }

        if (!std::filesystem::exists(path))
        {
            if (!std::filesystem::create_directory(path.parent_path()))
            {
                return false;
            }
        }

        std::ofstream outputStream(path.string(), std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            return false;
        }

        std::stringstream ss;
        ss << document->GetGameName() << "\n";
        const auto documentObjects = document->GetObjects();
        for (size_t i = 0; i < documentObjects.size(); i++)
        {
            const auto object = documentObjects.at(i);
            const auto textObject = dynamic_cast<const TextObject*>(object.get());
            ss << object->GetName() << ", Translate(\"" << document->GetGameName() << "\", \"" << textObject->GetText() << "\");\n";
        }

        outputStream << ss.str();
        outputStream.close();

        return true;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    GameController::GameController(const GameDocument::Ptr gameDocument, const LocalizationManager::Ptr localizationManager)
        : _consoleManager(std::make_shared<ConsoleManager>())
        , _gameDocument(gameDocument)
        , _localizationManager(localizationManager)
    {
    }
    //--------------------------------------------------------------------------

    void GameController::Start()
    {
        auto currentObject = _gameDocument->GetEntryPoint();
        if (!currentObject)
        {
            _consoleManager->ClearConsole();
            _consoleManager->PrintCriticalHint();
            _consoleManager->WaitForKeyboardHit();
            return;
        }

        QuestObject* currentQuestObject = nullptr;
        const auto gameName = _gameDocument->GetGameName();
        auto finalReached = false;

        while (!finalReached)
        {
            if (currentObject->GetObjectType() != QuestObject::GetStaticObjectType())
            {
                _consoleManager->PrintCriticalHint();
                _consoleManager->WaitForKeyboardHit();
                return;
            }

            currentQuestObject = dynamic_cast<QuestObject*>(currentObject.get());
            _consoleManager->ClearConsole();
            _consoleManager->PrintMadeByString();
            _consoleManager->PrintSeparator();
            _consoleManager->PrintMessage(gameName);
            _consoleManager->PrintSeparator();
            _consoleManager->PrintMessage(currentQuestObject->GetText());

            const auto questActions = currentQuestObject->GetActions();
            std::vector<std::string> actionTexts(questActions.size());
            for (auto i = 0; i < questActions.size(); i++)
            {
                const auto object = _gameDocument->GetObject(questActions.at(i));
                if (object->GetObjectType() != ActionObject::GetStaticObjectType())
                {
                    _consoleManager->PrintCriticalHint();
                    _consoleManager->WaitForKeyboardHit();
                    return;
                }

                const auto actionObject = dynamic_cast<ActionObject*>(object.get());
                actionTexts.emplace_back(actionObject->GetText());
            }

            _consoleManager->PrintActions(actionTexts);

            int actionIndex = 0;
            while (!actionIndex || (actionIndex >= questActions.size()))
            {
                _consoleManager->PrintInputHint();

                std::string input;
                std::cin >> input;

                try
                {
                    actionIndex = std::stoi(input) - 1;
                    if (actionIndex >= 0 && actionIndex < questActions.size())
                    {
                        currentObject = _gameDocument->GetObject(questActions.at(actionIndex));
                        if (currentObject->GetObjectType() != QuestObject::GetStaticObjectType())
                        {
                            _consoleManager->PrintCriticalHint();
                            _consoleManager->WaitForKeyboardHit();
                            return;
                        }

                        currentQuestObject = dynamic_cast<QuestObject*>(currentObject.get());
                        finalReached = currentQuestObject->IsFinal();
                    }
                    else
                    {
                        _consoleManager->PrintErrorHint();
                    }
                }
                catch (const std::exception& e)
                {
                    _consoleManager->PrintErrorHint();
                }
            }
        }

        _consoleManager->ClearConsole();
        _consoleManager->PrintMadeByString();
        _consoleManager->PrintSeparator();
        _consoleManager->PrintMessage(gameName);
        _consoleManager->PrintSeparator();
        _consoleManager->PrintMessage(currentQuestObject->GetText());
        _consoleManager->PrintEndHint();
        _consoleManager->WaitForKeyboardHit();
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}
