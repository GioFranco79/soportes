#-------------------------------------------------
#
# Project created by QtCreator 2016-04-12T23:57:47
#
#-------------------------------------------------

QT       += core gui sql
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = Soporte
TEMPLATE = app


SOURCES += main.cpp\
        ventanaprincipal.cpp \
    ingresopdialog.cpp \
    crearbasedialog.cpp \
    clasedatabase.cpp \
    ingrerecudialog.cpp \
    lecturarecursodialog.cpp \
    modificarsoportedialog.cpp \
    generadorpdfdialog.cpp \
    borrarrecursosdialog.cpp

HEADERS  += ventanaprincipal.h \
    ingresopdialog.h \
    crearbasedialog.h \
    clasedatabase.h \
    ingrerecudialog.h \
    lecturarecursodialog.h \
    modificarsoportedialog.h \
    generadorpdfdialog.h \
    borrarrecursosdialog.h \
    definiciones.h

FORMS    += ventanaprincipal.ui \
    ingresopdialog.ui \
    crearbasedialog.ui \
    ingrerecudialog.ui \
    lecturarecursodialog.ui \
    modificarsoportedialog.ui \
    generadorpdfdialog.ui \
    borrarrecursosdialog.ui

RESOURCES += \
    imagenes.qrc \

RC_ICONS = images/soporte.ico
