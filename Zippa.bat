REM call Pulisci.bat
	
for %%* in (.) do set NomeDir=%%~n*

cd ..
del %NomeDir%.zip
7za a -r -tzip %NomeDir%.zip  %NomeDir%

cd %NomeDir%
