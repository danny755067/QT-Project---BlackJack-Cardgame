#include "winratechartdialog.h"
#include "ui_winratechartdialog.h"
#include <QPainter>
#include <QString>
#include <QDebug>
#include <string>
WinRateChartDialog::WinRateChartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WinRateChartDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Winrate Linechart");
    m_pdcurrentWinRate = new double[1000];
    m_igameCount = 0;
    m_idrawLineCount = 0;
}

WinRateChartDialog::~WinRateChartDialog()
{
    delete ui;
    if(m_pdcurrentWinRate != NULL){
        delete[] m_pdcurrentWinRate;
        m_pdcurrentWinRate = NULL;
    }
}

void WinRateChartDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //origin point text
    painter.drawText(30,460,"(0,0)");
    //draw x-ray
    painter.drawLine(60,450,670,450);
    //cut x-ray
    for(int i = 1; i <= 10; i++){
        painter.drawLine(60+60*i,440,60+60*i,450);
        painter.drawText(60+60*i,470,QString::number(5*i));
    }
    painter.drawText(650,490,"Recent Games");

    //draw y-ray
    painter.drawLine(60,450,60,20);
    //cut y-ray
    for(int i = 1; i <= 10; i++){
        painter.drawLine(60,10+40*i,70,10+40*i);
        painter.drawText(30,10+40*i,QString::number(110 - 10*i));
    }
    painter.drawText(40,15,"WinRate(%)");

    painter.setPen(Qt::blue);


    //if total games > 50 , draw recent 50 games
    if(m_igameCount > MAXRECENTGAME && m_igameCount> 0){
        painter.drawLine(60,450,72,450 - YSCALE*
                         (m_pdcurrentWinRate[m_igameCount - MAXRECENTGAME]*PERCENTAGE/TENPERCENT));
        for(int i = m_igameCount - MAXRECENTGAME + 1; i < m_igameCount; i++){
            drawTheLine(i,m_idrawLineCount);
            painter.drawLine(m_ix1,m_dy1,m_ix2,m_dy2);
        }
    }

    //else draw recent games
    else if(m_igameCount <= MAXRECENTGAME && m_igameCount> 0){
        painter.drawLine(60,450,72,450 - YSCALE*
                         (m_pdcurrentWinRate[0]*PERCENTAGE/TENPERCENT));

        for(int i = 1; i < m_igameCount; i++){
            drawTheLine(i,m_idrawLineCount);
            painter.drawLine(m_ix1,m_dy1,m_ix2,m_dy2);
        }
    }
    int inum;
    double drate;
    //set to two decimal places
    if(m_igameCount != 0){
        inum = (int)(m_pdcurrentWinRate[m_igameCount-1]*10000);
        drate = (double)inum / 100;
    }
    else
        drate = 0;
    QString strPercent = QString("%1%").arg(drate);
    //text winrate% near the last point
    painter.drawText(m_ix2 + 10,
                     450 - YSCALE*(m_pdcurrentWinRate[m_igameCount-1]*PERCENTAGE/TENPERCENT),strPercent);
}

void WinRateChartDialog::saveResultTree(Node *node)
{
    convertToLevelOrder(node);
}

void WinRateChartDialog::convertToLevelOrder(Node *node)
{
    int iretHeight = heightOfTree(node);
    for (int i = 1; i <= iretHeight; i++)
        getInCurrentLevel(node, i);
}

int WinRateChartDialog::heightOfTree(Node *node)
{
    if (node == NULL)
            return 0;
        else {
            /* compute the height of each subtree */
            int ilheight = heightOfTree(node->left);
            int irheight = heightOfTree(node->right);

            /* use the larger one */
            if (ilheight > irheight) {
                return (ilheight + 1);
            }
            else {
                return (irheight + 1);
            }
    }
}

void WinRateChartDialog::getInCurrentLevel(Node *node, int ilevel)
{
    if (node == NULL)
        return;
    if (ilevel == 1){
        m_pdcurrentWinRate[m_igameCount] = node->dcurrentWinRate;
        m_igameCount ++;
    }
    else if (ilevel > 1) {
        getInCurrentLevel(node->left, ilevel - 1);
        getInCurrentLevel(node->right, ilevel - 1);
    }
}

void WinRateChartDialog::drawTheLine(int icount, int idrawCount)
{

    m_ix1 =  60 + XSCALE*(idrawCount + 1);
    m_dy1 = 450 - YSCALE*(m_pdcurrentWinRate[icount-1]*PERCENTAGE/TENPERCENT);
    m_ix2 =  60 + XSCALE*(idrawCount + 2);
    m_dy2 = 450 - YSCALE*(m_pdcurrentWinRate[icount]*PERCENTAGE/TENPERCENT);
    m_idrawLineCount += 1;
}


