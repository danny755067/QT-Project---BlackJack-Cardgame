#include "gamescene.h"
#include "QPixmap"
#include "QPainter"

GameScene::GameScene(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(1200,768);
    this->setWindowTitle("BlackJack Card Game");
    this->setWindowIcon(QIcon(":/res/ICON.png")); //set icon
}

void GameScene::paintEvent(QPaintEvent *)
{
    //paint background picture by using drawPixmap ,call Qlabel and load picture can also do the same thing
    QPainter backGroundPainter(this);//point window to paint
    QPixmap pixScenes ;
    pixScenes.load(":/res/background.jpg");
    backGroundPainter.drawPixmap(0, 0, this->width(),this->height(), pixScenes);
    //x,y,w,h, add width & height can extand the picture
}

