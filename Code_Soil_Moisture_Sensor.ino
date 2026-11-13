#include <DHT.h>
#include <Fuzzy.h>
#include <Firebase_ESP_Client.h>

#define DHTTYPE DHT11
#define dht_pin A1
#define SensorPin A0
#define RelayPin 3

DHT dht(dht_pin, DHTTYPE);
Fuzzy *fuzzy = new Fuzzy();

FuzzySet *kering = new FuzzySet(0, 30, 30, 35);
FuzzySet *lembab = new FuzzySet(30, 35, 50, 55);
FuzzySet *basah = new FuzzySet(50, 55, 55, 55);

FuzzySet *rendah = new FuzzySet(0, 45, 45, 50);
FuzzySet *sedangsuhu = new FuzzySet(45, 50, 55, 60);
FuzzySet *tinggi = new FuzzySet(55, 60, 60, 60);

FuzzySet *cepat = new FuzzySet(0, 0, 0, 1000);
FuzzySet *sedangdurasi = new FuzzySet(1000, 2000, 3500, 4500);
FuzzySet *lama = new FuzzySet(3500, 4000, 4500, 5000);

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RelayPin, OUTPUT);
  dht.begin();

  FuzzyInput *kelembapan = new FuzzyInput(1);
  kelembapan->addFuzzySet(kering);
  kelembapan->addFuzzySet(lembab);
  kelembapan->addFuzzySet(basah);
  fuzzy->addFuzzyInput(kelembapan);

  FuzzyInput *suhu = new FuzzyInput(2);
  suhu->addFuzzySet(rendah);
  suhu->addFuzzySet(sedangsuhu);
  suhu->addFuzzySet(tinggi);
  fuzzy->addFuzzyInput(suhu);

  FuzzyOutput *durasi = new FuzzyOutput(1);
  durasi->addFuzzySet(cepat);
  durasi->addFuzzySet(sedangdurasi);
  durasi->addFuzzySet(lama);
  fuzzy->addFuzzyOutput(durasi);

  // Firebase configuration
  config.api_key = "Your_API_Key";
  config.database_url = "Your_Database_URL";
  auth.user.email = "Your_Email";
  auth.user.password = "Your_Password";

  Firebase.begin(&config, &auth);
}

void loop() {
  if (Serial.available() > 0) {
    char data = Serial.read();

    if (!data) Serial.end();

    if (data == '1') {
      while (data == '1') {
        int SensorValue = analogRead(SensorPin);
        float Kelembapan = map(SensorValue, 1023, 0 , 0, 100);
        float Suhu = dht.readTemperature();
        float KelembapanDHT = dht.readHumidity();

        fuzzy->setInput(1, Kelembapan);
        fuzzy->setInput(2, Suhu);
        fuzzy->fuzzify();

        int durasi = fuzzy->defuzzify(1);

        Serial.print("Nilai Sensor: ");
        Serial.print(SensorValue);
        Serial.println(" (0-1023)");
        
        Serial.print("Kelembapan: ");
        Serial.print(Kelembapan);
        Serial.println("%");

        Serial.print("Suhu: ");
        Serial.print(Suhu);
        Serial.println(" *C");

        Serial.print("Durasi: ");
        Serial.print(durasi);
        Serial.println("");

        Serial.print("Kelembapan Suhu: ");
        Serial.print(KelembapanDHT);
        Serial.println("%");

        // Send data to Firebase
        if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)) {
          sendDataPrevMillis = millis();

          if (Firebase.RTDB.setInt(&fbdo, "/sensor/nilai", SensorValue)) {
            Serial.println("Sensor data sent to Firebase successfully");
          } else {
            Serial.println("Failed to send sensor data to Firebase");
          }

          if (Firebase.RTDB.setFloat(&fbdo, "/sensor/kelembapan", Kelembapan)) {
            Serial.println("Kelembapan data sent to Firebase successfully");
          } else {
            Serial.println("Failed to send kelembapan data to Firebase");
          }

          if (Firebase.RTDB.setFloat(&fbdo, "/sensor/suhu", Suhu)) {
            Serial.println("Suhu data sent to Firebase successfully");
          } else {
            Serial.println("Failed to send suhu data to Firebase");
          }

          if (Firebase.RTDB.setInt(&fbdo, "/sensor/durasi", durasi)) {
            Serial.println("Durasi data sent to Firebase successfully");
          } else {
            Serial.println("Failed to send durasi data to Firebase");
          }
        }

        if(Kelembapan > 30 && Kelembapan < 38) {
          digitalWrite(RelayPin, HIGH);
          Serial.println("Pompa Air: 1");
          delay(durasi);
          digitalWrite(RelayPin, LOW);
          Serial.println("Pompa Air: 0");
        } else{
          digitalWrite(RelayPin, LOW);
          Serial.println("Pompa Air: 0");
        }

        if (Serial.available() > 0) {
          data = Serial.read(); // Update data value
        }
        delay(1000);
      }
    }

    if (data == '2') {
      digitalWrite(RelayPin, LOW);
      Serial.println("Pompa Air: 0");
    }
  }
}
