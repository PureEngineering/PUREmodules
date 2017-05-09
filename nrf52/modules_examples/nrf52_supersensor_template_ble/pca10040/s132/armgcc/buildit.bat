make 
nrfutil pkg generate --hw-version 52 --sd-req 0x8C --application-version 4 --application ./_build/nrf52832_xxaa.hex --key-file ../../../../nrf52_secure_bootloader/private.pem puremodules_uart_dfu.zip
pause
