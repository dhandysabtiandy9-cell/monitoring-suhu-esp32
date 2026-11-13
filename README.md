# monitoring-suhu-esp32
Sistem penyiraman tanaman otomatis berbasis mikrokontroler yang memakai logika fuzzy untuk menentukan durasi penyiraman dari kelembapan tanah dan suhu udara. Data sensor dipantau secara real-time lewat Firebase Realtime Database. 

Deskripsi
Proyek ini menyesuaikan lama penyiraman dengan kondisi lingkungan. Sistem terdiri dari dua modul:
1. Modul kontrol penyiraman (Code_Soil_Moisture_Sensor.ino)
Membaca kelembapan tanah dengan soil moisture sensor (pin A0) dan suhu udara dengan sensor DHT11 (pin A1).
Memproses kedua nilai itu dengan library Fuzzy (eFLL):
Kelembapan tanah: kering, lembap, basah
Suhu: rendah, sedang, tinggi
Durasi penyiraman (keluaran): cepat, sedang, lama (0-5000 ms)
Mengaktifkan pompa air lewat relay (pin 3) selama durasi hasil defuzzifikasi, saat kelembapan tanah berada di rentang 30-38%.
Dikendalikan lewat Serial Monitor:
1 : mode otomatis
2 : matikan pompa
Mengirim nilai sensor, kelembapan, suhu, dan durasi ke Firebase Realtime Database.
2. Modul monitoring suhu dan kelembapan udara (dht_firebase.ino)
Berjalan di NodeMCU (ESP8266/ESP32) dengan sensor DHT11 (pin D5).
Terhubung ke Wi-Fi dan mengirim suhu serta kelembapan udara ke Firebase (DHT/temperature dan DHT/humidity) setiap detik.
Fitur Utama
Penyiraman adaptif berbasis logika fuzzy
Pemantauan jarak jauh lewat Firebase
Kendali manual lewat Serial Monitor
Efisiensi penggunaan air
Teknologi dan Komponen
Arduino / NodeMCU
Sensor DHT11
Soil Moisture Sensor
Modul relay dan pompa air
Library eFLL (Fuzzy Logic)
Firebase Realtime Database (Firebase_ESP_Client)
Struktur Repository
.
├── Code_Soil_Moisture_Sensor.ino   # Kontrol penyiraman + fuzzy logic
├── dht_firebase.ino                # Monitoring DHT11 ke Firebase
└── README.md
Konfigurasi

Sebelum diunggah ke board, isi kredensial berikut di kode dengan milik Anda sendiri:
cpp
#define WIFI_SSID "Your_WiFi_SSID"
#define WIFI_PASSWORD "Your_WiFi_Password"
#define API_KEY "Your_API_Key"
#define DATABASE_URL "Your_Database_URL"
Jangan mengunggah password Wi-Fi atau API key asli ke repository publik.

Catatan
Code_Soil_Moisture_Sensor.ino memakai Firebase_ESP_Client tetapi belum memiliki kode koneksi Wi-Fi. Jika dijalankan di board ESP, tambahkan WiFi.begin(). Pin A0/A1 khas Arduino Uno, yang tidak mendukung library tersebut.
Bentuk himpunan fuzzy cepat dan basah tidak lazim, sehingga perlu diuji ulang agar hasilnya sesuai harapan.
Cara Penggunaan
1. Instalasi library
Pasang lewat Arduino IDE > Library Manager:
Library	Dipakai oleh
DHT sensor library (Adafruit) + Adafruit Unified Sensor	Kedua modul
eFLL (Fuzzy)	Code_Soil_Moisture_Sensor.ino
Firebase ESP Client (Mobizt)	Kedua modul Untuk dht_firebase.ino, pasang juga board package ESP8266 (atau ESP32) lewat Boards Manager, lalu pilih board NodeMCU 1.0 (ESP-12E Module).

2. Wiring
Modul kontrol penyiraman
Komponen	Pin
Soil moisture sensor (AO)	A0
DHT11 (DATA)	A1
Relay (IN)	3
Sensor dan relay (VCC / GND)	5V / GND
Pompa air	Lewat kontak relay (COM dan NO), dengan catu daya sendiri
Modul monitoring DHT11
Komponen	Pin NodeMCU
DHT11 (DATA)	D5
DHT11 (VCC / GND)	3V3 / GND

3. Setup Firebase
Buat project di Firebase Console dan aktifkan Realtime Database.
Di Authentication > Sign-in method, aktifkan:
Anonymous, untuk dht_firebase.ino (memakai Firebase.signUp tanpa email).
Email/Password, untuk Code_Soil_Moisture_Sensor.ino, lalu buat satu akun.
Salin API Key dan Database URL, lalu isi ke kode sesuai bagian Konfigurasi di atas.
Atur rules database agar hanya pengguna terautentikasi yang bisa menulis.

4. Menjalankan
Modul kontrol penyiraman
Unggah Code_Soil_Moisture_Sensor.ino ke board.
Buka Serial Monitor dengan baud rate 9600.
Kirim 1 untuk memulai mode otomatis. Sistem membaca sensor tiap 1 detik dan menyalakan pompa selama durasi hasil fuzzy saat kelembapan tanah berada di 30-38%.
Kirim 2 untuk mematikan pompa.

Modul monitoring DHT11
Isi SSID dan password Wi-Fi, lalu unggah dht_firebase.ino ke NodeMCU.
Buka Serial Monitor dengan baud rate 115200. Setelah terhubung ke Wi-Fi, suhu dan kelembapan dikirim ke Firebase tiap 1 detik.
5. Struktur database Firebase
/
├── sensor
│   ├── nilai          (int)    nilai mentah soil moisture (0-1023)
│   ├── kelembapan     (float)  kelembapan tanah dalam %
│   ├── suhu           (float)  suhu dalam °C
│   └── durasi         (int)    durasi penyiraman hasil fuzzy (ms)
└── DHT
    ├── humidity       (float)  kelembapan udara dalam %
    └── temperature    (float)  suhu udara dalam °C
