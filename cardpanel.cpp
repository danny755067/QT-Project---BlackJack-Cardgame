#include "cardpanel.h"
#include "ui_cardpanel.h"
#include "gameplay.h"
#include "gamerecord.h"
#include <recorddialog.h>
#include <ruledialog.h>
#include <winratechartdialog.h>
#include <stdlib.h> /* srand, rand */
#include <time.h>   /* time */
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

CardPanel::CardPanel(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CardPanel)
{
    ui->setupUi(this);
    m_gameplay = new GamePlay;                  //new a gameplay class to initialize
    m_bplayButtonClicked = false;               //used to handle closeEvent
    m_bpassButtonClicked = false;               //used to handle save data
    this->setFixedSize(1200,768);
    this->setWindowTitle("BlackJack Card Game");
    setButtonIcon();                            //set the start button icon
    setIniButtonAndAction();                    //disable and hide some button in the beginning
    setIniLabel();                              //set the card label location on table

    //when click play icon,trigger the signal
    connect(ui->startNewGameButton, &QPushButton::clicked, [=](){
        m_bplayButtonClicked = true;
        ui->startNewGameButton->hide();         //hide play icon

        m_gameplay->setIniGame();               //set variable to default value and call shuffle function
                                                //in Class Shuffle
        m_gameplay->callIniDeck();              //
        setNewGame();
    });



}

CardPanel::~CardPanel()
{
    delete m_gameplay;
    delete ui;
}

void CardPanel::closeEvent(QCloseEvent *event)
{
    if(m_bplayButtonClicked == true){
        QMessageBox  msgQuitGame(QMessageBox::Warning,"This app will be closed",
                             "Do you want to save your changes?",
                             QMessageBox::Yes|QMessageBox::No);
        int iret = msgQuitGame.exec();
        switch (iret) {
        case QMessageBox::Yes:
            // Yes was clicked
            m_gameplay->saveData(m_bpassButtonClicked);
            m_gameplay->callDeleteTree();
            QApplication::quit();
            break;
        case QMessageBox::No:
            // No was clicked
            m_gameplay->callDeleteTree();
            QApplication::quit();
            break;
        }
    }
    else
        QApplication::quit();
}


//painter
void CardPanel::paintEvent(QPaintEvent *)
{
    //paint background picture by using drawPixmap ,call Qlabel and load picture can also do the same thing
    QPainter backGroundPainter(this);//point window to paint
    QPixmap pixScenes ;
    pixScenes.load(":/res/background.jpg"); 
    backGroundPainter.drawPixmap(0, 0, this->width(),this->height(), pixScenes);
    //x,y,w,h, add width & height can extand the picture
}

void CardPanel::setButtonIcon()
{
    ui->nextButton->hide();
    ui->drawButton->hide();
    ui->passButton->hide();
    this->setWindowIcon(QIcon(":/res/ICON.png")); //set icon
    //create new game button and initial
    QPixmap pixStartIcon(":/res/starticon.png");
    QIcon startButtonIcon(pixStartIcon);
    ui->startNewGameButton->setIcon(startButtonIcon);
    ui->startNewGameButton->setIconSize(QSize(400, 200));
    ui->startNewGameButton->setFixedSize(QSize(400, 200));
    ui->startNewGameButton->setStyleSheet("border:0px");//set startNewGameButton's border to be unseen
    ui->startNewGameButton->move(this->width()*0.5 - ui->startNewGameButton->width()*0.5, 400);
}




void CardPanel::on_drawButton_clicked()
{

    m_gameplay->setPlayerDraw();
    //when player bust , turn to com1 state, when the game end ,class Gameplay will return EndGameSignal
    int icardCount = m_gameplay->getCardCount(Game_ID::PLAYER);
    int iretID =0 ;
    switch (icardCount){
    case Card_Deal::THIRDCARD:
        iretID = m_gameplay->getCardID(Game_ID::PLAYER, Card_Deal::THIRDCARD - 1);
        ui->playerCard3->setPixmap(QPixmap(setCardDisplay(iretID)));
        ui->playerCard3->show();
        break;
    case Card_Deal::FOURTHCARD:
        iretID = m_gameplay->getCardID(Game_ID::PLAYER, Card_Deal::FOURTHCARD - 1);
        ui->playerCard4->setPixmap(QPixmap(setCardDisplay(iretID)));
        ui->playerCard4->show();
        break;
    case Card_Deal::FIFTHCARD:
        iretID = m_gameplay->getCardID(Game_ID::PLAYER, Card_Deal::FIFTHCARD - 1);
        ui->playerCard5->setPixmap(QPixmap(setCardDisplay(iretID)));
        ui->playerCard5->show();
        break;
    }
    bool iretBust = m_gameplay->checkEndGameSignal();
    if(iretBust)                                            //Bust = true
        showPlayerBust();                                   //force to end game
    bool iretFiveCard = m_gameplay->checkFiveCardSignal();
    if(iretFiveCard)
        ui->drawButton->setDisabled(true);                  //five cards ,can not draw anymore
}

