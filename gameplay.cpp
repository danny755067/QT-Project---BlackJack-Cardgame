#include "gameplay.h"
#include "shuffle.h"
#include "gamerecord.h"
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <QDebug>
#include <iostream>
#include <QFile>
#include <QDataStream>
#include <QFileDialog>
#include <QDialog>
#include <QSettings>
#include <QTextStream>
#include <QMessageBox>
#include <QWidget>
#include <QTranslator>
#include <QString>


//this class is for controll all the initialization and load/save/record

GamePlay::GamePlay()
{
    m_pigameResult = new int[1000];
    m_pdcurrentWinRate = new double[1000];
    m_shuffle = new Shuffle;
    m_gamerecord = new GameRecord;
}

GamePlay::~GamePlay()
{
    if(m_pigameResult != NULL){
        delete[] m_pigameResult;
        m_pigameResult = NULL;
    }
    if(m_pdcurrentWinRate != NULL){
        delete[] m_pdcurrentWinRate;
        m_pdcurrentWinRate = NULL;
    }
    delete m_shuffle;
    delete m_gamerecord;
}

//ini full deck(trigger when it is a new game or in each game's first round)
void GamePlay::callIniDeck()
{
    m_shuffle->setInitialDeck();
    for(int i = 0 ; i < m_shuffle->m_itotalNumberOfCards ; i++){
        m_icardValue[i] = 0;
    }
    setCardValue();
}
//set the bool firstround to false, means it becomes to second round
void GamePlay::callSetSecondRound()
{
    m_shuffle->setSecondRound();
}
//initialize variables
void GamePlay::setIniGame()
{
    m_iwhosTurn = 0;
    m_iplayerValueCount = 0;
    m_icom1ValueCount = 0;
    m_icom2ValueCount = 0;
    m_idealerValueCount = 0;
    m_iplayerNumberOfAce = 0;
    m_icom1NumberOfAce = 0;
    m_icom2NumberOfAce = 0;
    m_idealerNumberOfAce = 0;
    m_iplayerAceEqualOne = 0;
    m_icom1AceEqualOne = 0;
    m_icom2AceEqualOne = 0;
    m_idealerAceEqualOne = 0;
    m_bplayerBust = false;
    m_bcom1Bust = false;
    m_bcom2Bust = false;
    m_bdealerBust = false;
    m_bendGameSignal = false;
    m_bfirstRound = true;



}
//call func to deal cards and count player's value
void GamePlay::callSetDeal()
{
    m_shuffle->setDealCards();
    playerTwoCardValue();
}


//set 52 seperate cards' value
void GamePlay::setCardValue()
{
    for(int icardID = 0; icardID< m_shuffle->m_itotalNumberOfCards ; icardID++){
        if(icardID%13 == 0)
            m_icardValue[icardID] = ACE_DEFAULT_VALUE; //Ace default value
        else if((icardID % 13) > 0 && (icardID % 13) <= 8 )
            m_icardValue[icardID] = icardID%13 + 1; //bewteen 2~ 9
        else{
            m_icardValue[icardID] = 10; //over 9 value = 10
        }
    }
}
//when save func has been called ,use function to convert tree to array by level order traversal
void GamePlay::saveResultTree(Node *root)
{
    convertToLevelOrder(root);
}
//level order traversal the tree
void GamePlay::convertToLevelOrder(Node *root)
{
    m_iarraySaveCount = 0;
    int iret = heightOfTree(root);
    for (int i = 1; i <= iret; i++)
        getInCurrentLevel(root, i);
}
//record the result from tree to array
void GamePlay::getInCurrentLevel(Node *root, int ilevel)
{

    if (root == NULL)
        return;
    if (ilevel == 1){
        m_pigameResult[m_iarraySaveCount] = root->iresultRecord;
        m_pdcurrentWinRate[m_iarraySaveCount] = root->dcurrentWinRate;
        m_iarraySaveCount ++;
    }
    else if (ilevel > 1) {
        getInCurrentLevel(root->left, ilevel - 1);
        getInCurrentLevel(root->right, ilevel - 1);
    }
}

int GamePlay::heightOfTree(Node *root)
{
    if (root == NULL)
            return 0;
        else {
            /* compute the height of each subtree */
            int ilheight = heightOfTree(root->left);
            int irheight = heightOfTree(root->right);

            /* use the larger one */
            if (ilheight > irheight) {
                return (ilheight + 1);
            }
            else {
                return (irheight + 1);
            }
    }
}


