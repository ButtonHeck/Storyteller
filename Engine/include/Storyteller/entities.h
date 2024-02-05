#pragma once

#include "uuid.h"

#include <string>
#include <vector>
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
}