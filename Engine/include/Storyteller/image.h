#pragma once

#include <string>

namespace Storyteller
{
    class Image
    {
    public:
        enum Channels
        {
            Grey = 1,
            GreyAlpha,
            RGB,
            RGBAlpha
        };

    public:
        Image(const std::string& filename, Channels desiredChannels, bool flipVertically = false);
        Image(const char* filename, Channels desiredChannels, bool flipVertically = false);
        ~Image();

        int GetWidth() const;
        int GetHeight() const;
        int GetChannels() const;
        unsigned char* GetPixels() const;

    private:
        int _width;
        int _height;
        Channels _channels;
        unsigned char* _pixels;
    };
    //--------------------------------------------------------------------------
}
