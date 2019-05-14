#ifndef TERRITORY_H
#define TERRITORY_H

#include <vector>
#include <memory>
#include <QString>
#include <QObject>
#include "player.h"
#include "buildings.h"
#include "resource.h"

class Territory : public QObject
{
    Q_OBJECT

public:
    Territory(QString name,
              QString resource,
              unsigned int tank_number = 1,
              std::shared_ptr<Player> owner = nullptr);

    /* Dodaje suseda */
    void add_neighbour(std::shared_ptr<Territory> territory);

    /* Proverava da li je susedna sa prosledjenom teritorijom */
    bool has_neighbour(std::shared_ptr<Territory> territory) const;

    /* Sklanja tenkice sa teritorije */
    void remove_tanks(unsigned int number);

    /* Dodaje tenkice na teritoriju */
    void add_tanks(unsigned int number);

    /* Premesta tenkice sa ove teritorije na prosledjenu teritoriju) */
    void transfer_tanks(
            std::shared_ptr<Territory> new_territory,
            unsigned int tank_number);

    /* Izgradnja gradjevina */
    void build_attack();
    void build_deffend();
    void build_luck();

    /* Status gradjevina */
    unsigned int build_att_status();
    unsigned int build_def_status();

    /* Geteri i Seteri */
    QString name() const;
    void set_name(const QString& name);

    std::shared_ptr<Player> owner() const;
    void set_owner(const std::shared_ptr<Player> owner);

    unsigned int tank_number() const;
    void set_tank_number(unsigned int tank_number);

    void set_resource(const Resource& res);
    Resource resource() const;

    Buildings& buildings();

signals:
    void tank_number_changed(unsigned int);
    void owner_changed(void);

private:
    /* Kolicina resursa potrebna za izgradnju, sortirano po nivou gradjevine */
    static const std::vector<unsigned int> building_requirements;

    /* Trosi resurse na izgradnju */
    void spend_build_resources(unsigned int level);

    /* Proverava da li je izgradnja moguca */
    bool can_build(unsigned int current_level);

    /* Ime teritorije */
    QString m_name;

    /* Resurs */
    Resource m_resource;

    /* Broj tenkica na teritoriji */
    unsigned int m_tank_number;

    /* Vlasnik teritorije */
    std::shared_ptr<Player> m_owner;

    /* Niz susednih teritorija */
    std::vector<std::weak_ptr<Territory>> m_neighbours;

    /* Gradjevine */
    Buildings m_buildings;

    /* Ispisivanje teritorije */
    friend std::ostream& operator << (std::ostream& out, const Territory& territory);
};

std::ostream& operator << (std::ostream& out, const Territory& territory);

#endif // TERRITORY_H
