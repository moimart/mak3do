#include <mak3do/scenegraph/Color.h>

#ifndef MIN
#define MIN(x, y) (((x) > (y)) ? (y) : (x))
#endif // MIN

#ifndef MAX
#define MAX(x, y) (((x) < (y)) ? (y) : (x))
#endif

using namespace mak3do;
using namespace mak3do::color;

HSL color::rgb2hsl(const RGB& rgb)
{
    double r, g, b, h, s, l;

    r = rgb.x / 255.0f;
    g = rgb.y / 255.0f;
    b = rgb.z / 255.0f;

    double min, max, delta;

    min = MIN(MIN(r, g), b);
    max = MAX(MAX(r, g), b);

    delta = max - min;

    l = (max + min) * 0.5f;
    s = 0;

    if (delta < 0 || delta > 0)
        s = delta / (1.0f - fabs(2 * l - 1.0f));

    h = 0;

    if (delta <= 0)
        h = 0;
    else if (max == r) {
        h = fmodf((g - b) / delta, 6);
    } else if (max == g) {
        h = ((b - r) / delta) + 2;
    } else if (max == b) {
        h = ((r - g) / delta) + 4;
    }

    h *= 60.0;

    if (h < 0.0)
        h += 360.0;

    return HSL(h, s, l);
}

RGB color::hsl2rgb(const HSL& hsl)
{
    RGB out(0, 0, 0);

    float h = fmodf(hsl.x, 360.0f);
    float s = hsl.y;
    float l = hsl.z;

    float c = (1.0f - fabs(2.0f * l - 1.0f)) * s;
    float x = c * (1.0f - fabs(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = l - c / 2.0f;

    if (h >= 0 && h < 60) {
        out.x = c;
        out.y = x;
    } else if (h >= 60 && h < 120) {
        out.x = x;
        out.y = c;
    } else if (h >= 120 && h < 180) {
        out.y = c;
        out.z = x;
    } else if (h >= 180 && h < 240) {
        out.y = x;
        out.z = c;
    } else if (h >= 240 && h < 300) {
        out.x = x;
        out.z = c;
    } else if (h >= 300 && h < 360) {
        out.x = c;
        out.z = x;
    }

    out.x += m;
    out.y += m;
    out.z += m;
    out.x *= 255.0f;
    out.y *= 255.0f;
    out.z *= 255.0f;

    return out;
}

RGB color::rgb01(const RGB& rgb)
{
    RGB rgb01;
    rgb01.r = rgb.r / 255.0f;
    rgb01.g = rgb.g / 255.0f;
    rgb01.b = rgb.b / 255.0f;
    return rgb01;
}

RGBA color::rgba01(const RGBA& rgba)
{
    RGBA rgb01;
    rgb01.r = rgba.r / 255.0f;
    rgb01.g = rgba.g / 255.0f;
    rgb01.b = rgba.b / 255.0f;
    rgb01.a = rgba.a / 255.0f;
    return rgb01;
}

RGB color::hex2rgb(const HexColor& color)
{
    RGB rgb;

    rgb.x = (color & 0xff0000) >> 16;
    rgb.y = (color & 0x00ff00) >> 8;
    rgb.z = (color & 0x0000ff);

    return rgb;
}

RGB color::hex2rgb(const std::string& color)
{
    RGB rgb(0, 0, 0);

    if (color.length() < 6 || color.length() > 7)
        return rgb;

    int startIndex = 0;

    if (color[0] == '#' && color.length() != 7) {
        return rgb;
    } else if (color[0] == '#' && color.length() == 7) {
        startIndex = 1;
    } else {
        startIndex = 0;
    }

    for (int i = startIndex; i < 6; i++) {
    }

    return rgb;
}

const RGB RGB::WHITE = RGB(255, 255, 255);
const RGB RGB::BLACK = RGB(0, 0, 0);
const RGB RGB::RED = RGB(255, 0, 0);
const RGB RGB::GREEN = color::RGB(0, 255, 0);
const RGB RGB::BLUE = color::RGB(0, 0, 255);
const RGB RGB::YELLOW = color::RGB(255, 255, 0);
const RGB RGB::ORANGE = color::RGB(255, 127, 0);

const RGB RGB::WHITE_01 = color::RGB(1);
const RGB RGB::BLACK_01 = color::RGB(0);
const RGB RGB::RED_01 = color::RGB(1, 0, 0);
const RGB RGB::GREEN_01 = color::RGB(0, 1, 0);
const RGB RGB::BLUE_01 = color::RGB(0, 0, 1);
const RGB RGB::YELLOW_01 = color::RGB(1, 1, 0);
const RGB RGB::ORANGE_01 = color::RGB(1, .5f, 0);

const RGBA RGBA::WHITE = RGBA(255, 255, 255);
const RGBA RGBA::BLACK = RGBA(0, 0, 0);
const RGBA RGBA::RED = RGBA(255, 0, 0);
const RGBA RGBA::GREEN = color::RGBA(0, 255, 0);
const RGBA RGBA::BLUE = color::RGBA(0, 0, 255);
const RGBA RGBA::YELLOW = color::RGBA(255, 255, 0);
const RGBA RGBA::ORANGE = color::RGBA(255, 127, 0);

const RGBA RGBA::WHITE_01 = color::RGBA(1);
const RGBA RGBA::BLACK_01 = color::RGBA(0);
const RGBA RGBA::RED_01 = color::RGBA(1, 0, 0, 1);
const RGBA RGBA::GREEN_01 = color::RGBA(0, 1, 0, 1);
const RGBA RGBA::BLUE_01 = color::RGBA(0, 0, 1, 1);
const RGBA RGBA::YELLOW_01 = color::RGBA(1, 1, 0, 1.0f);
const RGBA RGBA::ORANGE_01 = color::RGBA(1, .5f, 0, 1.0);
