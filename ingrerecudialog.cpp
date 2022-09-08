#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include "ingrerecudialog.h"
#include "ui_ingrerecudialog.h"
#include "clasedatabase.h"

QString tipoRecurso;

IngreRecuDialog::IngreRecuDialog(QString recurso, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IngreRecuDialog)
{
    QSqlQuery llenado;
    QString dato;
    QString auxiliar;
    QStringList etiquetas;
    int fila;
    tipoRecurso = recurso;

    ui->setupUi(this);
    this->setFixedSize(this->size());

    auxiliar = "Ingrese " + recurso;
    ui->label->setText(auxiliar);
    etiquetas << "Tipo recurso" << recurso;
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(etiquetas);
    ui->tableWidget->setColumnWidth(1,269);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    llenado.prepare("SELECT tipo, nombre FROM recursos WHERE tipo = ?");
    llenado.addBindValue(tipoRecurso);

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

IngreRecuDialog::~IngreRecuDialog()
{
    delete ui;
}

void IngreRecuDialog::on_pushButton_clicked()
{
    QSqlQuery ingreso;
    QString auxiliar;
    QString valor;
    int fila;

    valor = ui->lineEdit->text();

    ingreso.prepare("INSERT INTO recursos VALUES(?, ?)");
    ingreso.addBindValue(tipoRecurso);
    ingreso.addBindValue(valor);

    if (!ingreso.exec()){
       valor = ingreso.lastError().text();
       QMessageBox::critical(this, tr("Error"), valor);
       return;
    }
    fila = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(fila);
    ui->tableWidget->setItem(fila, 0, new QTableWidgetItem(tipoRecurso));
    ui->tableWidget->setItem(fila, 1, new QTableWidgetItem(valor));
    ui->lineEdit->setText("");
    return;
}

void IngreRecuDialog::on_pushButton_2_clicked()
{
    this->close();
}
