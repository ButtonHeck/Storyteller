#include "game_document_sort_filter_proxy_view.h"

namespace Storyteller
{
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

    std::vector<BasicObject::Ptr> GameDocumentSortFilterProxyView::GetObjects(ObjectType type, bool noEmptyName) const
    {
        return _document->GetObjects(type, noEmptyName);
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

    void GameDocumentSortFilterProxyView::UpdateCache()
    {
        _cache = _document->GetObjects();
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
        std::vector<BasicObject::Ptr> temp;
        std::copy_if(_cache.cbegin(), _cache.cend(), std::back_inserter(temp), [&](const BasicObject::Ptr obj) { return _filter.Accept(obj->GetObjectType()); });
        std::swap(_cache, temp);
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}