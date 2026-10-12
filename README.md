# 🚰 Smart Touchless Faucet with Telegram Notifications

Proyek *Internet of Things* (IoT) berbasis ESP32 untuk membuat kran air otomatis (tanpa sentuh). Sistem ini menggunakan sensor ultrasonik untuk mendeteksi keberadaan tangan, membuka/menutup kran secara otomatis menggunakan motor servo, menampilkannya pada layar LCD, dan mengirimkan notifikasi *real-time* ke Telegram pengguna.

## ✨ Fitur Utama

* **Touchless Operation:** Membuka kran secara otomatis jika terdeteksi objek (tangan) pada jarak kurang dari 5 cm.
* **Auto-Close:** Menutup kran otomatis saat objek menjauh, menghemat penggunaan air.
* **Telegram Integration:** Mengirimkan notifikasi ke *smartphone* setiap kali status kran berubah (Terbuka/Tertutup).
* **LCD Display Interface:** Menampilkan jarak aktual dan status kran secara *real-time* pada layar LCD 16x2.
* **Anti-Spam Logic:** Dilengkapi dengan logika *state-change* sehingga bot Telegram dan LCD hanya memperbarui data saat ada perubahan status.

## 🛠️ Perangkat Keras (Hardware)

1. Board ESP32 (DOIT DevKit V1 atau setara)
2. Sensor Ultrasonik HC-SR04
3. Motor Servo (Micro Servo SG90 atau setara)
4. LCD 16x2 dengan Modul I2C
5. Kabel Jumper secukupnya
6. Modul Step-down / Power Supply Eksternal (Disarankan untuk mensuplai daya Servo)

### 🔌 Konfigurasi Pin (Wiring)

| Komponen | Pin ESP32 | Keterangan | 
| ----- | ----- | ----- | 
| **Ultrasonik Trigger** | `GPIO 2` | Pin pemicu sinyal ultrasonik | 
| **Ultrasonik Echo** | `GPIO 4` | Pin penerima pantulan | 
| **Servo Data** | `GPIO 15` | Sinyal kontrol posisi (PWM) | 
| **LCD I2C SDA** | `GPIO 21` | Jalur data I2C (Default ESP32) |
| **LCD I2C SCL** | `GPIO 22` | Jalur *clock* I2C (Default ESP32) |

## 📚 Kebutuhan Library (Software)

Pastikan Anda telah menginstal *library* berikut melalui **Library Manager** di Arduino IDE:

1. `UniversalTelegramBot` oleh Brian Lough
2. `ArduinoJson` oleh Benoit Blanchon
3. `ESP32Servo` oleh Kevin Harrington
4. `LiquidCrystal I2C` oleh Frank de Brabander

*(Catatan: Library `Adafruit GFX` dan `SSD1306` tidak diperlukan untuk proyek versi LCD I2C ini).*

## 🚀 Cara Penggunaan

1. Kloning repositori ini atau *download* *file* kode sumber `.ino`.
2. Buka aplikasi Telegram, cari **BotFather**, dan buat bot baru untuk mendapatkan **BOT TOKEN**.
3. Cari bot **IDBot** di Telegram untuk mendapatkan **CHAT ID** Anda.
4. Buka kode menggunakan Arduino IDE. **WAJIB** ubah pengaturan berikut dengan kredensial Anda sendiri:

```cpp
// Konfigurasi Wi-Fi
const char* ssid = "NAMA_WIFI_ANDA";       
const char* password = "PASSWORD_WIFI_ANDA"; 

// Initialize Telegram BOT
#define BOTtoken "TOKEN_BOT_DARI_BOTFATHER"  
#define CHAT_ID "CHAT_ID_ANDA" 
```

5. Hubungkan semua komponen sesuai tabel wiring di atas.
6. *Upload* program ke board ESP32 Anda.
7. Buka *Serial Monitor* (Baudrate 115200) untuk memantau status koneksi Wi-Fi.
8. Setelah terhubung, sistem akan mengirimkan pesan *"Bot Aktif! Monitoring Kran..."* ke Telegram Anda dan siap digunakan!

## ⚙️ Modifikasi Parameter

* **Sensitivitas Jarak:** Cari baris `if (jarak < 5)` di dalam kode. Ubah angka `5` menjadi jarak (dalam sentimeter) yang Anda inginkan untuk memicu kran terbuka.
* **Sudut Putar Servo:** Cari baris `servo.write(180)` dan `servo.write(0)`. Ubah nilai derajat ini untuk disesuaikan dengan mekanisme putaran fisik kran air Anda (misalnya menjadi `90` untuk kran model tuas).

## 📄 Lisensi

[MIT License](LICENSE)