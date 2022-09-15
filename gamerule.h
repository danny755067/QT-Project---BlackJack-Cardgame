#ifndef GAMERULE_H
#define GAMERULE_H

#include <QObject>

class GameRule : public QObject
{
    Q_OBJECT
public:
    explicit GameRule(QObject *parent = 0);

signals:

public slots:
};

#endif // GAMERULE_H
