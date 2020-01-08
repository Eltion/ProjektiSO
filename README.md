# ProjektiSO








PROGRAMI PËR KOMUNIKIM MES PROCESEVE PËRMES METODËS NAMED PIPES NE WINDOWS
















Janar, 2020

Përmbajtja

PERSHKRIMI I PROBLEMIT        3

TEKNOLOGJIA        4

3.IMPLEMENTIMI        5

3.1 Paraqitja vizuale e programit        5

3.2 FUNKSIONET KRYESORE TE PERDORURA        6

3.2.1 CreateNamedPipe        6

3.2.2.  CreateFile        7

3.2.3 WriteFile        7

3.2.4 ReadFile        8

3.3 Implemenitimi i programit ne kod        9

3.3.1 Procesi A        9

3.3.2 Procesi B        14

REFERENCAT        18



1. 1.PERSHKRIMI I PROBLEMIT



Përmes platformës Windows është mundësuar komunikimi mes proceseve duke shfrytëzuar metodën e ashtuquajtur &quot;Named Pipes&quot;.

Një Named Pipe mundëson komunikimin mes proceseve, i cili mund te jetë i dyanshëm, full-duplex dhe nuk nevojitet marrëdhënia prind-fëmijë mes proceseve.

Pas krijimit te një Named Pipe ai mund te shfrytëzohet për komunikim nga shumë procese njëkohësisht. Named Pipes vazhdojnë te ekzistojnë edhe pasi që komunikimi mes dy proceseve të ketë përfunduar.

Named Pipes përkrahet nga Windows dhe UNIX sistemet mirëpo implementimi i tyre ne këto sisteme është i ndryshëm.

Për dallim nga UNIX-i, Windows mundëson komunikim më të mirë sepse përkrahë komunikimin Full-Duplex dhe lejohet komunikimi message-oriented krahas atij byte-oriented.



1. 2.TEKNOLOGJIA



Gjuha Programuese : C/C++,

Editori : QT Creator 4.11.0

Kompailer :        MinGW 7.3.0

Platforma : Microsoft Windows 10.























1. 3.IMPLEMENTIMI


## 3.2 FUNKSIONET KRYESORE TE PERDORURA

Per implementimin e programit janë përdorur funksionet nga libraria e Windows-it ne C

### 3.2.1 CreateNamedPipe

**HANDLE**   **CreateNamedPipe** (

   **LPCSTR**                 lpName,

   **DWORD**                  dwOpenMode,

   **DWORD**                  dwPipeMode,

   **DWORD**                  nMaxInstances,

   **DWORD**                  nOutBufferSize,

   **DWORD**                  nInBufferSize,

   **DWORD**                  nDefaultTimeOut,

   **LPSECURITY\_ATTRIBUTES**  lpSecurityAttributes

);

lpName – Emri unik i pipe.

      \.\pipe\&lt;i\&gt;pipename

      Psh. \.\pipe\SOnamedpipe

dwOpenMode – modi i qasjes ne pipe

                **PIPE\_ACCESS\_DUPLEX**

                **PIPE\_ACCESS\_INBOUND**

                **PIPE\_ACCESS\_OUTBOUND**

dwPipeMode – Modi I mesazhit te pipe

                **PIPE\_TYPE\_BYTE**

**                PIPE\_TYPE\_MESSAGE**

nMaxInstances – Numri maksimal i instancave qe mund te krijohen per pipe

                NULL – I pa limituar

nOutBufferSize – Numri I bajtave per output buffer

nInBufferSize – Numri I bajtave per input buffer

nDefaultTimeOut – Koha per sa pipe mund te jete I zene

### 3.2.2.  CreateFile

Përdoret per tu konektuar ne named pipe

**HANDLE**   **CreateFile** (

   **LPCSTR**                 lpFileName,

   **DWORD**                  dwDesiredAccess,

   **DWORD**                  dwShareMode,

   **LPSECURITY\_ATTRIBUTES**  lpSecurityAttributes,

   **DWORD**                  dwCreationDisposition,

   **DWORD**                  dwFlagsAndAttributes,

   **HANDLE**                 hTemplateFile

);

lpFileName – Emri i pipe ku dëshirojmë te konektohemi

dwDesiredAccess – Qasja ne pipe

                **GENERIC\_READ**

**GENERIC\_WRITE**

lpSecurityAttributes – Atributet e sigurisë

### 3.2.3 WriteFile

**BOOL**   **WriteFile** (

   **HANDLE**        hFile,

   **LPCVOID**       lpBuffer,

   **DWORD**         nNumberOfBytesToWrite,

   **LPDWORD**       lpNumberOfBytesWritten,

   **LPOVERLAPPED**  lpOverlapped

);

hFile – Një hadle per pipe

lpBuffer – Pointer qe pointon tek buffer-i i te dhënave qe do te shkruhen ne pipe.

nNumberOfBytesToWrite – Numri i bajtave qe do te shkruhen ne pipe.

lpNumberOfBytesWritten – Numri i bajtave te shkruar ne pipe.



### 3.2.4 ReadFile

**BOOL**   **ReadFile** (

   **HANDLE**        hFile,

   **LPVOID**        lpBuffer,

   **DWORD**         nNumberOfBytesToRead,

   **LPDWORD**       lpNumberOfBytesRead,

   **LPOVERLAPPED**  lpOverlapped

);

hFile – Një hadle per pipe

lpBuffer – Pointer qe pointon tek buffer-i ku ruhen te dhenat e lexuara nga pipe.

nNumberOfBytesToRead – Numri i bajtave qe do te lexohen nga pipe.

lpNumberOfBytesRead – Numri i bajtave te lexuara nga pipe.




