#include "strtlr_assert.h"
#include "log.h"

namespace Storyteller
{
    Assertion::Assertion(std::string expression, const std::source_location& location, Consequence consequence)
        : _sourceLocation(location)
        , _consequence(consequence)
    {
        Utils::ToSStream(_stream, "Assertion failed! \"", expression, "\"");
    }
    //--------------------------------------------------------------------------

    Assertion::~Assertion()
    {
        if (_consequence == Consequence::Terminate)
        {
            STRTLR_CORE_LOG_CRITICAL("!!! [file {}, function {}, line {}]: {}", _sourceLocation.file_name(), _sourceLocation.function_name(), _sourceLocation.line(), _stream.str());
            std::terminate();
        }
        else
        {
            STRTLR_CORE_LOG_ERROR("!!! [file {}, function {}, line {}]: {}", _sourceLocation.file_name(), _sourceLocation.function_name(), _sourceLocation.line(), _stream.str());
        }
    }
    //--------------------------------------------------------------------------

    Assertion& Assertion::Message(const std::string& message)
    {
        Utils::ToSStream(_stream, "\n\tMessage: ", message);
        return *this;
    }
    //--------------------------------------------------------------------------

    void Assertion::Exception()
    {
        _consequence = Consequence::Exception;
        throw std::exception();
    }
    //--------------------------------------------------------------------------
}