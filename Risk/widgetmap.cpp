#include "widgetmap.h"
#include <iostream>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QDir>
#include <QImage>
#include "widgetterritory.h"

WidgetMap::WidgetMap(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    QString filepath = QDir::currentPath() + "/../Risk/img/map-c-resized.png";
    m_colored_map.load(filepath);

    init_color_to_id();
}

void WidgetMap::init_color_to_id()
{
    QString filepath = QDir::currentPath() + "/../Risk/territories.txt";

    auto data = GameController::read_from_file(filepath, ' ');

    for (auto row: data)
    {
        int id = row.at(0).toInt();
        unsigned color = row.at(3).toUInt();

        m_color_to_id.insert(std::pair<unsigned, int>(color, id));
    }
}

void WidgetMap::mousePressEvent(QMouseEvent *event)
{
    /*
    Izdvajam boju piksela na onoj obojenoj mapi na osnovu klika na mapu na kojoj se igra,
    i tako dobijamo info na koju drzavu je kliknuto.
    */
    QPoint click = event->pos();

    float stretch_x = (float) m_colored_map.width() / this->width();
    float stretch_y = (float) m_colored_map.height() / this->height();

    int transformed_x = click.x() * stretch_x;
    int transformed_y = click.y() * stretch_y;

    QRgb pixel_color = m_colored_map.pixel(transformed_x, transformed_y);

    auto color_id = m_color_to_id.find(pixel_color);

    if (color_id != m_color_to_id.end())
        emit territory_id_selected(color_id->second, event);
}

void WidgetMap::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void WidgetMap::resizeEvent(QResizeEvent* event)
{
    for (auto it = m_territory_widgets.begin(); it != m_territory_widgets.end(); ++it)
    {
        (*it)->reposition(width(), height(),
                      m_colored_map.width(), m_colored_map.height());
    }
}

void WidgetMap::init_territory_widgets(GameController& gc)
{
    QString filepath =
            QDir::currentPath() + "/../Risk/territory_widget_positions.txt";

    auto data = GameController::read_from_file(filepath, ' ');

    for (auto row : data)
    {
        auto territory = gc.territory_with_id(row.at(0).toInt());
        int x_pos = row.at(1).toInt();
        int y_pos = row.at(2).toInt();

        auto territory_widget = new WidgetTerritory(territory, x_pos, y_pos, this);

        territory_widget->reposition(
                    width(), height(),
                    m_colored_map.width(), m_colored_map.height());

        m_territory_widgets.append(territory_widget);
     }
}
