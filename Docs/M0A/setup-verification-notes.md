# Notes — Modul 0A: Setup Verification Sketch

Sketch ini bertujuan membuktikan bahwa seluruh rantai toolchain (Arduino IDE, board package, driver USB, koneksi serial, kemampuan upload ke hardware) benar-benar berfungsi sebelum masuk ke materi Modul 1.

## Anotasi baris-per-baris

```cpp
const int ledPin = 2;
```
Deklarasi pin LED = GPIO 2. **Beda dari Guided 1** (GPIO 13) — ini karena sketch ini menyalakan LED bawaan board (onboard LED), bukan LED eksternal di breadboard. Kebanyakan dev board ESP32 mengkabelkan LED bawaannya ke GPIO 2.

```cpp
const unsigned long interval = 500;
unsigned long previousMillis = 0;
bool ledState = LOW;
```
Sama persis peran dan tipenya dengan Guided 1 — interval antar toggle, titik referensi waktu, dan status LED.

```cpp
void setup() {
  Serial.begin(115200);
  delay(200);
```
**Pengecualian khusus**: satu-satunya `delay()` yang dipakai di seluruh sketch modul-modul kita, dan ini **sengaja**, bukan pelanggaran prinsip non-blocking. Alasannya: USB-to-serial bridge butuh waktu sesaat untuk "settle" (siap menerima data) setelah koneksi baru terbentuk saat board reset/upload. Tanpa delay ini, baris print pertama bisa hilang/tidak sempat terkirim sebelum port serial benar-benar siap. Ini terjadi **sekali** di `setup()`, bukan di `loop()`, jadi tidak melanggar prinsip "jangan blocking di loop()".

```cpp
  pinMode(ledPin, OUTPUT);
  Serial.println();
  Serial.println("=== ENBM605026 setup check ===");
```
Set pin sebagai output, lalu cetak header/judul laporan diagnostik.

```cpp
  Serial.printf("Chip model     : %s\n", ESP.getChipModel());
  Serial.printf("Chip revision  : %d\n", ESP.getChipRevision());
  Serial.printf("CPU cores      : %d\n", ESP.getChipCores());
  Serial.printf("CPU frequency  : %lu MHz\n", (unsigned long)getCpuFrequencyMhz());
  Serial.printf("Flash size     : %lu bytes\n", (unsigned long)ESP.getFlashChipSize());
  Serial.printf("Free heap      : %lu bytes\n", (unsigned long)ESP.getFreeHeap());
  Serial.printf("Core version   : %s\n", ESP_ARDUINO_VERSION_STR);
```
**Baru**: pakai `Serial.printf()`, bukan `Serial.print()`/`println()` seperti sebelumnya — printf memungkinkan format string dengan placeholder (`%s` untuk teks, `%d` untuk angka, `%lu` untuk angka besar tak bertanda), lebih ringkas untuk mencetak label+nilai dalam satu baris.

Beberapa nilai perlu **di-cast** secara eksplisit ke `(unsigned long)` supaya cocok dengan placeholder `%lu` yang dipakai — ini teknik konversi tipe yang belum muncul di Guided 1/2.

`ESP.getChipModel()`, `ESP.getChipRevision()`, dst adalah fungsi bawaan ESP32 yang membaca info langsung dari hardware.

**Yang paling penting dicek**: `Core version` harus diawali angka **3** (core 3.x), bukan 2.x, karena modul-modul berikutnya (2, 4, 7) pakai API yang cuma ada di versi 3.x.

```cpp
  Serial.println("If you can read this, your toolchain works.");
}
```
Pesan konfirmasi akhir — kalau baris ini muncul di Serial Monitor, artinya seluruh rantai toolchain berfungsi.

```cpp
void loop() {
  unsigned long now = millis();
  if (now - previousMillis >= interval) {
    previousMillis = now;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }
}
```
Sama persis pola non-blocking blink dari Guided 1. **Tidak ada** `Serial.print()` di sini, beda dari Guided 1 yang mencetak status tiap toggle — sketch ini cuma perlu bukti visual (LED berkedip), laporan detailnya sudah selesai dicetak sekali di `setup()`.

## Ringkasan

Sketch ini bukan untuk belajar konsep GPIO atau debounce, tapi murni sketch diagnostik. Dua hal baru yang diperkenalkan di sini yang tidak ada di Guided 1/2:
1. `Serial.printf()` dengan placeholder dan type casting
2. `delay()` yang dipakai secara sah — karena di `setup()`, bukan `loop()`, dan untuk alasan teknis spesifik (settling time USB-serial), bukan karena malas pakai `millis()`
