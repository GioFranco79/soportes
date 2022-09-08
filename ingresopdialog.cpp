#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QByteArray>
#include <QBuffer>
#include <QVariant>
#include "ingresopdialog.h"
#include "ui_ingresopdialog.h"
#include "definiciones.h"

QString caradir;
QFile archivo;
QByteArray mapa;

IngreSopDialog::IngreSopDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IngreSopDialog)
{
    QSqlQuery llenado;
    QString dato;
    QDate fecha;
    QString auxiliar;

    caradir = ":/images/No.ico";

    ui->setupUi(this);

    mapa = NULL;
    this->setFixedSize(this->size());

    ui->dateEdit->setDate(fecha.currentDate());
    ui->spinBox->setValue(1);

    llenado.prepare("SELECT nombre FROM recursos WHERE tipo = 'Soporte'");
    if (!llenado.exec()){
        auxiliar = llenado.lastError().text();
        QMessageBox::critical(this, tr("Error"), auxiliar);
        return;
    }
    while (llenado.next()){
        dato = llenado.value(0).toString();
        ui->comboBox->addItem(dato);
    }

    llenado.prepare("SELECT nombre FROM recursos WHERE tipo = 'Genero'");
    if (!llenado.exec()){
        auxiliar = llenado.lastError().text();
        QMessageBox::critical(this, tr("Error"), auxiliar);
        return;
    }
    while (llenado.next()){
        dato = llenado.value(0).toString();
        ui->comboBox_2->addItem(dato);
    }

    llenado.prepare("SELECT nombre FROM recursos WHERE tipo = 'Ubicacion'");
    if (!llenado.exec()){
        auxiliar = llenado.lastError().text();
        QMessageBox::critical(this, tr("Error"), auxiliar);
        return;
    }
    while (llenado.next()){
        dato = llenado.value(0).toString();
        ui->comboBox_3->addItem(dato);
    }
    ui->lineEdit->setFocus();
}

IngreSopDialog::~IngreSopDialog()
{
    delete ui;
}

void IngreSopDialog::on_buttonBox_accepted()
{
  QSqlQuery insertar;
  QString titulo;
  QString tipo;
  QString ubicacion;
  QString genero;
  QString sinopsis;
  QDate fecha;
  QString foto;
  int cantidad;
  QString aux;
  QString preparar;

  titulo = ui->lineEdit->text();
  tipo = ui->comboBox->currentText();
  genero = ui->comboBox_2->currentText();
  ubicacion = ui->comboBox_3->currentText(); 
  fecha = ui->dateEdit->date();
  cantidad = ui->spinBox->value();
  sinopsis = ui->textEdit->toPlainText();
  foto = caradir;
  aux = fecha.toString(Qt::ISODate);

  if (!foto.isEmpty()){
     archivo.setFileName(foto);
     archivo.open(QIODevice::ReadOnly);
     mapa = archivo.readAll();
  }
  else {
      foto = ":/images/No.ico";
      mapa = NULL;
  }
  preparar = "INSERT INTO soporte(tipo, ubicacion, titulo, cantidad, genero, fecha, sinopsis, caratext, caratula) "
             "VALUES (:T1, :U2, :I3, :C4, :G5, :F6, :S7, :O8, :W9)";
  insertar.prepare(preparar);
  insertar.bindValue(":T1",tipo);
  insertar.bindValue(":U2",ubicacion);
  insertar.bindValue(":I3",titulo);
  insertar.bindValue(":C4",cantidad);
  insertar.bindValue(":G5",genero);
  insertar.bindValue(":F6",aux);
  insertar.bindValue(":S7",QVariant(sinopsis));
  insertar.bindValue(":O8",foto);
  insertar.bindValue(":W9",QVariant(mapa));

  if (!insertar.exec()){
      aux = insertar.lastError().text();
      QMessageBox::critical(this, tr("Error"), aux);
      return;
  }

}

void IngreSopDialog::on_pushButton_clicked()
{
  caradir = QFileDialog::getOpenFileName(this,tr("Abrir Archivo de imagen"),"",tr("Imagenes(*.png *.jpg *.gif)"));
  if (caradir.isEmpty() || caradir.isNull()){
     caradir = ":/images/No.ico";
     return;
  }
  archivo.setFileName(caradir);
  archivo.open(QIODevice::ReadOnly);
  mapa = archivo.readAll();
  if (mapa.size()>TCARATULA){
      QMessageBox::critical(this, tr("Error"), tr("La imagen no puede ser superior a 512Kb. "));
      mapa = NULL;
      caradir = ":/images/No.ico";
      return;
  }
  ui->label_9->setText(caradir);
  ui->foto->setPixmap(caradir);
  ui->foto->setScaledContents(true);
}
