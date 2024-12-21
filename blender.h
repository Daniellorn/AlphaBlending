#ifndef BLENDER_H
#define BLENDER_H

#include <QImage>

struct PixelColor
{
    // tu pewnie uint8 powinien byc
    int R;
    int G;
    int B;

    PixelColor operator*(const PixelColor& other) const
    {
        return PixelColor{
            (R * other.R) >> 8,
            (G * other.G) >> 8,
            (B * other.B) >> 8
        };
    }

    PixelColor operator*(int value) const
    {
        return PixelColor{
            (R * value) / 255,
            (G * value) / 255,
            (B * value) / 255
        };
    }


    PixelColor operator+(const PixelColor& other) const
    {
        return PixelColor{
            std::min(255, (R + other.R)),
            std::min(255, (G + other.G)),
            std::min(255, (B + other.B))
        };
    }

    PixelColor operator-(const PixelColor& other) const
    {
        return PixelColor{
            std::max(0, (R - other.R)),
            std::max(0, (G - other.G)),
            std::max(0, (B - other.B))
        };
    }
};


class Blender
{
public:
    Blender();

    void blend(QImage& background, const QImage& foreground, float alpha, int mode);

private:

    PixelColor normal(PixelColor& backgroundPixel,PixelColor& foregroundPixel);
    PixelColor multiply(PixelColor& backgroundPixel,PixelColor& foregroundPixel);
    PixelColor screen(PixelColor& backgroundPixel,PixelColor& foregroundPixel);
    PixelColor overlay(PixelColor& backgroundPixel,PixelColor& foregroundPixel);
    PixelColor darken(PixelColor& backgroundPixel,PixelColor& foregroundPixel);
    PixelColor lighten(PixelColor& backgroundPixel,PixelColor& foregroundPixel);
    PixelColor difference(PixelColor& backgroundPixel,PixelColor& foregroundPixel);
    PixelColor additive(PixelColor& backgroundPixel,PixelColor& foregroundPixel);


    PixelColor getPixel(const QImage& img, int x, int y);
    void setPixel(QImage& img, int x, int y, const PixelColor& color);
    // Tu funkcje blendujace z moze jakas mapa<int, std::function>

};

#endif // BLENDER_H
