# 20190604 zie voorbeeld https://randomnerdtutorials.com/micropython-mqtt-esp32-esp8266/
import time
from umqtt.simple import MQTTClient
import ubinascii
import machine
import micropython
import network
import esp
esp.osdebug(None)
import gc
gc.collect()

ssid = 'Volksrepubliek'
password = 'C0mmodore64!'
mqtt_server = '192.168.2.109'
#EXAMPLE IP ADDRESS
#mqtt_server = '192.168.1.144'
client_id = ubinascii.hexlify(machine.unique_id())
topic_sub = b'esp8266_notification'
topic_pub = b'hello'

client = None
last_message = 0
counter = 0
aantalMinuteDeepSleep = 1
deepsleepTime = aantalMinuteDeepSleep * 60 * 1000
message_interval = 30
next_publishevent = 0
eersteStart = True

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('WiFi Connection successful')
print(station.ifconfig())


def getWeerdata():
    from bmp180 import BMP180
    from machine import Pin, PWM
    import machine
    
    bus = machine.I2C(scl=machine.Pin(5), sda=machine.Pin(4), freq=100000)
    bmp180 = BMP180(bus)
    bmp180.oversample_sett = 2
    bmp180.baseline = 101325

    temp = bmp180.temperature - 3.5  #  deze apparaat meet altijd 4 graden te hoog.
    druk1 = bmp180.pressure
    altitude1 = bmp180.altitude
    
    return druk1, altitude1, temp

def sub_cb(topic, msg):
    print((topic, msg))
    if topic == topic_sub and msg == b'received':
        print('ESP received hello message')

    try:
        if (topic == b'weatherstation/deepSleepIntervalSekonden/waarde'):
            print(b'aantalMinuteDeepSleep: ', msg)
            aantalMinuteDeepSleep = int(msg)
    except:
        pass


    if topic == b'LED_INTERNAL' and msg == b'knipperen':
        print('ESP knipperen LEDje')
        import time
        from machine import Pin

        led=Pin(2,Pin.OUT) #create LED object from pin2,Set Pin2 to output

        for herhaal1 in range(0, 6):
            led.value(1)            #Set led turn on
            time.sleep(0.2)
            led.value(0)            #Set led turn off
            time.sleep(0.5)


def connect_and_subscribe():
    global client_id, mqtt_server, topic_sub
    client = MQTTClient(client_id, mqtt_server, user="admin", password="admin")
    
    try:
        station.active(True)
        station.connect(ssid, password)

        while station.isconnected() == False:
          pass

        print('WiFi Connection successful')
        
        client.set_callback(sub_cb)
        print("clientID " + str(client_id) + " verbinden aan MQQT: " + mqtt_server)
        client.connect()
        
        client.publish("weatherStation/halloWereld",  str(client_id) + " connected.")
        client.subscribe(topic_sub)
        client.subscribe("LED_INTERNAL")
        client.subscribe("weatherstation/deepSleepIntervalSekonden/waarde")
        client.subscribe("weatherStation/ONTWAAKT")
        client.publish(topic_sub, client_id + " publicatie.")
        print(topic_sub, client_id + " publicatie.")

    except Exception as e6:
        print("connect_and_subscribe: " + str(e6))
    
    print('Verbonden aan %s MQTT broker, subscribed aan %s topic' % (mqtt_server, topic_sub))
    
    return client


def restart_and_reconnect():
    print('MQTT broker verbinding mislukt. Opnieuw proberen...')
    try:
        time.sleep(10)
        machine.reset()
    except:
        pass
        
def lekkerSlaap(mqqt_client) :
    import time, esp
    from machine import RTC
    strTopic = "weatherStation/DEBUG"
    
    try:
        if (deepsleepTime > 1):
            mqqt_client.publish("weatherStation/client_id", str(client_id))
            print("Deepsleep AAN: " + str(deepsleepTime))
            strTopic = "weatherStation/DEBUG"
            mqqt_client.publish(strTopic, "[{client: " + str(client_id) + ", topic: '" + strTopic + "', event: 'Deepsleep AAN', tijd: " + str(deepsleepTime) + " }]")
            deepsleepAanUit = 1
            
            # configure RTC.ALARM0 to be able to wake the device
            rtc = machine.RTC()
            rtc.irq(trigger=rtc.ALARM0, wake=machine.DEEPSLEEP)

            print("Zzzzzzzzz")
            mqqt_client.publish(strTopic, "client " + str(client_id) + " Deepsleep tyd:" + str(deepsleepTime))
            mqqt_client.publish(strTopic, "client " + str(client_id) + " Zzzzzzzzz")
            
            # put the device to sleep, milliseconds
            deep_sleep_esp(msecs=deepsleepTime)

            check_connect_mqqt()
            time.sleep_ms(50)
            mqqt_client.publish(strTopic, "client " + str(client_id) + " is wakker geworden.")
            publish_naar_mqqt(opdracht=msg)
            strTopic = "weatherStation/ONTWAAK"
            mqqt_client.publish(strTopic, "[{client: " + str(client_id) + ", topic: '" + strTopic + "', event: 'getWakkerBlijvenInterval'}]")
            mqqt_client.publish(strTopic,"getWakkerBlijvenInterval")
            
        else:
            print("Deepsleep staat UIT")
            mqqt_client.publish(strTopic, "[{client: " + str(client_id) + ", topic: '" + strTopic + "', event: 'Deepsleep staat UIT'}]")
            deepsleepAanUit = 0
    except OSError as e:
        print(e)
    
    
def deep_sleep_esp(msecs) :
    try:
        # configure RTC.ALARM0 to be able to wake the device 
        timer = machine.Timer(-1)
        timer.init(period=15000, mode=machine.Timer.ONE_SHOT, callback=lambda t:esp.deepsleep(10000000))

        # set RTC.ALARM0 to fire after X milliseconds (waking the device)
        rtc = machine.RTC()
        rtc.alarm(rtc.ALARM0, msecs)

        # put the device to sleep 
        machine.deepsleep()
    except Exception as e:
        print(e)


while True:
    try:
        if (eersteStart):
            next_publishevent = time.time() + message_interval
            eersteStart = False
        
        if(client == None):
            client = connect_and_subscribe()
            
    except Exception as e:
        print("")
        print("")
        print("OSError: " + str(e))
        print("")
        print("")
        restart_and_reconnect()


    try:
        client.check_msg()

        if (time.time() > next_publishevent):
            druk1, altitude1, temp1 = getWeerdata()
            print("Lugdruk:", druk1, " Hoogte: ", altitude1, " Temp: ", temp1)

            msg = b'Hello #%d' % counter
            client.publish(topic_pub, msg)

            client.publish("weatherStation/DHT/temperature", str(temp1))
            client.publish("weatherStation/DHT/luchtdruk", str(druk1))
            next_publishevent = time.time() + message_interval

            lekkerSlaap(mqqt_client=client)
            counter += 1
        else:
            print("time.time()=" + str(time.time()), "  counter: " , counter, "volgende publikasie oor: ", next_publishevent - time.time() )
    except OSError as e:
        print("")
        print("")
        print("OSError: " + str(e))
        print("")
        print("")
        restart_and_reconnect()

print("****EINDE****")

