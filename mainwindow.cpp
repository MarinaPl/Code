#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QRegExp>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui:: MainWindow)
{
    ui->setupUi(this);

    //соединения с кнопками
    connect(ui->exit_butt, SIGNAL(clicked()), this, SLOT(exit()));
    connect(ui->browse_butt, SIGNAL(clicked()), this, SLOT(browse()));
    connect(ui->code_butt, SIGNAL(clicked()), this, SLOT(code()));
    connect(ui->decode_butt, SIGNAL(clicked()), this, SLOT(decode()));
    connect(ui->display_butt, SIGNAL(clicked()), this, SLOT(display()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//кнопка выбора файла и определяем путь текущего файла
void MainWindow :: browse()
{
    file_name_imp=QFileDialog::getOpenFileName(this, tr("Open File"),"","*.txt"); //глобальная переменная
    if(!file_name_imp.isEmpty())
    {
        QFile file(file_name_imp);
        if (!file.open(QFile::ReadOnly))
        {
            QMessageBox::critical(this,tr("Error"),tr("Sorry, could not open "));
            return;
        }
        file.close();
    }
    ui->statusBar->showMessage(file_name_imp, 5000);
}

//вывод содержимого файла в окошко - цикл, читаем построчно

void MainWindow :: display_1 (QString file_name)
{
    QFile file(file_name);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString str;
        while (!stream.atEnd())
        {
            str = stream.readLine();
            ui->textBrowser_input->append(str);  //нельзя вынести в отдельную ф-ю тк in- out- put
        }
        if(stream.status()!= QTextStream::Ok)
        {
            qDebug() << "Sorry";
        }
        file.close();
    }
}
void MainWindow :: display ()
{
    display_1 (file_name_imp);
}

 //читаем наш файл, создаем зашифрованный; результат - файл "code_res.txt" и путь file_dir

QString MainWindow :: code_ff()
{
    int rowCount = 0; //количество строк
    QString text_str; //  все строки файла в нижнем регистре
    QString key_str; //ключ внужном регистре
    QString res_str; //результат который пишем в файл (по ходу все одна строка)
    QRegExp reg("[?!.;,:\\s]"); //чтобы почистить строчку от знаков пунктуации и пробелов
   //int start = 97; //код lat а

    QFile file_cur (file_name_imp);   //открыли выбранный файл
    QTextStream in(&file_cur);     //поток чтения
    QFile file_res ("file_res_code.txt");  //создаем файл с которым работаем res как его создать СРАЗУ в нужном месте
    QTextStream out(&file_res);// куда записываем

    if (file_cur.open(QIODevice::ReadOnly) && file_res.open(QIODevice::WriteOnly))
    {
        while (!in.atEnd()) {
            in.readLine();
            rowCount++;
        }
        in.seek(0);//Go to begin of file
        //запишем все в одну строку в одном регистре без знаков пунктуации
        for (int i = 0; i < rowCount; i++)
        {
           QString str = in.readLine();
            if (i == 0)
            {
                key_str = str.toLower();
            }
            else text_str = text_str + str.toLower()+" ";
        }
        text_str = text_str.replace(reg, "");
        //сравниваем длину ключа и длину текста
        int size_key = key_str.size();
        int size_text = text_str.size();
        if ((size_key < size_text)||(size_key == size_text))
        {
            for (int i = 0; i < size_text; i++)
            {
                int j = i % size_key;
                int cur = key_str[j].unicode() - 97;
                if (text_str[i].unicode()-cur >= 97)
                {
                    res_str[i] = QChar (text_str[i].unicode() - cur);
                }
                else
                {
                    res_str[i] = QChar (123 - 97 + (text_str[i].unicode() - cur));
                }
                out << res_str[i];
            }
        }
        else
        {
            //то же самое, только обрубить ключ, до размера текста

            QString new_key;
            for (int i = 0; i < size_text; i++)
            {
                new_key[i] = key_str[i];
            }
            for (int i = 0; i < size_text; i++)
            {
                int j = i % size_text;
                int cur = new_key[j].unicode() - 97;
                if (text_str[i].unicode()-cur >= 97)
                {
                    res_str[i] = QChar (text_str[i].unicode() - cur);
                }
                else
                {
                    res_str[i] = QChar (123 - 97 + (text_str[i].unicode() - cur));
                }
                out << res_str[i];
            }
        }
//        qDebug() << rowCount;
    }


    QString file_dir = QFileInfo(file_res).fileName();

    file_cur.close();
    file_res.close();
    return file_dir;    //вернули путь к файлу, чтобы его читать
}


//читаем наш файл, создаем расшифрованный; результат - файл "decode_res.txt" и путь file_dir

 QString MainWindow :: decode_ff()
{
     int rowCount = 0; //количество строк
     QString text_str; //  все строки файла в нижнем регистре
     QString key_str; //ключ внужном регистре
     QString res_str; //результат который пишем в файл (по ходу все одна строка)
     QRegExp reg("[?!.;,:\\s]"); //чтобы почистить строчку от знаков пунктуации и пробелов

     QFile file_cur (file_name_imp);//открыли выбранный файл
     QTextStream in(&file_cur);     //поток чтения
   //  QString file_res = QFileDialog::getSaveFileName(this,"Save", QDir::currentPath(), " *.txt;; "); //полное имя нашего файла

     QFile file_res ("file_res_decode.txt");  //создаем файл с которым работаем res как его создать СРАЗУ в нужном месте
     QTextStream out(&file_res);// куда записываем

     if (file_cur.open(QIODevice::ReadOnly) && file_res.open(QIODevice::WriteOnly))
     {
         while (!in.atEnd()) {
             in.readLine();
             rowCount++;
         }
         in.seek(0);//Go to begin of file
         //запишем все в одну строку в одном регистре без знаков пунктуации
         for (int i = 0; i < rowCount; i++)
         {
            QString str = in.readLine();
             if (i == 0)
             {
                 key_str = str.toLower();
             }
             else text_str = text_str + str.toLower()+" ";
         }
         text_str = text_str.replace(reg, "");
         //сравниваем длину ключа и длину текста
         int size_key = key_str.size();
         int size_text = text_str.size();
         if ((size_key < size_text)||(size_key == size_text))
         {
             for (int i = 0; i < size_text; i++)
             {
                 int j = i % size_key;
                 int cur = key_str[j].unicode() - 97;
                 if (text_str[i].unicode()+cur <= 122)
                 {
                     res_str[i] = QChar (text_str[i].unicode() + cur);
                 }
                 else
                 {
                     res_str[i] = QChar (96 + (text_str[i].unicode() + cur)-122);
                 }
                 out << res_str[i];
             }
         }
         else
         {
             //то же самое, только обрубить ключ, до размера текста

             QString new_key;
             for (int i = 0; i < size_text; i++)
             {
                 new_key[i] = key_str[i];
             }
             for (int i = 0; i < size_text; i++)
             {
                 int j = i % size_text;
                 int cur = new_key[j].unicode() - 97;
                 if (text_str[i].unicode() + cur >= 122)
                 {
                     res_str[i] = QChar (text_str[i].unicode() + cur);
                 }
                 else
                 {
                     res_str[i] = QChar (96 + (text_str[i].unicode() + cur)-122);
                 }
                 out << res_str[i];
             }
         }
 //        qDebug() << rowCount;
     }


     QString file_dir = QFileInfo(file_res).fileName();
     file_cur.close();
     file_res.close();
     return file_dir;    //вернули путь к файлу, чтобы его читать
 }


//читаем наш файл, создаем расшифрованный; результат - файл "decode_res.txt" и путь file_dir

//кнопка шифровать - просто отображаем результат
void MainWindow :: code()
{

    QString file_name_out = code_ff();
    //QString res_name = QFileDialog::getSaveFileName(this,"Save", QDir::currentPath(), " *.txt;; "); //полное имя нашего файла


    QFile file(file_name_out);  //открываем выбранный файл
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString str;
        while (!stream.atEnd())
        {
            str = stream.readLine();
            ui->textBrowser_output->append(str);
        }
        if(stream.status()!= QTextStream::Ok)
        {
            qDebug() << "Sorry";
        }

        file.close();
    }

}


//кнопка расшифровать- просто отображаем результат
void MainWindow :: decode()
{
    QString file_name_out = decode_ff();
    QFile file(file_name_out);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString str;
        while (!stream.atEnd())
        {
            str = stream.readLine();
            ui->textBrowser_output->append(str);
        }
        if(stream.status()!= QTextStream::Ok)
        {
            qDebug() << "Sorry";
        }
        file.close();
    }
}


//ВЫХОД
void MainWindow :: exit()
{
    QApplication::quit();
}
