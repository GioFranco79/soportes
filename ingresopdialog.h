#ifndef INGRESOPDIALOG_H
#define INGRESOPDIALOG_H

#include <QDialog>

namespace Ui {
class IngreSopDialog;
}

class IngreSopDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IngreSopDialog(QWidget *parent = 0);
    ~IngreSopDialog();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::IngreSopDialog *ui;
};

#endif // INGRESOPDIALOG_H
