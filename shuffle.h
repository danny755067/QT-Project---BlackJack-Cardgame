#ifndef SHUFFLE_H
#define SHUFFLE_H
#include "cardoutputtext.h"
#include <QObject>

//Card shuffle and deal

#define TOTAL_CARDS 52
enum Game_Round{
    FIRSTROUND,
    SECONDROUND
};


//use enum to define the cards ID(0~51 = Heart_A~Diamond_K)
enum Card_ID{
    Heart_A = 0,Heart_2,Heart_3,Heart_4,
    Heart_5,Heart_6,Heart_7,Heart_8,
    Heart_9,Heart_10,Heart_J,Heart_Q,Heart_K,
    Spade_A,Spade_2,Spade_3,Spade_4,
    Spade_5,Spade_6,Spade_7,Spade_8,
    Spade_9,Spade_10,Spade_J,Spade_Q,Spade_K,
    Club_A,Club_2,Club_3,Club_4,
    Club_5,Club_6,Club_7,Club_8,
    Club_9,Club_10,Club_J,Club_Q,Club_K,
    Diamond_A,Diamond_2,Diamond_3,Diamond_4,
    Diamond_5,Diamond_6,Diamond_7,Diamond_8,
    Diamond_9,Diamond_10,Diamond_J,Diamond_Q,Diamond_K
};


class Shuffle:public QObject
{
    Q_OBJECT
public:
    Shuffle();
    ~Shuffle();
    void dealCardInfo(int icardID, int iwhosTurn);
    void setInitialDeck();                                      //used to initialize the deck
    void setSecondRound();
    void setShuffleDeck();                                      //used to "shuffle" cards
    void setDealCards();                                        //used to "deal" cards
    void swap(int *picard_1, int *picard_2);                    //used to switch two cards
    int drawTheDeck(int iwhosTurn);                             //used to draw one card from the deck
    void writeShuffleDeck(int icardID, int ishuffleDeckOrder);  //write in the deck order from load file
    int getShuffleDeck(int ishuffleDeckOrder);                  //return cardID by deck order
    friend class GamePlay;
    void writeLeftOfDeck(int ileftOfDeck);                      //write in left of deck (when load file)
    int getLeftOfDeck();                                        //return leftofdeck
    int getTotalDraw();                                         //return total draw

public slots:

signals:

private:
    CardOutputText *m_cardoutputtext;
    int m_idealerCardID[TOTAL_CARDS];           //order of cards in Dealer's hand,store the card number ,not value
    int m_iplayerCardID[TOTAL_CARDS];           //order of cards in Player's hand,store the card number
    int m_icom1CardID[TOTAL_CARDS];             //order of cards in complayer1's hand,store the card number
    int m_icom2CardID[TOTAL_CARDS];             //order of cards in complayer2's hand,store the card number
    int m_iorderOfCardDeck[TOTAL_CARDS];        //order of cards in deck ,store the card number
    int m_ileftOfCardDeck;                      //number of left cards
    int m_idealerCardCount;                     //number of cards in dealer's hand
    int m_iplayerCardCount;                     //number of cards in Player's hand
    int m_icom1CardCount;                       //number of cards in com1's hand
    int m_icom2CardCount;                       //number of cards in com2's hand
    int m_itotalNumberOfCards;                  //total cards in the game
    int m_itotalCardDrawed;
};

#endif // SHUFFLE_H
