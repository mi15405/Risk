#ifndef WIDGETTERRITORY_H
#define WIDGETTERRITORY_H

#include <memory>
#include <QWidget>
#include <QLabel>
#include <QLCDNumber>
#include "territory.h"

class WidgetTerritory : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetTerritory(
            std::shared_ptr<Territory> territory,
            int pos_x,
            int pos_y,
            QWidget *parent = nullptr);

    void set_position(int x, int y);
    void reposition(
            int parent_width,
            int parent_height,
            int image_width,
            int image_height);

public slots:
    void set_tank_number(unsigned int);
    void set_owner_color(void);

private:
    void set_style();
    QLabel* create_image(const QString& url, QWidget* parent);

    std::shared_ptr<Territory> m_territory;
    int m_pos_x;
    int m_pos_y;

    QLabel m_name_label;
    QLCDNumber m_tank_number;
};

#endif // WIDGETTERRITORY_H
