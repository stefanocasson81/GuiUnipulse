REM 31.30.29.28.27.26.25.24.23.22.21.20.19.18.17.16.15.14.13.12.11.10.9.8.7.6.5.4.3.2.1.0
set "Versione=00"

copy 55022_UniPulsedTinyUI\55022_UniPulsedTinyUI.hex 55022_%Versione%_UniPulsedTinyUI.hex
c:\Usr\Utility\CodificaFirmware\CodificaFirmware -FileOrg=55022_%Versione%_UniPulsedTinyUI.hex -FileDest=55022_%Versione%_UniPulsedTinyUI.Helvi -SwVer=%Versione% -CodScheda=22755022 -CodSchedaBootLoaderCompatibility=26060437 -AddrMapMin=90000200 -TabConv=24.2.14.6.4.8.31.30.29.28.27.26.25.23.22.21.20.19.18.17.16.15.13.12.11.10.9.7.5.3.1.0
