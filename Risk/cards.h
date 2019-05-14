#ifndef CARDS_H
#define CARDS_H

#include <vector>
#include <iostream>

enum class Card
{
    Soldier, Horseman, Ship
};

enum class CardCombination
{
    ThreeSoldiers, ThreeHorseman, ThreeShips, ThreeDifferent
};

std::ostream& operator << (std::ostream& out, const Card& card);

class Cards
{
public:
    Cards(
            unsigned int soldiers = 0,
            unsigned int horsemen = 0,
            unsigned int ships = 0);

    /* Vuce kartu iz prosledjenog spila, i dodaje je u ovaj spil */
    void add_card_from(Cards& cards);

    /* Mesa karte */
    void shuffle_cards();

    /* Vraca broj pojavljivanja prosledjene karte */
    int number_of(const Card& selected_card) const;

    /* Proverava da li spil sadrzi kombinaciju prosledjenog nivoa */
    bool has_combination(const CardCombination& combination) const;

    /* Izbacuje kartice iskoriscene za kombinaciju i vraca broj bonus tenkica */
    unsigned int use_combination(const CardCombination& combination);

    /* Brise sve kartice iz spila */
    void clear();

    /* Stampa spil karata */
    void print();

private:
    /* Kombinacija level 1 */
    static const std::vector<Card> combination_1;

    /* Kombinacija level 2 */
    static const std::vector<Card> combination_2;

    /* Kombinacija level 3 */
    static const std::vector<Card> combination_3;

    /* Kombinacija level 4 */
    static const std::vector<Card> combination_4;

    /* Vraca kartu sa vrha i izbacuje je iz spila */
    Card draw_top_card();

    /* Izbacuje karte iz spila */
    void remove_cards(const std::vector<Card>& cards_to_remove);

    /* Proverava da li spil sadrzi prosledjene karte */
    bool contains(const std::vector<Card>& cards) const;

    /* Niz karata */
    std::vector<Card> m_cards;
};

#endif // CARDS_H
