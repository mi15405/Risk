#include "gamecontroller.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

const QList<Continent> GameController::continents = {
    {1, 8, 6},
    {9, 18, 9},
    {19, 26, 5},
    {27, 34, 11},
    {35, 42, 5}
};

GameController::GameController()
    : m_main_deck(50, 40, 30),
      m_current_player(0),
      m_turns_played(0),
      m_is_territory_won(false)
{
    init_map();
    connect_territories();
}

void GameController::init_map()
{
    /* Konstruisem putanju do fajla sa teritorijama */
    QString filepath = QDir::currentPath() + "/../Risk/territories.txt";

    /*
     * Ucitavam liniju po liniju, i sadrzaj odvojen separatorom(' ') cuvam
     * u QStringList-i. Dakle, dobijam niz QStringListi, po jedan element za
     * svaku liniju.
     */
    std::vector<QStringList> data = GameController::read_from_file(filepath, ' ');

    for (auto row: data)
    {
        /* Prvi broj je id teritorije */
        int id = row.at(0).toInt();

        /* Druga niska je ime teritorije */
        QString name = row.at(1);

        /* Resurs koji teritorija poseduje */
        QString resource = row.at(2);

        /* U mapu ubacujem konstruisani par (id -> teritorija) */
        m_map.insert(
            std::pair<int, std::shared_ptr<Territory>>(
                        id, std::make_shared<Territory>(name, resource))
        );
    }
}

void GameController::connect_territories()
{
    QString filepath = QDir::currentPath() + "/../Risk/neighbours.txt";
    std::vector<QStringList> data = read_from_file(filepath, ' ');

    for (auto row: data)
    {
        /* Id teritorije kojoj se dodaju susedi */
        int id = row.at(0).toInt();

        /* Teritorija kojoj se dodaju susedi, izvucena iz mape */
        auto territory = territory_with_id(id);

        for (int i = 1; i < row.size(); i++)
        {
            /* Id suseda koji se dodaje */
            int neighbour_id = row.at(i).toInt();

            /* Teritoriji dodajem suseda, koga vadim iz mape */
            territory->add_neighbour(territory_with_id(neighbour_id));
        }
    }
}

void GameController::set_territory_owners()
{
    /* Id-jevi teritorija */
    std::vector<int> keys = map_keys();

    /* Mesaju se id-jevi */
    std::random_shuffle(keys.begin(), keys.end());

    /* Indeks trenutnog vlasnika */
    unsigned int current_owner_index = 0;

    for (auto key: keys)
    {
        /* Trenutni vlasnik */
        auto current_owner = m_players[current_owner_index];

        /* Teritoriji sa id-jem (key) se postavlja trenutni vlasnik */
        territory_with_id(key)->set_owner(current_owner);

        /* Uvecava se broj teritorija trenutnog vlasnika (igraca) */
        current_owner->increase_territory_number(1);

        /* Prelazi se na sledeceg igraca */
        current_owner_index++;

        /* Vodi se racuna o prekoracenju */
        current_owner_index %= m_players.size();
    }
}

void GameController::attack(
        std::shared_ptr<Territory> attacker,
        std::shared_ptr<Territory> defender)
{
    bool attacker_won_territory = m_battle.attack(attacker, defender);

    if (attacker_won_territory)
        m_is_territory_won = true;
}

void GameController::give_rewards()
{
    give_resource_rewards(current_player());
    current_player()->add_bonus_tanks(tank_rewards());
}

void GameController::give_resource_rewards(std::shared_ptr<Player> player)
{
    for (auto it = m_map.cbegin(); it != m_map.cend(); ++it)
        if (it->second->owner() == player)
            player->add_resource(it->second->resource());
}

unsigned int GameController::tank_rewards()
{
    /* Bonus od broja teritorija koje igrac poseduje */
    unsigned int bonus_tanks = current_player()->territories_number() / 3 + 1;

    /* Dodaje se bonus od kontinenata koje igrac poseduje */
    bonus_tanks += bonus_from_continents(current_player());

    return bonus_tanks;
}

