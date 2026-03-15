/* Fill-in information from Blynk Device Info here */
/*
#define BLYNK_TEMPLATE_ID           "TMPL6LxFuxqDH"
#define BLYNK_TEMPLATE_NAME         "ESP32 IOT Project Study"
#define BLYNK_AUTH_TOKEN            "6yDMqJKzfI9IZVNX-wQFM19Upe4FQrBz"

#define BLYNK_TEMPLATE_ID "TMPL6ko3w3RkQ"
#define BLYNK_TEMPLATE_NAME "ProjectIOT"
#define BLYNK_AUTH_TOKEN "leZnmBvatlOcQiJxZ_GEy3e-TCYVVmMD"
*/
#define BLYNK_TEMPLATE_ID "TMPL6r2R5TYak"
#define BLYNK_TEMPLATE_NAME "ESP32 IOT Project Study"
#define BLYNK_AUTH_TOKEN "tdNXYhOaZ6m7NFIjmrlMsECW2YTi_cAY"


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <NewPing.h>

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "MiPoco";
//"KnDwifi";
char pass[] = "G0dBl355U";
//"KnD16082604";

// --- Definisi Pin Hardware ---
#define PIR_PIN 13      
#define RELAY_PIN 27     
#define TRIG_PIN 14     
#define ECHO_PIN 12     
#define MAX_DISTANCE 400 
#define LED_BUILTIN_PIN 2


NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
// --- Variabel Global ---
int counterOrang = 0;
long lastMotionTime = 0;
bool motionDetected = false;
const unsigned long TIMEOUT_LAMP_OFF = 60000; 
//unsigned int lastDistance = 0;
//unsigned int lastDistanceCount = 0;
unsigned int minDistanceRadar = 0;
unsigned int maxDistanceRadar = 90;

//const int LED_BUILTIN = 2;

BlynkTimer timer;

// --- Fungsi Kontrol Lampu ---
void turnOnLamp() {
    digitalWrite(RELAY_PIN, HIGH); 
    Blynk.virtualWrite(V0, 1);     
    Blynk.virtualWrite(V1, 1);
    printMessageToConsol("Lampu dinyalakan.");
}

void turnOffLamp() {
    digitalWrite(RELAY_PIN, LOW);
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 0);
    motionDetected = false;
    printMessageToConsol("Lampu Dipadamkan.");
}

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0)
{
 // Set incoming value from pin V0 to a variable
 int buttonState = param.asInt();
 if (buttonState == 1) {
      turnOnLamp();
 } else {
      turnOffLamp();
 }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  //Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  //Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "offImageUrl","https://imgur.com/WHx1WG6");
  Blynk.setProperty(V3, "onImageUrl","https://imgur.com/a/gASrmP1");
  //Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}


// --- Fungsi Cek Sensor Gerak (PIR) ---
void checkPIR() {
    int pirState = digitalRead(PIR_PIN);
    if (pirState == HIGH) {
        printMessageToConsol("Gerakan Terdeteksi.");
        if (!motionDetected) {  
            motionDetected = true;            
            turnOnLamp();
            
            // Gunakan Blynk.logEvent jika Anda sudah setting Events di dashboard Blynk
            Blynk.logEvent("gerakan_terdeteksi", "Gerakan terdeteksi! Lampu dinyalakan."); 
        }
        lastMotionTime = millis(); 
    }
}

// --- Fungsi Cek Sensor Ultrasonik (Counter) ---
void checkUltrasonic() {
    // Fungsi ping_cm() mengembalikan 0 jika tidak terdeteksi atau di luar jangkauan
    unsigned int distance = sonar.ping_cm(); 
    String msg ="";
    if (distance > 0 )
    {
      // Sesuaikan threshold ini (misalnya 10 cm) dengan jarak di mana sensor mendeteksi kepala/bahu orang
      if (distance > minDistanceRadar && distance < maxDistanceRadar) { 
        
        msg = "Jarak: " + String(distance);
        printMessageToConsol(msg);
        
        Blynk.virtualWrite(V5, distance); // Update nilai jarak objek di Blynk
        
        if (distance < maxDistanceRadar-1 ) 
        {
          counterOrang++;
          Blynk.virtualWrite(V4, counterOrang); // Update counter di Blynk
        }
        
        /*if(lastDistance != distance)
        {
          if (distance < lastDistanceCount-2 || distance> lastDistanceCount+2) 
          { 
            counterOrang++;
            Blynk.virtualWrite(V4, counterOrang); // Update counter di Blynk
            lastDistanceCount = distance;
          } 
          lastDistance = distance;
        }*/
        
        // Jeda singkat untuk menghindari penghitungan ganda saat seseorang melintas
        // Catatan: delay() di sini mungkin mengganggu loop Blynk, 
        // Solusi yang lebih baik adalah menggunakan state machine atau timer kedua untuk jeda.
        // Untuk saat ini, kita gunakan delay singkat.
        delay(500); 
      }
      
    }
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}
void blinkLED()
{
  digitalWrite(LED_BUILTIN_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);                       // wait for a second
  digitalWrite(LED_BUILTIN_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(300);                       // wait for a second
  digitalWrite(LED_BUILTIN_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);                       // wait for a second
  digitalWrite(LED_BUILTIN_PIN, LOW);   // turn the LED on (HIGH is the voltage level)
}

void printMessageToConsol(String iMsg)
{
  printCurrentTime();
  Serial.println(iMsg);
}
// --- Fungsi Baru untuk Menampilkan Waktu ---
void printCurrentTime() {
  // millis() mengembalikan jumlah milidetik sejak program berjalan
  unsigned long currentTime = millis(); 

  unsigned long seconds = currentTime / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;

  // Modulo (%) digunakan untuk mendapatkan sisa detik/menit agar nilainya tetap dalam 0-59
  seconds %= 60;
  minutes %= 60;
  hours %= 24; // Batasi jam hingga 24 jam (siklus harian)

  // Mencetak waktu dalam format HH:MM:SS
  // Kami menggunakan fungsi print padZero() untuk memastikan format konsisten (e.g., 05 bukan 5)
  Serial.print("[");
  padZero(hours);
  Serial.print(":");
  padZero(minutes);
  Serial.print(":");
  padZero(seconds);
  Serial.print("]: ");
}

void padZero(unsigned long value) {
  if (value < 10) {
    Serial.print('0'); // Jika angka tunggal (misal 5), cetak '0' dulu
  }
  Serial.print(value);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  turnOffLamp(); // Pastikan lampu mati saat boot
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // --- INISIALISASI TIMER DI SINI ---
  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
  // Memanggil fungsi checkPIR setiap 100 milidetik (0.1 detik)
  timer.setInterval(500L, checkPIR);
  // Memanggil fungsi checkUltrasonic setiap 500 milidetik (0.5 detik)
  timer.setInterval(500L, checkUltrasonic);
  // Memanggil fungsi blinkLEd setiap 1000 milidetik (1 detik)
  timer.setInterval(1000L, blinkLED);
}

// --- Loop Utama ---
void loop()
{
  Blynk.run(); // Wajib ada untuk menjalankan koneksi Blynk dan Timers
  timer.run(); // Wajib ada untuk menjalankan fungsi yang didaftarkan di atas
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  // Logika timeout lampu tetap berjalan di loop utama
  if (motionDetected && millis() - lastMotionTime >= TIMEOUT_LAMP_OFF) {
      
      printMessageToConsol("Tidak ada gerakan terdeteksi selama 60 detik.");
      turnOffLamp();
      Blynk.logEvent("lampu_mati_otomatis", "Tidak ada gerakan terdeteksi selama 60 detik.");
  }
}
