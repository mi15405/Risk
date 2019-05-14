#include "widgetterritory.h"
#include <QVBoxLayout>

WidgetTerritory::WidgetTerritory(
        std::shared_ptr<Territory> territory,
        int pos_x,
        int pos_y,
        QWidget *parent)
    : QWidget(parent),
      m_territory(territory),
      m_pos_x(pos_x),
      m_pos_y(pos_y)
{
    m_name_label.setText(territory->name());

    m_tank_number.display((int)territory->tank_number());
    m_tank_number.setSegmentStyle(QLCDNumber::Flat);
    m_tank_number.setMinimumWidth(40);
    m_tank_number.setMaximumWidth(40);

    QString resource_url;

    if (m_territory->resource() == Resource::Metal)
        resource_url = ":/resources/img/gold_icon.png";
    else if (m_territory->resource() == Resource::Stone)
        resource_url = ":/resources/img/stone_icon.png";
    else if (m_territory->resource() == Resource::Wood)
        resource_url = ":/resources/img/wood_icon.png";

    QLabel* resource_image = create_image(resource_url, this);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(&m_name_label);

    QHBoxLayout* tanks_and_resource = new QHBoxLayout();
    tanks_and_resource->addWidget(&m_tank_number);
    tanks_and_resource->addWidget(resource_image);

    main_layout->addLayout(tanks_and_resource);

    set_style();

    connect(&(*territory), SIGNAL(tank_number_changed(uint)), this, SLOT(set_tank_number(uint)));
    connect(&(*territory), SIGNAL(owner_changed()), this, SLOT(set_owner_color()));
}

QLabel* WidgetTerritory::create_image(const QString& url, QWidget* parent)
{
    QLabel* image = new QLabel(parent);
    image->setMinimumHeight(20);
    image->setMaximumHeight(20);

    image->setMinimumWidth(20);
    image->setMaximumWidth(20);

    image->setStyleSheet("border-image:url(" + url + ");");

    return image;
}

void WidgetTerritory::set_style()
{
    setStyleSheet("border-image: none;"
                  "background-color: " + m_territory->owner()->color() + ";"
                  "color: black;"
                  "font-weight: bold;"
                  "border: 1px solid black;");
}

void WidgetTerritory::set_position(int x, int y)
{
    move(x, y);
}

void WidgetTerritory::reposition(
        int parent_width,
        int parent_height,
        int image_width,
        int image_height)
{
    float relative_x = (float) m_pos_x / image_width;
    float relative_y = (float) m_pos_y / image_height;

    move(parent_width * relative_x, parent_height * relative_y);
}

void WidgetTerritory::set_tank_number(unsigned int tank_number)
{
    m_tank_number.display((int) tank_number);
}

void WidgetTerritory::set_owner_color(void)
{
    set_style();
    repaint();
}


