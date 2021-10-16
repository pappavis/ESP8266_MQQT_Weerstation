echo "** sqlite3 install"
sudo apt install -y sqlite3 git
mkdir /home/pi/Downloads
cd /home/pi/Downloads

sudo git clone https://github.com/pappavis/thescript
cd /home/pi/Downloads/thescript
sudo bash installIOT.sh
cd /home/pi
sudo chown pi:pi -R /home/pi
chmod 777 /home/pi/dbs
chmod 666 /home/pi/dbs/iot.db

echo "sqlite3 en iot.db is geïnstalleerd en gered voor gebruik met de node-red flow."
