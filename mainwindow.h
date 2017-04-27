#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_SendButton_clicked();

    void on_actionQuitter_triggered();

    void on_TextToSend_returnPressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
