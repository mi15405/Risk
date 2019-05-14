#include "player.h"

Player::Player(QString name, QString color)
    : m_name(name),
      m_color(color),
      m_territories_number(0),
      m_bonus_tanks(0),
      m_wood_num(0),
      m_stone_num(0),
      m_metal_num(0)
{}

void Player::add_bonus_tanks(unsigned int number)
{
    m_bonus_tanks += number;
}

void Player::remove_bonus_tanks(unsigned int number)
{
    if (m_bonus_tanks < number)
        m_bonus_tanks = 0;
    else
        m_bonus_tanks -= number;
}

void Player::add_resource(const Resource& resource)
{
    switch(resource)
    {
    case Resource::Wood:
        increase_wood(1);
        break;
    case Resource::Stone:
        increase_stone(1);
        break;
    case Resource::Metal:
        increase_metal(1);
        break;
    }
}

void Player::increase_territory_number(unsigned int number)
{
    m_territories_number += number;
}

void Player::decrease_territory_number(unsigned int number)
{
    m_territories_number -= number;
}

void Player::use_card_combination(CardCombination combination)
{
    unsigned int bonus_tanks = m_cards.use_combination(combination);
    if (bonus_tanks > 0)
        m_bonus_tanks += bonus_tanks;
}

void Player::reset()
{
    m_cards.clear();
    set_territories_number(0);
    set_bonus_tanks(0);
    set_wood(0);
    set_metal(0);
    set_stone(0);
}

QString Player::name()
{
    return m_name;
}

QString Player::color()
{
    return m_color;
}

void Player::set_name(const QString& name)
{
    m_name = name;
}

unsigned int Player::territories_number()
{
    return m_territories_number;
}

void Player::set_territories_number(unsigned int territories_number)
{
    m_territories_number = territories_number;
}

Cards& Player::cards()
{
    return m_cards;
}

unsigned int Player::bonus_tanks() const
{
    return m_bonus_tanks;
}

void Player::set_bonus_tanks(unsigned int number)
{
    m_bonus_tanks = number;
}

void Player::set_metal(unsigned int num)
{
    m_metal_num = num;
}

void Player::set_stone(unsigned int num)
{
    m_stone_num = num;
}

void Player::set_wood(unsigned int num)
{
    m_wood_num = num;
}

unsigned int Player::get_metal() const
{
    return m_metal_num;
}

unsigned int Player::get_stone() const
{
    return m_stone_num;
}

unsigned int Player::get_wood() const
{
    return m_wood_num;
}

void Player::increase_metal(unsigned int num)
{
    m_metal_num += num;
}

void Player::increase_stone(unsigned int num)
{
    m_stone_num += num;
}

void Player::increase_wood(unsigned int num)
{
    m_wood_num += num;
}

void Player::decrease_metal(unsigned int num)
{
    if (m_metal_num < num)
        m_metal_num = 0;
    else
        m_metal_num -= num;
}

void Player::decrease_stone(unsigned int num)
{
    if (m_stone_num < num)
        m_stone_num = 0;
    else
        m_stone_num -= num;
}

void Player::decrease_wood(unsigned int num)
{
    if (m_wood_num < num)
        m_wood_num = 0;
    else
        m_wood_num -= num;
}
