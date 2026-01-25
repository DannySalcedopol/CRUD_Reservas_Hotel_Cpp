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

void MainWindow::guardarEnArchivo(QString nombre, QString cedula,
                                  QString habitacion, int noches, double total)
{
    QFile file(archivoReservas);

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo abrir el archivo");
        return;
    }

    QTextStream out(&file);
    out << nombre << ";" << cedula << ";" << habitacion << ";"
        << noches << ";" << total << "\n";

    file.close();
}

void MainWindow::cargarDesdeArchivo()
{
    QFile file(archivoReservas);

    if (!file.exists()) return;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo leer el archivo");
        return;
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString linea = in.readLine();
        QStringList datos = linea.split(";");

        if (datos.size() != 5) continue;

        int fila = ui->tablaReservas->rowCount();
        ui->tablaReservas->insertRow(fila);

        for (int i = 0; i < datos.size(); ++i) {
            ui->tablaReservas->setItem(
                fila, i, new QTableWidgetItem(datos.at(i)));
        }
    }

    file.close();
}

void MainWindow::actualizarTotal()
{
    QString habitacion = ui->comboHabitacion->currentText();
    int noches = ui->spinNoches->value();

    double total = calcularTotal(habitacion, noches);
    ui->txtTotal->setText(QString::number(total));
}

void MainWindow::limpiarFormulario()
{
    ui->txtNombre->clear();
    ui->txtCedula->clear();
    ui->comboHabitacion->setCurrentIndex(0); // Simple
    ui->spinNoches->setValue(1);
    ui->tablaReservas->clearSelection();
    actualizarTotal();
}
