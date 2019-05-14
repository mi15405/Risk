#ifndef DEPLOYSLIDER_H
#define DEPLOYSLIDER_H

#include <memory>
#include <QWidget>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>
#include <QMessageBox>
#include "territory.h"

class DeploySlider : public QMessageBox
{
    Q_OBJECT
public:
    explicit DeploySlider(QWidget *parent = nullptr);

    void set_max_value(int max);

signals:
    void tanks_deployed();

public slots:
    void show_at(QPoint, std::shared_ptr<Territory>);
    void on_deploy_clicked();
    void on_exit_clicked();

private:
    QSlider* m_slider;
    QSpinBox* m_spinbox;
    QPushButton* m_deploy;
    QPushButton* m_exit;

    std::shared_ptr<Territory> m_territory;
};

#endif // DEPLOYSLIDER_H
