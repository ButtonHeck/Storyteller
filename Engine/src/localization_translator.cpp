#include "localization_translator.h"

#include <boost/locale.hpp>

namespace Storyteller
{
    std::string LocalizationTranslator::Translate(const std::string& domain, const std::string& message)
    {
        return boost::locale::translate(message).str(domain);
    }
    //--------------------------------------------------------------------------

    std::string LocalizationTranslator::Translate(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count)
    {
        return (boost::locale::format(boost::locale::translate(messageSingular, messagePlural, count).str(domain)) % count).str();
    }
    //--------------------------------------------------------------------------

    std::string LocalizationTranslator::TranslateCtx(const std::string& domain, const std::string& message, const std::string& context)
    {
        return boost::locale::translate(context, message).str(domain);
    }
    //--------------------------------------------------------------------------

    std::string LocalizationTranslator::TranslateCtx(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count, const std::string& context)
    {
        return (boost::locale::format(boost::locale::translate(context, messageSingular, messagePlural, count).str(domain)) % count).str();
    }
    //--------------------------------------------------------------------------
}