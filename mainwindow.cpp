#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "main.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SendButton_clicked()
{
    QString str1 =ui->TextToSend->text();
    QByteArray ba = str1.toLatin1();
    const char *toSend = ba.data();
    fprintf(stdin ,toSend);
    ui->plainTextEdit->appendPlainText(ui->TextToSend->text());
    ui->TextToSend->clear();
}



void MainWindow::on_actionQuitter_triggered()
{
    this->close();
}

void MainWindow::on_TextToSend_returnPressed()
{
    ui->plainTextEdit->appendPlainText(ui->TextToSend->text());
    ui->TextToSend->clear();
}
