#ifndef BORRARRECURSOSDIALOG_H
#define BORRARRECURSOSDIALOG_H

#include <QDialog>

namespace Ui {
class BorrarRecursosDialog;
}

class BorrarRecursosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BorrarRecursosDialog(QString recurso, QWidget *parent = 0);
    ~BorrarRecursosDialog();

private slots:
    void on_botonaceptar_clicked();

    void on_botonborrar_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::BorrarRecursosDialog *ui;
    void llenaWidgedt();
};

#endif // BORRARRECURSOSDIALOG_H
