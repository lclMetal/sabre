:: main script based on scale.bat creator's example on Stack Overflow: https://stackoverflow.com/a/46571919
:: loop iteration counting based on paxdiablo's example on Super User: https://stackoverflow.com/a/7522822
:: file name zero-padding based on Chris's answer on Super User: https://superuser.com/a/1229632

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
