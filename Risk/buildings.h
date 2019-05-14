#ifndef BUILDINGS_H
#define BUILDINGS_H

#include <iostream>

class Buildings
{
public:
    Buildings(
            unsigned int attack = 0,
            unsigned int defend = 0,
            unsigned int luck = 0,
            unsigned int max_level = 3);

    /* Unapredjivanje gradjevina */
    void upgrade_attack();
    void upgrade_defend();
    void upgrade_luck();

    unsigned int attack_level() const;
    unsigned int defend_level() const;
    unsigned int luck_level() const;
    unsigned int max_level() const;

private:
    unsigned int m_attack_level;

    unsigned int m_defend_level;

    unsigned int m_luck_level;

    unsigned int m_max_level;

};

#endif // BUILDINGS_H
