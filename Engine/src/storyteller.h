#include "uuid.h"
#include "console_printer.h"

#include <boost/locale.hpp>

#include <set>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <functional>

namespace Storyteller
{
    enum class ObjectType
    {
        QuestObjectType,
        ActionObjectType,
        ErrorObjectType
    };

    std::string ObjectTypeToString(ObjectType type);
    ObjectType StringToObjectType(const std::string& string);
    //--------------------------------------------------------------------------


    class BasicObject
    {
    public:
        typedef std::shared_ptr<BasicObject> Ptr;

        explicit BasicObject(const UUID& uuid = UUID(), const std::function<void()>& changeCallback = nullptr);
        virtual ~BasicObject() = default;

        UUID GetUuid() const;

        std::string GetName() const;
        void SetName(const std::string& name);

        void SetChangeCallback(std::function<void()> changeCallback);

        virtual ObjectType GetObjectType() const = 0;
        virtual bool IsConsistent() const = 0;

    protected:
        const UUID _uuid;
        std::string _name;
        std::function<void()> _changeCallback;
    };
    //--------------------------------------------------------------------------


    class TextObject : public BasicObject
    {
    public:
        typedef std::shared_ptr<TextObject> Ptr;

        explicit TextObject(const UUID& uuid = UUID(), const std::function<void()>& changeCallback = nullptr);

        std::string GetText() const;
        void SetText(const std::string& text);

        virtual bool IsConsistent() const override;

    protected:
        std::string _text;
    };
    //--------------------------------------------------------------------------


    class QuestObject : public TextObject
    {
    public:
        typedef std::shared_ptr<QuestObject> Ptr;

        explicit QuestObject(const UUID& uuid = UUID(), const std::function<void()>& changeCallback = nullptr);

        static ObjectType GetStaticObjectType();
        virtual ObjectType GetObjectType() const override;
        virtual bool IsConsistent() const override;

        const std::vector<UUID>& GetActions() const;
        bool AddAction(const UUID& actionUuid);
        bool RemoveAction(const UUID& actionUuid);
        bool ContainsAction(const UUID& actionUuid) const;
        void SetFinal(bool isFinal);
        bool IsFinal() const;

    protected:
        std::vector<UUID> _actions;
        bool _final;
    };
    //--------------------------------------------------------------------------


    class ActionObject : public TextObject
    {
    public:
        typedef std::shared_ptr<ActionObject> Ptr;

        explicit ActionObject(const UUID& uuid = UUID(), const std::function<void()>& changeCallback = nullptr);

        static ObjectType GetStaticObjectType();
        virtual ObjectType GetObjectType() const override;
        virtual bool IsConsistent() const override;

        UUID GetTargetUuid() const;
        void SetTargetUuid(const UUID& targetUuid);

    protected:
        UUID _targetUuid;
    };
    //--------------------------------------------------------------------------


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


    class GameDocumentSortFilterProxyView
    {
    public:
        typedef std::shared_ptr<GameDocumentSortFilterProxyView> Ptr;

        struct Sorter
        {
            enum SortValue
            {
                Type = 0,
                Uuid = 1,
                Name = 2
            };

            Sorter() = default;
            Sorter(bool ascending, SortValue sortValue);

            bool operator()(BasicObject::Ptr a, BasicObject::Ptr b) const;

            SortValue sortValue;
            bool ascending;
            bool active;
        };

        struct Filter
        {
            Filter();

            bool Accept(ObjectType type) const;
            bool NeedFilter() const;

            std::set<ObjectType> filterTypes;
            bool active;
        };

        explicit GameDocumentSortFilterProxyView(GameDocument::Ptr document);

        GameDocument::Ptr GetSourceDocument() const;

        bool AddObject(ObjectType type, const UUID& uuid = UUID());
        bool AddObject(const BasicObject::Ptr& object);
        bool RemoveObject(const UUID& uuid);
        BasicObject::Ptr GetObject(const UUID& uuid) const;
        BasicObject::Ptr GetObject(const std::string& name) const;
        const std::vector<BasicObject::Ptr>& GetObjects() const;
        void SetEntryPoint(const UUID& uuid);
        BasicObject::Ptr GetEntryPoint() const;

        void Select(const UUID& uuid);
        bool IsSelected(const UUID& uuid) const;
        bool RemoveSelected();
        BasicObject::Ptr GetSelectedObject() const;

        void DoSort(bool ascending, Sorter::SortValue sortValue);
        void DoFilter(ObjectType type, bool accept);

    private:
        void UpdateView();
        void DoSort();
        void DoFilter();

    private:
        GameDocument::Ptr const _document;
        std::vector<BasicObject::Ptr> _cache;
        UUID _selectedUuid;
        Sorter _sorter;
        Filter _filter;
    };
    //--------------------------------------------------------------------------


    class LocalizationManager
    {
    public:
        typedef std::shared_ptr<LocalizationManager> Ptr;

        explicit LocalizationManager(const std::string& defaultPath = "");

        void Build(const std::string& localeString);
        void AddMessagesPath(const std::string& path);
        void AddMessagesDomain(const std::string& domain);
        std::string Translate(const std::string& domain, const std::string& message);
        bool CreateTranslations(const GameDocument::Ptr document, const std::filesystem::path& path) const;

    private:
        boost::locale::generator _localeGenetator;
        std::string _localeString;

        typedef std::string SourceMessage;
        typedef std::string DomainName;
        typedef std::map<SourceMessage, std::string> MessagesTranslations;
        typedef std::map<DomainName, MessagesTranslations> DomainMessages;
        DomainMessages _messages;
    };
    //--------------------------------------------------------------------------


    class GameController
    {
    public:
        typedef std::shared_ptr<GameController> Ptr;

        GameController(const GameDocument::Ptr gameDocument, const LocalizationManager::Ptr localizationManager);

        void Start();

    private:
        ConsoleManager::Ptr const _consoleManager;
        GameDocument::Ptr const _gameDocument;
        LocalizationManager::Ptr const _localizationManager;
    };
    //--------------------------------------------------------------------------
}