//saving func

void GamePlay::saveData(bool bpassButtonClicked)
{
    //Save file to a disk
    QString saveDataName = QFileDialog::getSaveFileName(NULL,QString("Save File"),NULL,QString("Bin FIles(*.bin)"));
    if(saveDataName.isEmpty())
        return;

    QFile file(saveDataName);

    //Create a file
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text ))
        return;
    QDataStream out(&file);

    //output data in specific order

    out.setVersion(QDataStream::Qt_5_4);
    out << QString::number(checkFirstRound());
    if(bpassButtonClicked == false){
        out << QString::number(m_shuffle->getLeftOfDeck() + Card_Deal::EIGHTTHCARD +m_shuffle->getTotalDraw());
                                    //output leftofdeck before deal card
    }
    else{
        out << QString::number(m_shuffle->getLeftOfDeck());                    //output leftofdeck before deal card
    }
    out << QString::number(m_gamerecord->getResultCount()) ;                   //output gamecount
    saveResultTree(getResultTree());
    for(int icount = 0; icount < m_gamerecord->getResultCount(); icount++){
        out << QString::number(m_pigameResult[icount]);                       //output each game result
        out << QString::number(m_pdcurrentWinRate[icount]);                   //and current winrate in each game
    }
    for(int icount = 0; icount <TOTAL_CARDS;icount++){
        out << QString::number(m_shuffle->getShuffleDeck(icount));             //output order of deck

    }
    out << QString::number(m_gamerecord->getWinCount());                      //output wincount

    out << QString::number(m_gamerecord->getLoseCount());                     //output losecount

    out << QString::number(m_gamerecord->getDrawCount());                     //output drawcount

    out << QString::number(m_gamerecord->getWinRateCount());                  //output total winrate

    file.close();
}


//loading func

void GamePlay::loadData()
{

    bloadSuccess = false;
    QString loadDataName = QFileDialog::getOpenFileName(NULL,QString("Open File"),NULL,QString("Bin FIles(*.bin)"));
    if(loadDataName.isEmpty() ||loadDataName.isNull() )
        return ;
    else {
        qDebug () <<"file name:"<<loadDataName;
        QFile file(loadDataName);



        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return ;

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_4);
        m_gamerecord->setDeleteTree(m_gamerecord->passResultTree());//delete tree to rebuild

        setIniGame();
        callIniDeck();

        //load data in specific order

        QString loadIN;
        in >> loadIN;
        m_bfirstRound = loadIN.toInt();

        int ileftOfDeck;
        in >> loadIN;
        ileftOfDeck = loadIN.toInt();

        int iresultCount;
        in >> loadIN;
        iresultCount = loadIN.toInt();

        m_shuffle->writeLeftOfDeck(ileftOfDeck);                    //write in left of deck
        m_gamerecord->writeResultCount(iresultCount);               //write in game count


        for(int icount = 0; icount < iresultCount; icount++){
            int iresult  ;
            double dwinRate;                                         //write in each game result
                                                                    //and current winrate
            in >> loadIN;
            iresult = loadIN.toInt();


            in >> loadIN;
            dwinRate = loadIN.toDouble();
            m_gamerecord->writeResultRecord(iresult, dwinRate);     //write in each game result
                                                                    //and current winrate
        }
        for(int icount = 0; icount <TOTAL_CARDS;icount++){
            int icardID;
            in >> loadIN;              //output order of deck
            icardID = loadIN.toInt();
            m_shuffle->writeShuffleDeck(icardID, icount);           //write in order of deck
        }

        double dwinCount;
        in >> loadIN;
        dwinCount = loadIN.toDouble();
        m_gamerecord->writeWinCount(dwinCount);                     //write in win count

        double dloseCount;
        in >> loadIN;
        dloseCount = loadIN.toDouble();
        m_gamerecord->writeLoseCount(dloseCount);                   //write in lose count

        double ddrawCount;
        in >> loadIN;
        ddrawCount = loadIN.toDouble();
        m_gamerecord->writeDrawCount(ddrawCount);                   //write in draw count

        double dwinRate;
        in >> loadIN;
        dwinRate = loadIN.toDouble();
        m_gamerecord->writeWinRate(dwinRate);                       //write in total winrate

        file.close();
        bloadSuccess = true;
    }
}


//call func to delete full tree
void GamePlay::callDeleteTree()
{
    m_gamerecord->setDeleteTree(m_gamerecord->passResultTree());
}



