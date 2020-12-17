# ThingSpeak-SDM530-ModBus-IOT-project
Reading Inputs registers of a SMD503CT module of Eastron and upload data to ThingsSpeak of Matworks for IOT Device with a MCU ESP32 DevKit32 v1.1
Channel Public
https://thingspeak.com/channels/1021006

Los manuales y datasheet estan en la carpeta Datasheets a excepcion del modulo RS485 que se puede ver en (https://www.luisllamas.es/arduino-rs485-max485/)

Las Carpetas ThingSpeakProbe, PowerMeter_SDM530CT, FinishProject, tienen partes del proyecto por ejemplo en la ThingSpeakProbe se creo un dashboard con 8 campos en thingspeak, y se le envian  numeros enteros aleatorios del 0 al 99, con el fin de probar si hay conexion efectiva.

La Carpeta PowerMeter_SDM530CT lee 8 medidas del PMU via MODBUS y UART con RST a 9600 pbs con 8bits y uno de parada y lo imprime en el monitor serial a 115200 bps.

y Finalmente la Carpeta FinishProject lee 8 medidas del PMU via MODBUS y UART con RST a 9600 pbs con 8bits y uno de parada. Ademas sube las variables medidas a ThingSpeak con el siguiente orden (Vac, IacPh1, IacPh2, IacPh3, Pac, PF, Angulo, Freq)

### The Final Circuit:

![Circuito_Final](/images/Cto_PMU_IOT.png)
![Full_Project](/images/ESP32_PMU_ModBus_IOT.jpeg)

### ESP32Dev Board PINMAP

![Pin Functions](/images/ESP32-DOIT-DEVKIT-V1.jpg)

### SDM530CT-MODBUS

![Power Meter](/images/sdm530.jpg)

### ThingSpeak DashBoard

![Dashboard](/images/thingspeak.jpg)

### Helper

![Dog](/images/Rocky.jpg)

