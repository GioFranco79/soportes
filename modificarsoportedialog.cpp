#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QVariant>
#include <QDate>
#include "definiciones.h"
#include "modificarsoportedialog.h"
#include "ui_modificarsoportedialog.h"

QString carafile;
bool opcion;
QByteArray mifoto;
bool presionado;
QByteArray mapas;
QFile archivos;
int identidad;

modificarSoporteDialog::modificarSoporteDialog(bool modifica, int ident, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modificarSoporteDialog)
{
    opcion = modifica;
    presionado = false;
    ui->setupUi(this);

    this->setFixedSize(this->size());
    ui->lineEdit->setFocus();
    mapas = NULL;
    presionado = false;
    llenacombos();
    if (!modifica) {
        ui->botonvar->setText("Eliminar");
        ui->botonfoto->setEnabled(false);
        ui->cantidad->setEnabled(false);
        ui->caratula->setEnabled(false);
        ui->fecha->setEnabled(false);
        ui->genero->setEnabled(false);
        ui->sinopsis->setEnabled(false);
        ui->soporte->setEnabled(false);
        ui->titulo->setEnabled(false);
        ui->ubicacion->setEnabled(false);
    }
    identidad = ident;
    if (identidad){
        on_lineEdit_returnPressed();
    }

}

void modificarSoporteDialog::on_cancelar_clicked()
{
    this->close();
}

modificarSoporteDialog::~modificarSoporteDialog()
{
    delete ui;
}

void modificarSoporteDialog::on_botonvar_clicked()
{
  QSqlQuery insertar;
  QString titulo;
  QString tipo;
  QString ubicacion;
  QString genero;
  QString sinopsis;
  QDate fecha;
  QString foto;
  QString aux;
  QString preparar;
  int cantidad, numero;

  aux = ui->lineEdit->text();
  numero = aux.toInt();

  if (opcion){
     titulo = ui->titulo->text();
     tipo = ui->soporte->currentText();
     genero = ui->genero->currentText();
     ubicacion = ui->ubicacion->currentText();
     fecha = ui->fecha->date();
     cantidad = ui->cantidad->value();
     sinopsis = ui->sinopsis->toPlainText();
     foto = ui->caratula->text();

     if (!foto.isEmpty()){
         if (presionado){
             archivos.setFileName(carafile);
             archivos.open(QIODevice::ReadOnly);
             mapas = archivos.readAll();
         }
        else {
             mapas = mifoto;
        }
     }
     else {
         foto = ":/images/No.ico";
         mapas = NULL;
     }
     preparar = "UPDATE soporte SET tipo=:T1, ubicacion=:U2, titulo=:I3, cantidad=:C4, genero=:G5, fecha=:F6,"
                "sinopsis=:S7, caratext=:O8, caratula=:W9 WHERE id= :N10";
     insertar.prepare(preparar);
     insertar.bindValue(":T1",tipo);
     insertar.bindValue(":U2",ubicacion);
     insertar.bindValue(":I3",titulo);
     insertar.bindValue(":C4",cantidad);
     insertar.bindValue(":G5",genero);
     insertar.bindValue(":F6",fecha);
     insertar.bindValue(":S7",QVariant(sinopsis));
     insertar.bindValue(":O8",foto);
     insertar.bindValue(":W9",QVariant(mapas));
     insertar.bindValue(":N10",numero);

     if (!insertar.exec()){
         aux = insertar.lastError().text();
         QMessageBox::critical(this, tr("Error"), aux);
         return;
     }
     QMessageBox::information(this, tr("Exito"), tr("El titulo ha sido modificado"));
     this->close();
  }
  else {
     preparar = "DELETE FROM soporte WHERE id = ?";
     insertar.prepare(preparar);
     insertar.addBindValue(numero);
     if (!insertar.exec()){
         aux = insertar.lastError().text();
         QMessageBox::critical(this, tr("Error"), aux);
     }
     QMessageBox::information(this, tr("Exito"), tr("El titulo ha sido eliminado."));
     this->close();
  }
}

