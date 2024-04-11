#include "game_document_sort_filter_proxy_view.h"
#include "log.h"

namespace Storyteller
{
    GameDocumentSortFilterProxyView::Sorter::Sorter(bool ascending, SortValue sortValue)
        : sortValue(sortValue)
        , ascending(ascending)
        , active(false)
    {}
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::Sorter::operator()(const Ptr<BasicObject>& a, const Ptr<BasicObject>& b) const
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
        : filterTypes({ ObjectType::QuestObjectType, ObjectType::ActionObjectType })
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

    GameDocumentSortFilterProxyView::GameDocumentSortFilterProxyView(const Ptr<GameDocument> document)
        : _document(document)
        , _cache(document->GetObjects())
        , _selectedUuid(UUID::InvalidUuid)
        , _sorter()
    {
        STRTLR_CORE_LOG_INFO("GameDocumentSortFilterProxyView: create");
    }
    //--------------------------------------------------------------------------

    const Ptr<GameDocument> GameDocumentSortFilterProxyView::GetSourceDocument() const
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

    bool GameDocumentSortFilterProxyView::AddObject(const Ptr<BasicObject>& object)
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

    Ptr<BasicObject> GameDocumentSortFilterProxyView::GetObject(const UUID& uuid) const
    {
        return _document->GetObject(uuid);
    }
    //--------------------------------------------------------------------------

    Ptr<BasicObject> GameDocumentSortFilterProxyView::GetObject(const std::string& name) const
    {
        return _document->GetObject(name);
    }
    //--------------------------------------------------------------------------

    const std::vector<Ptr<BasicObject>>& GameDocumentSortFilterProxyView::GetObjects() const
    {
        return _cache;
    }
    //--------------------------------------------------------------------------

    std::vector<Ptr<BasicObject>> GameDocumentSortFilterProxyView::GetObjects(ObjectType type) const
    {
        return _document->GetObjects(type);
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::SetEntryPoint(const UUID& uuid)
    {
        _document->SetEntryPoint(uuid);
    }
    //--------------------------------------------------------------------------

    Ptr<BasicObject> GameDocumentSortFilterProxyView::GetEntryPoint() const
    {
        return _document->GetEntryPoint();
    }
    //--------------------------------------------------------------------------

    bool GameDocumentSortFilterProxyView::SetObjectName(const UUID& uuid, const std::string& name) const
    {
        return _document->SetObjectName(uuid, name);
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::Select(const UUID& uuid)
    {
        if (_selectedUuid != uuid)
        {
            STRTLR_CORE_LOG_INFO("GameDocumentSortFilterProxyView: select ({})", uuid);

            _selectedUuid = uuid;
        }
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

    Ptr<BasicObject> GameDocumentSortFilterProxyView::GetSelectedObject() const
    {
        if (_selectedUuid == UUID::InvalidUuid)
        {
            return nullptr;
        }

        const auto it = std::find_if(_cache.cbegin(), _cache.cend(), [&](const Ptr<BasicObject>& obj) { return obj->GetUuid() == _selectedUuid; });
        if (it != _cache.cend())
        {
            return *it;
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::UpdateCache()
    {
        STRTLR_CORE_LOG_INFO("GameDocumentSortFilterProxyView: updating cache");

        _cache = _document->GetObjects();
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::DoSort(bool ascending, Sorter::SortValue sortValue)
    {
        STRTLR_CORE_LOG_INFO("GameDocumentSortFilterProxyView: sorting '{}', ascending: '{}'", sortValue, ascending);

        _sorter.active = true;
        _sorter.ascending = ascending;
        _sorter.sortValue = sortValue;
        DoSort();
    }
    //--------------------------------------------------------------------------

    void GameDocumentSortFilterProxyView::DoFilter(ObjectType type, bool accept)
    {
        STRTLR_CORE_LOG_INFO("GameDocumentSortFilterProxyView: filtering '{}', accepted: '{}'", ObjectTypeToString(type), accept);

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
        STRTLR_CORE_LOG_INFO("GameDocumentSortFilterProxyView: updating view");

        UpdateCache();

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
        std::vector<Ptr<BasicObject>> temp;
        std::copy_if(_cache.cbegin(), _cache.cend(), std::back_inserter(temp), [&](const Ptr<BasicObject>& obj) { return _filter.Accept(obj->GetObjectType()); });
        std::swap(_cache, temp);
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}