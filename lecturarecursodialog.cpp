#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDate>
#include <QPixmap>
#include <QMessageBox>
#include "lecturarecursodialog.h"
#include "ui_lecturarecursodialog.h"

LecturaRecursoDialog::LecturaRecursoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LecturaRecursoDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

}

LecturaRecursoDialog::~LecturaRecursoDialog()
{
    delete ui;
}

void LecturaRecursoDialog::on_lineEdit_returnPressed()
{
   QString index;
   int numero;
   QPixmap fc;
   QSqlQuery llenado;
   QString dato;
   QString auxiliar;

   index = ui->lineEdit->text();
   numero = index.toInt();

   llenado.prepare("SELECT * FROM soporte WHERE id = ?");
   llenado.addBindValue(numero);

   if (!llenado.exec()){
       auxiliar = llenado.lastError().text();
       QMessageBox::critical(this, tr("Error"), auxiliar);
       return;
   }
   if (llenado.next()){
       ui->titulo->setText(llenado.value(3).toString());
       ui->soporte->setText(llenado.value(1).toString());
       ui->ubicacion->setText(llenado.value(2).toString());      
       ui->cantidad->setText(llenado.value(4).toString());
       ui->genero->setText(llenado.value(5).toString());
       ui->fecha->setText(llenado.value(6).toString());
       ui->sinopsis->setText(llenado.value(7).toString());
       dato = llenado.value(8).toString();
       ui->caratula->setText(dato);
       if (dato == ":/images/No.ico"){
          ui->foto->setPixmap(dato);
       }
       else {
          fc.loadFromData(llenado.value(9).toByteArray());
          ui->foto->setPixmap(fc);
       }
       return;
   }
   QMessageBox::critical(this, tr("Error"), tr("ID de soporte no existe."));
}

void LecturaRecursoDialog::on_pushButton_clicked()
{
 this->close();
}

void LecturaRecursoDialog::on_lineEdit_editingFinished()
{
    QString index;
    int numero;
    QPixmap fc;
    QSqlQuery llenado;
    QString dato;
    QString auxiliar;

    index = ui->lineEdit->text();
    numero = index.toInt();

    llenado.prepare("SELECT * FROM soporte WHERE id = ?");
    llenado.addBindValue(numero);

    if (!llenado.exec()){
        auxiliar = llenado.lastError().text();
        QMessageBox::critical(this, tr("Error"), auxiliar);
        return;
    }
    if (llenado.next()){
        ui->titulo->setText(llenado.value(3).toString());
        ui->soporte->setText(llenado.value(1).toString());
        ui->ubicacion->setText(llenado.value(2).toString());
        ui->cantidad->setText(llenado.value(4).toString());
        ui->genero->setText(llenado.value(5).toString());
        ui->fecha->setText(llenado.value(6).toString());
        ui->sinopsis->setText(llenado.value(7).toString());
        dato = llenado.value(8).toString();
        ui->caratula->setText(dato);
        if (dato == ":/images/No.ico"){
           ui->foto->setPixmap(dato);
        }
        else {
           fc.loadFromData(llenado.value(9).toByteArray());
           ui->foto->setPixmap(fc);
        }
        return;
    }
    QMessageBox::critical(this, tr("Error"), tr("ID de soporte no existe."));
}
