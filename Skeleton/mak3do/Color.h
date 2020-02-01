#pragma once

#include "types.h"

namespace mak3do {
namespace color {
    typedef int32_t HexColor;

    class RGB : public Vec3 {
    public:
        RGB(float r, float g, float b)
            : Vec3(r, g, b)
        {
        }
        RGB(float r)
            : Vec3(r, r, r)
        {
        }
        RGB()
            : Vec3(0, 0, 0)
        {
        }
        RGB(const Vec3& p)
            : Vec3(p.r, p.g, p.b)
        {
        }

        static const RGB WHITE;
        static const RGB BLACK;
        static const RGB RED;
        static const RGB GREEN;
        static const RGB BLUE;
        static const RGB YELLOW;
        static const RGB ORANGE;

        static const RGB WHITE_01;
        static const RGB BLACK_01;
        static const RGB RED_01;
        static const RGB GREEN_01;
        static const RGB BLUE_01;
        static const RGB YELLOW_01;
        static const RGB ORANGE_01;
    };

    class RGBA : public Vec4 {
    public:
        RGBA(const RGB& rgb, float a)
            : Vec4(rgb.r, rgb.g, rgb.b, a)
        {
        }
        RGBA(const RGB& rgb)
            : Vec4(rgb.r, rgb.g, rgb.b, 255)
        {
        }
        RGBA(float r, float g, float b, float a)
            : Vec4(r, g, b, a)
        {
        }
        RGBA(float r, float g, float b)
            : Vec4(r, g, b, 255)
        {
        }
        RGBA(float r)
            : Vec4(r, r, r, 255)
        {
        }
        RGBA()
            : Vec4(0, 0, 0, 255)
        {
        }
        RGBA(const Vec3& p)
            : Vec4(p.r, p.g, p.b, 255)
        {
        }

        static const RGBA WHITE;
        static const RGBA BLACK;
        static const RGBA RED;
        static const RGBA GREEN;
        static const RGBA BLUE;
        static const RGBA YELLOW;
        static const RGBA ORANGE;

        static const RGBA WHITE_01;
        static const RGBA BLACK_01;
        static const RGBA RED_01;
        static const RGBA GREEN_01;
        static const RGBA BLUE_01;
        static const RGBA YELLOW_01;
        static const RGBA ORANGE_01;
    };

    class HSL : public Vec3 {
    public:
        HSL(float h, float s, float l)
            : Vec3(h, s, l)
        {
        }
        HSL()
            : Vec3(0, 0, 0)
        {
        }
    };

    RGB hsl2rgb(const HSL& hsv);
    HSL rgb2hsl(const RGB& rgb);
    RGB hex2rgb(const HexColor& color);
    RGB hex2rgb(const std::string& color);
    RGB rgb01(const RGB& rgb);
    RGBA rgba01(const RGBA& rgb);
}
}
