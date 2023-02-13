@echo off
setlocal enabledelayedexpansion
set "source_folder=.\textures"
set "result_folder=.\data"

set loopcount=0

for %%a in ("%source_folder%\*") do (
    set /a loopcount+=1
    set "n=0000!loopcount!"
    set "padded=!n:~-4!"
    call scale-script\scale.bat -source "%%~fa" -target "%result_folder%\_thumb-!padded!.png" -max-height 64 -max-width 64 -keep-ratio no -force yes
)
endlocal
