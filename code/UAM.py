import MySQLdb
import serial
from time import strftime
from datetime import datetime, time

dbhost = 'localhost'
dbuser = 'root'
dbpass = 'raspberry'
dbname = 'MyDB'

ser = serial.Serial('/dev/ttyACM0',9600)

startTime = datetime.now()


conn = MySQLdb.connect(dbhost, dbuser, dbpass, dbname)
cursor = conn.cursor()

createTable = "CREATE TABLE IF NOT EXISTS UAM(DATE DATE, TIME TIME, LATITUDE FLOAT(10, 6), LONGITUDE FLOAT(10 ,6), TEMP FLOAT, HUMID FLOAT, MQ5 FLOAT, MQ9 FLOAT, Dust FLOAT)"
cursor.execute(createTable)

line = ser.readline()
line = ser.readline()
line = ser.readline()
line = ser.readline()

try:
    while 1:
        line = ser.readline().rstrip()
        lat,lon,temp,humid,mq5,mq9,dust = line.split(";")
        now = datetime.now()
        #print ( "%s, %s, %s, %s, %s, %s, %s" % (lat, lon, temp, humid, mq5, mq9, dust))
        sql = "INSERT INTO UAM(DATE, TIME, LATITUDE, LONGITUDE, TEMP, HUMID, MQ5, MQ9, Dust) VALUES ('%s', '%s', '%f', '%f', '%f', '%f', '%f', '%f', '%f' )" % (now.strftime("%Y-%m-%d"),now.strftime("%H:%M:%S"), float(lat), float(lon), float(temp), float(humid), float(mq5), float(mq9), float(dust))
        try:
            cursor.execute(sql)
            conn.commit()
        except:
            conn.rollback()
except:
    conn.close()
