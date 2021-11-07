#ifndef RGB_UTILS_H_
#define RGB_UTILS_H_

#include <Arduino.h>
#include <math_utils.h>
#include <arduino_utils.h>

#define RED 255, 0, 0
#define ORANGE 255, 128, 0
#define YELLOW 255, 255, 0
#define YELLOW_GREEN 128, 255, 0
#define GREEN 0, 255, 0
#define BLUE_GREEN 0, 255, 128
#define CYAN 0, 255, 255
#define AZURE 0, 128, 255
#define BLUE 0, 0, 255
#define VIOLET 128, 0, 255
#define MAGENTA 255, 0, 255
#define WHITE 255, 255, 255
struct ColorTone

{
    static const uint16_t red = 0;
    static const uint16_t orange = 30;
    static const uint16_t yellow = 60;
    static const uint16_t yellow_green = 90;
    static const uint16_t green = 120;
    static const uint16_t green_blue = 150;
    static const uint16_t cyan = 180;
    static const uint16_t azure = 210;
    static const uint16_t blue = 240;
    static const uint16_t violet = 270;
    static const uint16_t magenta = 300;
    static const uint16_t pink = 330;
    static const uint16_t white = 0xFFFF;
    static const uint16_t low_limit = 1000;
    static const uint16_t match = 1200;
    static const uint16_t candle = 1800;
    static const uint16_t sunset = 2000;
    static const uint16_t warm = 2400;
    static const uint16_t tungsten60 = 2750;
    static const uint16_t tungsten120 = 3000;
    static const uint16_t neutral = 3500;
    static const uint16_t sun_rised = 3501;
    static const uint16_t fluorescent = 4000;
    static const uint16_t flash = 5000;
    static const uint16_t daylight = 5600;
    static const uint16_t cloudy = 6000;
    static const uint16_t cold = 6500;
    static const uint16_t shade = 7000;
    static const uint16_t blue_sky = 10000;
};

/** Based on Kelvin2RGB library code **/
void temperature_to_rgb(uint16_t temperature, uint8_t brightness, uint8_t &red, uint8_t &green, uint8_t &blue)
{
    float temp = (constrain((float)temperature, 0.0, 65500.0)) / 100.0;

    // red & green
    float r = 255.0;
    float g;
    if (temp > 66.0)
    {
        r = 329.698727466 * pow(temp - 60.0, -0.1332047592);
        g = 288.1221695283 * pow(temp - 60.0, -0.0755148492);
    }
    else
    {
        g = (99.4708025861 * log(temp)) - 161.1195681661;
    }

    // blue
    float b = 255.0;
    if (temp < 65.0)
    {
        if (temp <= 19.0)
        {
            b = 0;
        }
        else
        {
            b = (138.5177312231 * log(temp - 10.0)) - 305.0447927307;
        }
    }

    red = map(brightness, 0, 255, 0, (uint8_t)constrain(r, 0, 255));
    green = map(brightness, 0, 255, 0, (uint8_t)constrain(g, 0, 255));
    blue = map(brightness, 0, 255, 0, (uint8_t)constrain(b, 0, 255));
}

/** From Wikipedia **/
void hsv_to_rgb(uint16_t hue, uint8_t saturation, uint8_t value, uint8_t &red, uint8_t &green, uint8_t &blue)
{
    float s = ((float)saturation) / 255.0;
    float v = ((float)value) / 255.0;
    float c = v * s;
    float h = ((float)(hue % 360)) / 60.0;
    float x = c * (1 - abs(floatMod(h, 2) - 1));
    float m = v - c;

    float r, g, b;

    if (h < 1.0)
    {
        r = c;
        g = x;
        b = 0;
    }
    else if (h < 2.0)
    {
        r = x;
        g = c;
        b = 0;
    }
    else if (h < 3.0)
    {
        r = 0;
        g = c;
        b = x;
    }
    else if (h < 4.0)
    {
        r = 0;
        g = x;
        b = c;
    }
    else if (h < 5.0)
    {
        r = x;
        g = 0;
        b = c;
    }
    else if (h < 6.0)
    {
        r = c;
        g = 0;
        b = x;
    }

    r += m;
    g += m;
    b += m;

    red = (uint8_t)(255.0 * r);
    green = (uint8_t)(255.0 * g);
    blue = (uint8_t)(255.0 * b);
}

/** From Wikipedia **/
void rgb_to_hsv(uint8_t red, uint8_t green, uint8_t blue, uint16_t &hue, uint8_t &saturation, uint8_t &value)
{

    uint8_t maxRGB = max(red, max(green, blue));
    uint8_t minRGB = min(red, min(green, blue));

    hue = 0;
    saturation = 0;
    value = maxRGB;

    if (maxRGB != minRGB)
    {
        float r = ((float)red) / 255.0;
        float g = ((float)green) / 255.0;
        float b = ((float)blue) / 255.0;

        float mx = ((float)maxRGB / 255.0);
        float mn = ((float)minRGB / 255.0);
        float diff = mx - mn;
        // float h = 60.0 / inc;
        float h = 60.0 / (diff);

        if (maxRGB == red)
        {

            h *= (g - b);
            if (green < blue)
            {
                h += 360.0;
            }
        }
        else if (maxRGB == green)
        {
            h *= (b - r);
            h += 120;
        }
        else if (maxRGB == blue)
        {
            h *= (r - g);
            h += 240;
        }

        saturation = (uint8_t)(255 * (1 - mn / mx));
        hue = h;
        hue %= 360;
    }
}

struct HSVOutput
{
    uint16_t hue;
    uint8_t saturation;
    uint8_t value;
    HSVOutput()
    {
    }
    HSVOutput(uint16_t h, uint8_t s, uint8_t v)
    {
        hue = h;
        saturation = s;
        value = v;
    }

