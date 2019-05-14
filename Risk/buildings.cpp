#include "buildings.h"

Buildings::Buildings(
        unsigned int attack,
        unsigned int defend,
        unsigned int luck,
        unsigned int max_level)
    : m_attack_level(attack),
      m_defend_level(defend),
      m_luck_level(luck),
      m_max_level(max_level)
{}

void Buildings::upgrade_attack()
{
    if (m_attack_level < m_max_level)
        m_attack_level++;
}

void Buildings::upgrade_defend()
{
    if (m_defend_level < m_max_level)
        m_defend_level++;
}

void Buildings::upgrade_luck()
{
    if (m_luck_level < m_max_level)
        m_luck_level++;
}

unsigned int Buildings::attack_level() const
{
    return m_attack_level;
}

unsigned int Buildings::defend_level() const
{
    return m_defend_level;
}

unsigned int Buildings::luck_level() const
{
    return m_luck_level;
}

unsigned int Buildings::max_level() const
{
    return m_max_level;
}
