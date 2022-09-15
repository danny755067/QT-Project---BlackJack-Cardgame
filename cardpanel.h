#ifndef CARDPANEL_H
#define CARDPANEL_H
#include <QMainWindow>
#include "gameplay.h"
#include <QFile>
#include <QPaintEvent>
#include <QPainter>

// main control Class

namespace Ui {
class CardPanel;
}

class CardPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit CardPanel(QWidget *parent = 0);
    ~CardPanel();
    //use paintEvent func to paint
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event); //paint the background
    void setButtonIcon();           //load button Img
    void setIniButtonAndAction();   //setDisable true to some actions and buttons
    void setIniLabel();             //hide label when the game start
    void showPlayerBust();          //Qmsg bust information
    void setGameEndButton();        //hide game button and show next game when game over
    void setGameStartButton();      //hide next game and show game button
    void setGameStartAction();      //setDisable false to some actions
    void setQuitGame();             //quit game func
    void setDealCardImg();          //display initial two cards
    void setNameShow();             //name display setting
    void setNewGame();              //when click new,load,or next game
    const char* setCardDisplay(int icardID);//use switch case method to decide card display


private slots:

    void on_action_Q_triggered();           //Quit button

    void on_action_N_triggered();           //New game button

    void on_drawButton_clicked();           //Draw card button

    void on_action_S_triggered();           //Save file button

    void on_action_L_triggered();           //Load file button

    void on_passButton_clicked();           //Pass button

    void on_nextButton_clicked();           //Next game button

    void on_action_H_triggered();           //Historical record button

    void on_action_R_triggered();           //Rule hint button

    void on_action_C_triggered();           //Linechart button
private:
    QString filename;
    QFile file;
    Ui::CardPanel *ui;
    GamePlay *m_gameplay;
    bool m_bplayButtonClicked;
    bool m_bpassButtonClicked;
};

#endif // CARDPANEL_H
