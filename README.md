# IoT Based Smart Irrigation System 

   An IoT Based smart irrigation system is built to monitor the field environment and to control the water pump remotely from anywhere in the world.
   This project built using NodeMCU-ESP8266,Soil Moisture Sensor,DHT11(Temperature and Humidity Sensor),Water Flow Sensor and PIR Motion Sensor.
   It uses a relay module to ON/OFF the pump .This project is designed with Bylnk IoT cloud which is used to send alert text messages when there is 
   less moisture level or high temperature/humidity or motion detected in the farm.

#### The list of components used in the smart irrigation system are:

  - Microcontroller (NodeMCU ESP8266).
  - Soil Moisture Sensor.
  - DHT11 Sensor.
  - Water Flow Sensor.
  - PIR Motion Sensor.
  - Relay Module and Motor.
  - Bread Board.
  - Jumper Wires.
  - Battery Supply.

#### Sensors Pin Configurations & Connection:

1. Soil Moisture Sensor:
    - GND --> G pin
    - VCC --> 3v3 pin
    - A0 --> A0 pin
    - D0 --> null
      
2. DHT11 Sensor:
   - minus(-) --> G pin
   - Plus(+) --> 3v3 pin
   - Out --> data pin (D5)
  
3. Water Flow Sensor:
   - Black wire --> G pin
   - Red wire --> 3v3 pin
   - Yellow wire --> data pin (D2)

 4. PIR Motion Sensor:
    - GND --> G pin
    - VCC --> 3v3 pin
    - OUT --> data pin (D4)

Refer Circuit Diagram image for connection.
