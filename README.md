# PROGRAMI PËR KOMUNIKIM MES PROCESEVE PËRMES METODËS NAMED PIPES NE WINDOWS

## PERSHKRIMI I PROBLEMIT


Përmes platformës Windows është mundësuar komunikimi mes proceseve duke shfrytëzuar metodën e ashtuquajtur &quot;Named Pipes&quot;.

Një Named Pipe mundëson komunikimin mes proceseve, i cili mund te jetë i dyanshëm, full-duplex dhe nuk nevojitet marrëdhënia prind-fëmijë mes proceseve.

Pas krijimit te një Named Pipe ai mund te shfrytëzohet për komunikim nga shumë procese njëkohësisht. Named Pipes vazhdojnë te ekzistojnë edhe pasi që komunikimi mes dy proceseve të ketë përfunduar.

Named Pipes përkrahet nga Windows dhe UNIX sistemet mirëpo implementimi i tyre ne këto sisteme është i ndryshëm.

Për dallim nga UNIX-i, Windows mundëson komunikim më të mirë sepse përkrahë komunikimin Full-Duplex dhe lejohet komunikimi message-oriented krahas atij byte-oriented.


