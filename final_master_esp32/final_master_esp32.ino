// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID "TMPL2uC70yQrK"
#define BLYNK_TEMPLATE_NAME "IoT Automation"
#define BLYNK_AUTH_TOKEN "BoG8PxCHUGoCzGDiIvoc19-JX3Y-pEG9"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial



#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


// For aircondition section
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>


const uint16_t kIrLed = 13;
IRsend irsend(kIrLed);

uint16_t rawData[147] = {10212, 5066,  622, 444,  622, 1924,  598, 444,  622, 1924,  600, 444,  620, 1924,  602, 444,  620, 1924,  622, 1924,  600, 444,  600, 442,  600, 442,  602, 440,  602, 442,  620, 1926,  620, 1926,  602, 440,  602, 442,  602, 442,  602, 442,  600, 442,  600, 442,  600, 444,  598, 444,  600, 442,  600, 444,  600, 442,  602, 440,  602, 442,  620, 1926,  620, 1924,  622, 1924,  602, 442,  600, 444,  602, 440,  602, 440,  600, 444,  600, 444,  602, 442,  602, 440,  622, 1922,  602, 442,  602, 442,  600, 444,  602, 442,  600, 442,  602, 440,  600, 444,  600, 442,  600, 444,  602, 442,  620, 1924,  602, 442,  602, 442,  622, 1924,  600, 444,  602, 442,  600, 442,  600, 442,  602, 440,  602, 440,  602, 442,  602, 440,  600, 444,  604, 438,  620, 1926,  620, 1926,  620, 1924,  600, 444,  600, 444,  602, 440,  620, 1924,  600};

uint16_t rawData_Off[197] = {10156, 5122,  610, 458,  588, 1956,  588, 456,  590, 1956,  588, 456,  586, 1958,  590, 454,  588, 1956,  588, 456,  588, 454,  588, 454,  586, 458,  588, 454,  586, 458,  588, 1956,  588, 1958,  588, 454,  588, 454,  590, 454,  588, 454,  568, 474,  566, 478,  590, 452,  588, 454,  590, 454,  586, 456,  590, 454,  590, 454,  588, 454,  588, 1956,  590, 1956,  588, 1956,  566, 478,  564, 478,  588, 454,  590, 454,  590, 454,  588, 454,  590, 452,  588, 456,  586, 1958,  590, 454,  588, 456,  588, 452,  590, 454,  586, 422,  600, 476,  566, 480,  586, 458,  562, 478,  592, 452,  590, 1958,  588, 454,  588, 458,  588, 1956,  588, 454,  588, 456,  590, 454,  588, 454,  588, 454,  590, 452,  588, 456,  586, 458,  588, 454,  590, 1956,  590, 1956,  590, 1956,  590, 1956,  590, 452,  590, 454,  588, 454,  588, 1956,  588};

bool Button_on;
bool Button_off;

// Grnimation control fan

#define fanPin 35

// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
char auth[] = BLYNK_AUTH_TOKEN;


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Freedom_71";
char pass[] = "bolajabena";



BlynkTimer timer;



#define pump1 15
#define pump2 2
#define pump3 4
#define pump4 16
#define pump5 17
#define pump6 5
#define pump7 18
#define pump8 19
#define pump9 21


#define light1 3
#define light2 1
#define light3 22
#define light4 23
#define light5 34
#define light6 12
#define light7 14
#define light8 27
#define light9 26
#define light10 25
#define light11 33


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
float dsTemp2;
float oxyValue2;

// Data used for Air condition
int acValue;





// Define a data structure for received data from the slave esp32
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


// Airconditioner automation
String rxChar;

IRsendRaw mySender;

int x = 0;

#define RAW_DATA_LEN 140
uint16_t rawDataOn[RAW_DATA_LEN] = {
  8910, 4542, 522, 674, 522, 674, 518, 674,
  522, 1766, 522, 1762, 522, 674, 522, 674,
  522, 674, 522, 1766, 522, 670, 522, 674,
  526, 1762, 522, 674, 522, 674, 522, 674,
  518, 678, 518, 674, 522, 674, 522, 674,
  518, 678, 518, 678, 522, 1762, 522, 674,
  522, 674, 522, 674, 518, 674, 522, 674,
  522, 674, 522, 1766, 518, 674, 522, 1766,
  522, 674, 522, 674, 518, 1770, 518, 674,
  522, 20058, 518, 682, 518, 678, 518, 674,
  522, 674, 522, 674, 518, 678, 518, 674,
  522, 674, 490, 702, 522, 674, 522, 674,
  518, 678, 518, 678, 518, 1766, 518, 678,
  518, 674, 522, 674, 522, 674, 518, 678,
  518, 674, 522, 674, 522, 674, 518, 678,
  518, 678, 518, 674, 518, 678, 518, 678,
  518, 674, 522, 1766, 518, 678, 522, 1766,
  518, 1766, 522, 1000
};



