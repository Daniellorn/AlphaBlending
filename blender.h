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

    PixelColor operator-(int value) const
    {
        return PixelColor{
            std::max(0, (value - R)),
            std::max(0, (value - G)),
            std::max(0, (value - B))
        };
    }
};


class Blender
{
public:
    Blender();

    void blend(QImage& background, const QImage& foreground, float alpha, const std::string& mode);

private:

    using functions = std::function<PixelColor(const PixelColor&, const PixelColor&)>;

    PixelColor normal(const PixelColor& backgroundPixel, const PixelColor& foregroundPixel);
    PixelColor multiply(const PixelColor& backgroundPixel,const PixelColor& foregroundPixel);
    PixelColor screen(const PixelColor& backgroundPixel, const PixelColor& foregroundPixel);
    PixelColor overlay(const PixelColor& backgroundPixel, const PixelColor& foregroundPixel);
    PixelColor darken(const PixelColor& backgroundPixel, const PixelColor& foregroundPixel);
    PixelColor lighten(const PixelColor& backgroundPixel, const PixelColor& foregroundPixel);
    PixelColor difference(const PixelColor& backgroundPixel, const PixelColor& foregroundPixel);
    PixelColor additive(const PixelColor& backgroundPixel, const PixelColor& foregroundPixel);


    PixelColor getPixel(const QImage& img, int x, int y) const;
    void setPixel(QImage& img, int x, int y, const PixelColor& color);

private:

    std::unordered_map<std::string, functions> m_mode;
};

#endif // BLENDER_H
