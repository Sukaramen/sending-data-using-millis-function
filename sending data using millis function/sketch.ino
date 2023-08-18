#include <DHT.h>
#include <FirebaseESP32.h>
#include <WiFi.h>

int led1 = 4;
int led2 = 2;
#define DHTPIN 5
#define DHTTYPE DHT22

unsigned long mulai_dari = 0;
unsigned long sebelum_pembacaan_led1 = millis();
unsigned long sebelum_pembacaan_led2 = millis();
unsigned long sebelum_pembacaan_data1 = millis();
unsigned long sebelum_pembacaan_data2 = millis();
unsigned long led1_interval = 1000; // Delay berkedip selama 1s
unsigned long led2_interval = 250;  // Delay berkedip selama 250ms
unsigned long data1_interval = 100; // Delay pengiriman data1 selama 100ms
unsigned long data2_interval = 100; // Delay pengiriman data2 selama 100ms

#define FIREBASE_HOST "https://data-send-bb778-default-rtdb.firebaseio.com/"
#define FIREBASE_TOKEN "VjqO7TLaY9GDl6NlL8ju9LmxmFt7SrujTEo8c5ca"
const char* ssid = "Wokwi-GUEST";
const char* pass = "";

int led1_state = LOW;
int led2_state = LOW;
int data1_state = LOW;
int data2_state = LOW;

DHT dht(DHTPIN, DHTTYPE);
FirebaseData datatemp1;
FirebaseData datatemp2;
void setup() {
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.println("Tunggu.");
  }
  Serial.println();
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_TOKEN);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  mulai_dari = millis();
  if (mulai_dari - sebelum_pembacaan_led1 > led1_interval){
    led1_state = !led1_state;
    digitalWrite(led1, led1_state);
    sebelum_pembacaan_led1 = mulai_dari;
  }
  if (mulai_dari - sebelum_pembacaan_led2 > led2_interval){
    led2_state = !led2_state;
    digitalWrite(led2, led2_state);
    sebelum_pembacaan_led2 = mulai_dari;
  }
  if (mulai_dari - sebelum_pembacaan_data1 > data1_interval){
    data1_state = !data1_state;
    Firebase.setFloat(datatemp1, "/data/temp", temp);
    sebelum_pembacaan_data1 = mulai_dari;
  }
  if (mulai_dari - sebelum_pembacaan_data2 > data2_interval){
    data2_state = !data2_state;
    Firebase.setFloat(datatemp2, "/data/hum", hum);
    sebelum_pembacaan_data2 = mulai_dari;
  }
}