//draw in player's turn
void GamePlay::setPlayerDraw()
{
    int icardID = m_shuffle->drawTheDeck(m_iwhosTurn);
    playerTotalValue(icardID);               //count total value after draw
}

//count value atfer m_shuffle and deal
void GamePlay::playerTwoCardValue()
{

    for(int i = 0; i < m_shuffle->m_iplayerCardCount; i++){
        if(m_icardValue[m_shuffle->m_iplayerCardID[i]] == ACE_DEFAULT_VALUE) //have Ace
            m_iplayerNumberOfAce += 1;
        m_iplayerValueCount += m_icardValue[m_shuffle->m_iplayerCardID[i]];
    }
    if(m_iplayerValueCount>MAXTOTALVALUE){                                   //have two Ace at start
        m_iplayerValueCount -= 10;
        m_iplayerAceEqualOne += 1;
    }

}

//count total value
void GamePlay::playerTotalValue(int icardID)
{
    if(m_icardValue[icardID] == ACE_DEFAULT_VALUE){             //draw Ace
        m_iplayerNumberOfAce += 1;
    }
    m_iplayerValueCount += m_icardValue[icardID];               //add value
    if(m_iplayerValueCount>MAXTOTALVALUE){                      //value over 21
        if(m_iplayerNumberOfAce == m_iplayerAceEqualOne){       //check if number of ACE in hand equal to
            m_bplayerBust = true;                               //number of value "1" card in hand
            m_bendGameSignal = true;                            //if true, means can's recount the total value
        }
        else if(m_iplayerNumberOfAce>0 && m_iplayerNumberOfAce>m_iplayerAceEqualOne){
            m_iplayerValueCount -= 10;                          //have Ace and not use to value as '1' yet
            m_iplayerAceEqualOne += 1;
        }
        else{
        }
    }
}

//com1 strategy,calculation same as player turn
void GamePlay::com1Turn()
{
    m_iwhosTurn = Game_State::COM1TURN;
    com1TwoCardValue();
    if(m_icom1ValueCount < DEALERMINVALUE && m_icom1NumberOfAce == 0){
        int icardID = m_shuffle->drawTheDeck(m_iwhosTurn);
        com1TotalValue(icardID);
        com2Turn();
    }
    else if(m_icom1ValueCount >= DEALERMINVALUE && m_icom1NumberOfAce == 0){
        com2Turn();
    }
    else if(m_icom1ValueCount < NPCPREFERVALUE && m_icom1NumberOfAce > 0){
        int icardID = m_shuffle->drawTheDeck(m_iwhosTurn);
        com1TotalValue(icardID);
        com2Turn();
    }
    else if(m_icom1ValueCount >= NPCPREFERVALUE && m_icom1NumberOfAce > 0){
        com2Turn();
    }
}

//com1 count value after m_shuffle and deal
void GamePlay::com1TwoCardValue()
{

    for(int i = 0; i < m_shuffle->m_icom1CardCount; i++){
        if(m_icardValue[m_shuffle->m_icom1CardID[i]] == ACE_DEFAULT_VALUE)
            m_icom1NumberOfAce += 1;
        m_icom1ValueCount += m_icardValue[m_shuffle->m_icom1CardID[i]];
    }
    if(m_icom1ValueCount>MAXTOTALVALUE){
        m_icom1ValueCount -= 10;
        m_icom1AceEqualOne += 1;
    }
}

//com1 count total value
void GamePlay::com1TotalValue(int icardID)
{
    if(m_icardValue[icardID] == ACE_DEFAULT_VALUE){
        m_icom1NumberOfAce += 1;
    }
    m_icom1ValueCount += m_icardValue[icardID];

    if(m_icom1ValueCount>MAXTOTALVALUE){
        if(m_icom1NumberOfAce == m_icom1AceEqualOne){
            m_bcom1Bust = true;
        }
        else if(m_icom1NumberOfAce>0 && m_icom1NumberOfAce>m_icom1AceEqualOne){
            m_icom1ValueCount -= 10;
            m_icom1AceEqualOne += 1;
        }
        else{

        }
    }
}

