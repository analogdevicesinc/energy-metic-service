@echo off
set OPENOCD=%1
set HEXFILE=%2
set HEXFILE_FWD=%HEXFILE:\=/%

"%OPENOCD%" ^
  -s C:/MaximSDK/Tools/OpenOCD/scripts ^
  -f interface/cmsis-dap.cfg ^
  -f target/max32670.cfg ^
  -c "program \"%HEXFILE_FWD%\" reset exit"