mergehex --merge ../../../../../nRF5_SDK_12.2.0_f012efa/components/softdevice/s132/hex/s132_nrf52_3.0.0_softdevice.hex ../../../../nrf52_secure_bootloader/hex/dfu_nrf52832_xxaa_s132.hex --output SD_BL.hex
nrfutil settings generate --family NRF52 --application ./_build/nrf52832_xxaa.hex --application-version 3 --bootloader-version 2 --bl-settings-version 1 settings.hex
nrfutil settings display settings.hex
mergehex --merge SD_BL.hex ./_build/nrf52832_xxaa.hex --output SD_BL_APP.hex
mergehex --merge SD_BL_APP.hex settings.hex --output SD_BL_APP_valid.hex
nrfjprog --chiperase --program SD_BL_APP_valid.hex -f NRF52 -r
pause
