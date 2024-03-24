#include "i18n_library.h"
#include "i18n_lookup_dictionary.h"

namespace Storyteller
{
    namespace I18N
    {
        static const TranslationStr noTranslation = TranslationStr("");

        Library::Library(const LocaleStr& defaultLocale)
            : _currentLocale(defaultLocale)
        {}
        //--------------------------------------------------------------------------

        void Library::SetLocale(const LocaleStr& localeString)
        {
            if (_currentLocale != localeString)
            {
                _currentLocale = localeString;
                for (auto& dictionary : _lookupDictionaries)
                {
                    dictionary.second->SetLocale(localeString);
                }
            }
        }
        //--------------------------------------------------------------------------

        Ptr<LookupDictionary> Library::AddLookupDictionary(const DomainStr& domain)
        {
            auto dictionary = CreatePtr<LookupDictionary>(domain, _currentLocale);
            _lookupDictionaries.insert(std::make_pair(domain, dictionary));
            return dictionary;
        }
        //--------------------------------------------------------------------------

        Ptr<LookupDictionary> Library::GetLookupDictionary(const DomainStr& domain) const
        {
            if (_lookupDictionaries.contains(domain))
            {
                return _lookupDictionaries.at(domain);
            }

            return nullptr;
        }
        //--------------------------------------------------------------------------

        void Library::RemoveLookupDictionary(const DomainStr& domain)
        {
            _lookupDictionaries.erase(domain);
        }
        //--------------------------------------------------------------------------

        void Library::Add(const DomainStr& domain, const SourceStr& source, const TranslationStr& translation)
        {
            if (_lookupDictionaries.contains(domain))
            {
                _lookupDictionaries.at(domain)->Add(source, translation);
            }
        }
        //--------------------------------------------------------------------------

        void Library::Add(const DomainStr& domain, const SourceStr& source, const ContextStr& context, const TranslationStr& translation)
        {
            if (_lookupDictionaries.contains(domain))
            {
                _lookupDictionaries.at(domain)->Add(source, context, translation);
            }
        }
        //--------------------------------------------------------------------------

        const TranslationStr& Library::Get(const DomainStr& domain, const SourceStr& source)
        {
            if (_lookupDictionaries.contains(domain))
            {
                return _lookupDictionaries.at(domain)->Get(source);
            }

            return noTranslation;
        }
        //--------------------------------------------------------------------------

        const TranslationStr& Library::Get(const DomainStr& domain, const SourceStr& source, const ContextStr& context)
        {
            if (_lookupDictionaries.contains(domain))
            {
                return _lookupDictionaries.at(domain)->Get(source, context);
            }

            return noTranslation;
        }
        //--------------------------------------------------------------------------
    }
}