uint16_t rawDataOff[RAW_DATA_LEN] = {
  8934, 4518, 490, 706, 490, 702, 522, 674,
  518, 678, 518, 1766, 522, 678, 518, 674,
  518, 678, 522, 1766, 518, 674, 518, 678,
  518, 1770, 518, 674, 518, 678, 490, 706,
  518, 674, 518, 678, 518, 678, 518, 674,
  518, 678, 518, 678, 518, 1766, 518, 678,
  518, 674, 518, 678, 518, 678, 518, 678,
  518, 678, 518, 1766, 518, 678, 518, 1766,
  518, 678, 514, 682, 518, 1766, 518, 678,
  518, 20058, 518, 682, 518, 678, 518, 678,
  518, 674, 518, 678, 518, 678, 518, 674,
  522, 674, 518, 678, 518, 678, 518, 678,
  490, 706, 518, 674, 518, 1770, 518, 678,
  518, 678, 518, 678, 518, 678, 518, 674,
  518, 678, 518, 678, 518, 678, 518, 678,
  518, 678, 518, 678, 518, 678, 518, 674,
  518, 678, 518, 1770, 518, 678, 518, 1770,
  518, 678, 514, 1000
};






uint16_t rawData16[RAW_DATA_LEN] = {
  8850, 4566, 514, 686, 482, 710, 514, 1770,
  494, 1794, 486, 1798, 486, 710, 514, 682,
  510, 682, 514, 678, 514, 686, 510, 682,
  486, 710, 486, 706, 490, 706, 514, 678,
  514, 682, 486, 710, 510, 686, 482, 710,
  510, 710, 486, 682, 514, 1770, 514, 682,
  514, 682, 510, 682, 514, 682, 486, 706,
  486, 710, 510, 1774, 486, 734, 462, 1794,
  490, 706, 514, 682, 510, 1770, 514, 682,
  514, 20070, 490, 686, 486, 706, 514, 682,
  514, 678, 514, 682, 486, 734, 486, 706,
  462, 706, 514, 682, 486, 710, 510, 682,
  510, 682, 486, 710, 514, 1770, 514, 682,
  486, 706, 514, 682, 510, 682, 514, 682,
  486, 706, 514, 682, 486, 710, 510, 682,
  486, 710, 510, 682, 486, 710, 510, 686,
  510, 682, 514, 682, 486, 706, 490, 706,
  514, 1770, 486, 1000
};




uint16_t rawData17[RAW_DATA_LEN] = {
  8874, 4574, 514, 682, 514, 678, 518, 1770,
  486, 1798, 490, 1798, 514, 682, 490, 706,
  514, 682, 514, 1770, 518, 678, 514, 682,
  514, 678, 514, 682, 490, 706, 498, 694,
  514, 682, 514, 682, 542, 650, 514, 682,
  514, 678, 518, 678, 514, 1774, 514, 678,
  514, 682, 514, 682, 514, 678, 518, 678,
  486, 706, 518, 1770, 486, 710, 510, 1774,
  514, 678, 490, 706, 486, 1798, 518, 678,
  486, 20082, 510, 690, 514, 682, 514, 682,
  510, 682, 514, 682, 486, 710, 514, 678,
  490, 706, 486, 706, 514, 678, 518, 678,
  546, 650, 514, 682, 486, 1798, 514, 678,
  518, 678, 486, 706, 490, 706, 514, 682,
  514, 678, 514, 678, 490, 706, 518, 678,
  510, 686, 510, 682, 514, 682, 514, 682,
  486, 706, 486, 1798, 490, 706, 514, 678,
  486, 1802, 486, 1000
};




uint16_t rawData18[RAW_DATA_LEN] = {
  8902, 4542, 490, 710, 486, 706, 518, 1770,
  486, 1798, 514, 1770, 514, 682, 514, 682,
  514, 682, 510, 686, 514, 1766, 546, 650,
  518, 678, 514, 682, 514, 682, 486, 706,
  514, 678, 514, 682, 514, 682, 486, 710,
  514, 678, 518, 678, 486, 1798, 518, 678,
  490, 706, 518, 678, 486, 706, 514, 682,
  486, 710, 514, 1770, 490, 706, 486, 1798,
  514, 682, 514, 682, 510, 1774, 514, 678,
  518, 20070, 466, 710, 514, 678, 490, 706,
  490, 706, 514, 678, 486, 710, 490, 706,
  510, 682, 490, 706, 514, 678, 514, 678,
  490, 706, 518, 678, 486, 1798, 486, 706,
  514, 678, 518, 678, 486, 706, 490, 706,
  486, 706, 514, 682, 486, 706, 514, 682,
  510, 678, 514, 682, 490, 702, 518, 678,
  514, 682, 514, 678, 514, 1770, 518, 678,
  514, 1770, 490, 1000
};





uint16_t rawData19[RAW_DATA_LEN] = {
  8882, 4550, 486, 710, 514, 682, 486, 1798,
  514, 1774, 514, 1774, 486, 710, 486, 710,
  510, 686, 510, 1778, 510, 1802, 462, 710,
  482, 714, 514, 682, 506, 686, 514, 682,
  510, 686, 514, 678, 490, 706, 490, 706,
  486, 738, 462, 706, 486, 1802, 482, 714,
  482, 710, 510, 686, 486, 710, 510, 682,
  486, 710, 486, 1802, 510, 710, 486, 1778,
  510, 686, 482, 710, 510, 1778, 514, 706,
  486, 20090, 462, 718, 514, 682, 510, 682,
  486, 710, 510, 686, 510, 682, 486, 710,
  486, 734, 462, 710, 486, 706, 510, 710,
  486, 686, 482, 734, 462, 1802, 486, 710,
  482, 710, 486, 710, 486, 710, 486, 706,
  486, 738, 482, 686, 486, 710, 486, 706,
  486, 710, 486, 734, 458, 710, 510, 686,
  510, 710, 486, 1770, 490, 1822, 462, 734,
  462, 1798, 510, 1000
};




