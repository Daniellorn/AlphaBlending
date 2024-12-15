#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QPainter>

struct Layer
{
    QImage image;
    float alpha;
    QString name;

    enum class BlendingMode {
        Normal, Multiply, Screen, Overlay, Darken,
        Lighter, Difference, Additive, Subtractive
    } blendingMode;
};

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);


protected:
    void paintEvent(QPaintEvent* event) override;


private:
    QImage m_image1;
    QImage m_image2;
    QImage m_image3;
    QImage m_background;


    std::array<Layer, 3> m_layers;

private:

    void LoadArray();

signals:
};

#endif // EKRAN_H