//when player push pass button, turn to com1 state
void CardPanel::on_passButton_clicked()
{

    m_gameplay->com1Turn();                                 //pass to com1
    setIniButtonAndAction();                                //ini button, same as game start
    int icardCount ;
    int icardID;
    //flip each player's card from back to front,reset the pixmap
    icardCount = m_gameplay->getCardID(Game_ID::COM1, Card_Deal::FIRSTCARD - 1); //array start from 0(FIRSTCARD-1)
    ui->com1Card1->setPixmap(QPixmap(setCardDisplay(icardCount)));

    icardCount = m_gameplay->getCardID(Game_ID::COM2, Card_Deal::FIRSTCARD - 1);
    ui->com2Card1->setPixmap(QPixmap(setCardDisplay(icardCount)));

    icardCount = m_gameplay->getCardID(Game_ID::DEALER, Card_Deal::FIRSTCARD - 1);
    ui->dealerCard1->setPixmap(QPixmap(setCardDisplay(icardCount)));

    //show com1's carddraw

    m_gameplay->callSetDelay();                             //delay 500 ms
    icardCount = m_gameplay->getCardCount(Game_ID::COM1);
    if(icardCount == Card_Deal::THIRDCARD){                 //if com1 has draw one card
        icardID = m_gameplay->getCardID(Game_ID::COM1, Card_Deal::THIRDCARD - 1);
        ui->com1Card3->setPixmap(QPixmap(setCardDisplay(icardID))); //set third card pixmap
        ui->com1Card3->show();
    }

    //show com2's carddraw
    m_gameplay->callSetDelay();
    icardCount = m_gameplay->getCardCount(Game_ID::COM2);
    if(icardCount == Card_Deal::THIRDCARD){
        icardID = m_gameplay->getCardID(Game_ID::COM2, Card_Deal::THIRDCARD - 1);
        ui->com2Card3->setPixmap(QPixmap(setCardDisplay(icardID)));
        ui->com2Card3->show();
    }


    //show dealer's carddraw
    icardCount = m_gameplay->getCardCount(Game_ID::DEALER);   //return how many cards dealer has
    int icardNumber = Card_Deal::THIRDCARD;                   //because FIRST and SECOND card are shown
                                                              //start from THIRD card
    while(icardNumber <= icardCount){
        switch(icardNumber){
        case Card_Deal::THIRDCARD:

            m_gameplay->callSetDelay();
            icardID = m_gameplay->getCardID(Game_ID::DEALER, Card_Deal::THIRDCARD - 1);
            ui->dealerCard3->setPixmap(QPixmap(setCardDisplay(icardID)));
            ui->dealerCard3->show();
            icardNumber++;
            break;
        case Card_Deal::FOURTHCARD:
            m_gameplay->callSetDelay();
            icardID = m_gameplay->getCardID(Game_ID::DEALER, Card_Deal::FOURTHCARD- 1);
            ui->dealerCard4->setPixmap(QPixmap(setCardDisplay(icardID)));
            ui->dealerCard4->show();
            icardNumber++;
            break;
        case Card_Deal::FIFTHCARD:

            m_gameplay->callSetDelay();
            icardID = m_gameplay->getCardID(Game_ID::DEALER, Card_Deal::FIFTHCARD - 1);
            ui->dealerCard5->setPixmap(QPixmap(setCardDisplay(icardID)));
            ui->dealerCard5->show();
            icardNumber++;
            break;
        }
    }
    //resume the button and shuffle/deal cards
    m_gameplay->setWinner();                        //decide winner
    m_gameplay->callSetDelay();                     //delay 500ms
    setGameEndButton();                             //enable/disable and hide some button when game end
    setGameStartAction();                           //enable some action
    m_bpassButtonClicked = true;
    if(m_gameplay->checkFirstRound() == true){      //go to second round if true
        m_gameplay->setIniGame();                   //ini variable and call shuffle func,same as game start
        m_gameplay->setSecondRound();
        m_gameplay->callSetSecondRound();
    }
    else{                                           //else start a new game , set in first round
        m_gameplay->setIniGame();                   //ini variable and call shuffle func,same as game start
        m_gameplay->callIniDeck();

    }
}