uint16_t rawData20[RAW_DATA_LEN] = {
  8906, 4542, 518, 682, 514, 678, 518, 1770,
  514, 1770, 518, 1766, 518, 678, 514, 682,
  514, 678, 518, 678, 514, 682, 486, 1798,
  514, 682, 514, 682, 486, 706, 514, 682,
  514, 678, 490, 706, 514, 682, 514, 682,
  486, 706, 518, 678, 514, 1774, 486, 706,
  490, 706, 514, 678, 490, 706, 514, 682,
  486, 710, 486, 1798, 490, 706, 514, 1770,
  514, 682, 518, 674, 518, 1770, 486, 706,
  518, 20058, 514, 682, 518, 678, 490, 706,
  514, 682, 510, 682, 514, 682, 510, 682,
  514, 682, 486, 706, 486, 710, 514, 678,
  514, 682, 514, 682, 510, 1774, 514, 678,
  514, 682, 510, 686, 486, 706, 514, 682,
  510, 682, 542, 654, 514, 678, 486, 710,
  514, 678, 514, 682, 486, 710, 510, 682,
  486, 710, 514, 678, 486, 710, 486, 1798,
  490, 1798, 486, 1000
};








uint16_t rawData21[RAW_DATA_LEN] = {
  8906, 4542, 522, 678, 514, 682, 514, 1774,
  514, 1770, 518, 1770, 518, 678, 518, 678,
  514, 678, 518, 1770, 518, 678, 518, 1766,
  518, 678, 518, 678, 490, 706, 518, 678,
  514, 682, 514, 678, 518, 678, 518, 674,
  518, 682, 514, 682, 518, 1766, 518, 678,
  514, 682, 514, 678, 518, 678, 518, 678,
  514, 678, 518, 1770, 514, 678, 518, 1770,
  518, 678, 514, 678, 518, 1770, 518, 674,
  518, 20058, 542, 658, 518, 678, 514, 682,
  514, 678, 490, 706, 518, 678, 518, 678,
  514, 678, 518, 678, 518, 678, 514, 682,
  514, 678, 518, 678, 518, 1770, 514, 678,
  518, 678, 518, 678, 514, 682, 514, 678,
  514, 682, 518, 674, 518, 678, 490, 706,
  518, 678, 514, 678, 518, 678, 518, 678,
  514, 678, 518, 1770, 518, 678, 514, 1774,
  514, 1770, 518, 1000
};






uint16_t rawData22[RAW_DATA_LEN] = {
  8902, 4518, 490, 734, 486, 686, 482, 1806,
  482, 1802, 510, 1798, 514, 658, 510, 686,
  506, 686, 486, 734, 462, 1798, 514, 1774,
  486, 710, 486, 734, 458, 738, 458, 710,
  486, 734, 486, 710, 458, 714, 482, 734,
  486, 686, 482, 714, 482, 1802, 510, 686,
  486, 706, 542, 654, 486, 710, 510, 686,
  510, 682, 490, 1798, 510, 686, 482, 1802,
  486, 710, 486, 734, 486, 1774, 490, 706,
  486, 20114, 486, 686, 490, 706, 514, 682,
  514, 682, 510, 682, 494, 702, 510, 686,
  510, 682, 542, 654, 510, 682, 514, 682,
  510, 682, 486, 714, 510, 1770, 514, 706,
  486, 682, 486, 710, 486, 710, 538, 658,
  510, 682, 514, 682, 486, 710, 510, 686,
  486, 706, 542, 654, 514, 682, 510, 686,
  482, 710, 486, 710, 510, 1774, 486, 1802,
  486, 1802, 514, 1000
};




uint16_t rawData23[RAW_DATA_LEN] = {
  8878, 4546, 486, 714, 506, 710, 458, 1802,
  514, 1774, 482, 1802, 486, 734, 490, 678,
  514, 710, 482, 1778, 514, 1774, 510, 1802,
  462, 706, 486, 710, 514, 682, 510, 686,
  486, 706, 514, 686, 534, 658, 486, 706,
  542, 654, 514, 682, 514, 1774, 482, 738,
  486, 682, 514, 706, 490, 678, 518, 706,
  486, 682, 486, 1806, 482, 710, 514, 1774,
  482, 738, 486, 686, 514, 1770, 486, 710,
  482, 20118, 490, 690, 510, 682, 486, 710,
  514, 678, 514, 686, 486, 734, 458, 714,
  510, 682, 486, 710, 482, 710, 514, 682,
  510, 710, 458, 710, 486, 1802, 510, 682,
  514, 682, 514, 682, 482, 714, 510, 682,
  514, 682, 510, 710, 486, 682, 522, 674,
  514, 682, 510, 682, 542, 654, 514, 706,
  486, 710, 486, 1770, 490, 1798, 486, 1826,
  462, 1798, 514, 1000
};






