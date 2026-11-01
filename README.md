# 1. Judul dan Deskripsi Sistem

**Sistem Monitoring dan Kontrol Suhu Ruangan (Kelompok 5)**

Sistem *Internet of Things* (IoT) untuk memantau suhu dan kelembapan secara *real-time*, serta mengendalikan aktuator (AC dan Heater) pada berbagai ruangan. Sistem ini menggunakan mikrokontroler ESP32 yang mengirim dan menerima data melalui *broker* MQTT lokal berkeamanan (*authentication*), yang kemudian divisualisasikan pada *dashboard* Node-RED dan riwayatnya disimpan dalam basis data SQLite.

## 2. Anggota Kelompok

| Nama | NRP | Ruangan yang Dipegang |
|---|---|---|
| M. Nasirudin | 2124600062 | Ruang 4 |
| Helmi Zavelda P | 2124600064 | Ruang 2 |
| Efendi Choiri S | 2124600084 | Ruang 3 |
| [Nama Anggota 4] | [NRP] | [Misal: Ruang 4] |

## 3. Link Video

* **Video Presentasi:** [Masukkan Link Video Presentasi di Sini](#)
* **Video Demo:** [Masukkan Link Video Demo di Sini](#)

## 4. Arsitektur Sistem

```text
[Node ESP32 + Sensor DHT22 + LED] 
        | (WiFi)
        v
[Broker MQTT Lokal (Mosquitto dengan Password)] 
        |
        v
[Node-RED (Server)] ---> [SQLite (Database Riwayat)]
        |
        v
[Node-RED Dashboard (Web UI)]
```
*(Atau ganti blok teks di atas dengan memanggil gambar arsitektur: `![Arsitektur](docs/images/arsitektur.png)`)*

## 5. Perangkat Keras

**Daftar Komponen:**
* ESP32 devkit v1(Ruang 1-3)
* Esp32 C3 Super Mini(Ruang 4)
* Sensor Suhu & Kelembapan (DHT11 / DHT22)
* LED (Indikator AC)
* LED (Indikator Heater)
* Resistor & Kabel Jumper
* Breadboard

**Tabel Pin:**

| Komponen | Pin ESP32 |
|---|---|
| Sensor DHT | [Isi Pin, misal: D4] |
| LED AC | [Isi Pin] |
| LED Heater | [Isi Pin] |

**Foto Rangkaian (Satu Ruangan):**
![Foto Rangkaian](docs/images/rangkaian_ruang.png)
*(Catatan: Sesuaikan nama dan path gambar dengan yang ada di repository)*

## 6. Tabel Topic Final dan Broker

* **Broker yang dipakai:** Mosquitto Localhost

| Aktuator/Sensor | Topic MQTT | Keterangan |
|---|---|---|
| Sensor Suhu | `pens/klp5/ruang1/temperature` | Publish dari ESP32 |
| Sensor Kelembapan | `pens/klp5/ruang1/humidity` | Publish dari ESP32 |
| Kontrol AC | `pens/klp5/ruang1/ac` | Subscribe dari Node-RED |
| Kontrol Heater | `pens/klp5/ruang1/heater` | Subscribe dari Node-RED |

## 7. Cara Menjalankan

**A. Sisi Arduino/ESP32:**
1. Pastikan *library* berikut terpasang di Arduino IDE:
   * `WiFi.h` (bawaan board ESP32)
   * `PubSubClient` (oleh Nick O'Leary)
   * `DHT sensor library` (oleh Adafruit)
2. Buka kode program `.ino`, lalu ubah variabel berikut:
   * `WIFI_SSID` dan `WIFI_PASSWORD`
   * `mqtt_user` dan `mqtt_password` (sesuai akun mosquitto)
   * Variabel `ROOM` (ganti menjadi `"ruang1"`, `"ruang2"`, dst sesuai hardware-nya).
3. Lakukan proses *Upload* ke ESP32.

**B. Sisi Node-RED & Database:**
1. Buka Node-RED, masuk ke *Manage Palette*, dan instal paket:
   * `node-red-dashboard`
   * `node-red-node-sqlite`
2. Lakukan *Import* file `flows.json` milik kelompok.
3. Sesuaikan *Username* dan *Password* pada node MQTT in/out sesuai keamanan broker Mosquitto. Klik **Deploy**.
4. **Alamat dashboard:** Buka browser dan akses `http://localhost:1880/ui`

## 8. Screenshot Flow Node-RED dan Dashboard

**Flow Node-RED:**
![Flow Node-RED](docs/images/flow_nodered.png)

**Dashboard UI:**
![Dashboard](docs/images/dashboard.png)

## 9. Hasil Pengujian T1-T12

| Kode | Deskripsi Pengujian | Hasil (✓ / ×) | Keterangan Singkat |
|---|---|---|---|
| T1 | [Isi deskripsi T1] | [✓] | [Keterangan] |
| T2 | [Isi deskripsi T2] | [✓] | [Keterangan] |
| T3 | [Isi deskripsi T3] | [✓] | [Keterangan] |
| T4 | [Isi deskripsi T4] | [✓] | [Keterangan] |
| T5 | [Isi deskripsi T5] | [✓] | [Keterangan] |
| T6 | [Isi deskripsi T6] | [✓] | [Keterangan] |
| T7 | [Isi deskripsi T7] | [✓] | [Keterangan] |
| T8 | [Isi deskripsi T8] | [✓] | [Keterangan] |
| T9 | [Isi deskripsi T9] | [✓] | [Keterangan] |
| T10 | [Isi deskripsi T10] | [✓] | [Keterangan] |
| T11 | [Isi deskripsi T11] | [✓] | [Keterangan] |
| T12 | [Isi deskripsi T12] | [✓] | [Keterangan] |

## 10. Ruangan yang Disimulasikan

* [Isi jika ada anggota yang tidak hadir sehingga ada ESP32 yang memegang simulasi 2 ruangan, atau tulis "Keempat ruangan disimulasikan lengkap" jika 4 alat lengkap]

## 11. Kendala dan Solusi

* **Kendala:** Node-RED tidak bisa terhubung ke broker (Pesan error *Connection failed to broker*).
  * **Solusi:** Memasukkan *Username* dan *Password* pada tab *Security* node MQTT di Node-RED karena broker Mosquitto sudah diatur dengan `allow_anonymous false`.
* **Kendala:** [Tambahkan kendala lain di sini]
  * **Solusi:** [Tambahkan solusi di sini]

## 12. Pembagian Tugas Anggota

| Nama Anggota | Tugas & Bagian Pekerjaan |
|---|---|
| [Nama Anggota 1] | Mengerjakan [Misal: Setup Broker Mosquitto & ESP32 Ruang 1] |
| [Nama Anggota 2] | Mengerjakan [Misal: Flow Node-RED & ESP32 Ruang 2] |
| [Nama Anggota 3] | Mengerjakan [Misal: Database SQLite & ESP32 Ruang 3] |
| [Nama Anggota 4] | Mengerjakan [Misal: UI Dashboard & ESP32 Ruang 4] |
