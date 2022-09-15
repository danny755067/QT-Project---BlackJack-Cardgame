#include "shuffle.h"
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <QDebug>
#include <QMessageBox>

//this class deal with deck initial and deal cards

Shuffle::Shuffle()
{
    m_cardoutputtext = new CardOutputText;
}

Shuffle::~Shuffle()
{
    delete m_cardoutputtext;
}

void Shuffle::dealCardInfo(int icardID, int iwhosTurn)
{
    m_cardoutputtext->showDealAndDraw(icardID, iwhosTurn);
}
//it's actually open a new deck

void Shuffle::setInitialDeck()
{
    m_itotalNumberOfCards = TOTAL_CARDS;                //A deck contains TOTAL_CARDS cards
    m_idealerCardCount = 0;
    m_iplayerCardCount = 0;                             //Player has no card in the beginning
    m_icom1CardCount = 0;
    m_icom2CardCount = 0;
    m_itotalCardDrawed = 0;
    m_ileftOfCardDeck = TOTAL_CARDS;                    //Deck has full cards


    //the following loop mean's the deck is in the sorted order of {Heart_A, Heart_2, Heart_3 ...... Diamond K}
    for(int i = 0; i< m_itotalNumberOfCards; i++){
        m_iorderOfCardDeck[i]=i;
        m_idealerCardID[i] = 0;
        m_iplayerCardID[i] = 0;
        m_icom1CardID[i] = 0;
        m_icom2CardID[i] = 0;
    }

    //after setup the deck, call the function to shuffle the deck
    setShuffleDeck();
}

void Shuffle::setSecondRound()
{
    m_idealerCardCount = 0;
    m_iplayerCardCount = 0;                             //Player has no card in the beginning
    m_icom1CardCount = 0;
    m_icom2CardCount = 0;
    m_itotalCardDrawed = 0;
}

//shuffle and deal cards
void Shuffle::setShuffleDeck()
{
    //Initialize
    srand (time(NULL));

    //do the shuffles
    for (int icard_1 = 0;icard_1 < m_itotalNumberOfCards; icard_1++){
        int icard_2 = rand() % TOTAL_CARDS+1 ;
        swap(&icard_1,&icard_2);
    }
//    for (int i = 0;i < m_itotalNumberOfCards; i++){
//        qDebug() << m_iorderOfCardDeck[i];
//    }

}

void Shuffle::setDealCards()
{

    //deal two cards to complayer
    m_idealerCardID[Card_Deal::FIRSTCARD - 1] = m_iorderOfCardDeck[m_ileftOfCardDeck - Card_Deal::FIRSTCARD];
    m_idealerCardID[Card_Deal::SECONDCARD - 1] = m_iorderOfCardDeck[m_ileftOfCardDeck - Card_Deal::SECONDCARD];
    //deal two cards to complayer1 and record the number of cards in complayer1's hand
    m_icom1CardID[Card_Deal::FIRSTCARD - 1] = m_iorderOfCardDeck[m_ileftOfCardDeck - Card_Deal::THIRDCARD];
    m_icom1CardID[Card_Deal::SECONDCARD - 1] = m_iorderOfCardDeck[m_ileftOfCardDeck - Card_Deal::FOURTHCARD];
    //deal two cards to complayer2 and record the number of cards in complayer2's hand
    m_icom2CardID[Card_Deal::FIRSTCARD - 1] = m_iorderOfCardDeck[m_ileftOfCardDeck - Card_Deal::FIFTHCARD];
    m_icom2CardID[Card_Deal::SECONDCARD - 1] = m_iorderOfCardDeck[m_ileftOfCardDeck - Card_Deal::SIXTHCARD];
    //deal two cards to player and record the number of cards in player's hand
    m_iplayerCardID[Card_Deal::FIRSTCARD - 1] = m_iorderOfCardDeck[m_ileftOfCardDeck - Card_Deal::SEVENTHCARD];
    m_iplayerCardID[Card_Deal::SECONDCARD - 1] = m_iorderOfCardDeck[m_ileftOfCardDeck - Card_Deal::EIGHTTHCARD];

    m_iplayerCardCount += 2;
    m_idealerCardCount += 2;
    m_icom1CardCount += 2;
    m_icom2CardCount += 2;
    m_ileftOfCardDeck -= 8;
}

//switch two cards
void Shuffle::swap( int *picard_1, int *picard_2)
{
    int itemp = m_iorderOfCardDeck[*picard_2-1];
    m_iorderOfCardDeck[*picard_2-1]=m_iorderOfCardDeck[*picard_1];
    m_iorderOfCardDeck[*picard_1] = itemp;
}

//draw one card from the deck
int Shuffle::drawTheDeck(int iwhosTurn)
{  

    int icardID = m_iorderOfCardDeck[m_ileftOfCardDeck-1];
    if(iwhosTurn == PLAYERTURN){
        m_iplayerCardID[m_iplayerCardCount]=m_iorderOfCardDeck[m_ileftOfCardDeck-1];
        m_iplayerCardCount += 1;
        m_itotalCardDrawed += 1;
    }
    else if(iwhosTurn == COM1TURN){
        m_icom1CardID[m_icom1CardCount]=m_iorderOfCardDeck[m_ileftOfCardDeck-1];
        m_icom1CardCount += 1;
        m_itotalCardDrawed += 1;
    }
    else if(iwhosTurn == COM2TURN){
        m_icom2CardID[m_icom2CardCount]=m_iorderOfCardDeck[m_ileftOfCardDeck-1];
        m_icom2CardCount += 1;
        m_itotalCardDrawed += 1;
    }
    else if(iwhosTurn == DEALERTURN){
        m_idealerCardID[m_idealerCardCount]=m_iorderOfCardDeck[m_ileftOfCardDeck-1];
        m_idealerCardCount += 1;
        m_itotalCardDrawed += 1;
    }
    else{

    }
    m_ileftOfCardDeck -= 1;
    return icardID;
}

void Shuffle::writeShuffleDeck(int icardID, int ishuffleDeckOrder)
{
    m_iorderOfCardDeck[ishuffleDeckOrder] = icardID;
}

int Shuffle::getShuffleDeck(int ishuffleDeckOrder)
{
    return m_iorderOfCardDeck[ishuffleDeckOrder];
}

void Shuffle::writeLeftOfDeck(int ileftOfDeck)
{
    m_ileftOfCardDeck = ileftOfDeck;
}

int Shuffle::getLeftOfDeck()
{
    return m_ileftOfCardDeck;
}

int Shuffle::getTotalDraw()
{
    return m_itotalCardDrawed;
}
