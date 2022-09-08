#ifndef INGRERECUDIALOG_H
#define INGRERECUDIALOG_H

#include <QDialog>
#include "clasedatabase.h"

namespace Ui {
class IngreRecuDialog;
}

class IngreRecuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IngreRecuDialog(QString recurso, QWidget *parent = 0);
    ~IngreRecuDialog();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::IngreRecuDialog *ui;
};

#endif // INGRERECUDIALOG_H
