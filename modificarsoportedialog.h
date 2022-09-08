#ifndef MODIFICARSOPORTEDIALOG_H
#define MODIFICARSOPORTEDIALOG_H

#include <QDialog>

namespace Ui {
class modificarSoporteDialog;
}

class modificarSoporteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit modificarSoporteDialog(bool modifica, int ident, QWidget *parent = 0);
    ~modificarSoporteDialog();

private slots:
    void on_botonfoto_clicked();

    void on_botonvar_clicked();

    void on_lineEdit_returnPressed();

    void on_cancelar_clicked();

private:
    Ui::modificarSoporteDialog *ui;
    void llenacombos();
};

#endif // MODIFICARSOPORTEDIALOG_H
