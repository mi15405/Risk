#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <vector>
#include <memory>
#include <map>
#include "player.h"
#include "territory.h"
#include "battle.h"
#include "buildings.h"

typedef struct {
   int start_territory_id;
   int end_territory_id;
   unsigned int bonus_tanks;
} Continent;

class GameController
{
public:
    GameController();

    /* Inicijalizacija mape */
    void init_map();

    /* Povezivanje teritorija */
    void connect_territories();

    /* Izvrsava napad */
    void attack(
            std::shared_ptr<Territory> attacker,
            std::shared_ptr<Territory> defender);

    /* Menja igraca koji je na potezu */
    void end_turn();

    /* Vraca teritoriju sa prosledjenim id-jem */
    std::shared_ptr<Territory> territory_with_id(int id);

    /* Igrac na potezu */
    std::shared_ptr<Player> current_player();

    /* Proverava da li je u toku PRVI POTEZ */
    bool is_first_turn();

    /* Proverava da li je igra zavrsena */
    bool is_game_over();

    /* Proverava da li je teritorija pripada trenutnom igracu */
    bool is_friendly(std::shared_ptr<Territory> territory);

    /* Ponovo pokrece igru sa istim igracima */
    void restart_game();

    /* Postavlja pocetne vrednosti */
    void start_game();

    /* Inicijalizuje igrace sa prosledjenim imenima i bojama */
   // void set_players(QStringList names, QStringList colors);

    /* Inicijalizuje igrace sa prosledjenim imenima i bojama */
    void init_players(QVector<QPair<QString, QString>> players);

    /* Geteri i seteri */
    Battle& battle();

    /*
     * Cita informacije iz fajla, liniju po liniju i delove odvojene separatorom
     * cuva u QStringListi.
     */
    static std::vector<QStringList> read_from_file(QString filepath, char separator);

    /* Funkcije za ispis kockica */
    QString attacking_dice();
    QString defending_dice();

private:
    /* Lista kontinenata */
    static const QList<Continent> continents;

    /*
     * Proverava da li igrac poseduje sve teritorije sa id-jevima
     * od start_id do end_id
     */
    bool has_all_territories(std::shared_ptr<Player> player, int start_id, int end_id);

    /* Dodeljuje nagrade na pocetku poteza */
    void give_rewards();

    /* Dodeljuje resurse prosledjenom igracu */
    void give_resource_rewards(std::shared_ptr<Player> player);

    /* Izracunava bonuse za posedovanje celih kontinenata */
    unsigned int bonus_from_continents(std::shared_ptr<Player> player);

    /* Dodatni tenkici na pocetku poteza */
    unsigned int tank_rewards();

    /* Nasumicno dodeljuje teritorije igracima */
    void set_territory_owners();

    /* Postavlja pocetni broj tenkica svim igracima */
    void set_starting_tanks();

    /* Postavlja fleg da je teritorija osvojena u trenutnom potezu */
    void set_is_territory_won(bool won);

    /* Id-jevi teritorija */
    std::vector<int> map_keys();

    /* Niz igraca */
    std::vector<std::shared_ptr<Player>> m_players;

    /* Spil kartica */
    Cards m_main_deck;

    /* Indeks igraca koji je na potezu */
    unsigned int m_current_player;

    /* Broj odigranih poteza */
    unsigned int m_turns_played;

    /* Da li je osvojena teritorija */
    bool m_is_territory_won;

    /* Objekat za borbu */
    Battle m_battle;

    /* Teritorije se cuvaju u mapi. Kljuc za svaku teritoriju je njen id. */
    std::map<int, std::shared_ptr<Territory>> m_map;
};

#endif // GAMECONTROLLER_H
