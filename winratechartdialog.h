#ifndef WINRATECHARTDIALOG_H
#define WINRATECHARTDIALOG_H

#include <QDialog>
#include <cardpanel.h>
#define MAXRECENTGAME 50
#define XSCALE 12
#define YSCALE 40
#define PERCENTAGE 100
#define TENPERCENT 10
namespace Ui {
class WinRateChartDialog;
}

class WinRateChartDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WinRateChartDialog(QWidget *parent = 0);
    ~WinRateChartDialog();
    void paintEvent(QPaintEvent *event);
    void saveResultTree(Node *node);
    void convertToLevelOrder(Node *node);                       //convert from tree to array in level order
    int heightOfTree(Node* node);                               //return height of tree
    void getInCurrentLevel(Node* node, int ilevel);             //save in array
    void drawTheLine(int icount, int idrawCount);               //draw the line chart

private:
    Ui::WinRateChartDialog *ui;
    double *m_pdcurrentWinRate;
    int m_igameCount;
    int m_ix1, m_ix2;
    double m_dy1,m_dy2;
    int m_idrawLineCount;
};

#endif // WINRATECHARTDIALOG_H
