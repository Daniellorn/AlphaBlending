#include "blender.h"

#include <QDebug>

#include <algorithm>

Blender::Blender()
{
    m_mode = {
        {"Normal",     [this](const PixelColor& bg, const PixelColor& fg) {return normal(bg, fg);}     },
        {"Multiply",   [this](const PixelColor& bg, const PixelColor& fg) {return multiply(bg, fg);}   },
        {"Screen",     [this](const PixelColor& bg, const PixelColor& fg) {return screen(bg, fg);}     },
        {"Overlay",    [this](const PixelColor& bg, const PixelColor& fg) {return overlay(bg, fg);}    },
        {"Darken",     [this](const PixelColor& bg, const PixelColor& fg) {return darken(bg, fg);}     },
        {"Lighten",    [this](const PixelColor& bg, const PixelColor& fg) {return lighten(bg, fg);}    },
        {"Difference", [this](const PixelColor& bg, const PixelColor& fg) {return difference(bg, fg);} },
        {"Additive",   [this](const PixelColor& bg, const PixelColor& fg) {return additive(bg, fg);}   }
    };
}

void Blender::blend(QImage &background, const QImage &foreground, float alpha, const std::string& mode)
{
    if (background.size() != foreground.size())
    {
        qDebug() << "Obrazki roznej wielkosci";
        std::exit(-1);
    }

    int ymax = background.height();
    int xmax = background.width();

    auto blendingFunction = m_mode[mode];

    for(int y = 0; y < ymax; y++)
    {
        for (int x = 0; x < xmax; x++)
        {
            PixelColor pixelB = getPixel(background, x, y);
            PixelColor pixelF = getPixel(foreground, x, y);

            PixelColor afterBlend = blendingFunction(pixelB, pixelF);

            PixelColor result
                {
                    .R = int(alpha * afterBlend.R + (1 - alpha) * pixelB.R),
                    .G = int(alpha * afterBlend.G + (1 - alpha) * pixelB.G),
                    .B = int(alpha * afterBlend.B + (1 - alpha) * pixelB.B)

                };

            setPixel(background, x, y, result);

        }
    }
}

PixelColor Blender::normal(const PixelColor &backgroundPixel, const PixelColor &foregroundPixel)
{
    return foregroundPixel;
}

PixelColor Blender::multiply(const PixelColor &backgroundPixel, const PixelColor &foregroundPixel)
{
    return backgroundPixel * foregroundPixel;
}

PixelColor Blender::screen(const PixelColor& backgroundPixel, const PixelColor& foregroundPixel)
{
    return backgroundPixel + foregroundPixel - (backgroundPixel * foregroundPixel);
}

PixelColor Blender::overlay(const PixelColor& backgroundPixel, const PixelColor& foregroundPixel)
{
    PixelColor result;

    result.R = (foregroundPixel.R <= 128)
                   ? multiply(backgroundPixel, foregroundPixel).R
                   : screen(backgroundPixel, foregroundPixel).R;

    result.G = (foregroundPixel.G <= 128)
                   ? multiply(backgroundPixel, foregroundPixel).G
                   : screen(backgroundPixel, foregroundPixel).G;

    result.B = (foregroundPixel.B <= 128)
                   ? multiply(backgroundPixel, foregroundPixel).B
                   : screen(backgroundPixel, foregroundPixel).B;

    return result;

}

PixelColor Blender::darken(const PixelColor &backgroundPixel, const PixelColor &foregroundPixel)
{
    int BrightnessBackground = (backgroundPixel.R + backgroundPixel.G + backgroundPixel.B) / 3;
    int BrightnessForeground = (foregroundPixel.R + foregroundPixel.G + foregroundPixel.B) / 3;

    return (BrightnessBackground <= BrightnessForeground) ? backgroundPixel: foregroundPixel;
}

PixelColor Blender::lighten(const PixelColor &backgroundPixel, const PixelColor &foregroundPixel)
{
    int BrightnessBackground = (backgroundPixel.R + backgroundPixel.G + backgroundPixel.B) / 3;
    int BrightnessForeground = (foregroundPixel.R + foregroundPixel.G + foregroundPixel.B) / 3;

    return (BrightnessBackground <= BrightnessForeground) ? foregroundPixel: backgroundPixel;
}

PixelColor Blender::difference(const PixelColor &backgroundPixel, const PixelColor &foregroundPixel)
{
    PixelColor result;

    result.R = std::abs(backgroundPixel.R - foregroundPixel.R);
    result.G = std::abs(backgroundPixel.G - foregroundPixel.G);
    result.B = std::abs(backgroundPixel.B - foregroundPixel.B);

    return result;
}

PixelColor Blender::additive(const PixelColor &backgroundPixel, const PixelColor &foregroundPixel)
{
    PixelColor result;

    result.R = std::clamp(backgroundPixel.R + foregroundPixel.R, 0, 255);
    result.G = std::clamp(backgroundPixel.G + foregroundPixel.G, 0, 255);
    result.B = std::clamp(backgroundPixel.B + foregroundPixel.B, 0, 255);

    return result;
}


PixelColor Blender::getPixel(const QImage &img, int x, int y) const
{    
    const uchar* line = img.scanLine(y);
    uint8_t blue = line[4*x]; //blue
    uint8_t green = line[4*x + 1]; //green
    uint8_t red = line[4*x + 2]; //red
    //uint8_t alpha = line[4*x + 3]; // alpha


    return PixelColor{red, green, blue};

}

void Blender::setPixel(QImage &img, int x, int y, const PixelColor& color)
{
    int red = color.R;
    int green = color.G;
    int blue = color.B;

    uchar* line = img.scanLine(y);
    line[4*x] = blue; //blue
    line[4*x + 1] = green; //green
    line[4*x + 2] = red; //red
    //line[4*x + 3] = 255; // alpha

}
