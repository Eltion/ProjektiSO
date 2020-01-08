#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>



#include <QMessageBox>

//Madhsia e buffer-it
#define BUFSIZE 512


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked(){
    QMessageBox msgBox;
    SendToPipe(ui->textEdit->toPlainText().toStdWString().c_str());
}

void MainWindow::Logger(std::string a){
    QString b = QString::fromStdString(a);
    ui->textEdit_2->append(b);
}

void MainWindow::Logger(std::wstring a){
    QString b = QString::fromStdWString(a);
    ui->textEdit_2->append(b);
}

int MainWindow::SendToPipe(LPCTSTR lpvMessage)
{

   HANDLE hPipe;
   TCHAR  chBuf[BUFSIZE];
   BOOL   fSuccess = FALSE;
   DWORD  cbRead, cbToWrite, cbWritten, dwMode;
   LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\SOnamedpipe");


// Tenton ta hapë pipe-in

   while (1)
   {
      hPipe = CreateFile(
         lpszPipename,   // emri i pipe-it
         GENERIC_READ |  // qasja shkrim/lexim
         GENERIC_WRITE,
         0,              // no sharing
         NULL,           // atributet e sigurisë
         OPEN_EXISTING,  // hap pipe-in ekzistues
         0,              // atributet default
         NULL);          // file template(jo)

   // Kontrollo për errors

      if (hPipe != INVALID_HANDLE_VALUE) //nuk ka error
         break;

      // Kontrollo nese ka error-a tjerë perpos nese pipe është i zënë

      if (GetLastError() != ERROR_PIPE_BUSY)
      {
         Logger("ERROR: Nuk u ralizua lidhja. Kodi="+std::to_string(GetLastError()));
         return -1;
      }

      // Pipe është i zënë, prit 20 sekonda.

      if ( ! WaitNamedPipe(lpszPipename, 20000))
      {
         Logger("Nuk mund te hapet Pipe, Pipe busy. Timeout 20s ");
         return -1;
      }
   }

// Pipe eshte konektuar. modi per lexim

   dwMode = PIPE_READMODE_MESSAGE;
   fSuccess = SetNamedPipeHandleState(
      hPipe,    // pipe handle
      &dwMode,  // mod i ri i pipe
      NULL,     // numri maksimal i bajtave
      NULL);    // koha maksimale
   if ( ! fSuccess)
   {
      Logger("Pipe handle deshtoj. KODI="+GetLastError());
      return -1;
   }


// Dergo mesazhin te procesi B.

   cbToWrite = (lstrlen(lpvMessage)+1)*sizeof(TCHAR);
   Logger( "Duke derguar "+std::to_string(cbToWrite)+" bytes. Mesazhi:");
   Logger(std::wstring(lpvMessage));

   fSuccess = WriteFile(
      hPipe,                  // pipe handle
      lpvMessage,             // message
      cbToWrite,              // message length
      &cbWritten,             // bytes written
      NULL);                  // not overlapped

   if ( ! fSuccess)
   {
      Logger("Shkrimi ne Pipe deshtoj. Kodi="+ std::to_string(GetLastError()));
      return -1;
   }

   Logger("\nMesazhi u dergua, Pergjigjia:");

   do
   {
   // Lexo prej pipe.

      fSuccess = ReadFile(
         hPipe,    // handle i pipe-it
         chBuf,    // buffer per pranimin e pergjigjes
         BUFSIZE*sizeof(TCHAR),  // madhesia e buffer-it
         &cbRead,  // numri i bajtave te lexuar
         NULL);    // jo overlapped

      if ( ! fSuccess && GetLastError() != ERROR_MORE_DATA )
         break;

      Logger( chBuf );
   } while ( ! fSuccess);  // perserit loop nese ka te dhena me shume

   if ( ! fSuccess)
   {
      _tprintf( TEXT("Leximi nga Pipe deshtoj. Kodi=%d\n"), GetLastError() );
      return -1;
   }

   _getch();

   CloseHandle(hPipe);

   return 0;
}

