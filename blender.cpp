#include "blender.h"

#include <QDebug>

Blender::Blender() {}

void Blender::blend(QImage &background, const QImage &foreground, float alpha, int mode)
{
    if (background.size() != foreground.size())
    {
        qDebug() << "Obrazki roznej wielkosci";
        std::exit(-1);
    }

    int ymax = background.height();
    int xmax = background.width();

    for(int y = 0; y < ymax; y++)
    {
        for (int x = 0; x < xmax; x++)
        {
            PixelColor pixelB = getPixel(background, x, y);
            PixelColor pixelF = getPixel(foreground, x, y);




        }
    }
}

PixelColor Blender::normal(PixelColor &backgroundPixel, PixelColor &foregroundPixel)
{
    return foregroundPixel;
}

PixelColor Blender::multiply(PixelColor &backgroundPixel, PixelColor &foregroundPixel)
{
    return backgroundPixel * foregroundPixel;
}

PixelColor Blender::screen(PixelColor& backgroundPixel, PixelColor& foregroundPixel)
{
    return backgroundPixel + foregroundPixel - (backgroundPixel * foregroundPixel);
}

PixelColor Blender::overlay(PixelColor& backgroundPixel, PixelColor& foregroundPixel)
{
    PixelColor result;

    result.R = (backgroundPixel.R <= 128)
                   ? multiply(backgroundPixel, foregroundPixel).R
                   : screen(backgroundPixel, foregroundPixel).R;

    result.G = (backgroundPixel.G <= 128)
                   ? multiply(backgroundPixel, foregroundPixel).G
                   : screen(backgroundPixel, foregroundPixel).G;

    result.B = (backgroundPixel.B <= 128)
                   ? multiply(backgroundPixel, foregroundPixel).B
                   : screen(backgroundPixel, foregroundPixel).B;

    return result;

}

PixelColor Blender::darken(PixelColor &backgroundPixel, PixelColor &foregroundPixel)
{
    int BrightnessBackground = (backgroundPixel.R + backgroundPixel.G + backgroundPixel.B) / 3;
    int BrightnessForeground = (foregroundPixel.R + foregroundPixel.G + foregroundPixel.B) / 3;

    return (BrightnessBackground < BrightnessForeground) ? backgroundPixel: foregroundPixel;
}

PixelColor Blender::lighten(PixelColor &backgroundPixel, PixelColor &foregroundPixel)
{
    int BrightnessBackground = (backgroundPixel.R + backgroundPixel.G + backgroundPixel.B) / 3;
    int BrightnessForeground = (foregroundPixel.R + foregroundPixel.G + foregroundPixel.B) / 3;

    return (BrightnessBackground < BrightnessForeground) ? foregroundPixel: backgroundPixel;
}

PixelColor Blender::difference(PixelColor &backgroundPixel, PixelColor &foregroundPixel)
{
    return backgroundPixel - foregroundPixel;
}

PixelColor Blender::additive(PixelColor &backgroundPixel, PixelColor &foregroundPixel)
{
    return backgroundPixel + foregroundPixel;
}

PixelColor Blender::getPixel(const QImage &img, int x, int y)
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
