#ifndef RGB_UTILS_H_
#define RGB_UTILS_H_

#include <Arduino.h>
#include <math_utils.h>


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

struct RGBOutput
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;

    void set(uint16_t r, uint16_t g, uint16_t b, uint16_t level = 1, uint16_t maxLevel = 1)
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
    RGBOutput(uint16_t r, uint16_t g, uint16_t b, uint16_t level = 1, uint16_t maxLevel = 1)
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

    // void dim(uint16_t level, uint16_t maxLevel)
    // {
    //     set(red, green, blue, level, maxLevel);
    // }

    void maximize(uint16_t maxValue = 255)
    {

        uint16_t maxComponent = brightness();
        // uint16_t scale = maxValue / maxComponent;

        if (maxComponent > 0)
        {
            red = ((red * maxValue) / maxComponent) % (maxValue + 1);
            green = ((green * maxValue) / maxComponent) % (maxValue + 1);
            blue = ((blue * maxValue) / maxComponent) % (maxValue + 1);
        }

        // red *= scale;
        // green *= scale;
        // blue *= scale;
    }

    uint16_t brightness()
    {
        return max(red, max(green, blue));
    }

    

    void setTemperature(uint16_t temperature, uint16_t brightness = 255, uint16_t maxBrightness = 255)
    {

        // uint16_t b = (((uint16_t)100) * brightness) / maxBrightness;
        // Kelvin2RGB k = Kelvin2RGB(temperature, b);
        // red = k.Red;
        // green = k.Green;
        // blue = k.Blue;
        // uint16_t b = (((uint16_t)100) * brightness) / maxBrightness;
        RGBOutput o = FROM_TEMPERATURE(temperature, brightness, maxBrightness);
        set(o);
    }

    static RGBOutput FROM_TEMPERATURE(uint16_t temperature, uint16_t brightness = 255 , uint16_t maxBrightness = 255)
    {

        float temp = (constrain((float)temperature, 0.0, 65500.0)) / 100.0;
        uint16_t br = constrain((((uint16_t)100) * brightness) / maxBrightness, 0, 100);
        RGBOutput o;

        // red
        float red = 255.0;
        if (temp > 66.0)
        {
            red = 329.698727466 * pow(temp - 60.0, -0.1332047592);
        }
        // green
        float green;
        if (temp <= 66.0)
        {
            green = (99.4708025861 * log(temp)) - 161.1195681661;
        }
        else
        {
            green = 288.1221695283 * pow(temp - 60.0, -0.0755148492);
        }
        // blue
        float blue = 255.0;
        if (temp < 65.0)
        {
            if (temp <= 19.0)
            {
                blue = 0;
            }
            else
            {
                blue = (138.5177312231 * log(temp - 10.0)) - 305.0447927307;
            }
        }

        o.set(
            map(br, 0, 100, 0, (uint16_t)constrain(red, 0, 255)),
            map(br, 0, 100, 0, (uint16_t)constrain(green, 0, 255)),
            map(br, 0, 100, 0, (uint16_t)constrain(blue, 0, 255)));
        return o;
    }

    // static RGBOutput FROM_TEMPERATURE(uint16_t temperature, uint16_t brightness, uint16_t maxBrightness = 100)
    // {
    //     RGBOutput o;
    //     o.setTemperature(temperature, brightness, maxBrightness);
    //     return o;
    // }

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