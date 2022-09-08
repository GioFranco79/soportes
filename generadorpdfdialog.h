#ifndef GENERADORPDFDIALOG_H
#define GENERADORPDFDIALOG_H

#include <QDialog>

namespace Ui {
class GeneradorPDFDialog;
}

class GeneradorPDFDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneradorPDFDialog(QWidget *parent = 0);
    ~GeneradorPDFDialog();

private slots:
    void on_generador_clicked();

    void on_aceptar_clicked();

private:
    Ui::GeneradorPDFDialog *ui;
    QString preparaconsulta();

    void ImprimePDFEncabezado(QPainter *painter);
    void ImprimePDFTexto(QPainter *painter, int y, QString dato, QString dato1, QString dato2, QString dato3, QString dato4, QColor color);
};

#endif // GENERADORPDFDIALOG_H