uint16_t rawData24[RAW_DATA_LEN] = {
  8902, 4546, 486, 714, 482, 710, 486, 1798,
  514, 1774, 494, 1790, 514, 682, 486, 710,
  510, 682, 514, 706, 490, 682, 514, 682,
  510, 1774, 486, 710, 486, 706, 486, 710,
  514, 682, 510, 682, 514, 682, 514, 682,
  514, 682, 486, 710, 482, 1802, 510, 686,
  510, 682, 486, 710, 514, 682, 486, 706,
  486, 710, 486, 1802, 510, 682, 490, 1798,
  486, 710, 510, 682, 490, 1798, 514, 682,
  486, 20110, 490, 690, 538, 654, 514, 682,
  486, 710, 486, 706, 510, 686, 510, 682,
  490, 710, 482, 710, 514, 682, 486, 706,
  510, 686, 522, 698, 462, 1798, 510, 686,
  486, 710, 482, 714, 510, 682, 510, 682,
  514, 682, 486, 710, 514, 678, 514, 682,
  490, 730, 486, 686, 510, 682, 514, 682,
  486, 710, 486, 706, 514, 686, 510, 682,
  486, 710, 486, 1000
};






uint16_t rawData25[RAW_DATA_LEN] = {
  8874, 4546, 490, 710, 482, 710, 486, 1802,
  510, 1774, 510, 1774, 510, 686, 510, 686,
  482, 710, 514, 1774, 482, 710, 486, 710,
  510, 1774, 514, 682, 486, 710, 510, 682,
  486, 710, 482, 710, 514, 686, 482, 734,
  482, 686, 510, 686, 482, 1802, 514, 706,
  486, 682, 510, 686, 510, 710, 486, 706,
  462, 710, 510, 1774, 510, 682, 514, 1774,
  510, 710, 486, 682, 510, 1778, 482, 734,
  462, 20110, 490, 690, 482, 710, 486, 710,
  510, 682, 514, 706, 486, 686, 510, 682,
  486, 710, 482, 710, 486, 710, 486, 734,
  486, 710, 486, 686, 510, 1774, 482, 738,
  486, 682, 510, 710, 486, 686, 510, 710,
  482, 710, 486, 686, 510, 682, 486, 710,
  510, 706, 486, 686, 486, 710, 510, 706,
  462, 734, 458, 1802, 510, 686, 510, 682,
  510, 686, 482, 1000
};






uint16_t rawData26[RAW_DATA_LEN] = {
  8878, 4546, 514, 682, 510, 686, 486, 1798,
  514, 1770, 514, 1774, 514, 678, 518, 678,
  514, 682, 514, 682, 514, 1770, 514, 682,
  486, 1798, 518, 678, 514, 682, 486, 706,
  514, 682, 514, 682, 514, 682, 514, 678,
  518, 678, 518, 678, 514, 1770, 518, 678,
  514, 682, 514, 682, 486, 706, 542, 654,
  514, 682, 514, 1770, 510, 682, 490, 1798,
  486, 710, 538, 654, 514, 1774, 514, 678,
  514, 20074, 490, 690, 486, 706, 486, 710,
  510, 682, 514, 682, 486, 706, 490, 706,
  510, 682, 514, 682, 514, 682, 486, 706,
  514, 682, 486, 706, 490, 1798, 514, 678,
  486, 710, 486, 706, 514, 682, 514, 678,
  514, 682, 514, 682, 514, 682, 486, 706,
  486, 710, 486, 706, 514, 682, 486, 710,
  486, 706, 514, 682, 514, 1770, 486, 710,
  514, 678, 486, 1000
};
//
//
//
//

uint16_t rawData27[RAW_DATA_LEN] = {
  8882, 4546, 514, 682, 514, 682, 514, 1770,
  514, 1778, 482, 1802, 486, 734, 462, 710,
  510, 682, 514, 1770, 514, 1778, 510, 706,
  490, 1770, 514, 710, 486, 682, 510, 686,
  510, 686, 482, 710, 514, 682, 514, 682,
  514, 682, 486, 710, 510, 1802, 462, 706,
  486, 710, 514, 682, 510, 686, 482, 714,
  482, 710, 510, 1778, 482, 738, 486, 1774,
  486, 710, 486, 710, 506, 1778, 514, 682,
  510, 20086, 462, 742, 458, 710, 514, 682,
  514, 706, 486, 686, 510, 682, 486, 734,
  462, 706, 486, 710, 486, 710, 482, 714,
  482, 710, 510, 686, 514, 1770, 486, 734,
  462, 710, 486, 710, 510, 682, 514, 682,
  510, 710, 486, 710, 486, 686, 482, 734,
  490, 682, 510, 682, 514, 706, 486, 686,
  510, 710, 486, 1770, 498, 1794, 482, 710,
  510, 706, 462, 1000
};
//
//
//
//

