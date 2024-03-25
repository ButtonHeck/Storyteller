#pragma once

namespace Storyteller
{
    namespace Utils
    {
#define STRTLR_BIND(func) [this](auto&& ... args) { return this->func(std::forward<decltype(args)>(args)...); }
    }
}