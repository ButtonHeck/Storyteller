#include "localization_manager.h"
#include "localization_library.h"
#include "log.h"

#include <iostream>

namespace Storyteller
{
    LocalizationManager::LocalizationManager(const std::string& defaultLocale, const std::string& defaultPath)
        : _localeGenerator()
        , _library(CreatePtr<LocalizationLibrary>(""))
        , _currentLocaleString("")
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: create, default path '{}'", defaultPath);

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

    void LocalizationManager::SetLocale(const std::string& localeString)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: set locale '{}'", localeString);

        _currentLocaleString = localeString;
        _library->SetLocale(localeString);

        ImbueLocale();
        NotifyLocaleListeners();
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::GetLocale() const
    {
        return _currentLocaleString;
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::ImbueLocale() const
    {
        std::locale::global(_localeGenerator(_currentLocaleString));
        std::cout.imbue(std::locale());
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesPath(const std::string& path)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: add messages path '{}'", path);

        _localeGenerator.add_messages_path(path);
    }
    //--------------------------------------------------------------------------

    Ptr<LocalizationLookupDictionary> LocalizationManager::AddMessagesDomain(const std::string& domain)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: add messages domain '{}'", domain);

        _localeGenerator.add_messages_domain(domain);
        ImbueLocale();
        
        return _library->AddLookupDictionary(domain);
    }
    //--------------------------------------------------------------------------

    Ptr<LocalizationLookupDictionary> LocalizationManager::GetLookupDictionary(const std::string& domain) const
    {
        return _library->GetLookupDictionary(domain);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddLocaleChangedCallback(const LocaleChangeCallback& callback)
    {
        _localeChangedCallbacks.push_back(callback);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::NotifyLocaleListeners() const
    {
        for (const auto& callback : _localeChangedCallbacks)
        {
            callback();
        }
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::Translate(const std::string& domain, const std::string& message)
    {
        const auto translation = LocalizationTranslator::Translate(domain, message);
        _library->Add(domain, message, translation);
        return translation;
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::Translate(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count)
    {
        return LocalizationTranslator::Translate(domain, messageSingular, messagePlural, count);
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::TranslateCtx(const std::string& domain, const std::string& message, const std::string& context)
    {
        const auto translation = LocalizationTranslator::TranslateCtx(domain, message, context);
        _library->Add(domain, message, context, translation);
        return translation;
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::TranslateCtx(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count, const std::string& context)
    {
        return LocalizationTranslator::TranslateCtx(domain, messageSingular, messagePlural, count, context);
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::Translation(const std::string& domain, const std::string& message)
    {
        return _library->Get(domain, message);
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::Translation(const std::string& domain, const std::string& message, const std::string& context)
    {
        return _library->Get(domain, message, context);
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::TranslationOr(const std::string& domain, const std::string& message, const std::string& defaultString)
    {
        const auto& translation = Translation(domain, message);
        return translation.empty() ? defaultString : translation;
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::TranslationOr(const std::string& domain, const std::string& message, const std::string& context, const std::string& defaultString)
    {
        const auto& translation = Translation(domain, message, context);
        return translation.empty() ? defaultString : translation;
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::TranslationOrSource(const std::string& domain, const std::string& message)
    {
        const auto& translation = Translation(domain, message);
        return translation.empty() ? message : translation;
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::TranslationOrSource(const std::string& domain, const std::string& message, const std::string& context)
    {
        const auto& translation = Translation(domain, message, context);
        return translation.empty() ? message : translation;
    }
    //--------------------------------------------------------------------------
}