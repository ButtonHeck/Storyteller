#pragma once

#include "pointers.h"
#include "uuid.h"
#include "entities.h"
#include "game_document.h"

#include <set>

namespace Storyteller
{
    class GameDocumentSortFilterProxyView
    {
    public:
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

            bool operator()(Ptr<BasicObject> a, Ptr<BasicObject> b) const;

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

        explicit GameDocumentSortFilterProxyView(Ptr<GameDocument> document);

        Ptr<GameDocument> GetSourceDocument() const;

        bool AddObject(ObjectType type, const UUID& uuid = UUID());
        bool AddObject(const Ptr<BasicObject>& object);
        bool RemoveObject(const UUID& uuid);
        Ptr<BasicObject> GetBasicObject(const UUID& uuid) const;
        Ptr<BasicObject> GetBasicObject(const std::string& name) const;
        const std::vector<Ptr<BasicObject>>& GetObjects() const;
        std::vector<Ptr<BasicObject>> GetObjects(ObjectType type, bool noEmptyName) const;
        void SetEntryPoint(const UUID& uuid);
        Ptr<BasicObject> GetEntryPoint() const;

        void Select(const UUID& uuid);
        bool IsSelected(const UUID& uuid) const;
        bool RemoveSelected();
        Ptr<BasicObject> GetSelectedObject() const;

        void UpdateCache();
        void DoSort(bool ascending, Sorter::SortValue sortValue);
        void DoFilter(ObjectType type, bool accept);

    private:
        void UpdateView();
        void DoSort();
        void DoFilter();

    private:
        Ptr<GameDocument> const _document;
        std::vector<Ptr<BasicObject>> _cache;
        UUID _selectedUuid;
        Sorter _sorter;
        Filter _filter;
    };
    //--------------------------------------------------------------------------
}
