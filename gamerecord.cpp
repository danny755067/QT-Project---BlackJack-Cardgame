#include "gamerecord.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <queue>
#include <iostream>
#include <QTextStream>
#include <QFile>
#include <QEventLoop>
 using std::queue;
GameRecord::GameRecord(QObject *parent) : QObject(parent)
{
    m_dataGameRecord = new Data();                        //ini struct Data in construct func
    m_root = nullptr;
    m_dataGameRecord->iresultCount = 0;
    m_dataGameRecord->dwinCount = 0;
    m_dataGameRecord->dloseCount = 0;
    m_dataGameRecord->ddrawCount = 0;
    m_dataGameRecord->dwinRate = 0;
}

GameRecord::~GameRecord()
{
    if(m_dataGameRecord != NULL){
        delete m_dataGameRecord;
        m_dataGameRecord = NULL;
    }
    if(m_root != NULL){
        delete m_root;
        m_root = NULL;
    }


}



Node *GameRecord::CreateNode(int iresultRecord, double dwinRate)
{
    Node* newNode = new Node();
        if (!newNode) {
            return NULL;
        }
        newNode->iresultRecord = iresultRecord;
        newNode->dcurrentWinRate = dwinRate;
        newNode->left = newNode->right = NULL;
        return newNode;
}

Node *GameRecord::InsertNode( int iresultRecord, double dwinRate)
{
    // If the tree is empty, assign new node address to root
    if (m_root == NULL) {
            m_root = CreateNode(iresultRecord,  dwinRate);
            return m_root;
        }
        // Else, do level order traversal until we find an empty
        // place, i.e. either left child or right child of some
        // node is pointing to NULL.
        queue<Node*> qlevelOrderTraversal;
        qlevelOrderTraversal.push(m_root);

        while (!qlevelOrderTraversal.empty()) {
            Node* temp = qlevelOrderTraversal.front();
            qlevelOrderTraversal.pop();

            if (temp->left != NULL)
                qlevelOrderTraversal.push(temp->left);
            else {
                temp->left = CreateNode(iresultRecord, dwinRate);
                return m_root;
            }

            if (temp->right != NULL)
                qlevelOrderTraversal.push(temp->right);
            else {
                temp->right = CreateNode(iresultRecord, dwinRate);
                return m_root;
            }
        }
}

Data *GameRecord::passData()
{
    return m_dataGameRecord;
}

void GameRecord::setDeleteTree(Node *node)
{

    if (node == NULL){
        m_root = nullptr;
        return;
    }
    /* first delete both subtrees */
    setDeleteTree(node->left);
    setDeleteTree(node->right);

    /* then delete the node */
    delete node;
    node = NULL;
}

void GameRecord::setDelay()
{
    QEventLoop loop;
    QTimer::singleShot(500, &loop, &QEventLoop::quit);
    loop.exec();
}



Node *GameRecord::passResultTree()
{

    if(m_root != NULL){
        return m_root;
    }
    else{
        return NULL;
    }
}



//game start initialize, set record all to 0


void GameRecord::iniData()
{

    m_dataGameRecord->iresultCount = 0;
    m_dataGameRecord->dwinCount = 0;
    m_dataGameRecord->dloseCount = 0;
    m_dataGameRecord->ddrawCount = 0;
    m_dataGameRecord->dwinRate = 0;

}

void GameRecord::iniTree()
{
    m_root = nullptr;
}

//count the winrate
double GameRecord::winRateCount()
{
    m_dataGameRecord->dwinRate = (double)m_dataGameRecord->dwinCount/(m_dataGameRecord->dwinCount+
                                m_dataGameRecord->dloseCount+m_dataGameRecord->ddrawCount);
    return m_dataGameRecord->dwinRate;
}

//record when lose the game
void GameRecord::loseRecord()
{
    m_dataGameRecord->iresultCount += 1;
    m_dataGameRecord->dloseCount += 1;
    m_root = InsertNode(Game_Result::LOSE, getWinRateCount());

    showLoseMsg();
}

//record when win the game
void GameRecord::winRecord()
{
    m_dataGameRecord->iresultCount += 1;
    m_dataGameRecord->dwinCount += 1;
    m_root = InsertNode(Game_Result::WIN, getWinRateCount());

    showWinMsg();
}

void GameRecord::drawRecord()
{
    m_dataGameRecord->iresultCount += 1;
    m_dataGameRecord->ddrawCount += 1;
    m_root = InsertNode( Game_Result::DRAW, getWinRateCount());


    showDrawMsg();
}

//can show some record info in text
void GameRecord::showRecord()
{
    qDebug()<<"Total Win : "<<m_dataGameRecord->dwinCount;
    qDebug()<<"Total Lose : " << m_dataGameRecord->dloseCount;
    qDebug()<<"Total Draw : "<< m_dataGameRecord->ddrawCount;
    qDebug()<<"Win Rate : "<< winRateCount()*100<<"%";
}

//the following are save game operations


int GameRecord::getResultCount()
{
    return m_dataGameRecord->iresultCount;
}

double GameRecord::getWinCount()
{
    return m_dataGameRecord->dwinCount;
}

double GameRecord::getLoseCount()
{
    return m_dataGameRecord->dloseCount;
}

double GameRecord::getDrawCount()
{
    return m_dataGameRecord->ddrawCount;
}

double GameRecord::getWinRateCount()
{
    m_dataGameRecord->dwinRate = (double)m_dataGameRecord->dwinCount/
            (m_dataGameRecord->dwinCount+m_dataGameRecord->dloseCount+m_dataGameRecord->ddrawCount);
    if(m_dataGameRecord->iresultCount == 0)
        return 0;
    else{
        int inum;
        inum =(int)(m_dataGameRecord->dwinRate *10000);
        m_dataGameRecord->dwinRate = (double)inum /10000;
        return m_dataGameRecord->dwinRate;
    }
}


//==========================================================

//the following are load game operations
void GameRecord::writeResultRecord(int iresult, double dwinRate)
{
    InsertNode(iresult, dwinRate);
}

void GameRecord::writeResultCount(int iresultCount)
{
    m_dataGameRecord->iresultCount = iresultCount;
}

void GameRecord::writeWinCount(double dwinCount)
{
    m_dataGameRecord->dwinCount = dwinCount;
}

void GameRecord::writeLoseCount(double dloseCount)
{
    m_dataGameRecord->dloseCount = dloseCount;
}

void GameRecord::writeDrawCount(double ddrawCount)
{
    m_dataGameRecord->ddrawCount = ddrawCount;
}

void GameRecord::writeWinRate(double dwinRate)
{
    m_dataGameRecord->dwinRate = dwinRate;
}

void GameRecord::showLoseMsg()
{
    setDelay();
    QMessageBox  msgLose;
    msgLose.setText("You Lose");
    msgLose.exec();
}


void GameRecord::showWinMsg()
{
    setDelay();
    QMessageBox  msgWin;
    msgWin.setText("You Win");
    msgWin.exec();
}

void GameRecord::showDrawMsg()
{
    setDelay();
    QMessageBox  msgDraw;
    msgDraw.setText("You Draw");
    msgDraw.exec();
}


