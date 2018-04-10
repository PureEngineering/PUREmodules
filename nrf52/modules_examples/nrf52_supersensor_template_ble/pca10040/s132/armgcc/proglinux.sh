mergehex -m ../../../../../nRF5_SDK_12.2.0_f012efa/components/softdevice/s132/hex/s132_nrf52_3.0.0_softdevice.hex ./_build/nrf52832_xxaa.hex -o nrf52832_s132.hex
nrfjprog --chiperase --program nrf52832_s132.hex -f NRF52 -r