void modificarSoporteDialog::on_lineEdit_returnPressed()
{
    QString index;
    int numero;
    QPixmap fc;

    if (identidad){
       numero = identidad;
       index = QString::number(numero);
       ui->lineEdit->setText(index);
    }
    else {
       index = ui->lineEdit->text();
       numero = index.toInt();
    }

    QSqlQuery llenado;
    QString dato;
    QString auxiliar;

    llenado.prepare("SELECT * FROM soporte WHERE id = ?");
    llenado.addBindValue(numero);

    if (!llenado.exec()){
        auxiliar = llenado.lastError().text();
        QMessageBox::critical(this, tr("Error"), auxiliar);
        return;
    }
    if (llenado.next()){
        ui->titulo->setText(llenado.value(3).toString());
        ui->soporte->setCurrentText(llenado.value(1).toString());
        ui->ubicacion->setCurrentText(llenado.value(2).toString());
        numero = llenado.value(4).toInt();
        ui->cantidad->setValue(numero);
        ui->genero->setCurrentText(llenado.value(5).toString());
        ui->fecha->setDate(llenado.value(6).toDate());
        ui->sinopsis->setText(llenado.value(7).toString());
        ui->caratula->setText(llenado.value(8).toString());
        dato = llenado.value(8).toString();
        mifoto = mapas = llenado.value(9).toByteArray();
        if (dato == ":/images/No.ico"){
           ui->foto->setPixmap(dato);
        }
        else {
           fc.loadFromData(mapas);
           ui->foto->setPixmap(fc);
        }

        return;
    }
    QMessageBox::critical(this, tr("Error"), tr("ID de soporte no existe."));
}


void modificarSoporteDialog::on_botonfoto_clicked()
{
  QString muestra;

  carafile = QFileDialog::getOpenFileName(this,tr("Abrir Archivo de imagen"),"",tr("Imagenes(*.png *.jpg *.gif)"));

  if (carafile.isEmpty() || carafile.isNull()){
     carafile = ":/images/No.ico";
     presionado = false;
     return;
  }
  archivos.setFileName(carafile);
  archivos.open(QIODevice::ReadOnly);
  mapas = archivos.readAll();
  if (mapas.size()>TCARATULA){
      QMessageBox::critical(this, tr("Error"), tr("La imagen no puede ser superior a 512Kb. "));
      mapas = NULL;
      carafile = ":/images/No.ico";
      presionado = false;
      return;
  }
  ui->label_9->setText(carafile);
  ui->caratula->setText(carafile);
  ui->foto->setPixmap(carafile);
  ui->foto->setScaledContents(true);
  presionado = true;
}

void modificarSoporteDialog::llenacombos()
{
    QSqlQuery llenado;
    QString dato;
    QDate fecha;
    QString auxiliar;

    llenado.prepare("SELECT nombre FROM recursos WHERE tipo = 'Soporte'");
    if (!llenado.exec()){
        auxiliar = llenado.lastError().text();
        QMessageBox::critical(this, tr("Error"), auxiliar);
        return;
    }
    while (llenado.next()){
        dato = llenado.value(0).toString();
        ui->soporte->addItem(dato);
    }

    llenado.prepare("SELECT nombre FROM recursos WHERE tipo = 'Genero'");
    if (!llenado.exec()){
        auxiliar = llenado.lastError().text();
        QMessageBox::critical(this, tr("Error"), auxiliar);
        return;
    }
    while (llenado.next()){
        dato = llenado.value(0).toString();
        ui->genero->addItem(dato);
    }

    llenado.prepare("SELECT nombre FROM recursos WHERE tipo = 'Ubicacion'");
    if (!llenado.exec()){
        auxiliar = llenado.lastError().text();
        QMessageBox::critical(this, tr("Error"), auxiliar);
        return;
    }
    while (llenado.next()){
        dato = llenado.value(0).toString();
        ui->ubicacion->addItem(dato);
    }
}
