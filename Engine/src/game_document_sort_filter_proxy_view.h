#pragma once

#include "uuid.h"
#include "entities.h"
#include "game_document.h"

#include <set>
#include <memory>

namespace Storyteller
{
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
        BasicObject::Ptr GetBasicObject(const UUID& uuid) const;
        BasicObject::Ptr GetBasicObject(const std::string& name) const;
        const std::vector<BasicObject::Ptr>& GetObjects() const;
        std::vector<BasicObject::Ptr> GetObjects(ObjectType type, bool noEmptyName) const;
        void SetEntryPoint(const UUID& uuid);
        BasicObject::Ptr GetEntryPoint() const;

        void Select(const UUID& uuid);
        bool IsSelected(const UUID& uuid) const;
        bool RemoveSelected();
        BasicObject::Ptr GetSelectedObject() const;

        void UpdateCache();
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
}
