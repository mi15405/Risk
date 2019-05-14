#include "loadplayers.h"
#include "ui_loadplayers.h"
#include "mainwindow.h"
#include <QStringList>
#include <QDebug>

LoadPlayers::LoadPlayers(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoadPlayers)
{
    ui->setupUi(this);
}

LoadPlayers::~LoadPlayers()
{
    delete ui;

    if (mw != nullptr)
        delete mw;
}

void LoadPlayers::on_pushButton_clicked()
{
    /* Imena i boje igraca */
    QVector<QPair<QString, QString>> players = {
        { ui->lineEdit->text(), "blue" },
        { ui->lineEdit_2->text(), "green" },
        { ui->lineEdit_3->text(), "orange" },
        { ui->lineEdit_4->text(), "purple" },
        { ui->lineEdit_5->text(), "red" },
        { ui->lineEdit_6->text(), "cyan" }
    };

    /* Izbacujemo parove koji nemaju ime */
    auto iter = players.begin();
    while (iter != players.end())
    {
        if (iter->first == "")
            iter = players.erase(iter);
        else
            iter++;
    }


    if (players.size() > 1) {
        /* Brisemo uneta imena */
        ui->lineEdit->setText("");
        ui->lineEdit_2->setText("");
        ui->lineEdit_3->setText("");
        ui->lineEdit_4->setText("");
        ui->lineEdit_5->setText("");
        ui->lineEdit_6->setText("");

        /* Sakrivamo prozor */
        hide();

        /* Pravimo glavni prozor */
        mw = new MainWindow(players, this);
        mw->showMaximized();
    }
}

void LoadPlayers::closeEvent(QCloseEvent* event)
{
    QApplication::quit();
}

