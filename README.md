# **Sistem Monitoring dan Kontrol Suhu Ruangan (Kelompok 5)**

Sistem *Internet of Things* (IoT) untuk memantau suhu dan kelembapan secara *real-time*, serta mengendalikan aktuator (AC dan Heater) pada berbagai ruangan. Sistem ini menggunakan mikrokontroler ESP32 yang mengirim dan menerima data melalui *broker* MQTT lokal berkeamanan (*authentication*), yang kemudian divisualisasikan pada *dashboard* Node-RED dan riwayatnya disimpan dalam basis data SQLite.

## Anggota Kelompok

| Nama | NRP | Ruangan yang Dipegang |
|---|---|---|
| M. Nasirudin | 2124600062 | Ruang 4 |
| Helmi Zavelda P | 2124600064 | Ruang 2 |
| Efendi Choiri S | 2124600084 | Ruang 3 |
| [Nama Anggota 4] | [NRP] | [Misal: Ruang 4] |

## Link Video

* **Video Presentasi:** [Masukkan Link Video Presentasi di Sini](#)
* **Video Demo:** [Masukkan Link Video Demo di Sini](#)

## Arsitektur Sistem

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

## Hardware

**Daftar Komponen:**
* Ruang 1
  *  ESP32 devkit v1
  *  SHT40
* Ruang 2
  *  ESP32 devkit v1
  *  SHT30
* Ruang 3
  *  ESP32 devkit v1
  *  Potensiometer(Simulasi Suhu)
  *  Potensiometer(Simulasi Kelembaban)
* Ruang 4
  *  Esp32 C3 Super Mini
  *  DHT22
* Komponen Pendukung
  * LED 1
  * LED 2
  * Resistor & Kabel Jumper
  * Breadboard

**Tabel Pin:**

| Ruangan / Node | Board | Sensor / Input | Lampu 1 (Pin & Modus) | Lampu 2 (Pin & Modus) |
| :--- | :--- | :--- | :--- | :--- |
| **Ruang 1** | SHT40 (SDA: GPIO21, SCL: GPIO22) | GPIO 25 (*Active HIGH*) | GPIO 26 (*Active HIGH*) |
| **Ruang 2** | SHT30 (SDA: GPIO21, SCL: GPIO22) | GPIO 25 (*Active HIGH*) | GPIO 26 (*Active HIGH*) |
| **Ruang 3** | Potensiometer 1 (ADC: GPIO34), Potensiometer (ADC: GPIO34) | GPIO 25 (*Active HIGH*) | GPIO 26 (*Active HIGH*) |
| **Ruang 4** | DHT22 (Data: GPIO 4) | GPIO 5 (*Active HIGH*) | GPIO 6 (*Active LOW*) |

**Foto Rangkaian (Satu Ruangan):**
![Foto Rangkaian](docs/images/rangkaian_ruang.png)
*(Catatan: Sesuaikan nama dan path gambar dengan yang ada di repository)*

## Tabel Topic Final dan Broker

* **Broker yang dipakai:** Mosquitto Localhost

| Aktuator/Sensor | Topic MQTT | Keterangan |
|---|---|---|
| Sensor Suhu | `pens/klp5/ruang1/temperature` | Publish dari ESP32 |
| Sensor Kelembapan | `pens/klp5/ruang1/humidity` | Publish dari ESP32 |
| Kontrol AC | `pens/klp5/ruang1/ac` | Subscribe dari Node-RED |
| Kontrol Heater | `pens/klp5/ruang1/heater` | Subscribe dari Node-RED |

## Cara Menjalankan

### A. Sisi Firmware (VS Code & PlatformIO)

1. **Buka Proyek:**
   - Buka VS Code, pilih menu **File** → **Open Folder...**, lalu buka folder proyek `firmware` (atau `node-redd`).

2. **Konfigurasi Kode (`src/*/main.cpp`):**
   - Buka file `main.cpp` pada folder node yang akan di-upload (misal `src/esp32c3/main.cpp` untuk Ruang 4).
   - Sesuaikan konstanta jaringan dan broker di bagian atas file:
     - `WIFI_SSID` dan `WIFI_PASS`
     - `BROKER_IP`
     - `MQTT_USER` dan `MQTT_PASS`

3. **Proses Build & Upload:**
     Buka Terminal (`Ctrl + ~`) dan jalankan perintah sesuai board target:
     - **Ruang 1 (SHT40):** `pio run -e esp1 -t upload`
     - **Ruang 2 (SHT30):** `pio run -e esp2 -t upload`
     - **Ruang 3 (Potensio):** `pio run -e esp3 -t upload`
     - **Ruang 4 (ESP32-C3):** `pio run -e esp32c3 -t upload`
   
**B. Sisi Node-RED & Database:**
1. Buka Node-RED, masuk ke *Manage Palette*, dan instal paket:
   * `node-red-dashboard`
   * `node-red-node-sqlite`
2. Lakukan *Import* file `flows.json` milik kelompok.
3. Sesuaikan *Username* dan *Password* pada node MQTT in/out sesuai keamanan broker Mosquitto. Klik **Deploy**.
4. **Alamat dashboard:** Buka browser dan akses `http://localhost:1880/ui`

## Screenshot Flow Node-RED dan Dashboard

**Flow Node-RED:**
![Flow Node-RED](docs/images/flow_Node-RED.jpeg)

**Dashboard UI:**
![Dashboard](docs/images/dashboard1.jpg)

## Hasil Pengujian T1-T12

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

## Kendala dan Solusi

* **Kendala:** Node-RED tidak bisa terhubung ke broker (Pesan error *Connection failed to broker*).
  * **Solusi:** Memasukkan *Username* dan *Password* pada tab *Security* node MQTT di Node-RED karena broker Mosquitto sudah diatur dengan `allow_anonymous false`.
* **Kendala:** [Tambahkan kendala lain di sini]
  * **Solusi:** [Tambahkan solusi di sini]

## Pembagian Tugas Anggota

| Nama Anggota | Tugas & Bagian Pekerjaan |
|---|---|
| [Nama Anggota 1] | Mengerjakan [Misal: Setup Broker Mosquitto & ESP32 Ruang 1] |
| [Nama Anggota 2] | Mengerjakan [Misal: Flow Node-RED & ESP32 Ruang 2] |
| [Nama Anggota 3] | Mengerjakan [Misal: Database SQLite & ESP32 Ruang 3] |
| [Nama Anggota 4] | Mengerjakan [Misal: UI Dashboard & ESP32 Ruang 4] |
