#include "i18n_manager.h"
#include "i18n_library.h"
#include "log.h"

#include <iostream>

namespace Storyteller
{
    namespace I18N
    {
        Manager::Manager(const LocaleStr& defaultLocale, const std::string& defaultPath)
            : _localeGenerator()
            , _library(CreatePtr<Library>(""))
            , _currentLocale("")
        {
            STRTLR_CORE_LOG_INFO("I18NManager: create, default path '{}'", defaultPath);

            if (!defaultLocale.empty())
            {
                SetLocale(defaultLocale);
            }

            if (!defaultPath.empty())
            {
                _localeGenerator.add_messages_path(defaultPath);
            }
        }
        //--------------------------------------------------------------------------

        void Manager::SetLocale(const LocaleStr& localeString)
        {
            STRTLR_CORE_LOG_INFO("I18NManager: set locale '{}'", localeString);

            _currentLocale = localeString;
            _library->SetLocale(localeString);

            ImbueLocale();
            NotifyLocaleListeners();
        }
        //--------------------------------------------------------------------------

        const LocaleStr& Manager::GetLocale() const
        {
            return _currentLocale;
        }
        //--------------------------------------------------------------------------

        void Manager::ImbueLocale() const
        {
            std::locale::global(_localeGenerator(_currentLocale));
            std::cout.imbue(std::locale());
        }
        //--------------------------------------------------------------------------

        void Manager::AddMessagesPath(const std::string& path)
        {
            STRTLR_CORE_LOG_INFO("I18NManager: add messages path '{}'", path);

            _localeGenerator.add_messages_path(path);
        }
        //--------------------------------------------------------------------------

        Ptr<LookupDictionary> Manager::AddMessagesDomain(const DomainStr& domain)
        {
            STRTLR_CORE_LOG_INFO("I18NManager: add messages domain '{}'", domain);

            _localeGenerator.add_messages_domain(domain);
            ImbueLocale();

            return _library->AddLookupDictionary(domain);
        }
        //--------------------------------------------------------------------------

        void Manager::RemoveMessagesDomain(const DomainStr& domain)
        {
            _library->RemoveLookupDictionary(domain);
        }
        //--------------------------------------------------------------------------

        Ptr<LookupDictionary> Manager::GetLookupDictionary(const DomainStr& domain) const
        {
            return _library->GetLookupDictionary(domain);
        }
        //--------------------------------------------------------------------------

        void Manager::AddLocaleChangedCallback(const LocaleChangeCallback& callback)
        {
            _localeChangedCallbacks.push_back(callback);
        }
        //--------------------------------------------------------------------------

        void Manager::NotifyLocaleListeners() const
        {
            for (const auto& callback : _localeChangedCallbacks)
            {
                callback();
            }
        }
        //--------------------------------------------------------------------------

        TranslationStr Manager::Translate(const DomainStr& domain, const SourceStr& message)
        {
            const auto translation = Translator::Translate(domain, message);
            _library->Add(domain, message, translation);
            return translation;
        }
        //--------------------------------------------------------------------------

        TranslationStr Manager::Translate(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count)
        {
            return Translator::Translate(domain, messageSingular, messagePlural, count);
        }
        //--------------------------------------------------------------------------

        TranslationStr Manager::TranslateCtx(const DomainStr& domain, const SourceStr& message, const ContextStr& context)
        {
            const auto translation = Translator::TranslateCtx(domain, message, context);
            _library->Add(domain, message, context, translation);
            return translation;
        }
        //--------------------------------------------------------------------------

        TranslationStr Manager::TranslateCtx(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count, const ContextStr& context)
        {
            return Translator::TranslateCtx(domain, messageSingular, messagePlural, count, context);
        }
        //--------------------------------------------------------------------------

        const TranslationStr& Manager::Translation(const DomainStr& domain, const SourceStr& message)
        {
            return _library->Get(domain, message);
        }
        //--------------------------------------------------------------------------

        const TranslationStr& Manager::Translation(const DomainStr& domain, const SourceStr& message, const ContextStr& context)
        {
            return _library->Get(domain, message, context);
        }
        //--------------------------------------------------------------------------

        const TranslationStr& Manager::TranslationOr(const DomainStr& domain, const SourceStr& message, const TranslationStr& defaultString)
        {
            const auto& translation = Translation(domain, message);
            return translation.empty() ? defaultString : translation;
        }
        //--------------------------------------------------------------------------

        const TranslationStr& Manager::TranslationOr(const DomainStr& domain, const SourceStr& message, const ContextStr& context, const TranslationStr& defaultString)
        {
            const auto& translation = Translation(domain, message, context);
            return translation.empty() ? defaultString : translation;
        }
        //--------------------------------------------------------------------------

        const TranslationStr& Manager::TranslationOrSource(const DomainStr& domain, const SourceStr& message)
        {
            const auto& translation = Translation(domain, message);
            return translation.empty() ? message : translation;
        }
        //--------------------------------------------------------------------------

        const TranslationStr& Manager::TranslationOrSource(const DomainStr& domain, const SourceStr& message, const ContextStr& context)
        {
            const auto& translation = Translation(domain, message, context);
            return translation.empty() ? message : translation;
        }
        //--------------------------------------------------------------------------
    }
}