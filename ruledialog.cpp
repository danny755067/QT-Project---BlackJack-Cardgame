#include "ruledialog.h"
#include "ui_ruledialog.h"

RuleDialog::RuleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuleDialog)
{
    ui->setupUi(this);
}

RuleDialog::~RuleDialog()
{
    delete ui;
}

void RuleDialog::showRule()
{
    ui->ruleEdit->setFixedSize(this->width(),this->height());
    ui->ruleEdit->setText("THE PACK:"
                          "\nThe standard 52-card pack is used."
                          "\n\nOBJECT OF THE GAME:"
                          "\nEach participant attempts to beat the dealer by getting a count as close to 21"
                          " as possible, without going over 21."

                          "\n\nCARD VALUES/SCORING:"
                          "\nIt is up to each individual player if an ace is worth 1 or 11. "
                          "Face cards are 10 and any other card is its pip value."
                          "\n\nBLACKJACK RULES:"
                          "\nBlackJack(2 card 21 point) > 5 Card Charlie(draw 5 cards without over 21 point)"
                          " > nonBust > Bust"
                          );
}
