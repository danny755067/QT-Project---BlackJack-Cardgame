#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "shuffle.h"
#include "gamerecord.h"

#define TOTAL_CARDS 52
#define ACE_DEFAULT_VALUE 11
#define MAXTOTALVALUE 21
#define DEALERMINVALUE 17
#define MAXDRAW 5
#define NPCPREFERVALUE 19

enum Hand_Type{
    BUST,
    NONBUST,
    FIVECARDCHARLIE,//draw five cards and not bust
    BLACKJACK
};

enum Game_ID{
    PLAYER,
    COM1,
    COM2,
    DEALER,
    NONE
};

//Rules,playing and win/lose condition is setting here
class GamePlay
{
public:
    GamePlay();
    ~GamePlay();
    void setIniGame();                              //initialize game data
    void callIniDeck();                             //call func to shuffle and set all order of card's value
    void callSetSecondRound();                      //second round also need to ini some variables
    void callSetDeal();                             //call function to deal cards
    void callIniData();                             //initialize struct Data when "New" is pressed
    void setPlayerDraw();                           //when player clicked draw button
    void callInitialDeckAndShuffle();               //call the deck initialization func in class Shuffle
    void setCardValue();                            //use array cardValue[] to initialize each cards' value.

    void saveResultTree(Node *root);                //convert from tree to array then save
    void convertToLevelOrder(Node *root);           //travel tree by level-order
    void getInCurrentLevel(Node* root, int ilevel); //pass data to array in current level
    int heightOfTree(Node* root);                   //count height of tree

    void saveData(bool bpassButtonClicked);                        //when player clicked save button
    void loadData();                        //when player clicked load button
    void callDeleteTree();                  //call the delete tree func
    void playerTwoCardValue();              //calculate player's value in hand after dealing cards
    void com1TwoCardValue();                //calculate comp1's value in hand after dealing cards
    void com2TwoCardValue();                //calculate comp2's value in hand after dealing cards
    void dealerTwoCardValue();              //calculate dealer's value in hand after dealing cards

    void playerTotalValue(int icardID);     //calculate player's value in hand after drawing cards
    void com1TotalValue(int icardID);       //calculate com1's value in hand after drawing cards
    void com2TotalValue(int icardID);       //calculate com2's value in hand after drawing cards
    void dealerTotalValue(int icardID);     //calculate dealer's value in hand after drawing cards

    void com1Turn();                        //when changing state to com1
    void com2Turn();                        //when changing state to com2
    void dealerTurn();                      //when changing state to dealer

    bool checkEndGameSignal();              //when win/lose condition happened
    bool checkFiveCardSignal();             //when five cards have been draw,disable draw button
    void setWinner();                       //compare all player's total value and decide the winner
    Node *getResultTree();                  //pass tree
    Data *getData();                        //pass data

    bool checkFirstRound();                         //return true if is in the firstround
    void setSecondRound();                          //set firstround to false
    void callSetDelay();                            //set 500ms delay
    int getCardID(int iplayerID,int icardNumber);   //return cardID from player's hand
    int getCardCount(int iplayerID);                //return cardCount from player's hand
    bool bloadSuccess;                              //true when load a file, otherwise false.


private:
    Shuffle *m_shuffle;
    GameRecord *m_gamerecord;

    int m_iwhosTurn;                    //dicide game state
    int m_icardValue[TOTAL_CARDS];      //0~51 means the ID of the cards and it will record the value of each card .

    int m_iplayerValueCount;            //record how many cards are in player's hand right now.
    int m_icom1ValueCount;              //record how many cards are in com1's hand right now.
    int m_icom2ValueCount;              //record how many cards are in com2's hand right now.
    int m_idealerValueCount;            //record how many cards are in dealer's hand right now.

    int m_iplayerNumberOfAce;           //record how many Ace cards are in player's hand right now.
    int m_icom1NumberOfAce;             //record how many Ace cards are in com1's hand right now.
    int m_icom2NumberOfAce;             //record how many Ace cards are in com2's hand right now.
    int m_idealerNumberOfAce;           //record how many Ace cards are in dealer's hand right now.

    int m_iplayerAceEqualOne;           //record how many Ace cards are in player's hand of '1' value right now
    int m_icom1AceEqualOne;             //record how many Ace cards are in com1's hand of '1' value right now
    int m_icom2AceEqualOne;             //record how many Ace cards are in com2's hand of '1' value right now
    int m_idealerAceEqualOne;           //record how many Ace cards are in dealer's hand of '1' value right now

    int m_iarraySaveCount;              //count number from 0
    int *m_pigameResult;                //an array to save game result(win/lose/draw)
    double *m_pdcurrentWinRate;         //an array to save current winrate when a game end

    bool m_bplayerBust;                 //whether player bust or not
    bool m_bcom1Bust;                   //whether com1 bust or not
    bool m_bcom2Bust;                   //whether com2 bust or not
    bool m_bdealerBust;                 //whether dealer bust or not
    bool m_bfirstRound;                 //whether the game is first round or not
    bool m_bendGameSignal;              //whether game is over
};


#endif // GAMEPLAY_H