uint16_t rawData28[RAW_DATA_LEN] = {
  8874, 4554, 510, 682, 514, 682, 486, 1802,
  514, 1770, 518, 1770, 510, 686, 514, 678,
  514, 686, 510, 682, 490, 702, 514, 1774,
  486, 1798, 514, 682, 486, 710, 510, 682,
  486, 710, 514, 678, 514, 682, 542, 654,
  510, 686, 510, 706, 490, 1770, 514, 682,
  514, 682, 482, 710, 514, 682, 510, 686,
  486, 706, 514, 1770, 514, 686, 506, 1774,
  514, 682, 486, 710, 482, 1802, 514, 678,
  514, 20078, 490, 686, 514, 682, 514, 678,
  514, 682, 486, 734, 486, 682, 542, 678,
  490, 706, 458, 734, 486, 686, 510, 682,
  514, 682, 486, 710, 482, 1802, 510, 710,
  486, 682, 510, 686, 510, 682, 486, 714,
  510, 682, 510, 710, 486, 682, 514, 682,
  514, 678, 542, 654, 514, 682, 486, 710,
  486, 734, 458, 710, 514, 682, 486, 1798,
  514, 682, 486, 1000
};
//
//
//
//
//

uint16_t rawData29[RAW_DATA_LEN] = {
  8878, 4550, 510, 710, 458, 738, 458, 1802,
  510, 1774, 514, 1774, 510, 710, 458, 738,
  486, 706, 458, 1830, 486, 710, 458, 1802,
  510, 1778, 482, 738, 486, 682, 510, 710,
  486, 710, 486, 706, 490, 706, 458, 714,
  486, 734, 458, 734, 458, 1830, 486, 710,
  482, 710, 486, 710, 482, 714, 458, 710,
  482, 738, 486, 1778, 482, 734, 458, 1802,
  514, 682, 486, 710, 510, 1802, 510, 658,
  510, 20082, 490, 690, 482, 734, 486, 710,
  458, 714, 482, 734, 458, 738, 462, 730,
  462, 714, 510, 706, 458, 738, 462, 710,
  482, 734, 462, 734, 486, 1798, 486, 710,
  486, 710, 486, 686, 482, 734, 486, 682,
  514, 706, 486, 686, 510, 682, 510, 710,
  462, 710, 510, 710, 458, 734, 486, 710,
  458, 710, 486, 1802, 514, 706, 462, 1806,
  482, 710, 510, 1000
};
//
//
//
//

uint16_t rawData30[RAW_DATA_LEN] = {
  8906, 4546, 514, 682, 490, 702, 518, 1770,
  518, 1766, 518, 1770, 518, 678, 518, 678,
  514, 678, 518, 678, 518, 1770, 514, 1774,
  514, 1770, 514, 682, 514, 678, 518, 678,
  518, 678, 514, 678, 518, 678, 518, 678,
  518, 678, 486, 706, 518, 1770, 518, 678,
  490, 706, 514, 678, 518, 678, 518, 678,
  542, 650, 514, 1774, 514, 682, 490, 1794,
  490, 706, 518, 678, 486, 1798, 514, 682,
  514, 20058, 510, 686, 514, 682, 490, 702,
  490, 706, 514, 678, 518, 678, 518, 674,
  518, 678, 490, 706, 486, 710, 486, 706,
  514, 682, 514, 678, 490, 1798, 490, 706,
  514, 678, 514, 686, 514, 678, 514, 682,
  490, 706, 514, 678, 518, 678, 514, 682,
  490, 706, 486, 710, 486, 706, 514, 682,
  514, 682, 514, 682, 510, 1774, 514, 1774,
  486, 706, 514, 1000
};





uint16_t rawDatam1[RAW_DATA_LEN] = {                    ///Internal
  8898, 4546, 514, 702, 518, 678, 518, 678,
  518, 1746, 542, 1770, 518, 678, 518, 678,
  518, 678, 518, 1770, 518, 678, 518, 678,
  518, 1770, 514, 682, 518, 674, 518, 682,
  514, 678, 518, 678, 518, 678, 518, 678,
  518, 678, 518, 678, 514, 1774, 518, 674,
  518, 682, 514, 678, 518, 682, 514, 678,
  518, 678, 518, 1770, 518, 682, 514, 1770,
  518, 682, 518, 678, 518, 1770, 518, 678,
  490, 20102, 522, 682, 518, 678, 518, 678,
  514, 678, 518, 678, 518, 678, 518, 678,
  518, 678, 518, 678, 518, 678, 518, 678,
  518, 678, 514, 682, 514, 1774, 518, 678,
  518, 678, 518, 678, 514, 682, 518, 678,
  518, 678, 518, 674, 518, 682, 514, 678,
  518, 678, 518, 678, 518, 678, 518, 678,
  518, 678, 518, 1770, 518, 682, 514, 1770,
  518, 1770, 518, 1000
};


