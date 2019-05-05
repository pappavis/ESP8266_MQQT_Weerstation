# ESP8266_MQQT_Weerstation
Weatherstation based around ESP8266 and MQQT with deepsleep<br>
<br>
Deze code behore bij Youtube videoclip:<br> <a href="https://www.youtube.com/watch?v=l8MCuDvR2ek" target="_blank"><img src="https://i.ytimg.com/vi/l8MCuDvR2ek/hqdefault.jpg?sqp=-oaymwEZCNACELwBSFXyq4qpAwsIARUAAIhCGAFwAQ==&rs=AOn4CLCukInh4sH33ztIjzF9xCvjv1pOaw"></a><br>
<br><br>
 20190122 Wemos weerstation door Michiel Erasmus.<br>
 Die programma verbind aan mqqt, publiceert meetdata en terug naar deepsleep.<br>
 wanneer de Wemos in deepsleep is kan jy geen scketch upload. Ontkoppel eerst USB en daarna weer uploaden.<br>
<br>
 Wat doet deze sketch;<br>
  - Maakt verbinding met WiFi & MQQT<br>
  - Melden aan MQQT dat de wemos gereboot is.<br>
  - ontvangt enige berichten van mqqt naar wemos, zoals deepSleepinterval.<br>
  - Ga in deepsleep voor de gespcificeerde tijd.<br>
<br>
 randvoorwaarde: WiFi, Ubuntu linux / Raspberry Pi Zero met dietpi image. Daarop staat sqlite, mosquitto en node-red geïnstalleerd.<br>
 Je kunt ook Ubuntu gebruiken ipv Pi Zero.<br>
<br>
 Om deze kode te gebruiken;<br>
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
