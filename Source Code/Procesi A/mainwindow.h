#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Logger(std::string a);
    void Logger(std::wstring a);

private slots:
    void on_pushButton_clicked();
    int SendToPipe(LPCTSTR lpvMessage);

public:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
