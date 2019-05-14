#include "territory.h"
#include <iostream>
#include <QDebug>

const std::vector<unsigned int> Territory::building_requirements({7, 14, 21});

Territory::Territory(QString name,
                     QString resource,
                     unsigned int tank_number,
                     std::shared_ptr<Player> owner)
    : m_name(name),
      m_resource(Reward::get_resource(resource)),
      m_tank_number(tank_number),
      m_owner(owner)
{}

void Territory::add_neighbour(std::shared_ptr<Territory> territory)
{
    m_neighbours.push_back(territory);
}

bool Territory::has_neighbour(std::shared_ptr<Territory> territory) const
{
    for (auto neighbour_ptr: m_neighbours)
    {
        /*
         * lock() se koristi da se od weak_ptr napravi shared_ptr,
         * a if granom se ispituje da li objekat na koji pokazivac
         * pokazuje jos uvek postoji
         */
        if (auto neighbour = neighbour_ptr.lock())
        {
            if (neighbour == territory)
                return true;
        }
    }
    return false;
}

void Territory::remove_tanks(unsigned int number)
{
    /* Posto je m_tank_number unsigned int, proveravam potkoracenje */
    if (m_tank_number < number)
        m_tank_number = 0;
    else
        m_tank_number -= number;

    emit tank_number_changed(m_tank_number);
}

void Territory::add_tanks(unsigned int number)
{
    m_tank_number += number;
    emit tank_number_changed(m_tank_number);
}

/*
 * U ovoj funkciji se ne proverava susednost teritorija, vec je implementirano
 * samo prebacivanje tenkica
 */
void Territory::transfer_tanks(
        std::shared_ptr<Territory> new_territory,
        unsigned int tank_number)
{
    /* Broj uspesno prebacenih tenkica */
    unsigned int possible_transfers = 0;
    /*
     * Vodi se racuna da na teritoriji mora da ostane bar jedan tenkic
     * posle transfera
     */
    while (m_tank_number > 1 && possible_transfers < tank_number)
    {
        possible_transfers++;
        remove_tanks(1);
    }

    /* Prebacuju se uspesno prebaceni tenkici na novu teritoriju */
    new_territory->add_tanks(possible_transfers);
}


bool Territory::can_build(unsigned int current_level){
    unsigned int num_metal = owner()->get_metal();
    unsigned int num_stone = owner()->get_stone();
    unsigned int num_wood = owner()->get_wood();

    /*
     * "building_requirements" sadrzi kolicinu resursa potrebnih za dati nivo izgradnje.
     * Potrebno je imati datu kolicinu svakog resursa
     */
    if (std::min(std::min(num_metal, num_stone), num_wood) < building_requirements[current_level])
    {
        std::cout << "Nedovoljno resursa za izgradnju";
        return false;
    }

    return true;
}

void Territory::build_attack()
{
    unsigned int current_level = m_buildings.attack_level();

    if (current_level == m_buildings.max_level())
        return;

    if(can_build(current_level))
    {
        spend_build_resources(current_level);
        m_buildings.upgrade_attack();
    }
}

void Territory::build_deffend()
{
    unsigned int current_level = m_buildings.defend_level();

    if (current_level == m_buildings.max_level())
        return;

    if(can_build(current_level))
    {
        spend_build_resources(current_level);
        m_buildings.upgrade_defend();
    }
}

void Territory::build_luck()
{
    unsigned int current_level = m_buildings.luck_level();

    if (current_level == m_buildings.max_level())
        return;

    if (can_build(current_level))
    {
        spend_build_resources(current_level);
        m_buildings.upgrade_luck();
    }
}

void Territory::spend_build_resources(unsigned int level)
{
    owner()->decrease_wood(building_requirements[level]);
    owner()->decrease_stone(building_requirements[level]);
    owner()->decrease_metal(building_requirements[level]);
}

unsigned int Territory::build_att_status()
{
     return  m_buildings.attack_level();
}

unsigned int Territory::build_def_status(){
    return m_buildings.defend_level();
}

QString Territory::name() const
{
    return m_name;
}

void Territory::set_name(const QString& name)
{
    m_name = name;
}

std::shared_ptr<Player> Territory::owner() const
{
    return m_owner;
}

void Territory::set_owner(const std::shared_ptr<Player> owner)
{
    m_owner = owner;
    emit owner_changed();
}

unsigned int Territory::tank_number() const
{
    return m_tank_number;
}

void Territory::set_tank_number(unsigned int tank_number)
{
    m_tank_number = tank_number;
    emit tank_number_changed(m_tank_number);
}

Buildings& Territory::buildings()
{
    return m_buildings;
}


void Territory::set_resource(const Resource& resource)
{
    m_resource = resource;
}

Resource Territory::resource() const
{
  return m_resource;
}

std::ostream& operator << (std::ostream& out, const Territory& territory)
{
    out << "Name: " << territory.m_name.toStdString()
        << " Resource: " << territory.m_resource
        << " Owner: " << territory.m_owner->name().toStdString()
        << " Tanks: " << territory.m_tank_number
        << " Neighbours: ";

    for (auto neighbour_ptr: territory.m_neighbours)
    {
        if (auto neighbour = neighbour_ptr.lock())
            out << neighbour->name().toStdString() << " ";
    }

    return out << std::endl;
}
