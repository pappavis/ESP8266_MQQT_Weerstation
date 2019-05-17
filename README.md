# ESP8266_MQQT_Weerstation
ADVANCED level: Weatherstation based around ESP8266 and MQQT with deepsleep<br>
<br>
Deze code behore bij Youtube videoclip:<br> <a href="https://www.youtube.com/watch?v=l8MCuDvR2ek" target="_blank"><img src="https://i.ytimg.com/vi/l8MCuDvR2ek/hqdefault.jpg?sqp=-oaymwEZCNACELwBSFXyq4qpAwsIARUAAIhCGAFwAQ==&rs=AOn4CLCukInh4sH33ztIjzF9xCvjv1pOaw"></a><br>
<br><br>
 ### Wemos D1 weerstation door Michiel Erasmus.<br>
 Die programma verbind aan mqqt, publiceert meetdata en terug naar deepsleep.<br>
 wanneer de Wemos in deepsleep is kan jy geen scketch upload. Ontkoppel eerst USB en daarna weer uploaden.<br>
<br>
### Wat doet deze sketch;<br>
  - Maakt verbinding met WiFi & MQQT<br>
  - Melden aan MQQT dat de wemos gereboot is.<br>
  - ontvangt enige berichten van mqqt naar wemos, zoals deepSleepinterval.<br>
  - Ga in deepsleep voor de gespcificeerde tijd.<br>
<br>

### Requirements
 -  Raspberry Pi Zero W 1.3
 - Arduino IDE
 - Wemos D1 Mini, of ESP-01, of andere ESP8266 apparaat.
 - Mosquitto MQQT server
 - SQLlite
 - Node-red + sqlite extensies
 - Optioneel: SQLite Administrator
 
### Installatie
 1. Download en installeer Dietpi image, zie <a href="http://www.sensorsiot.org/tag/mqtt/" target="_blank">Andreas Spiess</a> op Raspberry Pi Zero W 1.3
 2. Installeer Mosquitto MQQT
 3. Importeer de Node-red flow naar node-red
 4. In Node-red flow, check dat je endpoints heb aangemaakt 
 <img src="https://github.com/pappavis/ESP8266_MQQT_Weerstation/blob/master/plaatjes/node-red_node.jpg?raw=true">
 5. Open de arduino sketch in Arduino IDE, en upload naar ESP8266.
 6. Check of die flows werkt op http://dietpi:1880/ui/
 
### Randvoorwaarde
 WiFi, Ubuntu linux / Raspberry Pi Zero met dietpi image. Daarop staat sqlite, mosquitto en node-red geïnstalleerd.<br>
 Je kunt ook Ubuntu gebruiken ipv Pi Zero.<br>
<br>
 ### Samenvattend
 1. Voldoen aan de software randvoorwaarden.<br>
 2. Importeer onderstaand flows naar Node red. Installeer SQLlite in palette van node-red.<br>
 3. Download arduino bibliotheken zoals hieronder vermeld, en installeer in Arduino IDE.<br>
 4. Download ESP8266 wemos boards uit Arduino boards manager en kies Wemos D1 R1.<br>
 5. Upload deze sketch naar Wemos / ESP8266.<br>
 <br>
 * Node red flows om te deployen;<br>
 <img src="https://github.com/pappavis/ESP8266_MQQT_Weerstation/blob/master/plaatjes/node-red_flow.jpg?raw=true" width="70%" height="70%"><br>
<br>
2019-01-21 door Michiel Erasmus #easylab4kids<br>
<br>