uint16_t rawDatam2[RAW_DATA_LEN] = {                         //// cooling
  8922, 4542, 518, 1774, 514, 678, 518, 682,
  490, 1798, 514, 1778, 514, 682, 490, 706,
  522, 674, 490, 706, 518, 678, 514, 682,
  514, 1774, 490, 706, 490, 706, 506, 690,
  514, 682, 518, 678, 494, 702, 518, 678,
  518, 678, 514, 682, 514, 1774, 490, 706,
  522, 674, 494, 702, 490, 706, 514, 682,
  514, 682, 514, 1770, 518, 678, 518, 1774,
  514, 678, 494, 702, 518, 1770, 518, 678,
  518, 20074, 518, 682, 522, 674, 518, 678,
  518, 654, 538, 682, 490, 706, 498, 698,
  490, 706, 490, 702, 498, 702, 514, 678,
  518, 682, 490, 702, 494, 1794, 518, 682,
  514, 678, 518, 678, 522, 674, 494, 702,
  518, 678, 490, 710, 514, 678, 518, 678,
  494, 702, 522, 674, 494, 702, 490, 706,
  494, 702, 494, 1794, 490, 706, 494, 1798,
  514, 1774, 514, 1000
};



uint16_t rawDatam3[RAW_DATA_LEN] = {                     ///// Droplet
  8914, 4546, 490, 706, 490, 1798, 490, 706,
  490, 1798, 490, 1798, 514, 682, 514, 682,
  514, 682, 514, 682, 490, 706, 514, 682,
  514, 1770, 494, 706, 490, 706, 490, 706,
  490, 706, 490, 706, 518, 678, 490, 706,
  490, 706, 490, 706, 494, 1794, 518, 678,
  490, 706, 518, 678, 514, 682, 490, 706,
  490, 706, 490, 1798, 490, 706, 490, 1798,
  490, 706, 490, 706, 490, 1798, 490, 706,
  510, 20082, 514, 686, 518, 678, 498, 698,
  490, 706, 518, 678, 490, 706, 490, 702,
  518, 682, 514, 678, 494, 706, 514, 678,
  494, 706, 490, 706, 490, 1794, 518, 682,
  490, 702, 494, 702, 498, 698, 518, 678,
  494, 702, 490, 706, 518, 678, 514, 682,
  490, 706, 490, 706, 490, 706, 514, 682,
  514, 678, 494, 702, 518, 1770, 494, 1794,
  518, 1774, 514, 1000
};




uint16_t rawDatam4[RAW_DATA_LEN] = {                          /////Fan
  8906, 4550, 514, 1774, 498, 1790, 514, 682,
  514, 1774, 490, 1798, 514, 682, 514, 682,
  490, 706, 514, 682, 514, 682, 490, 706,
  514, 1778, 510, 682, 514, 682, 486, 710,
  514, 682, 514, 682, 514, 682, 514, 682,
  486, 710, 486, 710, 486, 1802, 514, 682,
  514, 682, 514, 682, 514, 682, 486, 710,
  490, 706, 514, 1774, 514, 682, 486, 1802,
  486, 710, 486, 710, 514, 1774, 514, 682,
  514, 20106, 490, 690, 514, 682, 486, 710,
  514, 678, 518, 682, 486, 710, 514, 682,
  514, 682, 486, 710, 514, 682, 514, 682,
  486, 710, 514, 682, 514, 1774, 486, 710,
  486, 710, 514, 682, 486, 710, 514, 682,
  514, 682, 514, 682, 514, 682, 514, 682,
  486, 710, 486, 710, 514, 682, 486, 710,
  486, 710, 486, 1802, 486, 1802, 514, 1774,
  486, 1802, 486, 1000
};






uint16_t rawDatam5[RAW_DATA_LEN] = {                      ///Sun
  8914, 4542, 518, 678, 518, 678, 514, 1774,
  514, 1774, 514, 1770, 490, 706, 518, 678,
  518, 678, 490, 706, 514, 682, 514, 682,
  514, 1770, 518, 678, 518, 678, 514, 682,
  514, 682, 486, 710, 514, 682, 514, 682,
  514, 678, 518, 678, 490, 1798, 514, 682,
  518, 678, 518, 678, 514, 682, 518, 678,
  518, 678, 514, 1774, 490, 706, 514, 1774,
  514, 682, 514, 682, 518, 1770, 490, 706,
  518, 20074, 514, 686, 514, 682, 490, 706,
  518, 678, 518, 678, 518, 682, 514, 678,
  518, 678, 518, 678, 518, 678, 514, 682,
  514, 682, 514, 682, 514, 1774, 514, 682,
  518, 678, 514, 682, 518, 678, 514, 682,
  514, 682, 490, 706, 514, 682, 514, 682,
  514, 682, 518, 678, 514, 682, 518, 678,
  518, 678, 518, 678, 518, 678, 518, 682,
  514, 678, 518, 1000
};





