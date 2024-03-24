#pragma once

#include "localization_base.h"

#include <boost/locale.hpp>

#include <string>

namespace Storyteller
{
    namespace I18N
    {
        namespace Translator
        {
            TranslationStr Translate(const DomainStr& domain, const SourceStr& source);
            TranslationStr Translate(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count);
            TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& source, const ContextStr& context);
            TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context);

            inline void TranslateDefer(const DomainStr& domain, const SourceStr& source) {};
            inline void TranslateDefer(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count) {};
            inline void TranslateCtxDefer(const DomainStr& domain, const SourceStr& source, const ContextStr& context) {};
            inline void TranslateCtxDefer(const DomainStr& domain, const SourceStr& sourceSingular, const SourceStr& sourcePlural, int count, const ContextStr& context) {};

            template<typename... Types>
            std::string Format(const SourceStr& source, Types&&... args)
            {
                return (boost::locale::format(source) % ... % std::forward<Types>(args)).str();
            }
        }
    }
    //--------------------------------------------------------------------------
}