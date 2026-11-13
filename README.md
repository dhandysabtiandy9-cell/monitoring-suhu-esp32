Penyiraman Otomatis dengan Sensor Kelembapan Tanah dan Logika Fuzzy

Sistem penyiraman tanaman otomatis. Sensor kelembapan tanah dan sensor suhu DHT11 dibaca oleh mikrokontroler, lalu logika fuzzy menentukan berapa lama pompa air harus menyala. Data sensor juga dikirim ke Firebase Realtime Database untuk dipantau.

Cara Kerja
Sensor kelembapan tanah (pin A0) dibaca dan diubah menjadi persentase 0 sampai 100%.
Sensor DHT11 (pin A1) membaca suhu.
Kelembapan tanah dan suhu menjadi input fuzzy:
Kelembapan tanah: kering, lembab, basah
Suhu: rendah, sedang, tinggi
Logika fuzzy menghasilkan durasi penyiraman (cepat, sedang, lama) dalam milidetik.
Relay (pin 3) menyalakan pompa air selama durasi tersebut jika kelembapan tanah berada di antara 30% dan 38%.
Nilai sensor, suhu, kelembapan, dan durasi dikirim ke Firebase pada jalur /sensor/....
Kontrol lewat Serial
Kirim	Fungsi
1	Mulai mode otomatis (baca sensor, hitung fuzzy, siram)
2	Matikan pompa
Komponen
Komponen	Keterangan
Board	(isi board yang kamu pakai)
Sensor kelembapan tanah	Output analog ke A0
Sensor suhu	DHT11 ke A1
Relay	Sinyal ke pin 3
Pompa air	Dikendalikan lewat relay
Library yang Dibutuhkan

Install lewat Library Manager di Arduino IDE:

DHT sensor library (Adafruit)
eFLL (Embedded Fuzzy Logic Library, menyediakan Fuzzy.h)
Firebase ESP Client (Mobizt)
Cara Menjalankan
Buat proyek di Firebase Console, aktifkan Realtime Database dan Authentication (Email/Password), lalu buat satu akun pengguna.
Buka file Code_Soil_Moisture_Sensor/Code_Soil_Moisture_Sensor.ino di Arduino IDE.
Ganti nilai berikut dengan milikmu sendiri:
cpp
   config.api_key = "Your_API_Key";
   config.database_url = "Your_Database_URL";
   auth.user.email = "Your_Email";
   auth.user.password = "Your_Password";
Pilih board dan port yang sesuai, lalu upload.
Buka Serial Monitor (9600 baud), kirim 1 untuk memulai.
