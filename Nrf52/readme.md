1- Run 'make' in this directory. ('PureModules/Nrf52/').
      This will download the "nRF5_SDK_12.2.0_f012efa" Nordic SDK. 
      
2- Match your ARM GCC Version in this project to the one your computer is using. 
      A- Open '..PureModules\Nrf52\nRF5_SDK_12.2.0_f012efa\components\toolchain\gcc\Makefile.windows
      B- Change the settings to match your computer's GCC settings. 
      ie- Mine is:
      
            GNU_INSTALL_ROOT := C:\Program Files (x86)\GNU Tools ARM Embedded\5.4 2016q3
            GNU_VERSION := 5.4
            GNU_PREFIX := arm-none-eabi
            
          
3- Open the Example Project directory you wish to program. 
     ie- To open SuperSensorNrf52 Project go to: 
     ..PureModules\Nrf52\Modules_examples\superSensorNrf52\pca10040\blank\armgcc
     
 4- Run 'make' in this directory. This will compile the code. 
 
 5- Run 'program' to flash the nrf52 board with the project code
