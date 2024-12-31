#include "ekran.h"
#include "blender.h"


Ekran::Ekran(QWidget *parent)
    : QWidget{parent}
{
    m_image1 = QImage("Pictures/Obrazek1.jpg");
    m_image2 = QImage("Pictures/Obrazek2.jpg");
    m_image3 = QImage("Pictures/Obrazek3.jpg");
    m_background = QImage(960, 640, QImage::Format_RGB32);
    m_background.fill(0);

    m_background = m_image1;

   // setFixedSize(1280,720);
    setupUI();
    loadArray();
}

int Ekran::getValue() const
{
    return m_slider->value();
}

void Ekran::paintEvent(QPaintEvent *event)
{
    QPainter p(this);

    p.drawImage(25, 25, m_background);
}

void Ekran::loadArray()
{
    m_layers[0] = { m_image1, 1, "Ciri", Layer::BlendingMode::Normal };
    m_layers[1] = { m_image2, 1, "Wilk", Layer::BlendingMode::Normal };
    m_layers[2] = { m_image3, 1, "Rys", Layer::BlendingMode::Normal };

    for(const auto& pic: m_layers)
    {
        if (pic.image.isNull())
        {
            qDebug() << "Blad wczytania obrazu " << pic.name;
            std::exit(-1);
        }

        m_listWidget->addItem(pic.name);
    }

    if (!m_layers.empty())
    {
        m_listWidget->setCurrentRow(0);
        m_selectedItem = m_listWidget->currentItem();
        m_selectedLayer = m_selectedItem->text();
        m_selectedMode = "Normal";
        blendLayers();
    }
}

void Ekran::setupUI()
{
    m_mainLayout = new QHBoxLayout(this);

    m_leftPanel = new QWidget(this);
    m_leftPanel->setMinimumSize(1000, 700);
    m_mainLayout->addWidget(m_leftPanel);

    m_spacer = new QSpacerItem(20, 640, QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_mainLayout->addItem(m_spacer);

    m_rightPanel = new QWidget(this);
    m_rightLayout = new QVBoxLayout(m_rightPanel);

    m_combobox = new QComboBox(this);
    m_combobox->addItem("Normal");
    m_combobox->addItem("Multiply");
    m_combobox->addItem("Screen");
    m_combobox->addItem("Overlay");
    m_combobox->addItem("Darken");
    m_combobox->addItem("Lighten");
    m_combobox->addItem("Difference");
    m_combobox->addItem("Additive");

    m_rightLayout->addWidget(m_combobox);

    m_alphaLabel = new QLabel("Alpha: 100", this);
    m_rightLayout->addWidget(m_alphaLabel);

    m_slider = new QSlider(Qt::Horizontal, this);
    m_slider->setRange(0, 100);
    m_slider->setValue(100);

    m_rightLayout->addWidget(m_slider);

    connect(m_slider, &QSlider::valueChanged, this, &Ekran::updateLabel);
    connect(m_slider, &QSlider::valueChanged, this, &Ekran::updateSliderValue);
    connect(m_combobox, &QComboBox::currentTextChanged, this, &Ekran::updateMode);


    m_listWidget = new QListWidget(this);
    m_rightLayout->addWidget(m_listWidget);

    connect(m_listWidget, &QListWidget::itemSelectionChanged, this, &Ekran::updateUI);

    m_rightPanel->setLayout(m_rightLayout);
    m_mainLayout->addWidget(m_rightPanel);

    setLayout(m_mainLayout);

}

void Ekran::blendLayers()
{
    Blender blender;

    for (int i = m_layers.size() - 1; i >= 0; i--)
    {
        blender.blend(m_background, m_layers[i].image, m_layers[i].alpha, getModeString(m_layers[i].blendingMode).toStdString());
    }


    update();
}

QString Ekran::getModeString(Layer::BlendingMode mode) const
{
    switch (mode)
    {
        case Layer::BlendingMode::Normal: return "Normal";
        case Layer::BlendingMode::Multiply: return "Multiply";
        case Layer::BlendingMode::Screen: return "Screen";
        case Layer::BlendingMode::Overlay: return "Overlay";
        case Layer::BlendingMode::Darken: return "Darken";
        case Layer::BlendingMode::Lighter: return "Lighten";
        case Layer::BlendingMode::Difference: return "Difference";
        case Layer::BlendingMode::Additive: return "Additive";
        default: return "Normal";
    }
}

Layer::BlendingMode Ekran::stringToEnum(const QString &str) const
{
    if (str == "Normal")        return Layer::BlendingMode::Normal;
    if (str == "Multiply")      return Layer::BlendingMode::Multiply;
    if (str == "Screen")        return Layer::BlendingMode::Screen;
    if (str == "Overlay")       return Layer::BlendingMode::Overlay;
    if (str == "Darken")        return Layer::BlendingMode::Darken;
    if (str == "Lighten")       return Layer::BlendingMode::Lighter;
    if (str == "Difference")    return Layer::BlendingMode::Difference;
    if (str == "Additive")      return Layer::BlendingMode::Additive;

    return Layer::BlendingMode::Normal;
}

void Ekran::updateSliderValue()
{

    m_selectedItem = m_listWidget->currentItem();
    if (!m_selectedItem) return;

    m_selectedLayer = m_selectedItem->text();


    m_sliderValue = getValue() / 100.0f;

    std::string Layername = m_selectedLayer.toStdString();

    for (auto& layer : m_layers)
    {

        if (layer.name == Layername)
        {
            layer.alpha = m_sliderValue;
            break;
        }
    }

    //updateUI();
    blendLayers();
}

void Ekran::updateMode()
{
    m_selectedItem = m_listWidget->currentItem();
    m_selectedLayer = m_selectedItem->text();
    m_selectedMode = m_combobox->currentText();

    std::string Layername = m_selectedLayer.toStdString();

    if (m_selectedItem)
    {
        for (auto& layer: m_layers)
        {
            if (layer.name == Layername)
            {
                layer.blendingMode = stringToEnum(m_selectedMode);
                break;
            }
        }
    }

    blendLayers();
}

void Ekran::updateUI()
{
    m_selectedItem = m_listWidget->currentItem();
    m_selectedLayer = m_selectedItem->text();

    if (m_selectedItem)
    {
        for (auto& layer: m_layers)
        {
            if (layer.name == m_selectedLayer.toStdString())
            {
                m_slider->setValue(layer.alpha * 100);
                m_combobox->setCurrentText(getModeString(layer.blendingMode));

                //qDebug() << layer.name << " " << getModeString(layer.blendingMode);

                break;
            }
        }

    }

    blendLayers();
}

void Ekran::updateLabel(int value)
{
    m_alphaLabel->setText(QString("Alpha: %1").arg(value));
}
