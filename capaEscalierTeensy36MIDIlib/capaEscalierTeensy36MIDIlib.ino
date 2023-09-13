byte pin[11] = { 0, 1, 23, 22, 19, 18, 17, 16, 15, 29, 30 };
int lastcs[11];
int cs[11];
int midiCC[11];
unsigned long lastMillis;
float smooth = 0.75;  // 0.9 - 0.99
unsigned long calib[11];
int i;
//#define DEBUG // SERIAL DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

void setup() {
  pinMode(13, OUTPUT);
    delay(1000);
  calibration();
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial) {
    digitalWrite(13, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(100);              // wait for a second
    digitalWrite(13, LOW);   // turn the LED off by making the voltage LOW
    delay(100);
  }
  Serial.println("calib");
   for (i = 0; i < 11; i++) {
    Serial.println(calib[i] ); 
  }
  delay(2222);
#endif
digitalWrite(13, HIGH); 
}

void calibration() {
  for (i = 0; i < 11; i++) {
    calib[i] = 0;
  }
  for (int n = 0; n < 50; n++) {
    for (int m = 0; m < 11; m++) {
      calib[m] = calib[m] + touchRead(pin[m]);
      delay(10);
    }
    
  }
  for (i = 0; i < 11; i++) {
    calib[i] = calib[i] / 50;
  }
  delay(100);
}
void loop() {
  for (i = 0; i < 11; i++) {  // 1 pin SEUL!!!!
    cs[i] = smooth * lastcs[i] + (1.0 - smooth) * (touchRead(pin[i]) - calib[i] + 8192);
    lastcs[i] = cs[i];
    midiCC[i] = constrain(cs[i] - 8191, -8191, 8192);
  }
  delay(5);  // arbitrary delay to limit data to serial port
  if (millis() > lastMillis + 20) {
    lastMillis = millis();
    for (i = 0; i < 11; i++) {
      usbMIDI.sendPitchBend(midiCC[i], i + 1);
    }
#ifdef DEBUG
    Serial.print(6000);
    Serial.print(",");
    Serial.print(9000);
    Serial.print(",");
    Serial.print(cs[0]);
    Serial.print(",");
    Serial.println(cs[1]);
#endif
  }
}
