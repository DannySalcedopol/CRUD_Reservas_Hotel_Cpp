#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHeaderView>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tablaReservas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablaReservas->verticalHeader()->setVisible(false);
    ui->tablaReservas->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tablaReservas->setAlternatingRowColors(true);
    ui->tablaReservas->setShowGrid(true);
    ui->tablaReservas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    actualizarTotal();
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::calcularTotal(QString habitacion, int noches)
{
    double precio = 0;

    if (habitacion == "Simple")
        precio = 40;
    else if (habitacion == "Doble")
        precio = 70;
    else if (habitacion == "Suite")
        precio = 120;

    return precio * noches;
}

void MainWindow::actualizarTotal()
{
    QString habitacion = ui->comboHabitacion->currentText();
    int noches = ui->spinNoches->value();
    double total = calcularTotal(habitacion, noches);
    ui->txtTotal->setText(QString::number(total, 'f', 2));
}

void MainWindow::guardarEnArchivo(QString nombre, QString cedula, QString habitacion, int noches, double total)
{
    QFile archivo("reservas.txt");
    archivo.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&archivo);
    out << nombre << ";" << cedula << ";" << habitacion << ";" << noches << ";" << total << "\n";
    archivo.close();
}

void MainWindow::on_botonRegistrar_clicked()
{
    QString nombre = ui->txtNombre->text().trimmed();
    QString cedula = ui->txtCedula->text().trimmed();
    QString habitacion = ui->comboHabitacion->currentText();
    int noches = ui->spinNoches->value();

    if (nombre.isEmpty() || cedula.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Todos los campos son obligatorios");
        return;
    }

    if (cedula.length() != 10 || !cedula.toLongLong())
    {
        QMessageBox::warning(this, "Error", "Cedula invalida");
        return;
    }

    double total = calcularTotal(habitacion, noches);

    int fila = ui->tablaReservas->rowCount();
    ui->tablaReservas->insertRow(fila);

    ui->tablaReservas->setItem(fila, 0, new QTableWidgetItem(nombre));
    ui->tablaReservas->setItem(fila, 1, new QTableWidgetItem(cedula));
    ui->tablaReservas->setItem(fila, 2, new QTableWidgetItem(habitacion));
    ui->tablaReservas->setItem(fila, 3, new QTableWidgetItem(QString::number(noches)));
    ui->tablaReservas->setItem(fila, 4, new QTableWidgetItem(QString::number(total, 'f', 2)));

    guardarEnArchivo(nombre, cedula, habitacion, noches, total);

    on_botonLimpiar_clicked();

    QMessageBox::information(this, "Correcto", "Reserva registrada");
}

void MainWindow::on_botonModificar_clicked()
{
    int fila = ui->tablaReservas->currentRow();

    if (fila < 0)
    {
        QMessageBox::warning(this, "Error", "Seleccione una reserva");
        return;
    }

    QString nombre = ui->txtNombre->text().trimmed();
    QString cedula = ui->txtCedula->text().trimmed();
    QString habitacion = ui->comboHabitacion->currentText();
    int noches = ui->spinNoches->value();

    if (nombre.isEmpty() || cedula.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Todos los campos son obligatorios");
        return;
    }

    double total = calcularTotal(habitacion, noches);

    ui->tablaReservas->item(fila, 0)->setText(nombre);
    ui->tablaReservas->item(fila, 1)->setText(cedula);
    ui->tablaReservas->item(fila, 2)->setText(habitacion);
    ui->tablaReservas->item(fila, 3)->setText(QString::number(noches));
    ui->tablaReservas->item(fila, 4)->setText(QString::number(total, 'f', 2));

    QFile archivo("reservas.txt");
    archivo.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&archivo);

    for (int i = 0; i < ui->tablaReservas->rowCount(); i++)
    {
        out << ui->tablaReservas->item(i,0)->text() << ";"
            << ui->tablaReservas->item(i,1)->text() << ";"
            << ui->tablaReservas->item(i,2)->text() << ";"
            << ui->tablaReservas->item(i,3)->text() << ";"
            << ui->tablaReservas->item(i,4)->text() << "\n";
    }

    archivo.close();

    on_botonLimpiar_clicked();

    QMessageBox::information(this, "Correcto", "Reserva modificada");
}

void MainWindow::on_botonEliminar_clicked()
{
    int fila = ui->tablaReservas->currentRow();

    if (fila < 0)
    {
        QMessageBox::warning(this, "Atencion", "Seleccione una reserva");
        return;
    }

    if (QMessageBox::question(this, "Confirmar",
        "Desea eliminar la reserva",
        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        ui->tablaReservas->removeRow(fila);

        QFile archivo("reservas.txt");
        archivo.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&archivo);

        for (int i = 0; i < ui->tablaReservas->rowCount(); i++)
        {
            out << ui->tablaReservas->item(i,0)->text() << ";"
                << ui->tablaReservas->item(i,1)->text() << ";"
                << ui->tablaReservas->item(i,2)->text() << ";"
                << ui->tablaReservas->item(i,3)->text() << ";"
                << ui->tablaReservas->item(i,4)->text() << "\n";
        }

        archivo.close();

        on_botonLimpiar_clicked();

        QMessageBox::information(this, "Correcto", "Reserva eliminada");
    }
}

void MainWindow::on_botonLimpiar_clicked()
{
    ui->txtNombre->clear();
    ui->txtCedula->clear();
    ui->comboHabitacion->setCurrentIndex(0);
    ui->spinNoches->setValue(1);
    ui->txtTotal->clear();
    ui->tablaReservas->clearSelection();
    actualizarTotal();
}

void MainWindow::on_comboHabitacion_currentIndexChanged(int)
{
    actualizarTotal();
}

void MainWindow::on_spinNoches_valueChanged(int)
{
    actualizarTotal();
}

void MainWindow::on_tablaReservas_cellClicked(int row, int)
{
    ui->txtNombre->setText(ui->tablaReservas->item(row, 0)->text());
    ui->txtCedula->setText(ui->tablaReservas->item(row, 1)->text());
    ui->comboHabitacion->setCurrentText(ui->tablaReservas->item(row, 2)->text());
    ui->spinNoches->setValue(ui->tablaReservas->item(row, 3)->text().toInt());
    ui->txtTotal->setText(ui->tablaReservas->item(row, 4)->text());
}
