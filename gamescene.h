#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QMainWindow>

class GameScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameScene(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);//paint the background

signals:

public slots:
};

#endif // GAMESCENE_H
