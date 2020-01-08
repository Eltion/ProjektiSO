#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

#define BUFSIZE 512

DWORD WINAPI InstanceThread(LPVOID);
VOID GetAnswerToRequest(LPTSTR, LPTSTR, LPDWORD);

int main()
{
   BOOL   fConnected = FALSE;
   DWORD  dwThreadId = 0;
   HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;
   LPCTSTR lpszPipename = TEXT("\\\\.\\pipe\\SOnamedpipe");


//Loop-a krijon ne neamed pipe dhe pret qe klienti te konektohet
//Kur klienti konektohet nje thread krjohet per te kryer kumunikimi
//me te. Me pas threadi kryen analizen e kerkeses dhe shkruan ne buffer
//pergjigjen e kerkeses derisa programi kryesore pret per klient te tjere

   for (;;)
   {
      _tprintf( TEXT("\nDuke pritur qe klienti te lidhet ne piped named  %s\n"), lpszPipename);

       hPipe = CreateNamedPipe(
           lpszPipename,             // Emri i pipe-it
           PIPE_ACCESS_DUPLEX,       // lexo/shkruaj acessi
           PIPE_TYPE_MESSAGE |       // lloji i komunikimit ne pipe PIPE_TYPE_BYTE/PIPE_TYPE_MESSAGE
           PIPE_READMODE_MESSAGE |   // message-read modi
           PIPE_WAIT,                // modi i bllokimit
           PIPE_UNLIMITED_INSTANCES, // numri maksimal i instancave
           BUFSIZE,                  // madhesia e output buffer-it
           BUFSIZE,                  // madhesia e input buffer-it
           0,                        // time-out
           NULL);

      if (hPipe == INVALID_HANDLE_VALUE)
      {
          _tprintf(TEXT("CreateNamedPipe deshtoje, Kodi=%d.\n"), GetLastError());
          return false;
      }


       //Prit deri sa te konektohet klienti
      fConnected = ConnectNamedPipe(hPipe, NULL) ?
         TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

      if (fConnected)
      {
         printf("\nKlienti u lidhe. Krijo thread-in.\n");

         // Krijon thread-in per klientin
         hThread = CreateThread(
            NULL,              // attributet e siguris
            0,                 // madhsia e steku-t
            InstanceThread,    // funksioni i thredit
            (LPVOID) hPipe,    // parametrat e funksionit
            0,                 // suspended
            &dwThreadId);      // kthen Id e thread-it

         if (hThread == NULL)
         {
            _tprintf(TEXT("Krijimi i thread-it deshtoj, Kodi=%d.\n"), GetLastError());
            return false;
         }
         else CloseHandle(hThread);
       }
      else
        // Mbyll pipe-in nese klienti nuk ka mundur te lidhet
         CloseHandle(hPipe);
   }
   return true;
}

//Funksioni i thread-it
DWORD WINAPI InstanceThread(LPVOID lpvParam)
{
   HANDLE hHeap      = GetProcessHeap();
   TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));
   TCHAR* pchReply   = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE*sizeof(TCHAR));

   DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
   BOOL fSuccess = FALSE;
   HANDLE hPipe  = NULL;

   printf("Thread u krijua, lexo and proceso mesashin.\n");

// The thread's parameter is a handle to a pipe object instance.

   hPipe = (HANDLE) lpvParam;

// Lexo te dhenat nga pipe deri ne fund
   while (1)
   {
      fSuccess = ReadFile(
         hPipe,        // handle-i i pipe
         pchRequest,    // Bufferi ku ruhen te dhenat e lexuara nga pipe
         BUFSIZE*sizeof(TCHAR), // madhsia e bufferit
         &cbBytesRead, // numri i bajtave te lexuar
         NULL);        // jo overlapped I/O

      if (!fSuccess || cbBytesRead == 0)
      {
          if (GetLastError() == ERROR_BROKEN_PIPE)
          {
              _tprintf(TEXT("Klienti u diskonektua.\n"));
          }
          else
          {
              _tprintf(TEXT("Leximi deshtoj, Kodi=%d.\n"), GetLastError());
          }
          break;
      }

   // Proceso kerkesen
       _tprintf( TEXT("Kerkesa:\"%s\"\n"), pchRequest );
      GetAnswerToRequest(pchRequest, pchReply, &cbReplyBytes);

   // Shkruaj pergjigjjen ne pipe
      fSuccess = WriteFile(
         hPipe,        // handle-i i pipe
         pchReply,     // Bufferi prej te cilit shkruhet
         cbReplyBytes, // numri i bajtave per te shkruar
         &cbWritten,   // numri i bajtave te shkruar
         NULL);        // jo overlapped I/O

      if (!fSuccess || cbReplyBytes != cbWritten)
      {
          _tprintf(TEXT("Shkrimi deshtoj, Kodi=%d.\n"), GetLastError());
          break;
      }
  }

   FlushFileBuffers(hPipe);
   DisconnectNamedPipe(hPipe); //Diskonekto pipe
   CloseHandle(hPipe); //mbylle handle e pipe
    \
   //Liro bufferin
   HeapFree(hHeap, 0, pchRequest);
   HeapFree(hHeap, 0, pchReply);

   printf("Mbyll thread-in\n");
   return 1;
}
std::vector<std::wstring> spitWstring(std::wstring a);
wstring getTime();


//Bene procesimin e kerkeses se klientit dhe vendos pergjigjjen ne buffer
VOID GetAnswerToRequest( LPTSTR pchRequest, LPTSTR pchReply, LPDWORD pchBytes ) {


    std::wstring kerkesa = std::wstring(pchRequest);
    vector<wstring> kerkesaArray= spitWstring(kerkesa);
    wstring c = L"KATROR";

    if(kerkesaArray[0] == L"KATROR"){
        //int numri = stoi(kerkesaArray[1]);
        //printf("%d",numri);
        StringCchCopy( pchReply, BUFSIZE, to_wstring(4).c_str());
    }else if(kerkesaArray[0] == L"ORA"){
        wstring koha = getTime();
        StringCchCopy(pchReply, BUFSIZE, koha.c_str());
    }else{
        StringCchCopy(pchReply, BUFSIZE, L"OK. Mesazhi u pranua");
    }


    //Madhesia e pergjigjjes
    *pchBytes = (lstrlen(pchReply)+1)*sizeof(TCHAR);
}


//Merr kohen nga sistemi
wstring getTime(){
    wchar_t koha [80];
    time_t t = time(nullptr);
    tm *timestamp = localtime(&t);
     wcsftime (koha,80,L"Ora eshte %I:%M%p.",timestamp);
    return koha;
}

//E ndane wstring sipas delimiterit ' '
std::vector<std::wstring> spitWstring(std::wstring a){
    std::vector<std::wstring> b;
    std::wstring st = L"";
    for (int i = 0; i < a.length(); i++) {

        if (a[i] == L' ') {
            b.push_back(st);
        }
        else{
            st += a[i];
        }

    }
    b.push_back(st);
    return b;

}