//com2 strategy,calculation same as player turn
void GamePlay::com2Turn()
{

    m_iwhosTurn = Game_State::COM2TURN;
    com2TwoCardValue();
    if(m_icom2ValueCount < DEALERMINVALUE && m_icom2NumberOfAce == 0){
        int icardID = m_shuffle->drawTheDeck(m_iwhosTurn);
        com2TotalValue(icardID);
        dealerTurn();
    }
    else if(m_icom2ValueCount >= DEALERMINVALUE && m_icom2NumberOfAce == 0){
        dealerTurn();
    }
    else if(m_icom2ValueCount < NPCPREFERVALUE && m_icom2NumberOfAce > 0){
        int icardID = m_shuffle->drawTheDeck(m_iwhosTurn);
        com2TotalValue(icardID);
        dealerTurn();
    }
    else if(m_icom2ValueCount >= NPCPREFERVALUE && m_icom2NumberOfAce > 0){
        dealerTurn();
    }

}

//com2 count value after m_shuffle and deal
void GamePlay::com2TwoCardValue()
{

    for(int i = 0; i < m_shuffle->m_icom2CardCount; i++){
        if(m_icardValue[m_shuffle->m_icom2CardID[i]] == ACE_DEFAULT_VALUE)
            m_icom2NumberOfAce += 1;
        m_icom2ValueCount += m_icardValue[m_shuffle->m_icom2CardID[i]];
    }
    if(m_icom2ValueCount>MAXTOTALVALUE){
        m_icom2ValueCount -= 10;
        m_icom2AceEqualOne += 1;
    }
}

//com2 count total value
void GamePlay::com2TotalValue(int icardID)
{
    if(m_icardValue[icardID] == ACE_DEFAULT_VALUE){
        m_icom2NumberOfAce += 1;
    }
    m_icom2ValueCount += m_icardValue[icardID];
    if(m_icom2ValueCount>MAXTOTALVALUE){
        if(m_icom2NumberOfAce == m_icom2AceEqualOne){
            m_bcom2Bust = true;
        }
        else if(m_icom2NumberOfAce > 0 && m_icom2NumberOfAce > m_icom2AceEqualOne){
            m_icom2ValueCount -= 10;
            m_icom2AceEqualOne += 1;
        }
        else{

        }
    }

}

//decide who is the winner after dealer draw,calculation same as player turn
void GamePlay::dealerTurn()
{
    m_iwhosTurn = Game_State::DEALERTURN;
    dealerTwoCardValue();  //dealer must draw until value > DEALERMINVALUE by rule
    while(m_idealerValueCount < DEALERMINVALUE && m_shuffle->m_idealerCardCount < MAXDRAW){
        int icardID = m_shuffle->drawTheDeck(m_iwhosTurn);
        dealerTotalValue(icardID);
    }

    return;
}

//dealer count value after m_shuffle and deal
void GamePlay::dealerTwoCardValue()
{

    for(int i = 0; i < m_shuffle->m_idealerCardCount; i++){
        if(m_icardValue[m_shuffle->m_idealerCardID[i]] == ACE_DEFAULT_VALUE)
            m_idealerNumberOfAce += 1;
        m_idealerValueCount += m_icardValue[m_shuffle->m_idealerCardID[i]];
    }
    if(m_idealerValueCount > MAXTOTALVALUE){
        m_idealerValueCount -= 10;
        m_idealerAceEqualOne += 1;
    }

}

//dealer count value after draw
void GamePlay::dealerTotalValue(int icardID)
{
    if(m_icardValue[icardID] == ACE_DEFAULT_VALUE){
        m_idealerNumberOfAce += 1;
    }
    m_idealerValueCount += m_icardValue[icardID];
    if(m_idealerValueCount > MAXTOTALVALUE){
        if(m_idealerNumberOfAce == m_idealerAceEqualOne){
            m_bdealerBust = true;
        }
        else if(m_idealerNumberOfAce > 0 && m_idealerNumberOfAce > m_idealerAceEqualOne){
            m_idealerValueCount -= 10;
            m_idealerAceEqualOne += 1;
        }
        else{

        }
    }

}

//if game is end ,return true
bool GamePlay::checkEndGameSignal()
{
    if(m_bendGameSignal)
        return true;
    else
        return false;
}

