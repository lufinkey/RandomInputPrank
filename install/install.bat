taskkill /f /IM rip.exe
mkdir "%userprofile%\AppData\Local\lufinkey"
mkdir "%userprofile%\AppData\Local\lufinkey\rip"
copy /y "%~dp0\defaultsettings.ini" "%userprofile%\AppData\Local\lufinkey\rip\settings.ini"
copy /y "%~dp0\rip.exe" "%userprofile%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\rip.exe"
attrib "%userprofile%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\rip.exe" +h
start "" "%userprofile%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\rip.exe"
exit