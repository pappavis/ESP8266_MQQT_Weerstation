## SQLite database <br>
Dit is de SQLite database voor de node-red flow. <br>
 <br>
 
# isntructie

Je moet die iot.db voorbereiden
~~~bash
pi@pi4:~ $ cd ./ESP8266_MQQT_Weerstation/
pi@pi4:~ $ cp -r -v ./dbs /home/pi
pi@pi4:~ $ sudo chmod 666 /home/pi
pi@pi4:~ $ sudo chmod 777 /home/pi/dbs/iot.db
pi@pi4:~ $ sudo chown pi:pi -r /home/pi/dbs/
~~~

Nu zult de node-red flow ook werken.
