#pragma once

#include <string>

namespace Storyteller
{
    namespace I18N
    {
        static constexpr auto TranslateKeyword = "Translate";
        static constexpr auto TranslateCtxKeyword = "TranslateCtx";
        static constexpr auto TranslateDeferKeyword = "TranslateDefer";
        static constexpr auto TranslateCtxDeferKeyword = "TranslateCtxDefer";

        static constexpr auto LocaleEnUTF8Keyword = "en_EN.UTF-8";
        static constexpr auto LocaleEnName = "English";
        static constexpr auto LocaleRuUTF8Keyword = "ru_RU.UTF-8";
        static constexpr auto LocaleRuName = "Russian";

        typedef std::string LocaleStr;
        typedef std::string DomainStr;
        typedef std::string ContextStr;
        typedef std::string SourceStr;
        typedef std::string TranslationStr;

        struct ContextedSource
        {
            SourceStr source;
            ContextStr context;

            bool operator==(const ContextedSource& other) const
            {
                return source == other.source && context == other.context;
            }
        };
        //--------------------------------------------------------------------------

        struct ContextedSourceHash
        {
            std::size_t operator()(const ContextedSource& p) const
            {
                const auto hs = std::hash<SourceStr>{}(p.source);
                const auto hc = std::hash<ContextStr>{}(p.context);

                return hs ^ hc;
            }
        };
        //--------------------------------------------------------------------------
    }
}
