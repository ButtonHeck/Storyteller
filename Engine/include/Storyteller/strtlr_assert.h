#pragma once

#include "macro.h"
#include "string_utils.h"

#include <string>
#include <sstream>
#include <source_location>

#ifndef STRTLR_ASSERTS_ACTIVE
    #ifdef NDEBUG
        #define STRTLR_ASSERTS_ACTIVE false
    #else
        #define STRTLR_ASSERTS_ACTIVE true
    #endif
#endif

namespace Storyteller
{
    class Assertion
    {
    public:
        enum class Consequence
        {
            Log,
            Terminate,
            Exception
        };

    public:
        Assertion(std::string expression, const std::source_location& location, Consequence consequence = Consequence::Terminate);
        ~Assertion();

        Assertion& Message(const std::string& message);

        template<typename T>
        Assertion& Watch(T&& val, const char* name)
        {
            Utils::ToSStream(_stream, "\n\t", name, " = ", std::forward<T>(val));
            return *this;
        }

        [[noreturn]]
        void Exception();

    private:
        const std::source_location _sourceLocation;
        Consequence _consequence;
        std::ostringstream _stream;
    };
    //--------------------------------------------------------------------------
}

#define STRTLR_ASSERT(expr) (!STRTLR_ASSERTS_ACTIVE || bool(expr)) ? void(0) : (void)Storyteller::Assertion{ STRTLR_M_STRINGIFY(expr), std::source_location::current() }
#define STRTLR_CHECK(expr) bool(expr) ? void(0) : (void)Storyteller::Assertion{ STRTLR_M_STRINGIFY(expr), std::source_location::current(), STRTLR_ASSERTS_ACTIVE ? Storyteller::Assertion::Consequence::Terminate : Storyteller::Assertion::Consequence::Log }

#define STRTLR_ASSERT_WATCH(...) STRTLR_M_DISPATCH_VA(STRTLR_AW_, __VA_ARGS__)
#define STRTLR_AW_(expr) Watch((expr), STRTLR_M_STRINGIFY(expr))
#define STRTLR_AW_1_(a) STRTLR_AW_(a)
#define STRTLR_AW_2_(a, b) STRTLR_AW_(a).STRTLR_AW_(b)
#define STRTLR_AW_3_(a, b, c) STRTLR_AW_(a).STRTLR_AW_(b).STRTLR_AW_(c)
#define STRTLR_AW_4_(a, b, c, d) STRTLR_AW_(a).STRTLR_AW_(b).STRTLR_AW_(c).STRTLR_AW_(d)
#define STRTLR_AW_5_(a, b, c, d, e) STRTLR_AW_(a).STRTLR_AW_(b).STRTLR_AW_(c).STRTLR_AW_(d).STRTLR_AW_(e)