#ifndef BLENDER_H
#define BLENDER_H

#include <QImage>

class Blender
{
public:
    Blender();

    void blend(QImage& background, const QImage& foreground, float alpha, int mode);

private:


    // Tu funkcje blendujace z moze jakas mapa<int, std::function>

};

#endif // BLENDER_H
