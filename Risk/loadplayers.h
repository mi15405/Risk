#ifndef LOADPLAYERS_H
#define LOADPLAYERS_H

#include <QMainWindow>
#include "mainwindow.h"
#include <QKeyEvent>

namespace Ui {
class LoadPlayers;
}

class LoadPlayers : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoadPlayers(QWidget *parent = nullptr);
    ~LoadPlayers();

    void closeEvent(QCloseEvent* event) override;

private slots:
    void on_pushButton_clicked();

private:
    Ui::LoadPlayers *ui;
    MainWindow *mw;
};

#endif // LOADPLAYERS_H
