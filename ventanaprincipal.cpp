#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QWidget>
#include <QFile>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include "ui_ventanaprincipal.h"
#include "ventanaprincipal.h"
#include "clasedatabase.h"
#include "crearbasedialog.h"
#include "ingresopdialog.h"
#include "ingrerecudialog.h"
#include "lecturarecursodialog.h"
#include "modificarsoportedialog.h"
#include "generadorpdfdialog.h"
#include "borrarrecursosdialog.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VentanaPrincipal)
{
    QStringList etiquetas;

    ui->setupUi(this);

    this->setFixedSize(this->size());
    etiquetas << "ID" << "Titulo" << "Genero" << "Soporte" << "Cant" << "Fecha";
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(etiquetas);
    ui->tableWidget->setColumnWidth(0,50);
    ui->tableWidget->setColumnWidth(1,528);
    ui->tableWidget->setColumnWidth(2,130);
    ui->tableWidget->setColumnWidth(3,120);
    ui->tableWidget->setColumnWidth(4,50);
    ui->tableWidget->addActions({ui->actionIngresar,ui->actionModificar,ui->actionBorrar});
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setEnabled(false);
    ui->menuSoportes->setEnabled(false);
    ui->menuRecursos->setEnabled(false);
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}

void VentanaPrincipal::on_actionSalir_triggered()
{
    mBase.cerrarConexion();
    close();
}

void VentanaPrincipal::on_actionCrear_Cat_logo_triggered()
{
   QString guardar;
   QString mensaje;
   QString texto;
   QString resultado;
   QSqlQuery consultar;

   guardar = QFileDialog::getSaveFileName(this,tr("Guardar base de datos"),tr(""),tr("archivo sqlite (*.sqlite)"));

   if (guardar.isEmpty() || guardar.isNull()){
      return;
   }
   mBase.cerrarConexion();
   mBase.datosConexion(guardar);
   if (!mBase.abreConexion(mensaje)){
       QMessageBox::critical(this, tr("Error creando archivo"), mensaje);
       return;
   }
   consultar.clear();
   texto = "CREATE TABLE IF NOT EXISTS soporte (id INTEGER PRIMARY KEY AUTOINCREMENT, tipo VARCHAR(20), ubicacion VARCHAR(40),"
           " titulo VARCHAR(100), cantidad INTEGER, genero VARCHAR(20), fecha DATE, sinopsis TEXT, caratext VARCHAR(35), caratula MEDIUMBLOB)";
   if (!consultar.exec(texto)){
       mensaje = consultar.lastError().text();
       resultado = "No se pudo crear Tabla soporte en SQLITE.\n" + mensaje;
       QMessageBox::critical(this, tr("Error creando Tabla"), resultado);
       return;

   }
   texto = "CREATE TABLE IF NOT EXISTS recursos (tipo VARCHAR(15), nombre VARCHAR(15))";
   if (!consultar.exec(texto)){
       mensaje = consultar.lastError().text();
       resultado = "No se pudo crear Tabla recursos en SQLITE.\n" + mensaje;
       QMessageBox::critical(this, tr("Error creando Tabla"), resultado);
       return;
   }
   QMessageBox::information(this, tr("Exito"), tr("Se ha creado la base de datos: \n") + guardar);
   ui->menuSoportes->setEnabled(true);
   ui->menuRecursos->setEnabled(true);
   ui->tableWidget->setEnabled(true);
   llenaVentanaPrincipal();
}

void VentanaPrincipal::on_actionSoporte_triggered()
{
    IngreRecuDialog miIngreso("Soporte", this);
    miIngreso.exec();
}

void VentanaPrincipal::on_actionGenero_triggered()
{
    IngreRecuDialog miIngreso("Genero", this);
    miIngreso.exec();
}

void VentanaPrincipal::on_actionUbicacion_triggered()
{
    IngreRecuDialog miIngreso("Ubicacion", this);
    miIngreso.exec();
}

void VentanaPrincipal::on_actionIngresar_Soporte_triggered()
{
    IngreSopDialog miSoporte(this);
    miSoporte.exec();
    llenaVentanaPrincipal();
}

void VentanaPrincipal::on_actionBuscar_Soporte_triggered()
{
    LecturaRecursoDialog miLectura(this);
    miLectura.exec();
}

void VentanaPrincipal::on_actionAbrir_Cat_logo_triggered()
{
    QString filename;
    QString mensaje;
    filename = QFileDialog::getOpenFileName(this,tr("Abrir base de datos"),tr(""),tr("archivo sqlite (*.sqlite *.db)"));
    if (!filename.isEmpty()){
        mBase.datosConexion(filename);
        if (!mBase.abreConexion(mensaje)){
            QMessageBox::critical(this, tr("Error de lectura de archivo"), mensaje);
            return;
        }
        llenaVentanaPrincipal();
        ui->menuSoportes->setEnabled(true);
        ui->menuRecursos->setEnabled(true);
        ui->tableWidget->setEnabled(true);
    }
}