bool GamePlay::checkFiveCardSignal()
{
    if(m_shuffle->m_iplayerCardCount == 5)
        return true;
    else
        return false;
}
//shows the winner at the end
void GamePlay::setWinner()
{
    //record 4 players' handtype
    int ihandType[TOTAL_PLAYER];
    //record 4 players' number of cards
    int icardCount[TOTAL_PLAYER] = {m_shuffle->m_iplayerCardCount, m_shuffle->m_icom1CardCount,
                                    m_shuffle->m_icom2CardCount,m_shuffle->m_idealerCardCount};
    //record 4 players' total value
    int ivalueCount[TOTAL_PLAYER] = {m_iplayerValueCount, m_icom1ValueCount, m_icom2ValueCount
                                     , m_idealerValueCount}; //record each player's total value
    //record whether 4 players are busted or not
    bool bIDBust[TOTAL_PLAYER] = {m_bplayerBust,m_bcom1Bust,m_bcom2Bust,m_bdealerBust}; //record each player's bust condition

    //define the winning rule and calcalate
    //Hand_Type::BLACKJACK is the biggest hand type,Bust is the smallest.
    for(int i = Game_ID::PLAYER; i <= Game_ID::DEALER; i++){
        if(ivalueCount[i] == MAXTOTALVALUE && icardCount[i] == 2)
            ihandType[i] = Hand_Type::BLACKJACK;
        else if(bIDBust[i] == false && icardCount[i] == MAXDRAW)
            ihandType[i] = Hand_Type::FIVECARDCHARLIE;
        else if(bIDBust[i] == false)
            ihandType[i] = Hand_Type::NONBUST;
        else if(bIDBust[i] == true)
            ihandType[i] = Hand_Type::BUST;
    }

    int ibiggestHandType = Hand_Type::BUST; //initialize the biggesthandtype ,then set.
    int ibiggestValueCount = 0;             //initialize the biggestvaluecount, then set.
    bool bsameTypeAndValueCount = false;    //default:no one has same type and value(draw condition)
    for(int i = Game_ID::PLAYER; i <= Game_ID::DEALER; i++){
        if(ihandType[i] > ibiggestHandType){
            ibiggestHandType = ihandType[i];
            ibiggestValueCount = ivalueCount[i];
            bsameTypeAndValueCount = false; //new biggest show up, so no one has same biggest hand type now, set false
        }
        else if(ihandType[i] == ibiggestHandType){
            if(ivalueCount[i] == ibiggestValueCount)
                bsameTypeAndValueCount = true;//same as biggest one ,so set same hand true
            else if(ivalueCount[i] > ibiggestValueCount){
                ibiggestValueCount = ivalueCount[i];
                bsameTypeAndValueCount = false;//new biggest show up , set false
            }
        }
    }

    //define win condition
    if(ihandType[Game_ID::PLAYER] == Hand_Type::BUST)  //player bust => lose
        m_gamerecord->loseRecord();
    //biggest hand type and biggest value and no same hand =>win
    //biggest hand type and biggest value and  same hand true =>draw
    //else =>lose
    else if(ihandType[Game_ID::PLAYER] == ibiggestHandType){
        if(ivalueCount[Game_ID::PLAYER] == ibiggestValueCount){
            if(bsameTypeAndValueCount == true)
                m_gamerecord->drawRecord();
            else
                m_gamerecord->winRecord();
        }
        else
            m_gamerecord->loseRecord();
    }
    else
        m_gamerecord->loseRecord();

}

Node *GamePlay::getResultTree()
{
    return m_gamerecord->passResultTree();
}

Data *GamePlay::getData()
{
    return m_gamerecord->passData();
}

bool GamePlay::checkFirstRound()
{
    return m_bfirstRound;
}

void GamePlay::setSecondRound()
{
    m_bfirstRound = false;
}

void GamePlay::callSetDelay()
{
    m_gamerecord->setDelay();
}



//return each card ID in player's hand
int GamePlay::getCardID(int iplayerID, int icardNumber)
{
    switch(iplayerID){
    case Game_ID::PLAYER:
        return m_shuffle->m_iplayerCardID[icardNumber];
        break;
    case Game_ID::COM1:
        return m_shuffle->m_icom1CardID[icardNumber];
        break;
    case Game_ID::COM2:
        return m_shuffle->m_icom2CardID[icardNumber];
        break;
    case Game_ID::DEALER:
        return m_shuffle->m_idealerCardID[icardNumber];
        break;
    }
}
//return each player's total card count
int GamePlay::getCardCount(int iplayerID)
{
    switch(iplayerID){
    case Game_ID::PLAYER:
        return m_shuffle->m_iplayerCardCount;
        break;
    case Game_ID::COM1:
        return m_shuffle->m_icom1CardCount;
        break;
    case Game_ID::COM2:
        return m_shuffle->m_icom2CardCount;
        break;
    case Game_ID::DEALER:
        return m_shuffle->m_idealerCardCount;
        break;
    }
}


//set data variable to 0
void GamePlay::callIniData()
{
    m_gamerecord->iniData();
}




