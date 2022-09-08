#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include "clasedatabase.h"

#include "borrarrecursosdialog.h"
#include "ui_borrarrecursosdialog.h"

QString tipoRec;

BorrarRecursosDialog::BorrarRecursosDialog(QString recurso, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BorrarRecursosDialog)
{
    QStringList etiquetas;
    tipoRec = recurso;

    ui->setupUi(this);

    this->setFixedSize(this->size());
    etiquetas << "Tipo recurso" << recurso;
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(etiquetas);
    ui->tableWidget->setColumnWidth(1,269);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->label->setText(recurso + " a borrar");
    llenaWidgedt();

}

BorrarRecursosDialog::~BorrarRecursosDialog()
{
    delete ui;
}

void BorrarRecursosDialog::on_botonaceptar_clicked()
{
    this->close();
}

void BorrarRecursosDialog::on_botonborrar_clicked()
{
    QSqlQuery ingreso;
    QString auxiliar;
    QString valor;

    valor = ui->lineEdit->text();

    ingreso.prepare("DELETE FROM recursos WHERE nombre = ? AND tipo = ?");
    ingreso.addBindValue(valor);
    ingreso.addBindValue(tipoRec);

    if (!ingreso.exec()){
       valor = ingreso.lastError().text();
       QMessageBox::critical(this, tr("Error"), valor);
       return;
    }
    llenaWidgedt();
    ui->lineEdit->setText("");
    return;
}

void BorrarRecursosDialog::llenaWidgedt()
{
    QSqlQuery llenado;
    QString dato;
    QString auxiliar;
    int fila;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    ui->label->setText(auxiliar);

    llenado.prepare("SELECT tipo, nombre FROM recursos WHERE tipo = ?");
    llenado.addBindValue(tipoRec);

    if (!llenado.exec()){
        auxiliar = llenado.lastError().text();
        QMessageBox::critical(this, tr("Error"), auxiliar);
        return;
    }
    while (llenado.next()){
        auxiliar = llenado.value(0).toString();
        dato = llenado.value(1).toString();
        fila = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(fila);
        ui->tableWidget->setItem(fila, 0, new QTableWidgetItem(auxiliar));
        ui->tableWidget->setItem(fila, 1, new QTableWidgetItem(dato));
    }
}

void BorrarRecursosDialog::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int fila;
    QString elemento;

    fila = ui->tableWidget->currentRow();
    elemento = ui->tableWidget->item(fila, 1)->text();

    ui->lineEdit->setText(elemento);

    return;
}
