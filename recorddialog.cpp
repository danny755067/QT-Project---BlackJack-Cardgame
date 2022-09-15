#include "recorddialog.h"
#include "ui_recorddialog.h"
#include "gameplay.h"
#include <QDebug>
RecordDialog::RecordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Game Record");
    m_pigameResult = new int[1000];
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);//hide ? mark on the title bar
}

RecordDialog::~RecordDialog()
{
    if(m_pigameResult != NULL){
        delete[] m_pigameResult;
        m_pigameResult = NULL;
    }
    delete ui;
}
void RecordDialog::saveRecordData(Data *data)
{
    m_igameCount = data->iresultCount;

    m_dwinCount = data->dwinCount;

    m_dloseCount = data->dloseCount;

    m_ddrawCount = data->ddrawCount;

    m_dwinRate = data->dwinRate;

}

void RecordDialog::saveResultTree(Node *root)
{
    convertToLevelOrder(root);
}



void RecordDialog::convertToLevelOrder(Node *root)
{
    m_iarraySaveCount = 0;
    int iretHeight = heightOfTree(root);
    for (int i = 1; i <= iretHeight; i++)
        getInCurrentLevel(root, i);
}

int RecordDialog::heightOfTree(Node *root)
{
    if (root == NULL)
            return 0;
        else {
            /* compute the height of each subtree */
            int ilheight = heightOfTree(root->left);
            int irheight = heightOfTree(root->right);

            /* use the larger one */
            if (ilheight > irheight) {
                return (ilheight + 1);
            }
            else {
                return (irheight + 1);
            }
    }
}

void RecordDialog::getInCurrentLevel(Node *root, int ilevel)
{
    if (root == NULL)
        return;
    if (ilevel == 1){
        m_pigameResult[m_iarraySaveCount] = root->iresultRecord;
        m_iarraySaveCount ++;
    }
    else if (ilevel > 1) {
        getInCurrentLevel(root->left, ilevel - 1);
        getInCurrentLevel(root->right, ilevel - 1);
    }
}

void RecordDialog::showRecordData()
{

    QFont font = QFont("Microsoft YaHei",20,2);
    ui->textEdit->setFont(font);
    QString strTitle = QString("Total %1 Games\n-------------------------------").arg(m_igameCount);

    ui->textEdit->append(strTitle);
    for(int i = 0; i<m_igameCount; i++){

        if(m_pigameResult[i] == Game_Result::WIN){
            printWinText(i+1);

        }
        else if(m_pigameResult[i] == Game_Result::LOSE){
            printLoseText(i+1);

        }
        else if(m_pigameResult[i] == Game_Result::DRAW){
            printDrawText(i+1);
        }
    }
    ui->textEdit->setTextColor(QColor("black"));
    int inum;
    double drate;
    if(m_igameCount != 0){
        inum = (int)(m_dwinRate*10000);
        drate = (double)inum / 100;
    }
    else
        drate = 0;
    QString strGameCount = QString("-------------------------------\nTotal Win : %1\n"
        "Total Lose : %2\n"
        "Total Draw : %3\n"
        "Total Game : %4\n"
        "Win Rate : %5%").arg(m_dwinCount).arg(m_dloseCount).arg(m_ddrawCount).arg(m_igameCount)
            .arg(drate);
    ui->textEdit->append(strGameCount);


}

void RecordDialog::printWinText(int igameNumber)
{
    ui->textEdit->setTextColor(QColor("black"));
    QString strGameResult = QString("Game %1 : ").arg(igameNumber);
    ui->textEdit->append(strGameResult);
    ui->textEdit->setTextColor(QColor("blue"));
    QString winText = QString("Win");
    ui->textEdit->insertPlainText(winText);
}

void RecordDialog::printLoseText(int igameNumber)
{
    ui->textEdit->setTextColor(QColor("black"));
    QString strGameResult = QString("Game %1 : ").arg(igameNumber);
    ui->textEdit->append(strGameResult);
    ui->textEdit->setTextColor(QColor("red"));
    QString loseText = QString("Lose");
    ui->textEdit->insertPlainText(loseText);
}

void RecordDialog::printDrawText(int igameNumber)
{
    ui->textEdit->setTextColor(QColor("black"));
    QString strGameResult = QString("Game %1 : ").arg(igameNumber);
    ui->textEdit->append(strGameResult);
    ui->textEdit->setTextColor(QColor("orange"));
    QString drawText = QString("Draw");
    ui->textEdit->insertPlainText(drawText);
}