void VentanaPrincipal::llenaVentanaPrincipal(){
    QSqlQuery llenado;
    QString auxiliar;
    int fila;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    llenado.prepare("SELECT id, titulo, genero, tipo, cantidad, fecha FROM soporte");

    if (!llenado.exec()){
       auxiliar = llenado.lastError().text();
       QMessageBox::critical(this, tr("Error de lectura tabla Soporte"), auxiliar);
       return;
    }
    ui->menuSoportes->setEnabled(true);
    ui->menuRecursos->setEnabled(true);

    while (llenado.next()){
        fila = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(fila);
        ui->tableWidget->setItem(fila, 0, new QTableWidgetItem(llenado.value(0).toString()));
        ui->tableWidget->item(fila,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(fila, 1, new QTableWidgetItem(llenado.value(1).toString()));
        ui->tableWidget->setItem(fila, 2, new QTableWidgetItem(llenado.value(2).toString()));
        ui->tableWidget->item(fila,2)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(fila, 3, new QTableWidgetItem(llenado.value(3).toString()));
        ui->tableWidget->item(fila,3)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(fila, 4, new QTableWidgetItem(llenado.value(4).toString()));
        ui->tableWidget->item(fila,4)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(fila, 5, new QTableWidgetItem(llenado.value(5).toString()));
        ui->tableWidget->item(fila,5)->setTextAlignment(Qt::AlignCenter);
    }
}

void VentanaPrincipal::on_actionModificar_Soporte_triggered()
{
    modificarSoporteDialog modiVentana(true, 0, this);
    modiVentana.exec();
    llenaVentanaPrincipal();

}

void VentanaPrincipal::on_actionBorrar_Soporte_triggered()
{
    modificarSoporteDialog modiVentana(false, 0, this);
    modiVentana.exec();
    llenaVentanaPrincipal();
}

void VentanaPrincipal::on_actionListado_filtrado_triggered()
{
    GeneradorPDFDialog miLista(this);
    miLista.exec();
}

void VentanaPrincipal::on_actionGenero_2_triggered()
{
    BorrarRecursosDialog miBorrado("Genero", this);
    miBorrado.exec();

}

void VentanaPrincipal::on_actionSoporte_2_triggered()
{
    BorrarRecursosDialog miBorrado("Soporte", this);
    miBorrado.exec();
}

void VentanaPrincipal::on_actionUbicacion_2_triggered()
{
    BorrarRecursosDialog miBorrado("Ubicacion", this);
    miBorrado.exec();
}

void VentanaPrincipal::on_actionListado_de_caratulas_triggered()
{
    QPrinter printer;
    QPainter pintar;
    QString archivo;
    QString titulo;
    QString dato;
    QString query;
    QByteArray file;
    QPixmap foto;
    QSqlQuery consulta;
    QPageSize hoja(QPageSize::AnsiA);
    QColor negro(Qt::black);
    int paginas, filaf, columnaf, lado;

    archivo = QFileDialog::getSaveFileName(this,tr("Guardar catálogo"),tr(""),tr("archivo PDF (*.pdf)"));
    if (archivo.isEmpty() || archivo.isNull()){
        return;
    }
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(archivo);
    printer.setPageSize(hoja);

    query = "SELECT titulo, caratula FROM soporte";

    if (! pintar.begin(&printer)) { // failed to open file
       QMessageBox::critical(this, tr("Error"), tr("Fallo al abrir el archivo."));
       return;
    }
    filaf = 30;
    columnaf = 25;
    paginas = lado = 1;
    consulta.exec(query);
    while (consulta.next()){
         dato = consulta.value(0).toString();
         file = consulta.value(1).toByteArray();
         if (file.size() == NULL) {
            QFile icono(":/images/No.ico");
            icono.open(QIODevice::ReadOnly);
            file = icono.readAll();
            pintar.drawText(columnaf, filaf, dato);
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
         consulta.previous();
    }
    dato = "Pagina " + QString::number(paginas);
    pintar.setPen(negro);
    pintar.drawText(370, 1005, dato);
    pintar.end();
    QMessageBox::information(this, tr("Exito"), tr("Se ha creado archivo PDF de caratulas: \n ") + archivo);
}

void VentanaPrincipal::on_actionCerrar_Cat_logo_triggered()
{
    mBase.cerrarConexion();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->menuSoportes->setEnabled(false);
    ui->menuRecursos->setEnabled(false);
}

void VentanaPrincipal::on_actionAcerca_de_triggered()
{
    CrearBaseDialog acercaDe(this);
    acercaDe.exec();
}

void VentanaPrincipal::on_actionModificar_triggered()
{
    QString aux;
    int ident;
    ident = ui->tableWidget->currentIndex().row();
    aux = ui->tableWidget->item(ident,0)->text();
    ident = aux.toInt();
    modificarSoporteDialog modiVentana(true, ident, this);
    modiVentana.exec();
    llenaVentanaPrincipal();
}

void VentanaPrincipal::on_actionBorrar_triggered()
{
    int ident;
    QString aux;
    QString preparar;
    QSqlQuery borrar;

    ident = ui->tableWidget->currentIndex().row();
    aux = ui->tableWidget->item(ident,0)->text();
    ident = aux.toInt();

    if (QMessageBox::question(this, tr("¿Desea borar elemento?"), tr("Borrar soporte con ID : ") + aux)==QMessageBox::Yes){
        preparar = "DELETE FROM soporte WHERE id = ?";
        borrar.prepare(preparar);
        borrar.addBindValue(ident);
        if (!borrar.exec()){
            aux = borrar.lastError().text();
            QMessageBox::critical(this, tr("Error"), aux);
            return;
        }
        QMessageBox::information(this, tr("Exito"), tr("El titulo ha sido eliminado."));
    }
    llenaVentanaPrincipal();
}

void VentanaPrincipal::on_actionIngresar_triggered()
{
    IngreSopDialog miSoporte(this);
    miSoporte.exec();
    llenaVentanaPrincipal();
}
