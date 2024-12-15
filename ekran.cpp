#include "ekran.h"



Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    m_image1 = QImage("Pictures/Obrazek1.jpg");
    m_image2 = QImage("Pictures/Obrazek2.jpg");
    m_image3 = QImage("Pictures/Obrazek3.jpg");
    m_background = QImage(960, 640, QImage::Format_RGB32);
    m_background.fill(0);

    setFixedSize(1280,720);

    LoadArray();
}

void Ekran::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    p.drawImage(25, 25, m_image3);
}

void Ekran::LoadArray()
{
    m_layers[0] = { m_image1, 1, "Rys", Layer::BlendingMode::Normal };
    m_layers[1] = { m_image2, 1, "Wilk", Layer::BlendingMode::Normal };
    m_layers[2] = { m_image3, 1, "Ciri", Layer::BlendingMode::Normal };

    for(const auto& pic: m_layers)
    {
        if (pic.image.isNull())
        {
            qDebug() << "Blad wczytania obrazu " << pic.name;
            std::exit(-1);
        }
    }
}