// Callback function executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("BME Temperature: ");
  bmeTemp = myData.bmeTemp;
  Serial.println(bmeTemp);
  Serial.print("BME Humidity: ");
  bmeHum = myData.bmeHum;
  Serial.println(bmeHum);
  Serial.print("BME pressure: ");
  pressure = myData.pressure;
  Serial.println(pressure);
  Serial.print("BME gasResistance: ");
  gasResistance = myData.gasResistance;
  Serial.println(gasResistance);
  Serial.print("BME bmealtitude: ");
  bmealtitude = myData.bmealtitude;
  Serial.println(bmealtitude);
  Serial.print("DHT22 Temperature: ");
  temperature = myData.temperature;
  Serial.println(temperature);
  Serial.print("DHT22 Humidity: ");
  humidity = myData.humidity;
  Serial.println(humidity);
  Serial.print("PH sensor: ");
  ph = myData.ph;
  Serial.println(ph);
  Serial.print("PH PPM: ");
  ppm = myData.ppm;
  Serial.println(ppm);
  Serial.print("DS Temperature: ");
  dsTemp = myData.dsTemp;
  Serial.println(dsTemp);
  Serial.print("Desolve Oxygen Value: ");
  oxyValue = myData.oxyValue;
  Serial.println(oxyValue);
  Serial.print("PH2 sensor: ");
  ph2 = myData.ph2;
  Serial.println(ph2);
  Serial.print("PH2 PPM: ");
  ppm2 = myData.ppm2;
  Serial.println(ppm2);
  Serial.print("DS2 Temperature: ");
  dsTemp2 = myData.dsTemp2;
  Serial.println(dsTemp2);
  Serial.print("Desolve Oxygen Value 2: ");
  oxyValue2 = myData.oxyValue2;
  Serial.println(oxyValue2);


  Serial.println();
}




// Pump section controlling

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  digitalWrite(pump1, pinValue);
}
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  digitalWrite(pump2, pinValue);
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  digitalWrite(pump3, pinValue);
}

BLYNK_WRITE(V3)
{
  int pinValue = param.asInt();
  digitalWrite(pump4, pinValue);
}

BLYNK_WRITE(V4)
{
  int pinValue = param.asInt();
  digitalWrite(pump5, pinValue);
}

BLYNK_WRITE(V5)
{
  int pinValue = param.asInt();
  digitalWrite(pump6, pinValue);
}

BLYNK_WRITE(V6)
{
  int pinValue = param.asInt();
  digitalWrite(pump7, pinValue);
}

BLYNK_WRITE(V7)
{
  int pinValue = param.asInt();
  digitalWrite(pump8, pinValue);
}
BLYNK_WRITE(V8)
{
  int pinValue = param.asInt();
  digitalWrite(pump9, pinValue);
}


// Light Section Controlling
BLYNK_WRITE(V9)
{
  int pinValue = param.asInt();
  digitalWrite(light1, pinValue);
}
BLYNK_WRITE(V10)
{
  int pinValue = param.asInt();
  digitalWrite(light2, pinValue);
}
BLYNK_WRITE(V11)
{
  int pinValue = param.asInt();
  digitalWrite(light3, pinValue);
}
BLYNK_WRITE(V12)
{
  int pinValue = param.asInt();
  digitalWrite(light4, pinValue);
}
BLYNK_WRITE(V13)
{
  int pinValue = param.asInt();
  digitalWrite(light5, pinValue);
}
BLYNK_WRITE(V14)
{
  int pinValue = param.asInt();
  digitalWrite(light6, pinValue);
}
BLYNK_WRITE(V15)
{
  int pinValue = param.asInt();
  digitalWrite(light7, pinValue);
}
BLYNK_WRITE(V16)
{
  int pinValue = param.asInt();
  digitalWrite(light8, pinValue);
}
BLYNK_WRITE(V17)
{
  int pinValue = param.asInt();
  digitalWrite(light9, pinValue);
}
BLYNK_WRITE(V18)
{
  int pinValue = param.asInt();
  digitalWrite(light10, pinValue);
}

BLYNK_WRITE(V19)
{
  int pinValue = param.asInt();
  digitalWrite(light11, pinValue);
}

BLYNK_WRITE(V36)
{
  int pinValue = param.asInt();
  digitalWrite(fanPin, pinValue);
}



// For airconditionar section

BLYNK_WRITE(V20) {
  if (param.asInt() == 1)
    Button_on = true;
  else
    Button_on = false;


  if (param.asInt() == 0)
    Button_off = true;
  else
    Button_off = false;



}

BLYNK_WRITE(V37)
{
  rxChar = param.asInt();
}








void setup()
{

  // Debug console
  Serial.begin(115200);

  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(pump3, OUTPUT);
  pinMode(pump4, OUTPUT);
  pinMode(pump5, OUTPUT);
  pinMode(pump6, OUTPUT);
  pinMode(pump7, OUTPUT);
  pinMode(pump8, OUTPUT);
  pinMode(pump9, OUTPUT);

  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);
  pinMode(light4, OUTPUT);
  pinMode(light5, OUTPUT);
  pinMode(light6, OUTPUT);
  pinMode(light7, OUTPUT);
  pinMode(light8, OUTPUT);
  pinMode(light9, OUTPUT);
  pinMode(light10, OUTPUT);
  pinMode(light11, OUTPUT);

  pinMode(fanPin, OUTPUT);


  digitalWrite(fanPin, LOW);
  digitalWrite(pump1, LOW);
  digitalWrite(pump2, LOW);
  digitalWrite(pump3, LOW);
  digitalWrite(pump4, LOW);
  digitalWrite(pump5, LOW);
  digitalWrite(pump6, LOW);
  digitalWrite(pump7, LOW);
  digitalWrite(pump8, LOW);
  digitalWrite(pump9, LOW);

  digitalWrite(light1, LOW);
  digitalWrite(light2, LOW);
  digitalWrite(light3, LOW);
  digitalWrite(light4, LOW);
  digitalWrite(light5, LOW);
  digitalWrite(light6, LOW);
  digitalWrite(light7, LOW);
  digitalWrite(light8, LOW);
  digitalWrite(light9, LOW);
  digitalWrite(light10, LOW);
  digitalWrite(light11, LOW);




  Blynk.begin(auth, ssid, pass);

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);

  // IR send begin for Air conditioner controlling
  irsend.begin();


}

