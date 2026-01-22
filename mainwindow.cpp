#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

// ================= CONSTANTES =================
const double PRECIO_SIMPLE = 30.0;
const double PRECIO_DOBLE  = 50.0;
const double PRECIO_SUITE  = 80.0;

QString archivoReservas = "reservas.txt";

// ================= FUNCIONES AUXILIARES =================
double MainWindow::calcularTotal(QString habitacion, int noches)
{
    double precio = 0;

    if (habitacion == "Simple")
        precio = PRECIO_SIMPLE;
    else if (habitacion == "Doble")
        precio = PRECIO_DOBLE;
    else if (habitacion == "Suite")
        precio = PRECIO_SUITE;

    return precio * noches;
}
