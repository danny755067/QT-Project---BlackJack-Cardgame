#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H
#include "gameplay.h"
#include <QDialog>

namespace Ui {
class RecordDialog;
}

class RecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordDialog(QWidget *parent = 0);
    ~RecordDialog();
    void saveRecordData(Data *data);                //get int/double data
    void saveResultTree(Node *root);                //get all result from tree
    void convertToLevelOrder(Node *root);           //convert from tree to array and save
    int heightOfTree(Node* root);                   //count height of tree
    void getInCurrentLevel(Node* root, int ilevel); //pass data to array in current level
    void showRecordData();                          //output text to label and show the win/lose record
    void printWinText(int igameNumber);             //output winText
    void printLoseText(int igameNumber);            //output loseText
    void printDrawText(int igameNumber);            //output drawText


private slots:


private:
    Ui::RecordDialog *ui;
    int m_iarraySaveCount;                          //count number from 0
    int m_igameCount;                               //record how many games are played
    int *m_pigameResult;                            //record each game's result
    double m_dwinCount;                             //record how many games are win
    double m_dloseCount;                            //record how many games are lose
    double m_ddrawCount;                            //record how many games are draw
    double m_dwinRate;                              //record winrate
};

#endif // RECORDDIALOG_H
