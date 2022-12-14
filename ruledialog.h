#ifndef RULEDIALOG_H
#define RULEDIALOG_H
#include <QDialog>

namespace Ui {
class RuleDialog;
}

class RuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RuleDialog(QWidget *parent = 0);
    ~RuleDialog();
    void showRule();                            //show rule dialog

private:
    Ui::RuleDialog *ui;
};

#endif // RULEDIALOG_H
