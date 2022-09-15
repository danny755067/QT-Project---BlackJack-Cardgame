#ifndef GAMERECORD_H
#define GAMERECORD_H
#include <QObject>
#include <QFile>

enum Game_Result{
    WIN = 1,
    LOSE,
    DRAW
};
struct Data{
    int iresultCount;
    double dwinCount;
    double dloseCount;
    double ddrawCount;
    double dwinRate;
};
//this class save the each game result , and can operate save/load call
struct Node
{
    int iresultRecord;
    double dcurrentWinRate;
    Node* left, * right;
};
class GameRecord : public QObject
{
    Q_OBJECT
public:
    explicit GameRecord(QObject *parent = 0);
    ~GameRecord();
    friend class Gameplay;
    Node *passResultTree();
    Node *CreateNode(int iresultRecord, double dwinrate);       //new method,insert the member tree in
                                                                //level order when new data come in


    Node *InsertNode(int iresultRecord, double dwinrate);       //when new result come in ,add to tree
    Data *passData();                                           //return struct Data
    void setDeleteTree(Node *node);                             //delete full tree
    void setDelay();                                            //set 500ms
    void iniData();                                             //to set all date to 0
    void iniTree();                                             //to set tree pointer to NULL
    double winRateCount();                                      //to calculate winRate
    void loseRecord();                                          //to record every lose condition
    void winRecord();                                           //to record every win condition
    void drawRecord();                                          //to record evety draw condition
    void showRecord();                                          //qDebug some record info
    int getResultCount();                                       //return member resultCount
    double getWinCount();                                       //return member winCount
    double getLoseCount();                                      //return member loseCount
    double getDrawCount();                                      //return member drawCount
    double getWinRateCount();                                   //return member winRate
    void writeResultRecord(int iresult, double dwinRate);       //write in member resultRecord[]
    void writeResultCount(int iresultCount);                    //write in member resultCount
    void writeWinCount(double dwinCount);                       //write in member winCount
    void writeLoseCount(double dloseCount);                     //write in member loseCount
    void writeDrawCount(double ddrawCount);                     //write in member drawCount
    void writeWinRate(double dwinRate);                         //write in member winRate
    void showLoseMsg();                                         //winmsgbox
    void showWinMsg();                                          //losemsgbox
    void showDrawMsg();                                         //drawmsgbox


private:

    Node *m_root;
    Data *m_dataGameRecord;

signals:

public slots:

};

#endif // GAMERECORD_H