void controlAC() {

  if (Button_on == true) {
    irsend.sendRaw(rawData, 197, 38);
    delay(500);
  }

  if (Button_off == true) {
    irsend.sendRaw(rawData_Off, 197, 38);
  }


  if (temperature >= 25) {
    irsend.sendRaw(rawData, 197, 38);
  }
  else if (temperature <= 15) {
    irsend.sendRaw( rawData_Off, 197, 38);
  }

}

void AutomaticPumpControl() {

  if (ppm < 200) {
    digitalWrite(pump1, HIGH);
  }
  else {
    digitalWrite(pump1, LOW);
  }

  if (ppm2 > 200 && ppm2 < 400) {
    digitalWrite(pump2, HIGH);
  }
  else {
    digitalWrite(pump2, LOW);
  }


  if ( ph < 4 ) {
    digitalWrite(pump1, HIGH);

  }
  else {
    digitalWrite(pump1, LOW);
  }

  if ( ph2 < 4 ) {
    digitalWrite(pump2, HIGH);
  }
  else {
  }






}

void automaticAcControl() {


  if (rxChar == "16") {
    mySender.send(rawData17, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    //   Serial.println("16");
  }
  //
  //
  //
  //
  //
  if (rxChar == "17") {
    mySender.send(rawData17, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    // Serial.println("17");
  }
  //
  //
  //
  //
  if (rxChar == "18") {
    mySender.send(rawData18, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    Serial.println("18");
  }
  //
  //
  //


  if (rxChar == "19") {
    mySender.send(rawData19, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    Serial.println("19");
  }




  if (rxChar == "20") {
    mySender.send(rawData20, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency

  }




  if (rxChar == "21") {
    mySender.send(rawData21, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency

  }



  if (rxChar == "22") {
    mySender.send(rawData22, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
  }
  ////
  ////
  ////
  ////
  if (rxChar == "23") {
    mySender.send(rawData23, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
  }


  if (rxChar == "24") {
    mySender.send(rawData24, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
  }




  if (rxChar == "25") {
    mySender.send(rawData25, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    Serial.println("25");
  }
  //
  //
  //
  if (rxChar == "26") {
    mySender.send(rawData26, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    // Serial.println("26");
  }
  //
  //
  //
  if (rxChar == "27") {
    mySender.send(rawData27, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    //  Serial.println("27");
  }
  //
  //
  //
  //
  if (rxChar == "28") {
    mySender.send(rawData28, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    // Serial.println("28");
  }
  ////
  ////
  //////
  if (rxChar == "29") {
    mySender.send(rawData29, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    //    Serial.println("29");
  }
  //
  //
  //
  //
  if (rxChar == "30") {
    mySender.send(rawData30, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    //  Serial.println("30");
  }

  //
  //    if(rxChar=="m1"){
  //            mySender.send(rawDatam1,RAW_DATA_LEN,40);//Pass the buffer,length, optionally frequency
  //          //  Serial.println("30");
  //  }

  if (rxChar == "m2") {
    mySender.send(rawDatam2, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    //  Serial.println("30");
  }


  if (rxChar == "m3") {
    mySender.send(rawDatam3, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    //  Serial.println("30");
  }



  //      if(rxChar=="m4"){
  //            mySender.send(rawDatam4,RAW_DATA_LEN,40);//Pass the buffer,length, optionally frequency
  //          //  Serial.println("30");
  //  }


  if (rxChar == "m5") {
    mySender.send(rawDatam5, RAW_DATA_LEN, 40); //Pass the buffer,length, optionally frequency
    //  Serial.println("30");
  }
}

void loop()
{


  Blynk.run();

  controlAC();
  automaticAcControl();
  //  AutomaticPumpControl();
  Blynk.virtualWrite(V21, bmeTemp);
  Blynk.virtualWrite(V22, bmeHum);
  Blynk.virtualWrite(V23, pressure);
  Blynk.virtualWrite(V24, gasResistance);
  Blynk.virtualWrite(V25, bmealtitude);
  Blynk.virtualWrite(V26, temperature);
  Blynk.virtualWrite(V27, humidity);
  Blynk.virtualWrite(V28, ph);
  Blynk.virtualWrite(V29, ppm);
  Blynk.virtualWrite(V30, dsTemp);
  Blynk.virtualWrite(V31, oxyValue);
  Blynk.virtualWrite(V32, ph2);
  Blynk.virtualWrite(V33, ppm2);
  Blynk.virtualWrite(V34, dsTemp2);
  Blynk.virtualWrite(V35, oxyValue2);


  delay(1000);
}
