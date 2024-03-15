#include "localization_translator.h"

namespace Storyteller::LocalizationTranslator
{
    std::string Translate(const std::string& domain, const std::string& message)
    {
        return boost::locale::translate(message).str(domain);
    }
    //--------------------------------------------------------------------------

    std::string Translate(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count)
    {
        return boost::locale::translate(messageSingular, messagePlural, count).str(domain);
    }
    //--------------------------------------------------------------------------

    std::string TranslateCtx(const std::string& domain, const std::string& message, const std::string& context)
    {
        return boost::locale::translate(context, message).str(domain);
    }
    //--------------------------------------------------------------------------

    std::string TranslateCtx(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count, const std::string& context)
    {
        return boost::locale::translate(context, messageSingular, messagePlural, count).str(domain);
    }
    //--------------------------------------------------------------------------
}