#pragma once

#include <boost/locale.hpp>

#include <string>

namespace Storyteller
{
    namespace LocalizationTranslator
    {
        std::string Translate(const std::string& domain, const std::string& message);
        std::string Translate(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count);
        std::string TranslateCtx(const std::string& domain, const std::string& message, const std::string& context);
        std::string TranslateCtx(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count, const std::string& context);

        template<typename... Types>
        std::string Format(const std::string& message, Types&&... args)
        {
            return (boost::locale::format(message) % ... % std::forward<Types>(args)).str();
        }
    };
    //--------------------------------------------------------------------------
}