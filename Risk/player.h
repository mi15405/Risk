#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "cards.h"
#include "resource.h"

class Player
{
public:
    Player(QString name = "UNKNOWN", QString color = "blue");

    /* Dodaje bonus tenkice */
    void add_bonus_tanks(unsigned int number);
    void remove_bonus_tanks(unsigned int number);

    /* Uvecava broj teritorija */
    void increase_territory_number(unsigned int number = 1);

    /* Smanjuje broj teritorija */
    void decrease_territory_number(unsigned int number = 1);

    /* Iskoriscavanje kartica za bonus tenkice */
    void use_card_combination(CardCombination combination);

    /* Resetuje igraca na pocetne vrednosti */
    void reset();

    /* Geteri i seteri */

    unsigned int bonus_tanks() const;
    void set_bonus_tanks(unsigned int number);

    Cards& cards();

    QString name();
    void set_name(const QString& name);

    QString color();

    unsigned int territories_number();
    void set_territories_number(unsigned int territories_number);

    void add_resource(const Resource& resource);

    /* Metodi za resurse */
    void set_metal(unsigned int num);
    void set_stone(unsigned int num);
    void set_wood(unsigned int num);

    unsigned int get_metal() const;
    unsigned int get_stone() const;
    unsigned int get_wood() const;

    void increase_metal(unsigned int num);
    void increase_stone(unsigned int num);
    void increase_wood(unsigned int num);

    void decrease_metal(unsigned int num);
    void decrease_stone(unsigned int num);
    void decrease_wood(unsigned int num);

private:
    /* Ime igraca */
    QString m_name;

    /* Boja igraca */
    QString m_color;

    /* Kartice koje igrac poseduje */
    Cards m_cards;

    /* Broj teritorija koje igrac poseduje */
    unsigned int m_territories_number;

    /* Dodatni tenkici */
    unsigned int m_bonus_tanks;

    /* Broj resursa koji igrac poseduje */
    unsigned int m_wood_num;

    unsigned int m_stone_num;

    unsigned int m_metal_num;
};

#endif // PLAYER_H
