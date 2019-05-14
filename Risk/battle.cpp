#include "battle.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <QDebug>

Battle::Battle()
    : m_min_dice_value(1),
      m_max_dice_value(10)
{
    /* Inicijalizuje seed za generator nasumicnih brojeva */
    std::srand(std::time(0));
}

bool Battle::attack(
        std::shared_ptr<Territory> attacker,
        std::shared_ptr<Territory> defender)
{
    if (!attack_is_possible(attacker, defender))
    {
        std::cout << "Napad nije moguc!" << std::endl;
        return false;
    }

    /* Bacanje kockica */
    set_attacker_dice(attacker);
    set_defender_dice(defender);

    /*
     * Broj borbi jednak je broju kockica koje se uporedjuju u jednom napadu.
     * Bira se manji broj izmedju broja napadackih tenkica
     * i broja odbrambenih tenkica
     */
    int battle_number = std::min(m_attacker_dice.size(), m_defender_dice.size());

    /* Brojim koliko puta je napadac pobedio (imao vecu kockicu) */
    int attacker_wins = 0;

    for (int i = 0; i < battle_number; i++)
        if (m_attacker_dice[i] > m_defender_dice[i])
            attacker_wins++;

    /* Odbrambenoj teritorij skidam onoliko tenkica koliko su izgubili */
    defender->remove_tanks(attacker_wins);

    /* Isto i napadackoj teritoriji */
    attacker->remove_tanks(battle_number - attacker_wins);

    /*
     * Ukoliko odbrambenoj teritoriji nije ostalo tenkica, napadac je
     * preuzima i prebacuje onoliko tenkica sa koliko je napao
     */
    if (defender->tank_number() == 0)
    {
        /* Napadacu se uvecava broj teritorija za 1 */
        attacker->owner()->increase_territory_number(1);

        /* Braniocu se smanjuje broj teritorija za 1 */
        defender->owner()->decrease_territory_number(1);

        /* Teritoriji se dodeljuje novi vlasnik */
        defender->set_owner(attacker->owner());

        /* Prebacuju se tenkici koji su napadali */
        attacker->transfer_tanks(defender, m_attacker_dice.size());

        /* Vraca se true jer je osvojena teritorija */
        return true;
    }
    /* Vraca se false jer nije osvojena teritorija */
    return false;
}

bool Battle::attack_is_possible(
        std::shared_ptr<Territory> attacker,
        std::shared_ptr<Territory> defender) const
{
    if (!attacker->has_neighbour(defender))
    {
        std::cout << "Teritorije nisu povezane." << std::endl;
        return false;
    }
    else if (attacker->owner() == defender->owner())
    {
        std::cout << "Napad na sopstvenu teritoriju nije moguc." << std::endl;
        return false;
    }
    else
    {
        std::cout << "Uspesno selektovan napad." << std::endl;
        return true;
    }
}

unsigned int Battle::roll_dice() const
{
    /* Vraca nasumican broj u opsegu [m_min_dice_value, m_max_dice_value] */
    return m_min_dice_value + std::rand() % (m_max_dice_value - m_min_dice_value + 1);
}

void Battle::set_attacker_dice(std::shared_ptr<Territory> attacker)
{
    /* Dodatni tenkici, ukoliko je izgradjena gradjevina za napad */
    unsigned int building_bonus = attacker->buildings().attack_level();

    /* Podrazumevan broj napadaca */
    unsigned int default_attacker_number = 3;

    /* Broj tenkica za napad */
    unsigned int number_of_attackers = std::min(
                default_attacker_number + building_bonus,
                attacker->tank_number() - 1);

    /* Bacanje kockica za napad */
    set_tanks_power(m_attacker_dice, number_of_attackers);

    /* Dodavanje snage za napad u zavisnosti od faktora srece */
    add_luck_bonus(m_attacker_dice, attacker->buildings().luck_level());
}

void Battle::set_defender_dice(
        std::shared_ptr<Territory> defender)
{
    /* Dodatni tenkici, ukoliko je izgradjena gradjevina za odbranu */
    unsigned int building_bonus = defender->buildings().defend_level();

    /* Podrazumevan broj odbrambenih tenkica */
    unsigned int default_defender_number = 2;

    /* Broj tenkica koji se brane */
    int number_of_defenders = std::min(
                default_defender_number + building_bonus,
                defender->tank_number());

    /* Bacanje kockica za odbranu */
    set_tanks_power(m_defender_dice, number_of_defenders);

    /* Dodavanje snage za odbranu u zavisnosti od faktora srece */
    add_luck_bonus(m_defender_dice, defender->buildings().defend_level());
}

void Battle::set_tanks_power(std::vector<unsigned int>& tanks, unsigned int tank_number)
{
    /* Brise prethodni niz */
    tanks.clear();

    /* Baca kockice i dodaje ih u niz */
    for (unsigned int i = 0; i < tank_number; i++)
        tanks.push_back(roll_dice());

    /* Sortira kockice opadajuce */
    std::sort(tanks.begin(), tanks.end(), std::greater<unsigned int>());
}

void Battle::add_luck_bonus(std::vector<unsigned int>& tanks, unsigned int luck_level)
{
    if (luck_level == 0)
        return;
    else if (luck_level == 1)
       increase_dice_value(tanks[0], 1);
    else if (luck_level == 2)
    {
        if (tanks.size() == 1)
            increase_dice_value(tanks[0], 2);
        else
        {
            increase_dice_value(tanks[0], 1);
            increase_dice_value(tanks[1], 1);
        }
    }
    else if (luck_level == 3)
    {
        if (tanks.size() == 1)
            increase_dice_value(tanks[0], 3);
        else
        {
            increase_dice_value(tanks[0], 2);
            increase_dice_value(tanks[0], 1);
        }
    }

    /* Sortiramo nove vrednosti */
    std::sort(tanks.begin(), tanks.end(), std::greater<unsigned int>());
}

void Battle::increase_dice_value(unsigned int& dice, unsigned int increase)
{
    dice += increase;

    if (dice > m_max_dice_value)
        dice = m_max_dice_value;
}

QString Battle::attacker_power_string() const
{
    return numbers_to_qstring(m_attacker_dice);
}

QString Battle::defender_power_string() const
{
    return numbers_to_qstring(m_defender_dice);
}

template <typename T>
QString Battle::numbers_to_qstring(const std::vector<T>& vec) const
{
    QStringList strList;
    for (auto element: vec)
        strList << QString::number(element);

    return strList.join(' ');
}
