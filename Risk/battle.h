#ifndef BATTLE_H
#define BATTLE_H

#include "territory.h"
#include <memory>
#include <vector>
#include <string>

class Battle
{
public:
    Battle();

    /*
     * Izvrsava napad sa teritorije attacker na teritoriju defender.
     * Vraca "true" ako je osvojena teritorija.
     */
    bool attack(
            std::shared_ptr<Territory> attacker,
            std::shared_ptr<Territory> defender);

    /* Proverava da li je napad moguc izmedju dve teritorije */
    bool attack_is_possible(
            std::shared_ptr<Territory> attacker,
            std::shared_ptr<Territory> defender) const;

    /* Baca kockice za napad */
    void set_attacker_dice(std::shared_ptr<Territory> attacker);

    /* Baca kockice za odbranu */
    void set_defender_dice(std::shared_ptr<Territory> defender);

    /* Vraca kockice za napad u obliku QStringa */
    QString attacker_power_string() const;

    /* Vraca kockice za odbranu u obliku QStringa */
    QString defender_power_string() const;

private:
    /* Najmanja vrednost na kockici */
    unsigned int m_min_dice_value;

    /* Najveca vrednost na kockici */
    unsigned int m_max_dice_value;

    /* Niz koji predstavlja kockice za napad */
    std::vector<unsigned int> m_attacker_dice;

    /* Niz koji predstavlja kockice za odbranu */
    std::vector<unsigned int> m_defender_dice;

    /* Baca kockice i cuva ih u proslednjenom nizu */
    void set_tanks_power(std::vector<unsigned int>& tanks, unsigned int tank_number);

    /* Povecava vrednost prosledjene kockice */
    void increase_dice_value(unsigned int& dice, unsigned int increase);

    /* Dodaje bonus za srecu, povecavajuci vrednost kockicama */
    void add_luck_bonus(std::vector<unsigned int>& tanks, unsigned int luck_level);

    /* Baca jednu kockicu */
    unsigned int roll_dice() const;

    /* Prebacuje niz brojeva u QString */
    template <typename T>
    QString numbers_to_qstring(const std::vector<T>& vec) const;
};

#endif// BATTLE_H
