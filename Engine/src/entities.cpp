#include "entities.h"
#include "log.h"
#include "i18n_manager.h"

namespace Storyteller
{
    std::string ObjectTypeToString(ObjectType type)
    {
        switch (type)
        {
        case ObjectType::QuestObjectType:
            I18N::Manager::TranslateDefer(STRTLR_TR_DOMAIN_ENGINE, "Quest object");
            return "Quest object";

        case ObjectType::ActionObjectType:
            I18N::Manager::TranslateDefer(STRTLR_TR_DOMAIN_ENGINE, "Action object");
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

    const std::string& BasicObject::GetName() const
    {
        return _name;
    }
    //--------------------------------------------------------------------------

    void BasicObject::SetName(const std::string& name)
    {
        if (_name != name)
        {
            STRTLR_CORE_LOG_DEBUG("BasicObject: ({}) set name '{}'", _uuid, name);

            _name = name;
            if (_changeCallback)
            {
                _changeCallback();
            }
        }
    }
    //--------------------------------------------------------------------------

    void BasicObject::SetChangeCallback(const std::function<void()>& changeCallback)
    {
        _changeCallback = changeCallback;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    TextObject::TextObject(const UUID& uuid, const std::function<void()>& changeCallback)
        : BasicObject(uuid, changeCallback)
        , _text("")
    {
        STRTLR_CORE_LOG_DEBUG("TextObject: create ({})", uuid);
    }
    //--------------------------------------------------------------------------

    const std::string& TextObject::GetText() const
    {
        return _text;
    }
    //--------------------------------------------------------------------------

    void TextObject::SetText(const std::string& text)
    {
        if (_text != text)
        {
            STRTLR_CORE_LOG_DEBUG("TextObject: ({}) set text '{}'", _uuid, text);

            _text = text;
            if (_changeCallback)
            {
                _changeCallback();
            }
        }
    }
    //--------------------------------------------------------------------------

    bool TextObject::IsConsistent() const
    {
        return !_text.empty() && !_name.empty();
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------


    QuestObject::QuestObject(const UUID& uuid, const std::function<void()>& changeCallback)
        : TextObject(uuid, changeCallback)
        , _final(false)
    {
        STRTLR_CORE_LOG_DEBUG("QuestObject: create ({})", uuid);
    }
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
        if (!TextObject::IsConsistent())
        {
            return false;
        }

        return !_actions.empty() ^ _final;
    }
    //--------------------------------------------------------------------------

    const std::vector<UUID>& QuestObject::GetActions() const
    {
        return _actions;
    }
    //--------------------------------------------------------------------------

    bool QuestObject::AddAction(const UUID& actionUuid)
    {
        STRTLR_CORE_LOG_DEBUG("QuestObject: ({}) add action '{}'", _uuid, actionUuid);

        if (ContainsAction(actionUuid))
        {
            STRTLR_CORE_LOG_WARN("QuestObject: ({}) already contains action '{}'", _uuid, actionUuid);
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
        STRTLR_CORE_LOG_DEBUG("QuestObject: ({}) remove action '{}'", _uuid, actionUuid);

        const auto it = std::find(_actions.cbegin(), _actions.cend(), actionUuid);
        if (it == _actions.cend())
        {
            STRTLR_CORE_LOG_WARN("QuestObject: ({}) does not contain action '{}'", _uuid, actionUuid);
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

    bool QuestObject::MoveActionUp(const UUID& actionUuid)
    {
        const auto actionIndex = IndexOfAction(actionUuid);
        if (actionUuid == UUID::InvalidUuid || (actionIndex <= 0) || (actionIndex >= _actions.size()))
        {
            return false;
        }

        std::swap(_actions[actionIndex], _actions[actionIndex - 1]);

        if (_changeCallback)
        {
            _changeCallback();
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool QuestObject::MoveActionDown(const UUID& actionUuid)
    {
        const auto actionIndex = IndexOfAction(actionUuid);
        if (actionUuid == UUID::InvalidUuid || (actionIndex >= _actions.size() - 1))
        {
            return false;
        }

        std::swap(_actions[actionIndex], _actions[actionIndex + 1]);

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

    int QuestObject::IndexOfAction(const UUID& actionUuid) const
    {
        return std::find(_actions.cbegin(), _actions.cend(), actionUuid) - _actions.cbegin();
    }
    //--------------------------------------------------------------------------

    void QuestObject::SetFinal(bool isFinal)
    {
        if (_final != isFinal)
        {
            STRTLR_CORE_LOG_DEBUG("QuestObject: ({}) set final '{}'", _uuid, isFinal);

            _final = isFinal;
            if (_changeCallback)
            {
                _changeCallback();
            }
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
    {
        STRTLR_CORE_LOG_DEBUG("ActionObject: create ({})", uuid);
    }
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
        return _targetUuid != UUID::InvalidUuid && TextObject::IsConsistent();
    }
    //--------------------------------------------------------------------------

    UUID ActionObject::GetTargetUuid() const
    {
        return _targetUuid;
    }
    //--------------------------------------------------------------------------

    void ActionObject::SetTargetUuid(const UUID& targetUuid)
    {
        if (_targetUuid != targetUuid)
        {
            STRTLR_CORE_LOG_DEBUG("ActionObject: ({}) set target '{}'", _uuid, targetUuid);
            _targetUuid = targetUuid;
            if (_changeCallback)
            {
                _changeCallback();
            }
        }
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}