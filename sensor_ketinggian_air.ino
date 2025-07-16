#define BLYNK_TEMPLATE_ID "TMPL6zhEa1RJr"
#define BLYNK_TEMPLATE_NAME "WaterHeightSensor"
#define BLYNK_AUTH_TOKEN "9rm3x2chOaDYDqr_Kzqeus8SFgmX2idt"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ganti dengan data WiFi kamu
char ssid[] = "sebelah";
char pass[] = "123456789";

// Pin sensor
#define TRIG_PIN D5
#define ECHO_PIN D6

int tinggiWadah = 30; // default

BLYNK_WRITE(V3) {
  tinggiWadah = param.asInt();
  Serial.print("Tinggi wadah diubah menjadi: ");
  Serial.println(tinggiWadah);
}

BlynkTimer timer;

void kirimKetinggianAirKeBlynk() {
  long durasi;
  float jarakAir, ketinggianAir;

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  durasi = pulseIn(ECHO_PIN, HIGH);
  jarakAir = durasi * 0.034 / 2;

  ketinggianAir = tinggiWadah - jarakAir;
  if (ketinggianAir < 0) ketinggianAir = 0;
  if (ketinggianAir > tinggiWadah) ketinggianAir = tinggiWadah;

  Serial.print("Ketinggian Air: ");
  Serial.print(ketinggianAir);
  Serial.println(" cm");

  // Kirim ke Blynk (misal Virtual Pin V0)
  Blynk.virtualWrite(V4, ketinggianAir);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Air: ");
  lcd.print(ketinggianAir);
  lcd.print(" cm");

  lcd.setCursor(0, 1);
  lcd.print("Wadah: ");
  lcd.print(tinggiWadah);
  lcd.print(" cm");

}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Ketinggian Air:");


  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, kirimKetinggianAirKeBlynk); // 2 detik
}

void loop() {
  Blynk.run();
  timer.run();
}
