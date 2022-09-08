#ifndef CREARBASEDIALOG_H
#define CREARBASEDIALOG_H

#include <QDialog>
#include "clasedatabase.h"

namespace Ui {
class CrearBaseDialog;
}

class CrearBaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CrearBaseDialog(QWidget *parent = 0);
    ~CrearBaseDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::CrearBaseDialog *ui; 
};

#endif // CREARBASEDIALOG_H
