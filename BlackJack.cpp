#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class Card
{
public:
    enum suit {
        CLUBS,
        DIAMONDS,
        HEARTS,
        SPADES
    };
    enum value {
        ACE = 1,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING
    };

    void setValue(value _value, suit _suit)
    {
        v = _value;
        s = _suit;
    }

    Card(value _value = ACE, suit _suit = SPADES, bool _cardPosition = true) : v(_value), s(_suit), cardPosition(_cardPosition) {}
    void Flip() 
    {
        cardPosition = !(cardPosition);
    }
    int getValue()
    {
        int vl = 0;
        if (cardPosition)
        {
            vl = v;
            if (vl > 10)
                vl = 10;
        }
        return vl;
    }

    friend ostream& operator<<(ostream& str, Card& cards)
    {
        const string RANKS[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9","10", "J", "Q", "K" };
        const string SUITS[] = { "c", "d", "h", "s" };
        if (cards.cardPosition)
            str << RANKS[cards.v - 1] << " " << SUITS[cards.s];
        else
            str << "XX";

        return str;
    }
protected:
    value v;
    suit s;
    bool cardPosition;
};

class Hand
{
public:
    Hand()
    {
        cards.reserve(7);
    }
    virtual ~Hand()
    {
        Clear();
    }
    void Add(Card* aCard)
    {
        cards.push_back(aCard);
    }
    void Clear()
    {
        vector<Card*>::iterator iter = cards.begin();
        for (iter = cards.begin(); iter != cards.end(); iter++)
        {
            delete* iter;
            *iter = 0;
        }
        cards.clear();
    }
    int getAmount() const
    {
        if (cards.empty())
            return 0;
        int test = 0;
        for (auto crds : cards)
            test += crds->getValue();
        bool tests = false;
        for (auto crds : cards)
            if (crds->getValue() == crds->ACE) tests = true;
        if (tests && test <= 11)
            test += 10;
        return test;
    }
protected:
    vector<Card*> cards;
};

class GenericPlayer : public Hand
{
    friend ostream& operator<<(ostream& str, const GenericPlayer& aGenericPlayer)
    {
        str << aGenericPlayer.name << ":\t";
        vector<Card*>::const_iterator crd;
        if (!aGenericPlayer.cards.empty())
        {
            for (crd = aGenericPlayer.cards.begin(); crd != aGenericPlayer.cards.end(); crd++)
                str << (*crd) << "\t";

            if (aGenericPlayer.getAmount() != 0)
                cout << "(" << aGenericPlayer.getAmount() << ")";
        }
        else
            str << "<empty>";

        return str;
    }
public:
    GenericPlayer(const string& _name = "") : name(_name) {}
    virtual ~GenericPlayer() {}

    virtual bool IsHitting() const = 0 {}

    bool IsBoosted()
    {
            return (getAmount() > 21);
    }

    void Bust() 
    {
            cout << "У игрока " << name << " перебор" << endl;
    }
protected:
    string name;
};

class Player : public GenericPlayer
{
public:
    Player(const string _name = "") : GenericPlayer(_name) {}

    virtual ~Player() {}

    virtual bool IsHitting() const 
    {
        cout << name << " нужна ещё карта?" << endl;
        char answer;
        cin >> answer;
        return (answer == 'y' || answer == 'Y');
    }

    void Win() const 
    {
        cout << name << " Победа" << endl;
    }

    void Lose() const 
    {
        cout << name << " Поражение" << endl;
    }

    void Push() const 
    {
        cout << name << " Ничья" << endl;
    }
};

class House : public GenericPlayer
{
public:
    House(const string& _name = "Диллер") : GenericPlayer(_name) {}

    virtual ~House() {}
    virtual bool IsHitting() const
    {
        return (getAmount() <= 16);
    }

    void FlipFirstCard() 
    {
        if (!cards.empty())
        {
            cards[0]->Flip();
        }
        else cout << "Нечего перевернуть" << endl;
    }
};

class Deck : public Hand
{
public:
    Deck()
    {
        cards.reserve(52);
        Populate();
    }

    virtual ~Deck() {}

    void Populate() 
    {
        Clear();
        for (int c = Card::CLUBS; c <= Card::SPADES; c++)
        {
            for (int s = Card::ACE; s <= Card::KING; s++)
                Add(new Card(static_cast<Card::value>(s), static_cast<Card::suit>(c)));
        }
    }
    void Shuffle()
    {
        random_shuffle(cards.begin(), cards.end());
    }
    void Deal(Hand& aHand)
    {
        if (!cards.empty())
        {
            aHand.Add(cards.back());
            cards.pop_back();
        }
        else
            cout << "Карты кончились. Невозможно раздать!";
    }
    void AddltionalCards(GenericPlayer& aGenericPlayer)
    {
        cout << endl;
        while (!(aGenericPlayer.IsBoosted()) && aGenericPlayer.IsHitting())
        {
            Deal(aGenericPlayer);
            cout << (aGenericPlayer) << endl;

            if (aGenericPlayer.IsBoosted())
                aGenericPlayer.Bust();
        }
    }
};

class Game
{
public:
    Game(const vector<string>& names)
    {
        vector<string>::const_iterator Names;
        for (Names = names.begin(); Names != names.end(); Names++)
        {
            players.push_back(Player(*Names));
        }

        srand(time(0));
        deck.Populate();
        deck.Shuffle();
    }
    ~Game() {}

    void Play()
    {
        vector<Player>::iterator plr;
        for (int i = 0; i < 2; i++)
        {
            for (plr = players.begin(); plr != players.end(); plr++)
                deck.Deal(*plr);
            deck.Deal(house);
        }

        house.FlipFirstCard();

        for (plr = players.begin(); plr != players.end(); plr++)
            cout << *plr << endl;

        cout << house << endl;

        for (plr = players.begin(); plr != players.end(); plr++)
            deck.AddltionalCards(*plr);

        house.FlipFirstCard();
        cout << endl << house;
        
        deck.AddltionalCards(house);

        if (house.IsBoosted())
        {
            for (plr = players.begin(); plr != players.end(); plr++)
            {
                if (!(plr->IsBoosted()))
                    plr->Win();
            }
        }
        else
        {
            for (plr = players.begin(); plr != players.end(); plr++)
            {
                if (!(plr->IsBoosted()))
                {
                    if (plr->getAmount() > house.getAmount())
                        plr->Win();
                    else if (plr->getAmount() < house.getAmount())
                        plr->Lose();
                    else
                        plr->Push();
                }
            }
        }
        
        for (plr = players.begin(); plr != players.end(); plr++)
            plr->Clear();

        house.Clear();
    }
private:
    Deck deck;
    House house;
    vector<Player> players;
};

int main()
{
    setlocale(LC_ALL, "rus");
    //Card* C = new Card;
    //Card S;
    //C->setValue(C->NINE, C->DIAMONDS);
    ////cout << C->getValue();
    //S.Flip();
    //cout << S << endl;
    //Hand H;
    //Hand* Hs = new Hand();
    //H.Add(C);
    //cout << H.getAmount() << endl;
    //H.Add(C);
    //H.Add(C);
    //H.Add(C);
    ////C->setValue(C->KING, C->DIAMONDS);
    ////H.Clear();
    //cout << H.getAmount();
    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7)
    {
        cout << "Сколько игроков играет? (1 - 7): ";
        cin >> numPlayers;
    }

    vector<string> names;
    string name;
    for (int i = 0; i < numPlayers; ++i)
    {
        cout << "Введите имя игрока: ";
        cin >> name;
        names.push_back(name);
    }
    cout << endl;

    //the game loop
    Game aGame(names);
    char again = 'y';
    while (again != 'n' && again != 'N')
    {
        aGame.Play();
        cout << "\nВы хотите поиграть ещё? (Y/N): ";
        cin >> again;
    }
}
