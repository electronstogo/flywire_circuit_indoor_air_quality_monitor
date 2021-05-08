# air_quality_monitor_prj_2

Flywire circuit that shows different measurement values to rate the indoor air quality.
To see what is meant, best way is to look at the images.


IMPORTANT: In this project the Sensirion sensor SGP30 was used. From my point of view the successor SGP40 works much better.





This circuit uses 2 OLED displays to show the following values:

- TVOC [PPB]
- CO2 [PPM]
- Relative humidity [%]
- Temperature [°C]
- Pressure [mbar]
- Time [hh:mm:ss]


Components in use:

- Sensirion SGP30 sensor
- Bosch BME280 sensor
- DS3231 RTC modul
- 2 x 0.96 inch OLED moduls
- Arduino Nano
- 2 x electronic switches
- Wires with different profiles
