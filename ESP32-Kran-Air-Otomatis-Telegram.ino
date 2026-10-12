#include <WiFi.h>                    // Library Wi-Fi untuk ESP32
#include <WiFiClientSecure.h>        // Library untuk koneksi aman
#include <UniversalTelegramBot.h>   // Library bot Telegram
#include <ESP32Servo.h>             // Library untuk servo ESP32
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
int status = 0; // 0 = otomatis, 1 = manual
// Konfigurasi Wi-Fi
const char* ssid = "NAMA_WIFI_ANDA";       // Ganti dengan SSID Wi-Fi Anda
const char* password = "PASSWORD_WIFI_ANDA"; // Ganti dengan password Wi-Fi Anda



// Initialize Telegram BOT
#define BOTtoken "TOKEN_BOT_DARI_BOTFATHER"  // your Bot Token
#define CHAT_ID "CHAT_ID_ANDA" // Replace with your chat ID

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Definisi pin
const int triggerPin = 2;
const int echoPin = 4;
const int SERVO_PIN = 15;
const int LCD_ADDRESS = 0x27;

// Inisialisasi LCD
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

// Inisialisasi Servo
Servo servo;

// Variabel jarak dan status servo
int jarak;
bool kranTerbuka = false;

// Fungsi untuk menghubungkan ke Wi-Fi
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Fungsi untuk mengirim pesan Telegram
void sendTelegramMessage(String message) {
  bot.sendMessage(CHAT_ID, message, "");
}

void setup() {
  // Inisialisasi Serial Monitor untuk debugging
  Serial.begin(115200);
  servo.write(180); // Buka 180 derajat
  
  // Setup Wi-Fi1
  connectToWiFi();
  
  // Setup Telegram Client
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  // Inisialisasi pin untuk sensor ultrasonik
  // Inisialisasi servo
  servo.attach(SERVO_PIN);
  servo.write(0); // Servo mulai dalam posisi tertutup
  
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  
  // Tampilan awal di LCD
  lcd.setCursor(0, 0);
  lcd.print("Kran: Tertutup");

  // Mengirim pesan pertama ke Telegram untuk memastikan bot online
  sendTelegramMessage("Bot Aktif! Monitoring Kran...");
}

void loop() {
  // Baca jarak dari sensor ultrasonik

  long duration, cm;


  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(triggerPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);

  // Tampilkan nilai jarak di Serial Monitor
  Serial.print(cm);
  Serial.print(" cm");
  Serial.println();

  // Update jarak ke variabel 'jarak' untuk LCD
  jarak = cm;

  delay(100);

  // Tampilkan jarak di LCD hanya jika ada perubahan
  static int lastJarak = -1;  // Variabel untuk menyimpan jarak sebelumnya

  // Mempercepat pembacaan sensor dan pembaruan LCD
  if (jarak != lastJarak) {
    // Hanya lakukan pembaruan LCD jika ada perubahan jarak
    lcd.setCursor(0, 0);
    lcd.print("Jarak: "); // Clear terlebih dahulu
    lcd.print(jarak);     // Menampilkan jarak yang terbaru
    lcd.print(" cm   ");   // Memberikan spasi ekstra untuk menghapus karakter lama

    // Update status kran berdasarkan jarak
    if (jarak < 5) {  // Jika jarak lebih kecil dari 5 cm
      if (!kranTerbuka) {
        servo.write(180); // Buka 180 derajat
        delay(500); // Memberikan waktu untuk servo bergerak
        kranTerbuka = true;
        lcd.setCursor(0, 1);
        lcd.print("Kran Terbuka");
        sendTelegramMessage("Kran Terbuka ");
      }
    } else {  // Jika jarak lebih dari 5 cm
      if (kranTerbuka) {
        servo.write(0); // Tutup kran
        delay(500); // Memberikan waktu untuk servo bergerak
        kranTerbuka = false;
        lcd.setCursor(0, 1);
        lcd.print("Kran Tertutup ");
        sendTelegramMessage("Kran Tertutup");
      }
    }

    lastJarak = jarak;  // Update jarak terakhir
  }

  // Mengambil pembaruan dari bot Telegram
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  
  // Jika ada pesan baru, tangani
  while (numNewMessages) {
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }

  // Adding a short delay to improve stability of loop
  delay(50);  // Mengurangi delay untuk mempercepat loop dan pembaruan bot Telegram
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
