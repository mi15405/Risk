#include "deployslider.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QStyleOption>
#include <QPainter>

DeploySlider::DeploySlider(QWidget *parent)
    : QMessageBox(parent),
      m_slider(new QSlider(this)),
      m_spinbox(new QSpinBox(this)),
      m_deploy(new QPushButton("Deploy", this)),
      m_exit(new QPushButton("Exit", this))
{
    /* Brisem postojece elemente */
    QLayoutItem *child;
    while ((child = layout()->takeAt(0)) != 0)
        delete child;

    setWindowTitle("Deploy tanks");

    m_slider->setOrientation(Qt::Horizontal);
    m_slider->setMaximumWidth(100);

    m_spinbox->setReadOnly(true);

    layout()->addWidget(m_slider);
    layout()->addWidget(m_spinbox);
    layout()->addWidget(m_deploy);
    layout()->addWidget(m_exit);

    connect(m_slider, SIGNAL(valueChanged(int)), m_spinbox, SLOT(setValue(int)));
    connect(m_deploy, SIGNAL(clicked()), this, SLOT(on_deploy_clicked()));
    connect(m_exit, SIGNAL(clicked()), this, SLOT(on_exit_clicked()));
}

void DeploySlider::set_max_value(int max)
{
    m_slider->setMaximum(max);
    m_slider->setValue(max);

    m_spinbox->setMaximum(max);
    m_spinbox->setValue(max);
}

void DeploySlider::show_at(QPoint p, std::shared_ptr<Territory> t)
{
    m_territory = t;
    move(p.x(), p.y());
    set_max_value(t->owner()->bonus_tanks());
    show();
}

void DeploySlider::on_deploy_clicked()
{
    int tank_number = m_spinbox->value();

    if (tank_number > 0)
    {
        m_territory->add_tanks(tank_number);
        m_territory->owner()->remove_bonus_tanks(tank_number);
        emit tanks_deployed();
    }

    hide();
}

void DeploySlider::on_exit_clicked()
{
    hide();
}