void GameController::end_turn()
{
    /* Ukoliko je osvojena teritorija, igrac vuce kartu iz spila */
    if (m_is_territory_won)
    {
        current_player()->cards().add_card_from(m_main_deck);
        m_is_territory_won = false;
    }

    /* Igrac na potezu postaje sledeci igrac iz niza */
    m_current_player++;

    /* Vodi se racuna o prekoracenju */
    m_current_player %= m_players.size();

    /* Uvecava se broj odigranih poteza */
    m_turns_played++;

    /* Ukoliko je igrac ispao (nema vise teritorija), predji na sledeg */
    if (current_player()->territories_number() == 0)
        end_turn();
    /* Ukoliko igracu nije prvi potez, zapocinje potez sa dodeljivanjem nagrada */
    else if (!is_first_turn())
        give_rewards();
}

/*
 * Proverava da li je igra zavrsena
 */
bool GameController::is_game_over()
{
    if (m_players[m_current_player]->territories_number() >= 28){
        return true;
    }
    return false;
}

bool GameController::is_first_turn()
{
    if (m_turns_played < m_players.size())
        return true;
    return false;
}

unsigned int GameController::bonus_from_continents(std::shared_ptr<Player> player)
{
    unsigned int bonus_tanks = 0;

    for (auto continent: continents)
    {
        if (has_all_territories(player,
                                continent.start_territory_id,
                                continent.end_territory_id))
            bonus_tanks += continent.bonus_tanks;
    }

    return bonus_tanks;
}

bool GameController::has_all_territories(
        std::shared_ptr<Player> player,
        int start_id, int end_id)
{
    /*
     * Proveravam da li je "player" vlasnik svih teritorija
     * sa id-jevima od start_id do end_id
     */
    for (int i = start_id; i <= end_id; ++i)
    {
        if (territory_with_id(i)->owner() != player)
            return false;
    }
    return true;
}

std::shared_ptr<Territory> GameController::territory_with_id(int id)
{
    return m_map.find(id)->second;
}

bool GameController::is_friendly(std::shared_ptr<Territory> territory)
{
    if (current_player() == territory->owner())
        return true;
    return false;
}

std::shared_ptr<Player> GameController::current_player()
{
    return m_players[m_current_player];
}

void GameController::restart_game()
{
    for (auto player: m_players)
        player->reset();

    for (auto it = m_map.begin(); it != m_map.end(); ++it)
        it->second->set_tank_number(1);

    start_game();
}

void GameController::start_game()
{
    set_territory_owners();
    set_starting_tanks();
    set_is_territory_won(false);

    m_turns_played = 0;
    m_current_player = 0;
}

void GameController::init_players(QVector<QPair<QString, QString>> players)
{
    for (auto it = players.begin(); it != players.end(); ++it)
        m_players.push_back(std::make_shared<Player> (it->first, it->second));
}

void GameController::set_starting_tanks()
{
    /*
     * Za 6 igraca, pocetni broj tenkica je 30.
     * Za svakog igraca koji ne igra, dodaje se po 5 tenkica
     * ostalim igracima
     */
    unsigned int tank_number = 30 + 5 * (6 - m_players.size());

    for (auto player: m_players)
        player->set_bonus_tanks(tank_number);
}

std::vector<int> GameController::map_keys()
{
    std::vector<int> keys;
    for (auto it = m_map.cbegin(); it != m_map.cend(); it++)
        keys.push_back(it->first);

    return keys;
}

void GameController::set_is_territory_won(bool won)
{
    m_is_territory_won = won;
}

Battle& GameController::battle()
{
    return m_battle;
}

std::vector<QStringList> GameController::read_from_file(QString filepath, char separator)
{
    /* Fajl objekat */
    QFile file(filepath);

    /* Proverava da li je fajl uspesno otvoren */
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw "FAILED TO OPEN FILE: " + filepath.toStdString();

    /* Tok iz kog citam tekstualne podatke */
    QTextStream in(&file);

    /* Procitani podaci */
    std::vector<QStringList> data;

    while (!in.atEnd())
    {
        /* Citam jednu liniju */
        QString line = in.readLine();

        /* Delim je separatorom i cuvam u nizu */
        data.push_back(line.split(separator));
    }

    /* Zatvaram fajl */
    file.close();

    return data;
}

QString GameController::attacking_dice()
{
    return m_battle.attacker_power_string();
}

QString GameController::defending_dice()
{
    return m_battle.defender_power_string();
}
