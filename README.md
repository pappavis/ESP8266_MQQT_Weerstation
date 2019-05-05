# ESP8266_MQQT_Weerstation
Weatherstation based around ESP8266 and MQQT with deepsleep<br>
<br>
Deze code behore bij Youtbe videoclip:<br> <a href="https://www.youtube.com/watch?v=l8MCuDvR2ek" target="_blank"><img src="https://i.ytimg.com/vi/l8MCuDvR2ek/hqdefault.jpg?sqp=-oaymwEZCNACELwBSFXyq4qpAwsIARUAAIhCGAFwAQ==&rs=AOn4CLCukInh4sH33ztIjzF9xCvjv1pOaw"></a><br>
<br><br>
 20190122 Wemos weerstation door Michiel Erasmus.<br>
 Die programma verbind ana mqqt, publiceert meetdata en terug naar deepsleep.<br>
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
 // Flow 1. wakker worden Wemos:<br>
 [{"id":"88277a33.f22a58","type":"mqtt in","z":"5a52a983.5025e8","name":"","topic":"deepsleep_hallo_wereld_20190123/hallo","qos":"2","broker":"a6a3041b.8ef2e8","x":247.16668701171875,"y":77.3333511352539,"wires":[["66d547a1.fb2298"]]},{"id":"66d547a1.fb2298","type":"debug","z":"5a52a983.5025e8","name":"","active":true,"console":"false","complete":"payload","x":661.1666831970215,"y":96.66667366027832,"wires":[]},{"id":"90817a94.6fc3e8","type":"mqtt in","z":"5a52a983.5025e8","name":"Wemos is wakker","topic":"deepsleep_hallo_wereld_20190123/reboot/setup","qos":"0","broker":"a6a3041b.8ef2e8","x":124,"y":407.0000009536743,"wires":[["94d05079.7dcf8"]]},{"id":"94d05079.7dcf8","type":"function","z":"5a52a983.5025e8","name":"setdeepSleepInterval","func":"var deepSleepInterval = 60*10;\n\nif(msg.topic == \"deepsleep_hallo_wereld_20190123/reboot/setup\")\n{\n    if(parseInt(msg.payload) == 1)\n    {\n        msg.payload = parseInt(deepSleepInterval);\n    }\n}\n\nreturn msg;\n","outputs":1,"noerr":0,"x":489.00001525878906,"y":358.6666965484619,"wires":[["ca04f79c.e1b9f8","9ec4a980.817dc8"]]},{"id":"ca04f79c.e1b9f8","type":"mqtt out","z":"5a52a983.5025e8","name":"","topic":"deepsleep_hallo_wereld_20190123/deepSleepIntervalSekonden/waarde","qos":"2","retain":"","broker":"a6a3041b.8ef2e8","x":989,"y":316.3333330154419,"wires":[]},{"id":"9ec4a980.817dc8","type":"debug","z":"5a52a983.5025e8","name":"","active":true,"console":"false","complete":"payload","x":820,"y":432.3333578109741,"wires":[]},{"id":"a1ffc9ea.dd7b98","type":"inject","z":"5a52a983.5025e8","name":"inject deepSleepIntervalSekonden","topic":"deepsleep_hallo_wereld_20190123/reboot/setup","payload":"30","payloadType":"str","repeat":"","crontab":"","once":false,"x":198.00001525878906,"y":214,"wires":[["ca04f79c.e1b9f8"]]},{"id":"fba66d89.3e7e3","type":"mqtt in","z":"5a52a983.5025e8","name":"halloWereld","topic":"halloWereld","qos":"2","broker":"a6a3041b.8ef2e8","x":205,"y":537.0000486373901,"wires":[["e49cf77d.a65f18"]]},{"id":"e49cf77d.a65f18","type":"debug","z":"5a52a983.5025e8","name":"","active":true,"console":"false","complete":"payload","x":523,"y":549.3333616256714,"wires":[]},{"id":"f606f3d2.82c61","type":"inject","z":"5a52a983.5025e8","name":"","topic":"weatherstation/bmp1/debug","payload":"1","payloadType":"num","repeat":"","crontab":"","once":false,"x":251.99998474121094,"y":668.0000655651093,"wires":[["6988fcf6.bc5954"]]},{"id":"6988fcf6.bc5954","type":"mqtt out","z":"5a52a983.5025e8","name":"","topic":"weatherstation/bmp1/debug","qos":"2","retain":"","broker":"a6a3041b.8ef2e8","x":694.9999961853027,"y":647.3333578109741,"wires":[]},{"id":"da74b68c.1b7828","type":"inject","z":"5a52a983.5025e8","name":"wakker worden!","topic":"deepsleep_hallo_wereld_20190123/reboot/setup","payload":"true","payloadType":"bool","repeat":"","crontab":"","once":false,"x":137.1666717529297,"y":282.333345413208,"wires":[["94d05079.7dcf8"]]},{"id":"a6a3041b.8ef2e8","type":"mqtt-broker","z":"","broker":"localhost","port":"1883","clientid":"admin@localhost","usetls":false,"compatmode":true,"keepalive":"60","cleansession":true,"willTopic":"","willQos":"0","willPayload":"","birthTopic":"","birthQos":"0","birthPayload":""}]<br>
<br>
<br>
2019-01-21 door Michiel Erasmus #easylab4kids<br>
<br>
