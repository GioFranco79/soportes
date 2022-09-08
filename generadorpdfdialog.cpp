#include <QPrinter>
#include <QPainter>
#include <QImage>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDate>
#include <QFile>
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>
#include "generadorpdfdialog.h"
#include "ui_generadorpdfdialog.h"

GeneradorPDFDialog::GeneradorPDFDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneradorPDFDialog)
{
    QDate inicio;
    QSqlQuery llenado;
    QString dato;
    QString auxiliar;
    QStringList etiquetas;

    ui->setupUi(this);

    ui->fecha->setDate(inicio.currentDate());

    this->setFixedSize(this->size());
    etiquetas << "Titulo" << "Genero" << "Soporte" << "Fecha" << "Cantidad";
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(etiquetas);
    ui->tableWidget->setColumnWidth(0,528);
    ui->tableWidget->setColumnWidth(1,140);
    ui->tableWidget->setColumnWidth(2,81);
    ui->tableWidget->setColumnWidth(4,80);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

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

GeneradorPDFDialog::~GeneradorPDFDialog()
{
    delete ui;
}

void GeneradorPDFDialog::on_generador_clicked()
{
    QPrinter printer;
    QPainter pintar;
    QString archivo;
    QString titulo;
    QString query;
    QString dato;
    QString dato1;
    QString dato2;
    QString dato3;
    QString dato4;
    QByteArray file;
    QPixmap foto;
    QSqlQuery consulta;
    QPageSize hoja(QPageSize::AnsiA);
    QColor negro(Qt::black);
    QColor azul(Qt::blue);
    QColor gris(Qt::gray);
    QColor color;
    bool par;
    int y, paginas, fila, filaf, columnaf, lado, tipol;

    titulo = ui->archivopdf->text();
    tipol = ui->tipolistado->currentIndex();

    if (tipol < 2 ){
       archivo = QFileDialog::getSaveFileName(this,tr("Guardar catálogo"), titulo, tr("archivo PDF (*.pdf)"));

       if (archivo.isEmpty() || archivo.isNull()){
          return;
       }
       printer.setOutputFormat(QPrinter::PdfFormat);
       printer.setOutputFileName(archivo);
       printer.setPageSize(hoja);
    }

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    query = preparaconsulta();

    if (tipol < 2 && !pintar.begin(&printer)) { // failed to open file
       QMessageBox::critical(this, tr("Error"), tr("Fallo al abrir el archivo."));
       return;
    }
    y = filaf = 30;
    columnaf = 25;
    par = true;
    paginas = 1;
    lado = 1;
    consulta.exec(query);
    while (consulta.next()){
        if (tipol == 0){
           if (y == 30){
              ImprimePDFEncabezado(&pintar);
               y = y + 25;
           }
           dato = consulta.value(0).toString();
           dato1 = consulta.value(1).toString();
           dato2 = consulta.value(2).toString();
           dato3 = consulta.value(3).toString();
           dato4 = consulta.value(4).toString();
           if (par){
               color = azul;
               par = false;
           }
           else {
               color = gris;
               par = true;
           }
           ImprimePDFTexto(&pintar, y, dato, dato1, dato2, dato3, dato4, color);

           y = y + 20;
           if (consulta.next() && y > 985){
               dato = "Pagina " + QString::number(paginas);
               pintar.drawText(370, 1005, dato);
               paginas = paginas + 1;
               if (!printer.newPage()) {
                  QMessageBox::critical(this, tr("Error"), tr("Fallo al escribir archivo."));
                  return;
               }
               y = 30;
           }

        }
        if (tipol == 1){

            file = consulta.value(5).toByteArray();
            if (file.size() == NULL) {
                QFile icono(":/images/No.ico");
                icono.open(QIODevice::ReadOnly);
                file = icono.readAll();
            }
            foto.loadFromData(file);
            pintar.drawPixmap(columnaf, filaf, 180, 237, foto);

            lado = lado + 1;
            columnaf = columnaf + 185;
            if (lado > 4){
               lado = 1;
               columnaf = 25;
               filaf = filaf + 242;
            }
            if (consulta.next() && filaf > 990){
                dato = "Pagina " + QString::number(paginas);
                pintar.drawText(370, 1005, dato);
                paginas = paginas + 1;
                if (!printer.newPage()) {
                   QMessageBox::critical(this, tr("Error"), tr("Fallo al escribir archivo."));
                   return;
                }
                filaf = 30;
            }

        }
        if (tipol < 2){
           consulta.previous();
        }
        fila = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(fila);
        ui->tableWidget->setItem(fila, 0, new QTableWidgetItem(consulta.value(0).toString()));
        ui->tableWidget->setItem(fila, 1, new QTableWidgetItem(consulta.value(1).toString()));
        ui->tableWidget->item(fila,1)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(fila, 2, new QTableWidgetItem(consulta.value(2).toString()));
        ui->tableWidget->item(fila,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(fila, 3, new QTableWidgetItem(consulta.value(3).toString()));
        ui->tableWidget->item(fila,3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(fila, 4, new QTableWidgetItem(consulta.value(4).toString()));
        ui->tableWidget->item(fila,4)->setTextAlignment(Qt::AlignCenter);
    }
    if (tipol < 2){
       dato = "Pagina " + QString::number(paginas);
       pintar.setPen(negro);
       pintar.drawText(370, 1005, dato);
       pintar.end();
       QMessageBox::information(this, tr("Exito"), tr("Se ha creado archivo PDF : \n ") + archivo);
    }
}

void GeneradorPDFDialog::ImprimePDFTexto(QPainter *painter, int y, QString dato, QString dato1, QString dato2, QString dato3, QString dato4, QColor color)
{
  QColor negro(Qt::black);

  painter->setPen(1);
  painter->setPen(negro);
  painter->drawText(20, y, dato);
  painter->drawText(400, y, dato1);
  painter->drawText(530, y, dato2);
  painter->drawText(605, y, dato3);
  painter->drawText(720, y, dato4);
  painter->setPen(color);
  painter->drawLine(395,y+3,395,y-13);
  painter->drawLine(525,y+3,525,y-13);
  painter->drawLine(600,y+3,600,y-13);
  painter->drawLine(698,y+3,698,y-13);
  painter->drawLine(17,y-13,770,y-13);
  painter->drawLine(17,y+3,770,y+3);
  painter->drawLine(16,y+3,16, y-13);
  painter->drawLine(770,y+3,770,y-13);
}

void GeneradorPDFDialog::ImprimePDFEncabezado(QPainter *painter)
{
    QColor rojo(Qt::magenta);
    QColor negro(Qt::black);

    painter->setPen(1);
    painter->setPen(negro);
    painter->drawText(20,30,"Titulo");
    painter->drawText(400,30,"Genero");
    painter->drawText(530,30,"Soporte");
    painter->drawText(605,30,"Fecha");
    painter->drawText(700,30,"Cantidad");
    painter->setPen(rojo);
    painter->drawLine(17,17,770,17);
    painter->drawLine(17,33,770,33);
    painter->drawLine(17,34,770,34);
    painter->drawLine(17,35,770,35);
    painter->drawLine(16,35,16, 17);
    painter->drawLine(770,35,770,17);
}


void GeneradorPDFDialog::on_aceptar_clicked()
{
    this->close();
}

QString GeneradorPDFDialog::preparaconsulta()
{
    QString sop;
    QString tit;
    QString ubi;
    QString gen;
    QString aux;
    QString resultado;
    QString f;
    QDate fec;
    bool desde = false;
    int can, ordenar, fechas;

    fechas = ui->fechatipo->currentIndex();
    sop = ui->soporte->currentText();
    tit = ui->titulo->text();
    ubi = ui->ubicacion->currentText();
    gen = ui->genero->currentText();
    fec = ui->fecha->date();
    can = ui->cantidad->value();

    aux = "SELECT titulo, genero, tipo, fecha, cantidad, caratula FROM soporte";
    resultado = aux;

    if (sop != "Todos"){
       resultado = aux + " WHERE tipo = '" + sop + "'";
       desde = true;
    }
    aux = resultado;
    /********************************************************************************************/
    if (tit != "Todos" && !tit.isEmpty()){
       if (desde){
          resultado = aux + " and titulo LIKE '%" + tit + "%'";
       }
       else {
          resultado = aux + " WHERE titulo LIKE '%" + tit + "%'";
       }
       desde = true;
    }
    aux = resultado;
    /********************************************************************************************/
    if (gen != "Todos"){
       if (desde){
          resultado = aux + " and genero = '" + gen + "'";
       }
       else {
          resultado = aux + " WHERE genero = '" + gen + "'";
       }
       desde = true;
    }
    aux = resultado;
    /********************************************************************************************/
    if (ubi != "Todas"){
       if (desde){
          resultado = aux + " and ubicacion = '" + ubi + "'";
       }
       else {
          resultado = aux + " WHERE ubicacion = '" + ubi + "'";
       }
       desde = true;
    }
    aux = resultado;
    /*********************************************************************************************/
    if (fec != fec.currentDate()){
       f = fec.toString(Qt::ISODate);
       if (fechas == 0){
          if (desde){
             resultado = aux + " and fecha = '" + f + "'";
          }
          else {
             resultado = aux + " WHERE fecha = '" + f + "'";
          }
          desde = true;
       }
       if (fechas == 1){
           if (desde){
              resultado = aux + " and fecha > '" + f + "'";
           }
           else {
              resultado = aux + " WHERE fecha > '" + f + "'";
           }
           desde = true;
       }
       if (fechas == 2){
           if (desde){
              resultado = aux + " and fecha < '" + f + "'";
           }
           else {
              resultado = aux + " WHERE fecha < '" + f + "'";
           }
           desde = true;
       }

    }
    else{
        f = fec.toString(Qt::ISODate);
        if (fechas > 2){
            if (desde){
               resultado = aux + " and fecha = '" + f + "'";
            }
            else {
               resultado = aux + " WHERE fecha = '" + f + "'";
            }
            desde = true;
        }
    }
    aux = resultado;
    /*********************************************************************************************/
    if (can != 0){
       if (desde){
          resultado = aux + " and cantidad = " + QString::number(can);
       }
       else {
          resultado = aux + " WHERE cantidad = " + QString::number(can);
       }
       desde = true;
    }
    ordenar = ui->orden->currentIndex();
    aux = resultado;
    switch (ordenar){
       case 1 : resultado = aux + " ORDER BY titulo"; break;
       case 2 : resultado = aux + " ORDER BY genero"; break;
       case 3 : resultado = aux + " ORDER BY tipo"; break;
       case 4 : resultado = aux + " ORDER BY fecha"; break;
       case 5 : resultado = aux + " ORDER BY ubicacion"; break;
    }

    return (resultado);
}
