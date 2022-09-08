#include <QtSql/QSqlError>
#include "clasedatabase.h"

ClaseDataBase::ClaseDataBase()
{
   miDB = QSqlDatabase::addDatabase("QSQLITE");
}

void ClaseDataBase::datosConexion(const QString &nombreBase)
{
   miDB.setDatabaseName(nombreBase);   
}

bool ClaseDataBase::abreConexion(QString &mensaje)
{
  if (!miDB.open()){
      mensaje = miDB.lastError().text();
      return false;
  }
  return true;
}

void ClaseDataBase::cerrarConexion()
{
    miDB.close();
}
