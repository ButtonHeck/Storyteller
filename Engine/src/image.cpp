#include "image.h"
#include "log.h"

#include <stb_image.h>

namespace Storyteller
{
    Image::Image(const std::string& filename, Channels desiredChannels, bool flipVertically)
        : Image(filename.c_str(), desiredChannels, flipVertically)
    {}
    //--------------------------------------------------------------------------

    Image::Image(const char* filename, Channels desiredChannels, bool flipVertically)
        : _width(0)
        , _height(0)
        , _channels(desiredChannels)
        , _pixels(nullptr)
    {
        stbi_set_flip_vertically_on_load(flipVertically);

        auto actualChannels = 0;
        _pixels = stbi_load(filename, &_width, &_height, &actualChannels, _channels);

        if (actualChannels != _channels)
        {
            STRTLR_CORE_LOG_WARN("Image: \"{}\" channels mismatch (desired: {}, actual: {})", filename, _channels, actualChannels);
        }

        if (!_pixels)
        {
            STRTLR_CORE_LOG_ERROR("Image: \"{}\" loading error", filename);
        }
    }
    //--------------------------------------------------------------------------

    Image::~Image()
    {
        if (_pixels)
        {
            stbi_image_free(_pixels);
        }
    }
    //--------------------------------------------------------------------------

    int Image::GetWidth() const
    {
        return _width;
    }
    //--------------------------------------------------------------------------

    int Image::GetHeight() const
    {
        return _height;
    }
    //--------------------------------------------------------------------------

    int Image::GetChannels() const
    {
        return _channels;
    }
    //--------------------------------------------------------------------------

    unsigned char* Image::GetPixels() const
    {
        return _pixels;
    }
    //--------------------------------------------------------------------------
}
