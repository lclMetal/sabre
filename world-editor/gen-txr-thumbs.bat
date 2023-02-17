:: scale.bat created by Vasil Arnaudov, see "scale.bat-LICENSE.txt" for more information
:: script based on user332474's  example on Stack Overflow: https://stackoverflow.com/a/2766240

@echo off
setlocal

for /F "tokens=1,2" %%i in (textures.conf) do call :generateThumbnail %%i %%j
goto :EOF

:generateThumbnail
set VAR1=%1
set VAR2=%2
call scale-script\scale.bat -source "data\%VAR2%.png" -target "data\_thumb-%VAR1%.png" -max-height 64 -max-width 64 -keep-ratio no -force yes
echo %VAR1% %VAR2%
goto :EOF

endlocal
