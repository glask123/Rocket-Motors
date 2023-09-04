#include <Adafruit_NAU7802.h>
#include <SPI.h>
#include <SD.h>

Adafruit_NAU7802 nau;

enum states {
  PRE,
  READ,
  POST,
};

enum states SYS_STATE;

int bluLED = 5;
int redLED = 7;
int grnLED = 6;

File data_file;

unsigned long start_time = 0;

void setup() {
  Serial.begin(115200);

  pinMode(bluLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(grnLED, OUTPUT);

  digitalWrite(bluLED, LOW);
  digitalWrite(redLED, HIGH);
  digitalWrite(grnLED, HIGH);

  while (! nau.begin()) {
    digitalWrite(bluLED, HIGH);
    digitalWrite(redLED, LOW);
    Serial.println("Failed to find NAU7802, trying again...");
    delay(1000);
  }

  digitalWrite(bluLED, LOW);
  digitalWrite(redLED, HIGH);
  Serial.println("Found NAU7802");

  nau.setLDO(NAU7802_3V0);
  nau.setGain(NAU7802_GAIN_128);
  nau.setRate(NAU7802_RATE_320SPS);

  for (uint8_t i=0; i<10; i++) {
    while (! nau.available()){
      digitalWrite(bluLED, HIGH);
      digitalWrite(redLED, LOW);
      Serial.println("NAU7802 is unavailable, trying again...");
      delay(100);
    } 
    int32_t val = nau.read();
    digitalWrite(bluLED, LOW);
    digitalWrite(redLED, HIGH);
  }

  while (! nau.calibrate(NAU7802_CALMOD_INTERNAL)) {
    Serial.println("Failed to calibrate internal offset, trying again...");
    digitalWrite(bluLED, HIGH);
    digitalWrite(redLED, LOW);
    delay(1000);
  }
  Serial.println("Calibrated internal offset");
  digitalWrite(bluLED, LOW);
  digitalWrite(redLED, HIGH);

  while (! nau.calibrate(NAU7802_CALMOD_OFFSET)) {
    Serial.println("Failed to calibrate system offset, trying again...");
    digitalWrite(bluLED, HIGH);
    digitalWrite(redLED, LOW);
    delay(1000);
  }
  Serial.println("Calibrated system offset");
  digitalWrite(bluLED, LOW);
  digitalWrite(redLED, HIGH);
  /*
  if (!SD.begin(0)) {
    Serial.println("Failed to connect to SD card");
    while (1){
      digitalWrite(bluLED, HIGH);
      digitalWrite(redLED, LOW);
    }
  } else {
    Serial.println("Connected to SD card");
  }
  data_file = SD.open("aug18test2.txt", FILE_WRITE);
  if (data_file) {
    data_file.println("TIME (ms), FORCE (N)");
  } else {
    Serial.println("error opening file");
    digitalWrite(bluLED, HIGH);
    digitalWrite(redLED, LOW);
  }
  */
  digitalWrite(bluLED, LOW);
  digitalWrite(redLED, HIGH);


  digitalWrite(bluLED, HIGH);
  digitalWrite(redLED, HIGH);
  digitalWrite(grnLED, LOW);
    
  for (int z = 0; z < 10; z++){
    digitalWrite(bluLED, LOW);
    digitalWrite(redLED, HIGH);
    digitalWrite(grnLED, LOW);
    delay(500);
    digitalWrite(bluLED, HIGH);
    digitalWrite(redLED, HIGH);
    digitalWrite(grnLED, HIGH);
    delay(500);
  }

  for (int z = 0; z < 5; z++){
    digitalWrite(bluLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(grnLED, LOW);
    delay(500);
    digitalWrite(bluLED, HIGH);
    digitalWrite(redLED, HIGH);
    digitalWrite(grnLED, HIGH);
    delay(500);
  }

  SYS_STATE = READ;
  digitalWrite(bluLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(grnLED, HIGH);
  start_time = millis();
}

int k = 0;

int32_t data_arr[20];
unsigned long time_arr[20];

void loop() {
  if (SYS_STATE == READ){
    while (! nau.available()) {
      delay(1);
    }

    int32_t val = nau.read();
    unsigned long read_time = millis();

    data_arr[k] = val;
    time_arr[k] = read_time;
    

    if (k == 19){
      for (int j = 0; j < 20; j++){
        unsigned long adjusted_time = time_arr[j] - start_time;
        //data_file.print(adjusted_time);
        //data_file.print(", ");
        //data_file.println(data_arr[j]);
        Serial.print(adjusted_time);
        Serial.print(", ");
        Serial.println(data_arr[j]);
      }
      k = 0;
    } else {
      k = k + 1;
    }
    
    Serial.println(val);
    if (millis() - start_time > 5000){
      for (int j = 0; j < k; j++){
        unsigned long adjusted_time = time_arr[j] - start_time;
        //data_file.print(adjusted_time);
        //data_file.print(", ");
        //data_file.println(data_arr[j]);
        Serial.print(adjusted_time);
        Serial.print(", ");
        Serial.println(data_arr[j]);
      }
      SYS_STATE = POST;
      digitalWrite(bluLED, HIGH);
      digitalWrite(redLED, HIGH);
      digitalWrite(grnLED, LOW);
      //data_file.close();
      Serial.println("closed");
    }
  } else {
    
  }
}
