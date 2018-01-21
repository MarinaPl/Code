#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QString file_name_imp;
   // QString alf = "abcdefghijklmnopqrstuvwxyz";


private slots:

    void browse();
    void code ();
    QString code_ff();
    void display_1 (QString file_name);
    void display ();
    void decode ();
    QString decode_ff();
    void exit ();



};

#endif // MAINWINDOW_H
