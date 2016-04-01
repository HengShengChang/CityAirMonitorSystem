#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;
double latitude = 100;
double longitude = 100;
unsigned long starttime_gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

//from "-" to "s" 5V, ground, A1
#include <dht.h>   
dht DHT;   

#define gasMQ5_pin A0
#define dht_pin A1
#define gasMQ9_pin A2
#define dust_pin 8
#define gpsLedPin 13

double humidity;
double temperature;

unsigned long duration;
unsigned long starttime_dust;
unsigned long sampletime_ms = 30000;//sampe 30s ;
unsigned long lowPulseOccupancy = 0;
float ratio = 0;
float concentration = 0;
float vol_MQ5;
float vol_MQ9;


void setup() {
	Serial.begin(9600);  
    delay(300);
	ss.begin(GPSBaud);
	InitGPS();
	InitDustSensor();
	InitGasMQ5Sensor();
	InitGasMQ9Sensor();
	delay(700);
}

void InitGPS() {
	starttime_gps = millis();
	pinMode(gpsLedPin, OUTPUT);
	pinMode(12, OUTPUT);
}

void InitDustSensor() {
	pinMode(dust_pin,INPUT);
	starttime_dust = millis();
}

void InitGasMQ5Sensor() {
	pinMode(gasMQ5_pin, INPUT);
}

void InitGasMQ9Sensor() {
	pinMode(gasMQ5_pin, INPUT);
}

void InitDHT(){
	pinMode(dht_pin,OUTPUT);
	digitalWrite(dht_pin,HIGH);
}

void loop() {
	GPS();
	DHT11();
	GasSensorMQ5();
	GasSensorMQ9();
	DustSensor();

	Serial.print(latitude, 6);
	Serial.print(";");
	Serial.print(longitude, 6);
    Serial.print(";");
	Serial.print(temperature);
	Serial.print(";");
	Serial.print(humidity);
	Serial.print(";");
	Serial.print(vol_MQ5);
	Serial.print(";");
	Serial.print(vol_MQ9);
	Serial.print(";");
	Serial.println(concentration/283);    
}

void GPS() {
	while((millis()-starttime_gps)<3000) {
		while (ss.available() > 0) {
			if (gps.encode(ss.read())) {
				displayInfo();
			}
		}
	}
	starttime_gps = millis();
}

void displayInfo()
{
  if (gps.location.isValid())
  {
	latitude = gps.location.lat();
	longitude = gps.location.lng();
	digitalWrite(gpsLedPin,HIGH);
  }
  else //GPS INVALID
  {
        latitude = 100;
        longitude = 100;
	digitalWrite(gpsLedPin,LOW);
  }
}

void DustSensor() {
  duration = pulseIn(dust_pin, LOW);
  lowPulseOccupancy += duration;
  if ((millis()-starttime_dust) > sampletime_ms)//if the sampel time == 30s
  {
    ratio = lowPulseOccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve

    lowPulseOccupancy = 0;
    starttime_dust = millis();
  } 
}
void GasSensorMQ5() {
	int sensorValue = analogRead(gasMQ5_pin);
    vol_MQ5 = (float)sensorValue;
    delay(10);
}
void GasSensorMQ9() {
    int sensorValue = analogRead(gasMQ9_pin);
    vol_MQ9 = (float)sensorValue;
    delay(10);
}
void DHT11() {
	DHT.read11(dht_pin);   
	humidity = DHT.humidity; 
	temperature = DHT.temperature;
	delay(1000);           
}
