#include "crearbasedialog.h"
#include "ui_crearbasedialog.h"
#include "clasedatabase.h"

CrearBaseDialog::CrearBaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CrearBaseDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());

}

CrearBaseDialog::~CrearBaseDialog()
{
    delete ui;
}

void CrearBaseDialog::on_pushButton_clicked()
{
    this->close();
}
