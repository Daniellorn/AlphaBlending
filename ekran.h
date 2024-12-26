#ifndef EKRAN_H
#define EKRAN_H

#include <QWidget>
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QSpacerItem>
#include <QSizePolicy>

struct Layer
{
    QImage image;
    float alpha;
    QString name;

    enum class BlendingMode {
        Normal, Multiply, Screen, Overlay,
        Darken, Lighter, Difference, Additive
    } blendingMode;
};

class Ekran : public QWidget
{
    Q_OBJECT
public:
    explicit Ekran(QWidget *parent = nullptr);
    ~Ekran();

    int getValue() const;

protected:
    void paintEvent(QPaintEvent* event) override;


private:
    QImage m_image1;
    QImage m_image2;
    QImage m_image3;
    QImage m_background;

    float m_sliderValue;
    QString m_selectedMode;
    QString m_selectedLayer;
    QListWidgetItem* m_selectedItem;

    QHBoxLayout* m_mainLayout;
    QWidget* m_leftPanel;
    QWidget* m_rightPanel;
    QVBoxLayout* m_rightLayout;
    QComboBox* m_combobox;
    QSlider* m_slider;
    QLabel* m_alphaLabel;
    QListWidget* m_listWidget;
    QSpacerItem* m_spacer;


    std::array<Layer, 3> m_layers;

private:

    void loadArray();
    void setupUI();
    void blendLayers();

    QString getModeString(Layer::BlendingMode mode) const;
    Layer::BlendingMode stringToEnum(const QString& str) const;
    void updateSliderValue();
    void updateMode();
    void updateUI();

private slots:
    void updateLabel(int value);

signals:
    void valueChanged(int new_value);
    void currentTextChanged(QString new_string);

};

#endif // EKRAN_H
