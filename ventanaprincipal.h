#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include <clasedatabase.h>

namespace Ui {
class VentanaPrincipal;
}

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget *parent = 0);
    ~VentanaPrincipal();
    ClaseDataBase mBase;


private slots:
    void on_actionSalir_triggered();

    void on_actionAbrir_Cat_logo_triggered();

    void on_actionSoporte_triggered();

    void on_actionGenero_triggered();

    void on_actionUbicacion_triggered();

    void on_actionIngresar_Soporte_triggered();

    void on_actionBuscar_Soporte_triggered();

    void on_actionModificar_Soporte_triggered();

    void on_actionBorrar_Soporte_triggered();

    void on_actionListado_filtrado_triggered();

    void on_actionGenero_2_triggered();

    void on_actionSoporte_2_triggered();

    void on_actionUbicacion_2_triggered();

    void on_actionListado_de_caratulas_triggered();

    void on_actionCerrar_Cat_logo_triggered();

    void on_actionCrear_Cat_logo_triggered();

    void on_actionAcerca_de_triggered();

    void on_actionModificar_triggered();

    void on_actionBorrar_triggered();

    void on_actionIngresar_triggered();

private:
    Ui::VentanaPrincipal *ui;
    void llenaVentanaPrincipal();
};

#endif // VENTANAPRINCIPAL_H
