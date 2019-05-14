#include <algorithm>
#include <iostream>
#include "cards.h"

/* Kombinacija level 2 */
const std::vector<Card> Cards::combination_1 {
    Card::Soldier, Card::Soldier, Card::Soldier};

/* Kombinacija level 2 */
const std::vector<Card> Cards::combination_2 {
    Card::Horseman, Card::Horseman, Card::Horseman};

/* Kombinacija level 3 */
const std::vector<Card> Cards::combination_3 {
    Card::Ship, Card::Ship, Card::Ship};

/* Kombinacija level 4 */
const std::vector<Card> Cards::combination_4 {
    Card::Soldier, Card::Horseman, Card::Ship};

Cards::Cards(
        unsigned int soldiers,
        unsigned int horsemen,
        unsigned int ships)
{
    for (unsigned int i = 0; i < soldiers; i++)
        m_cards.push_back(Card::Soldier);

    for (unsigned int i = 0; i < horsemen; i++)
        m_cards.push_back(Card::Horseman);

    for (unsigned int i = 0; i < ships; i++)
        m_cards.push_back(Card::Ship);

    shuffle_cards();
}

void Cards::add_card_from(Cards& cards)
{
    m_cards.push_back(cards.draw_top_card());
}

Card Cards::draw_top_card()
{
    /* Poslednja kartica */
    Card top_card = m_cards.back();

    /* Izbacujem poslednju karticu iz spila */
    m_cards.pop_back();

    return top_card;
}

void Cards::shuffle_cards()
{
    std::random_shuffle(m_cards.begin(), m_cards.end());
}

int Cards::number_of(const Card& selected_card) const
{
    return std::count(m_cards.cbegin(), m_cards.cend(), selected_card);
}

/* Proverava da li spil sadrzi prosledjene karte */
bool Cards::contains(const std::vector<Card>& selected_cards) const
{
    /* Pravim kopiju odabranih kartica */
    std::vector<Card> cards(selected_cards);

    for (auto card: m_cards)
    {
        for (unsigned int i = 0; i < cards.size(); i++)
            if (card == cards[i])
            {
                /* Izbacujem karticu iz kopiranog niza */
                cards.erase(cards.begin() + i);
                break;
            }
    }

    /* Ukoliko je kopirani niz prazan, sve kartice se nalaze u spilu */
    return cards.empty();
}

bool Cards::has_combination(const CardCombination& combination) const
{
    /* Proveravam da li spil sadrzi kombinaciju */
    switch (combination) {

    case CardCombination::ThreeSoldiers:
        if (contains(combination_1))
            return true;
        return false;

    case CardCombination::ThreeHorseman:
        if (contains(combination_2))
            return true;
        return false;

    case CardCombination::ThreeShips:
        if (contains(combination_3))
            return true;
        return false;

    case CardCombination::ThreeDifferent:
        if (contains(combination_4))
            return true;
        return false;

    default:
        std::cout << "Nepoznata kombinacija! " << std::endl;
        return false;
    }
}

unsigned int Cards::use_combination(const CardCombination& combination)
{
    if (!has_combination(combination))
        return 0;

    /*
     * Ako spil sadrzi kombinaciju, izbacujem kartice koje su
     * iskoriscene za kombinaciju i vracam vrednost bonus tenkica
     */
    switch (combination) {
    case CardCombination::ThreeSoldiers:
        remove_cards(combination_1);
        return 4;

    case CardCombination::ThreeHorseman:
        remove_cards(combination_2);
        return 6;

    case CardCombination::ThreeShips:
        remove_cards(combination_3);
        return 9;

    case CardCombination::ThreeDifferent:
        remove_cards(combination_4);
        return 12;

    default:
        /* Ovo se verovatno nikad ne izvrsava */
        std::cout << "Nepoznata kombinacija!" << std::endl;
        return 0;
    }
}

void Cards::remove_cards(const std::vector<Card>& cards_to_remove)
{
    for (auto card: cards_to_remove)
    {
        /* Pozicija karte koju bi trebalo izbaciti */
        auto positon = std::find(m_cards.cbegin(), m_cards.cend(), card);

        if (positon == m_cards.cend())
        {
            std::cout << "Karta " << card << " nije pronadjena!" << std::endl;
            continue;
        }
        else
            m_cards.erase(positon);
    }
}

void Cards::clear()
{
    m_cards.clear();
}

void Cards::print()
{
    std::cout << "CARDS: " << std::endl;
    std::cout << "------------------------------" << std::endl;

    for (auto card: m_cards)
        std::cout << card << std::endl;

    std::cout << "------------------------------" << std::endl;
}


std::ostream& operator << (std::ostream& out, const Card& card)
{
    switch (card)
    {
    case Card::Soldier:
        out << "SOLDIER";
        break;

    case Card::Horseman:
        out << "HORSEMAN";
        break;

    case Card::Ship:
        out << "SHIP";
        break;
    }
    return out;
}
