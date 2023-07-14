#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include "Adafruit_ADS1X15.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
 


// BME sensor section
//#define BME_SCK 13
//#define BME_MISO 12
//#define BME_MOSI 11
//#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

// Define the BME680 sensor
Adafruit_BME680 bme;
float bmeTemp;
float bmeHum;
float pressure;
float gasResistance;
float bmealtitude;


// Define the DHT22 sensor
#define DHTPIN 14
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float temperature;
float humidity;



// For Tank number 1
// PH sensor section
#define phPin 34
float ph;
float Value = 0;

// Define the TDS / EC SENSORS.
#define tdsPin 27
Adafruit_ADS1115 ads1115;
float tdsValue;
float ppm;


// Disolve oxygen sensor
#define oxyPin 26
float oxyValue;

// GPIO where the DS18B20 is connected to
#define oneWireBus 12
float dsTemp;
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);



//For Tank number 2

// PH sensor section
#define phPin2 35
float ph2;
float Value2 = 0;

// Define the TDS / EC SENSORS.
#define tdsPin2 32
float tdsValue2;
float ppm2;


// Disolve oxygen sensor
#define oxyPin2 33
float oxyValue2;

// GPIO where the DS18B20 is connected to
#define oneWireBus 25
float dsTemp2;


 

// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x7A, 0xAE, 0x7C};  // Replace your master esp32 MAC address

// Define a data structure
typedef struct struct_message {
  float bmeTemp;
  float bmeHum;
  float pressure;
  float gasResistance;
  float bmealtitude;
  float temperature;
  float humidity;
  float ph;
  float ppm;
  float dsTemp;
  float oxyValue;
  float ph2;
  float ppm2;
  float oxyValue2;
  float dsTemp2;
  
  
  
  
} struct_message;
 
// Create a structured object
struct_message myData;
 
// Peer info
esp_now_peer_info_t peerInfo;
 
// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


 

void setup() {
  Serial.begin(115200);

  // For tank section 1 
  pinMode(phPin, INPUT);
  pinMode(tdsPin, INPUT);
  pinMode(oxyPin, INPUT);
    
 // For tank section 2
  pinMode(phPin, INPUT);
  pinMode(tdsPin, INPUT);
  pinMode(oxyPin, INPUT);
//  pinMode(oneWireBus, INPUT);

   // Initialize the TDS / EC SENSORS.
  ads1115.begin();
  
  // Initialize the DHT22 sensor
  dht.begin();

   // Start the DS18B20 sensor
  sensors.begin();
  
  // Initialize the I2C bus.
  Wire.begin();

  while (!Serial);
  Serial.println(F("BME680 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms


// Section for slave esp32 for setup wifi connection as station mode

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}



void readDht22() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }
}

void readBME680() {

  if (! bme.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  // Read the sensor data.
  bmeTemp = bme.temperature;
  bmeHum = bme.humidity;
  pressure = bme.pressure / 100.0;
  gasResistance = bme.gas_resistance / 1000.0;
  bmealtitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  // Print the sensor data.
  Serial.print("BME Temperature: ");
  Serial.print(bmeTemp);
  Serial.println(" *C");
  Serial.print("BME Humidity: ");
  Serial.print(bmeHum);
  Serial.println(" %");
  Serial.print("Pressure: ");
  Serial.print(pressure);
  Serial.println(" hPa");
  Serial.print("Gas Resistance: ");
  Serial.print(gasResistance);
  Serial.println(" ohm");
  Serial.print("Approx. Altitude: ");
  Serial.print(bmealtitude);
  Serial.println(" m");
}

void readPh() {
  Value = analogRead(phPin);
  Serial.print(Value);
  Serial.print(" | ");
  float voltage = Value * (3.3 / 4095.0);
  ph = (3.3 * voltage);
  Serial.println(ph);
  delay(500);
}

void readPh2(){
  Value2 = analogRead(phPin2);
  Serial.print(Value2);
  Serial.print(" | ");
  float voltage = Value2 * (3.3 / 4095.0);
  ph2 = (3.3 * voltage);
  Serial.println(ph2);
  delay(500);
}
void readTDS(){
   // Read the TDS / EC value.
  tdsValue = ads1115.readADC_SingleEnded(tdsPin);

  // Convert the TDS / EC value to ppm.
  ppm = tdsValue * 0.01;

  // Print the TDS / EC value in ppm.
  Serial.print("TDS value in ppm: ");
  Serial.println(ppm);
}

void readTDS2(){
   // Read the TDS / EC value.
  tdsValue2 = ads1115.readADC_SingleEnded(tdsPin2);

  // Convert the TDS / EC value to ppm.
  ppm2 = tdsValue2 * 0.01;

  // Print the TDS / EC value in ppm.
  Serial.print("TDS value in ppm2: ");
  Serial.println(ppm2);
}



void readDS18B20(){
  sensors.requestTemperatures();
  dsTemp = sensors.getTempCByIndex(0);
  Serial.print("DS_Temperature: ");
  Serial.print(dsTemp);
  Serial.println("ºC");
}

void readDS18B20_2(){
  sensors.requestTemperatures();
  dsTemp2 = sensors.getTempCByIndex(0);
  Serial.print("DS_2_Temperature: ");
  Serial.print(dsTemp2);
  Serial.println("ºC");
}



void readOxydo(){
  oxyValue = ads1115.readADC_SingleEnded(oxyPin);
  Serial.print("Desolve Oxygen: ");
  Serial.println(oxyValue);
}
void readOxydo2(){
  oxyValue2 = ads1115.readADC_SingleEnded(oxyPin);
  Serial.print("Desolve Oxygen_2: ");
  Serial.println(oxyValue2);
}



void loop() {
  readDht22();
  readBME680();
  readPh();
  readTDS();
  readDS18B20();
  readOxydo();
  readPh2();
  readTDS2();
  readDS18B20_2();
  readOxydo2();

  // Format structured data
  myData.bmeTemp = bmeTemp;
  myData.bmeHum = bmeHum;
  myData.pressure = pressure;
  myData.gasResistance = gasResistance;
  myData.bmealtitude = bmealtitude;
  myData.temperature = temperature;
  myData.humidity = humidity;
  myData.ph = ph;
  myData.ppm = ppm;
  myData.dsTemp = dsTemp;
  myData.oxyValue = oxyValue;
  myData.ph2 = ph2;
  myData.ppm2 = ppm2;
  myData.dsTemp2 = dsTemp2;
  myData.oxyValue2 = oxyValue2;
  
  

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
  }
  else {
    Serial.println("Sending error");
  }
  

  
  

  delay(1000);
}
