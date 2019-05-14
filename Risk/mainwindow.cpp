#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QStringList>
#include <QDebug>
#include <QSpinBox>
#include <QPushButton>

#include <QMessageBox>

MainWindow::MainWindow(QVector<QPair<QString, QString>> players, QWidget *parent)
    : 	QMainWindow(parent),
        ui(new Ui::MainWindow),
        m_turn_phase(TurnPhase::Start),
        m_deploy_slider(new DeploySlider(this))
{
    ui->setupUi(this);

    m_gc.init_players(players);
    m_gc.start_game();

    ui->map->init_territory_widgets(m_gc);

    connect(ui->map, SIGNAL(territory_id_selected(int, QMouseEvent*)),
            this, SLOT(territory_id_received(int, QMouseEvent*)));

    connect(this, SIGNAL(show_deploy_slider(QPoint, std::shared_ptr<Territory>)),
            m_deploy_slider, SLOT(show_at(QPoint, std::shared_ptr<Territory>)));

    connect(m_deploy_slider, SIGNAL(tanks_deployed()),
            this, SLOT(tanks_are_deployed()));

    update_ui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::territory_id_received(int id, QMouseEvent* event)
{
    auto territory = m_gc.territory_with_id(id);

    switch (m_turn_phase)
    {
    case TurnPhase::Start :
        qDebug() << territory->name();
        break;

    case TurnPhase::Deploy :
        if (m_gc.is_friendly(territory))
        {
            if (event->button() == 1)
                deploy_tank(territory);
            else
                emit show_deploy_slider(event->pos(), territory);
        }
        else
            std::cout << "Teritorija je neprijateljska!" << std::endl;
        break;

    case TurnPhase::Build :
        build_phase(territory);
        break;

    case TurnPhase::Attack :
        attack_phase(territory);
        break;
    }
}

void MainWindow::deploy_tank(std::shared_ptr<Territory> territory)
{
    auto player = m_gc.current_player();

    if (player->bonus_tanks() > 0)
    {
        territory->add_tanks(1);
        player->remove_bonus_tanks(1);
        update_bonus_tanks();
    }
    else
        std::cout << "Nedovoljno bonus tenkica!" << std::endl;
}

void MainWindow::attack_phase(std::shared_ptr<Territory> territory)
{
    if (m_gc.is_first_turn())
        return;

    if (m_gc.is_friendly(territory))
    {
        m_attacker_territory = territory;
        ui->attacker_name->setText(territory->name());

        m_defender_territory = nullptr;
        ui->defender_name->setText("");
    }
    else
    {
        if (m_attacker_territory == nullptr)
            std::cout << "Izaberi svoju teritoriju!" << std::endl;
        else if (m_attacker_territory->has_neighbour(territory))
        {
            m_defender_territory = territory;
            ui->defender_name->setText(territory->name());
        }
        else
            std::cout << "Teritorije nisu povezane!" << std::endl;
    }
}

void MainWindow::build_phase(std::shared_ptr<Territory> territory)
{
    m_build_territory = territory;
    update_building();
}

void MainWindow::update_ui()
{
    update_player_name();
    update_resourses();
    update_cards();
    update_bonus_tanks();
    update_building();
    update_battle();
}

void MainWindow::update_player_name()
{
    auto player = m_gc.current_player();
    ui->player_name->setText(player->name());
    ui->player_name->setStyleSheet("background: " + player->color());
}

void MainWindow::update_cards()
{
    auto cards = m_gc.current_player()->cards();

    ui->sold_card_num->setText(QString::number(cards.number_of(Card::Soldier)));
    ui->horse_card_num->setText(QString::number(cards.number_of(Card::Horseman)));
    ui->ship_card_num->setText(QString::number(cards.number_of(Card::Ship)));
}

void MainWindow::update_bonus_tanks()
{
    ui->bonus_tanks_num->setText(QString::number(m_gc.current_player()->bonus_tanks()));
}

void MainWindow::update_resourses()
{
    ui->stone_res_num->setText(QString::number(m_gc.current_player()->get_stone()));
    ui->wood_res_num->setText(QString::number(m_gc.current_player()->get_wood()));
    ui->gold_res_num->setText(QString::number(m_gc.current_player()->get_metal()));
}

void MainWindow::update_building()
{
    QString territory_name = "";
    QString att_level = "";
    QString def_level = "";
    QString luck_level = "";
    QString color = "none";

    if (m_build_territory != nullptr)
    {
        territory_name = m_build_territory->name();
        att_level = QString::number(m_build_territory->buildings().attack_level());
        def_level= QString::number(m_build_territory->buildings().defend_level());
        luck_level = QString::number(m_build_territory->buildings().luck_level());
        color = m_build_territory->owner()->color();
    }

    ui->build_name->setText(territory_name);
    ui->build_name->setStyleSheet("color: black; background-color: " + color);

    ui->sword_level_upgrade->setText(att_level);
    ui->shield_level_upgrade->setText(def_level);
    ui->luck_level_upgrade->setText(luck_level);
}

void MainWindow::update_battle()
{
    QString attacker = "";
    QString defender = "";

    if (m_attacker_territory != nullptr)
    {
        attacker = m_attacker_territory->name();

        if (m_defender_territory != nullptr)
            defender = m_defender_territory->name();
    }

    ui->attacker_name->setText(attacker);
    ui->defender_name->setText(defender);
    ui->AttackerDiceResult->setText("");
    ui->DeffenderDiceResult->setText("");
}

void MainWindow::on_three_sold_button_clicked()
{
    use_card_combination(CardCombination::ThreeSoldiers);
}

void MainWindow::on_three_hors_button_clicked()
{
    use_card_combination(CardCombination::ThreeHorseman);
}

void MainWindow::on_three_ship_button_clicked()
{
    use_card_combination(CardCombination::ThreeShips);
}

void MainWindow::on_one_of_all_button_clicked()
{
    use_card_combination(CardCombination::ThreeDifferent);
}

void MainWindow::use_card_combination(CardCombination combination)
{
    m_gc.current_player()->use_card_combination(combination);
    update_cards();
    update_bonus_tanks();
}

void MainWindow::on_end_turn_button_clicked()
{
    m_gc.end_turn();

    m_attacker_territory = nullptr;
    m_defender_territory = nullptr;
    m_build_territory = nullptr;

    m_turn_phase = TurnPhase::Start;

    update_ui();
}

void MainWindow::on_bonus_tenks_deploy_button_clicked()
{
    m_turn_phase = TurnPhase::Deploy;
}

void MainWindow::on_select_territory_button_clicked()
{
    m_turn_phase = TurnPhase::Build;
}

void MainWindow::on_battle_button_clicked()
{
    if (m_turn_phase != TurnPhase::Attack)
        m_turn_phase = TurnPhase::Attack;
    else if (m_attacker_territory != nullptr && m_defender_territory != nullptr)
    {
        m_gc.attack(m_attacker_territory, m_defender_territory);

        ui->AttackerDiceResult->setText(m_gc.attacking_dice());
        ui->DeffenderDiceResult->setText(m_gc.defending_dice());

        if (m_gc.is_game_over())
        {
            std::cout << "Gotovo!!!" << std::endl;

            /* Igra je gotova */

            QMessageBox msg_box;
            msg_box.setText(m_attacker_territory->owner()->name() + " is the winner!");

            QAbstractButton* exit =
                    msg_box.addButton("EXIT GAME", QMessageBox::NoRole);

            QAbstractButton* again =
                    msg_box.addButton("PLAY AGAIN", QMessageBox::YesRole);

            QAbstractButton* new_game =
                    msg_box.addButton("START NEW GAME", QMessageBox::ApplyRole);

            msg_box.exec();

            if (msg_box.clickedButton() == exit)
                QApplication::quit();
            else if (msg_box.clickedButton() == again)
            {
                m_gc.restart_game();
                update_ui();
            }
            else if (msg_box.clickedButton() == new_game)
            {
                parentWidget()->show();
                close();
            }
        }
    }
}

void MainWindow::on_sword_upgrade_button_clicked()
{
    if (m_build_territory != nullptr && m_gc.is_friendly(m_build_territory))
    {
        m_build_territory->build_attack();
        update_resourses();
        update_building();
    }
}

void MainWindow::on_shield_upgrade_button_clicked()
{
    if (m_build_territory != nullptr && m_gc.is_friendly(m_build_territory))
    {
        m_build_territory->build_deffend();
        update_resourses();
        update_building();
    }
}

void MainWindow::on_luck_upgrade_button_clicked()
{
    if (m_build_territory != nullptr && m_gc.is_friendly(m_build_territory))
    {
        m_build_territory->build_luck();
        update_resourses();
        update_building();
    }
}

void MainWindow::tanks_are_deployed()
{
    update_bonus_tanks();
}

TurnPhase& MainWindow::turn_phase()
{
    return m_turn_phase;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
        show_exit_message();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->ignore();
    show_exit_message();
}

void MainWindow::show_exit_message()
{
    auto reply = QMessageBox::question(this, "EXIT", "Exit game?",
                                          QMessageBox::No | QMessageBox::Yes);

    if (reply == QMessageBox::Yes)
        QApplication::quit();
}
