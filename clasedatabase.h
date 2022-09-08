#ifndef CLASEDATABASE_H
#define CLASEDATABASE_H
#include <QtSql/QSqlDatabase>


class ClaseDataBase
{
public:
    ClaseDataBase();
    void datosConexion(const QString &mombreBase);
    bool abreConexion(QString &mensaje);
    void cerrarConexion();

private:
    QSqlDatabase miDB;    
};

#endif // CLASEDATABASE_H
