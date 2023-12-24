#include "entities.h"

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
        if (_changeCallback)
        {
            _changeCallback();
        }
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
}