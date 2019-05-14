#ifndef WIDGETMAP_H
#define WIDGETMAP_H

#include <memory>
#include <vector>
#include <map>
#include <QWidget>
#include <QMouseEvent>
#include "gamecontroller.h"
#include "widgetterritory.h"

class WidgetMap : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetMap(QWidget* parent = nullptr);

    void init_territory_widgets(GameController& gc);

signals:
    void territory_id_selected(int id, QMouseEvent* event);

public slots:


protected:
    void mousePressEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void init_color_to_id();

    std::map<unsigned, int> m_color_to_id;
    QImage m_colored_map;
    //std::vector<std::shared_ptr<WidgetTerritory>> m_territory_widgets;
    QList<WidgetTerritory*> m_territory_widgets;
};

#endif // WIDGETMAP_H