//新檔案settings
void CardPanel::on_action_N_triggered()
{
    m_gameplay->callDeleteTree();                   //tree's data is no longer needed
                                                    //call delete func in Class Gamerecord
    m_gameplay->callIniData();                      //call ini func ,set variables in Data to Default
    m_gameplay->setIniGame();            //ini variable and call shuffle func,same as game start
    m_gameplay->callIniDeck();
    setIniButtonAndAction();                        //disable and hide some button in the beginning
    setIniLabel();                                  //set the card label location on table
    setNewGame();                                   //set the button and set card deal for first round
}


//save button
void CardPanel::on_action_S_triggered()
{
    //Save
    m_gameplay->saveData(m_bpassButtonClicked);
}
//load button
void CardPanel::on_action_L_triggered()
{    
    //Load

    m_gameplay->loadData();

    //Success to load in data, do the same initial setting as game start
    if(m_gameplay->bloadSuccess){
        setIniButtonAndAction();                //disable and hide some button in the beginning
        setIniLabel();                          //set the card label location on table
        setNewGame();                           ////set the button and set card deal for first round

    }
}

//離開遊戲setting
void CardPanel::on_action_Q_triggered()
{

    QMessageBox  msgQuitGame(QMessageBox::Warning,"This app will be closed",
                             "Do you want to save your changes?",
                             QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    int iret = msgQuitGame.exec();
    switch (iret) {
    case QMessageBox::Yes:
        // Yes was clicked
        m_gameplay->saveData(m_bpassButtonClicked);
        m_gameplay->callDeleteTree();
        QApplication::quit();
        break;
    case QMessageBox::No:
        // No was clicked
        m_gameplay->callDeleteTree();
        QApplication::quit();
        break;
    case QMessageBox::Cancel:
        // Cancel was clicked
        break;
    default:
        // should never be reached
        break;
    }

}

//next game button
void CardPanel::on_nextButton_clicked()
{
    //do whole same thing as game start
    setIniButtonAndAction();//disable and hide some button in the beginning

    setIniLabel();          //set the card label location on table
    setNewGame();           //set the button and set card deal for first round


}
//show msg when player draw to bust
void CardPanel::showPlayerBust()
{
    QMessageBox msgPlayerBust;
    msgPlayerBust.setText("You're Bust");
    msgPlayerBust.exec();
    ui->drawButton->setDisabled(true);
}
//button setting
void CardPanel::setIniButtonAndAction()
{

    ui->drawButton->setText("Draw");
    ui->passButton->setText("Pass");
    ui->nextButton->setText("Next Game");
    ui->drawButton->setDisabled(true);
    ui->passButton->setDisabled(true);
    ui->nextButton->setDisabled(true);
    ui->drawButton->hide();
    ui->passButton->hide();
    ui->nextButton->hide();
    ui->action_N->setDisabled(true);
    ui->action_S->setDisabled(true);
    ui->action_H->setDisabled(true);
    ui->action_L->setDisabled(true);
    ui->action_C->setDisabled(true);
}
//initialize each label display (size, location ,etc....)
void CardPanel::setIniLabel()
{
    QPalette com_palette;
    com_palette.setColor(QPalette::WindowText, Qt::red);
    com_palette.setColor(QPalette::Window, Qt::white);
    ui->com1Name->hide();
    ui->com1Name->move(190,305);
    ui->com1Name->setPalette(com_palette);
    ui->com1Name->setAutoFillBackground(true);
    ui->com1Card1->hide();
    ui->com1Card1->setFixedSize(80,120);
    ui->com1Card1->move(90,130);  
    ui->com1Card2->hide();
    ui->com1Card2->setFixedSize(80,120);
    ui->com1Card2->move(90,260);
    ui->com1Card3->hide();
    ui->com1Card3->setFixedSize(80,120);
    ui->com1Card3->move(90,390);

    ui->com2Name->hide();
    ui->com2Name->move(this->width()-250,305);
    ui->com2Name->setPalette(com_palette);
    ui->com2Name->setAutoFillBackground(true);
    ui->com2Card1->hide();
    ui->com2Card1->setFixedSize(80,120);
    ui->com2Card1->move(this->width()-170,130);
    ui->com2Card2->hide();
    ui->com2Card2->setFixedSize(80,120);
    ui->com2Card2->move(this->width()-170,260);
    ui->com2Card3->hide();
    ui->com2Card3->setFixedSize(80,120);
    ui->com2Card3->move(this->width()-170,390);

    ui->dealerName->hide();
    ui->dealerName->move(this->width()*0.5 - 30,170);
    ui->dealerName->setPalette(com_palette);
    ui->dealerName->setAutoFillBackground(true);
    ui->dealerCard1->hide();
    ui->dealerCard1->setFixedSize(80,120);
    ui->dealerCard1->move(this->width()*0.5 - 220, 30);
    ui->dealerCard2->hide();
    ui->dealerCard2->setFixedSize(80,120);
    ui->dealerCard2->move(this->width()*0.5 - 130, 30);
    ui->dealerCard3->hide();
    ui->dealerCard3->setFixedSize(80,120);
    ui->dealerCard3->move(this->width()*0.5 - 40, 30);
    ui->dealerCard4->hide();
    ui->dealerCard4->setFixedSize(80,120);
    ui->dealerCard4->move(this->width()*0.5 + 50, 30);
    ui->dealerCard5->hide();
    ui->dealerCard5->setFixedSize(80,120);
    ui->dealerCard5->move(this->width()*0.5 + 140, 30);

    QPalette player_palette;
    player_palette.setColor(QPalette::WindowText, Qt::blue);
    player_palette.setColor(QPalette::Window, Qt::white);
    ui->playerName->hide();
    ui->playerName->move(this->width()*0.5 - 30,460);
    ui->playerName->setAutoFillBackground(true);
    ui->playerName->setPalette(player_palette);
    ui->playerCard1->hide();
    ui->playerCard1->setFixedSize(80,120);
    ui->playerCard1->move(690,510);
    ui->playerCard2->hide();
    ui->playerCard2->setFixedSize(80,120);
    ui->playerCard2->move(600,510);
    ui->playerCard3->hide();
    ui->playerCard3->setFixedSize(80,120);
    ui->playerCard3->move(510,510);
    ui->playerCard4->hide();
    ui->playerCard4->setFixedSize(80,120);
    ui->playerCard4->move(420,510);
    ui->playerCard5->hide();
    ui->playerCard5->setFixedSize(80,120);
    ui->playerCard5->move(330,510);

}
//button setting when player press "Pass"
void CardPanel::setGameEndButton()
{
    ui->drawButton->setDisabled(true);
    ui->passButton->setDisabled(true);
    ui->nextButton->setDisabled(false);
    ui->drawButton->hide();
    ui->passButton->hide();
    ui->nextButton->show();

}
//button setting when player press "New" "Next" "Load"
void CardPanel::setGameStartButton()
{
    ui->drawButton->setDisabled(false);
    ui->passButton->setDisabled(false);
    ui->nextButton->setDisabled(true);
    ui->drawButton->show();
    ui->passButton->show();
    ui->nextButton->hide();
}
//each round resetting action
void CardPanel::setGameStartAction()
{
    ui->action_N->setDisabled(false);
    ui->action_S->setDisabled(false);
    ui->action_H->setDisabled(false);
    ui->action_L->setDisabled(false);
    ui->action_C->setDisabled(false);
}

void CardPanel::setQuitGame()
{
    QApplication::quit();
}
//set card deal Img
void CardPanel::setDealCardImg()
{

    setNameShow();                  //show name label

    //the following are set cardback Img, in order not to show cards info to player
    m_gameplay->callSetDelay();
    ui->dealerCard1->setPixmap(QPixmap(":/res/back.png"));
    ui->dealerCard1->show();

    m_gameplay->callSetDelay();
    ui->com1Card1->setPixmap(QPixmap(":/res/back.png"));
    ui->com1Card1->show();

    m_gameplay->callSetDelay();
    ui->com2Card1->setPixmap(QPixmap(":/res/back.png"));
    ui->com2Card1->show();

    //The following are set one of players'cards to show up
    //Card_Deal - 1 because of array is order in 0~4
    m_gameplay->callSetDelay();
    int iretID = m_gameplay->getCardID(Game_ID::PLAYER, Card_Deal::FIRSTCARD - 1);
    ui->playerCard1->setPixmap(QPixmap(setCardDisplay(iretID)));
    ui->playerCard1->show();

    m_gameplay->callSetDelay();
    iretID = m_gameplay->getCardID(Game_ID::DEALER, Card_Deal::SECONDCARD - 1);
    ui->dealerCard2->setPixmap(QPixmap(setCardDisplay(iretID)));
    ui->dealerCard2->show();

    m_gameplay->callSetDelay();
    iretID = m_gameplay->getCardID(Game_ID::COM1, Card_Deal::SECONDCARD - 1);
    ui->com1Card2->setPixmap(QPixmap(setCardDisplay(iretID)));
    ui->com1Card2->show();

    m_gameplay->callSetDelay();
    iretID = m_gameplay->getCardID(Game_ID::COM2, Card_Deal::SECONDCARD - 1);
    ui->com2Card2->setPixmap(QPixmap(setCardDisplay(iretID)));
    ui->com2Card2->show();

    m_gameplay->callSetDelay();
    iretID = m_gameplay->getCardID(Game_ID::PLAYER, Card_Deal::SECONDCARD - 1);
    ui->playerCard2->setPixmap(QPixmap(setCardDisplay(iretID)));
    ui->playerCard2->show();

}

void CardPanel::setNameShow()
{
    ui->dealerName->show();;
    ui->com1Name->show();
    ui->com2Name->show();
    ui->playerName->show();
}
//when New a game or second round is over,this function will ini to the first round situation
void CardPanel::setNewGame()
{
    m_bpassButtonClicked = false;
    m_gameplay->callSetDeal();              //call deal card function in Class Shuffle
    setDealCardImg();                       //set pixmap to card label
    m_gameplay->callSetDelay();             //delay 500ms
    setGameStartButton();                   //disable/hide some button
    setGameStartAction();                   //disable/hide some action
}
//return Img location
const char *CardPanel::setCardDisplay(int icardID)
{
    const char* ccardPicture[] = {":/res/heart_1.png",":/res/heart_2.png",":/res/heart_3.png",
                                 ":/res/heart_4.png",":/res/heart_5.png",":/res/heart_6.png",
                                 ":/res/heart_7.png",":/res/heart_8.png",":/res/heart_9.png",
                                 ":/res/heart_10.png",":/res/heart_jack.png",":/res/heart_queen.png",
                                 ":/res/heart_king.png",":/res/spade_1.png",":/res/spade_2.png",
                                 ":/res/spade_3.png",":/res/spade_4.png",":/res/spade_5.png",
                                 ":/res/spade_6.png",":/res/spade_7.png",":/res/spade_8.png",
                                 ":/res/spade_9.png",":/res/spade_10.png",":/res/spade_jack.png",
                                 ":/res/spade_queen.png",":/res/spade_king.png",":/res/club_1.png",
                                 ":/res/club_2.png",":/res/club_3.png",":/res/club_4.png",
                                 ":/res/club_5.png",":/res/club_6.png",":/res/club_7.png",
                                 ":/res/club_8.png",":/res/club_9.png",":/res/club_10.png",
                                 ":/res/club_jack.png",":/res/club_queen.png",":/res/club_king.png",
                                 ":/res/diamond_1.png",":/res/diamond_2.png",":/res/diamond_3.png",
                                 ":/res/diamond_4.png",":/res/diamond_5.png",":/res/diamond_6.png",
                                 ":/res/diamond_7.png",":/res/diamond_8.png",":/res/diamond_9.png",
                                 ":/res/diamond_10.png",":/res/diamond_jack.png",":/res/diamond_queen.png",
                                 ":/res/diamond_king.png"};
    return ccardPicture[icardID];   //return string "location"

}


//hitory action is triggered
void CardPanel::on_action_H_triggered()
{
    RecordDialog *recorddialog = new RecordDialog;
    recorddialog->saveRecordData(m_gameplay->getData());      //pass data from struct to func
    recorddialog->saveResultTree(m_gameplay->getResultTree());//pass gameresult from tree to func
    recorddialog->showRecordData();
    recorddialog->exec();
    delete recorddialog;
    recorddialog = NULL;
}
//rule action is triggered
void CardPanel::on_action_R_triggered()
{
    RuleDialog *ruledialog = new RuleDialog;
    ruledialog->showRule();
    ruledialog->exec();
    delete ruledialog;
    ruledialog = NULL;

}
//chart action is triggered
void CardPanel::on_action_C_triggered()
{
    WinRateChartDialog *winratechartdialog = new WinRateChartDialog;
    winratechartdialog->saveResultTree(m_gameplay->getResultTree());//pass winrate from tree to func
    winratechartdialog->exec();
    delete winratechartdialog;
    winratechartdialog = NULL;
}
