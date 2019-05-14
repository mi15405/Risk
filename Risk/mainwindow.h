#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>
#include "gamecontroller.h"
#include "deployslider.h"

namespace Ui {
class MainWindow;
}

enum class TurnPhase
{
    Start, Deploy, Build, Attack
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QVector<QPair<QString, QString>> players, QWidget *parent = 0);
    ~MainWindow();

    void update_ui();
    void update_player_name();
    void update_cards();
    void update_bonus_tanks();
    void update_resourses();
    void update_building();
    void update_battle();
    void use_card_combination(CardCombination combination);
    void deploy_tank(std::shared_ptr<Territory> territory);
    void attack_phase(std::shared_ptr<Territory> territory);
    void build_phase(std::shared_ptr<Territory> territory);

    TurnPhase& turn_phase();

signals:
    void show_deploy_slider(QPoint, std::shared_ptr<Territory>);
    void keyCaught(QKeyEvent* event);

protected:
    virtual void keyPressEvent(QKeyEvent* event);

public slots:
    void territory_id_received(int id, QMouseEvent* event);
    void tanks_are_deployed();
    void closeEvent(QCloseEvent* event) override;

private slots:

    void on_end_turn_button_clicked();

    void on_three_sold_button_clicked();

    void on_three_hors_button_clicked();

    void on_three_ship_button_clicked();

    void on_one_of_all_button_clicked();

    void on_bonus_tenks_deploy_button_clicked();

    void on_select_territory_button_clicked();

    void on_battle_button_clicked();

    void on_sword_upgrade_button_clicked();

    void on_shield_upgrade_button_clicked();

    void on_luck_upgrade_button_clicked();

private:
    void show_exit_message();

    Ui::MainWindow *ui;
    TurnPhase m_turn_phase;
    DeploySlider* m_deploy_slider;

    GameController m_gc;

    std::shared_ptr<Territory> m_build_territory;
    std::shared_ptr<Territory> m_attacker_territory;
    std::shared_ptr<Territory> m_defender_territory;
};

#endif // MAINWINDOW_H
