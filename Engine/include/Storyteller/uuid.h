#pragma once

#include <cstdint>

namespace Storyteller
{
    class UUID
    {
    public:
        static UUID InvalidUuid;

        UUID();
        explicit UUID(uint64_t uuid);

        operator uint64_t() const;

    private:
        uint64_t _uuid;
    };
    //--------------------------------------------------------------------------
}

namespace std
{
    template <typename T> struct hash;

    template<>
    struct hash<Storyteller::UUID>
    {
        size_t operator()(const Storyteller::UUID& uuid) const
        {
            return uint64_t(uuid);
        }
    };
    //--------------------------------------------------------------------------
}