    void debugConversion(uint8_t red, uint8_t green, uint8_t blue)
    {

        rgb_to_hsv(red, green, blue, hue, saturation, value);

        char s[12];
        snprintf_P(s, 12, PSTR("%03d,%03d,%03d"), red, green, blue);
        debugValue("Origin RGB", s);
        snprintf_P(s, 12, PSTR("%03d,%03d,%03d"), hue, saturation, value);
        debugValue("Converted HSV", s);
    }
};
class RGBOutput
{
public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    void set(uint8_t r, uint8_t g, uint8_t b, uint8_t level = 255, uint8_t maxLevel = 255)
    {

        red = ((r * level) / maxLevel);
        green = ((g * level) / maxLevel);
        blue = ((b * level) / maxLevel);
    }

    void set(RGBOutput other)
    {
        set(other.red, other.green, other.blue);
    }
    RGBOutput()
    {
        red = green = blue = 0;
    }
    RGBOutput(uint8_t r, uint8_t g, uint8_t b, uint8_t level = 255, uint8_t maxLevel = 255)
    {
        set(r, g, b, level, maxLevel);
    }

    bool equals(RGBOutput other)
    {

        return ((other.red == red) && (other.green == green) && (other.blue == blue)) ? true : false;
    }

    bool operator==(RGBOutput other)
    {
        return equals(other);
    }
    bool operator!=(RGBOutput other)
    {
        return !equals(other);
    }

    void operator=(RGBOutput other)
    {
        set(other);
    }

    void maximize(uint8_t maxValue = 255)
    {

        uint8_t maxComponent = brightness();
        // uint16_t scale = maxValue / maxComponent;

        if (maxComponent > 0)
        {
            red = ((red * maxValue) / maxComponent) % (maxValue + 1);
            green = ((green * maxValue) / maxComponent) % (maxValue + 1);
            blue = ((blue * maxValue) / maxComponent) % (maxValue + 1);
        }
    }

    uint8_t brightness()
    {
        return max(red, max(green, blue));
    }

    unsigned long toInt()
    {
        unsigned long r = 0xff & red;
        unsigned long g = 0xff & green;
        unsigned long b = 0xff & blue;
        return r | (g << 8) | (b << 16);
    }
    HSVOutput toHSV()
    {
        HSVOutput o;
        rgb_to_hsv(red,green,blue,o.hue,o.saturation,o.value);
        return o;
    }

    void setTemperature(uint16_t temperature, uint8_t brightness = 255, uint8_t maxBrightness = 255)
    {
        RGBOutput o = FROM_TEMPERATURE(temperature, brightness, maxBrightness);
        set(o);
    }
    RGBOutput progressTo(unsigned long progress, unsigned long progressStart, unsigned long progressEnd, RGBOutput endV)
    {

        if (progress >= progressEnd)
        {
            set(endV);
            return endV;
        }

        RGBOutput s = RGBOutput(red, green, blue);
        return PROGRESSION(progress, progressStart, progressEnd, s, endV);
    }

    RGBOutput progress100To(unsigned long progress100, RGBOutput endV)
    {
        return progressTo(progress100, 0, 100, endV);
    }

    static RGBOutput PROGRESSION(unsigned long progress, unsigned long progressStart, unsigned long progressEnd, RGBOutput startV, RGBOutput endV)
    {
        RGBOutput out;

        out.red = mapFromStartToEnd(progress, progressStart, progressEnd, (unsigned long)startV.red, (unsigned long)endV.red);
        out.green = mapFromStartToEnd(progress, progressStart, progressEnd, (unsigned long)startV.green, (unsigned long)endV.green);
        out.blue = mapFromStartToEnd(progress, progressStart, progressEnd, (unsigned long)startV.blue, (unsigned long)endV.blue);
        return out;
    }

    static RGBOutput PROGRESSION100(unsigned long progress100, RGBOutput startV, RGBOutput endV)
    {
        return PROGRESSION(progress100, 0, 100, startV, endV);
    }
    /** Based on Kelvin2RGB library code **/
    static RGBOutput FROM_TEMPERATURE(uint16_t temperature, uint8_t brightness = 255, uint8_t maxBrightness = 255)
    {
        RGBOutput o;
        temperature_to_rgb(temperature, brightness, (uint8_t &)o.red, (uint8_t &)o.green, (uint8_t &)o.blue);
        return o;
    }

    static RGBOutput FROM_HSV(uint16_t hue, uint8_t saturation, uint8_t value)
    {
        RGBOutput o;
        hsv_to_rgb(hue, saturation, value, (uint8_t &)o.red, (uint8_t &)o.green, (uint8_t &)o.blue);
        return o;
    }
};

class RGBTransition : public TimedInterpolationBase<RGBOutput>
{

public:
    RGBOutput value(RGBOutput &startV, RGBOutput endV, unsigned long transitionTime, bool autoChangeStartV = true)
    {

        RGBOutput out;
        out.set(startV);
        unsigned long t = millis();
        this->startCheck(startV, endV);

        if (this->canCalculate(startV, endV))
        {

            unsigned long endT = startTime + transitionTime;
            out = RGBOutput::PROGRESSION(t, startTime, endT, startV, endV);
            // out.red = (uint16_t)mapFromStartToEnd(t, startTime, endT, (unsigned long)startV.red, (unsigned long)endV.red);
            // out.green = (uint16_t)mapFromStartToEnd(t, startTime, endT, (unsigned long)startV.green, (unsigned long)endV.green);
            // out.blue = (uint16_t)mapFromStartToEnd(t, startTime, endT, (unsigned long)startV.blue, (unsigned long)endV.blue);
        }

        this->endCheck(out, startV, endV, autoChangeStartV);

        return out;
    }
};

#endif