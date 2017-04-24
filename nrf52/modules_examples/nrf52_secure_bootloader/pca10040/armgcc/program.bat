make erase
make flash_softdevice
make flash
nrfjprog --program nrf52832_xxaa.hex -f NRF52 -r
nrfjprog --program settings.hex -f NRF52 -r
pause
