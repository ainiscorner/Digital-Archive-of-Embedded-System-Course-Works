# Notes — Guided Example 2, Step 1: See the Problem (Bounce)

Versi ini sengaja **belum** dikoreksi, dipakai untuk melihat efek contact bounce secara langsung sebelum diperbaiki di Step 2.

## Anotasi baris-per-baris

```cpp
const int ledPin = 13;
```
Sama seperti Guided 1.

```cpp
const int buttonPin = 19;
```
Deklarasi pin tombol = GPIO 19, dipakai sebagai INPUT (baca sinyal).

```cpp
bool ledState = LOW;
```
Status LED saat ini, mulai dari OFF.

```cpp
int lastReading = HIGH;
```
Nilai MENTAH pin tombol dari iterasi `loop()` sebelumnya. HIGH karena idle state (belum ditekan) = HIGH akibat `INPUT_PULLUP`.

**Catatan penting**: di versi ini HANYA ADA SATU variabel state — tidak ada `buttonState` terpisah seperti di Step 2. Artinya tidak ada pembeda antara "nilai yang barusan dibaca" dan "nilai yang benar-benar dipercaya".

```cpp
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // idle HIGH, pressed LOW
}
```
Idle HIGH, pressed LOW — pull-up internal diaktifkan supaya pin tidak floating saat tombol tidak ditekan.

```cpp
void loop() {
  int reading = digitalRead(buttonPin);
```
Baca nilai MENTAH pin tombol, dilakukan setiap kali `loop()` berjalan (ribuan kali per detik).

```cpp
  if (reading != lastReading && reading == LOW) {
```
Dua syarat digabung sekaligus dalam SATU pengecekan:
1. `reading != lastReading` → nilai barusan BEDA dari pass sebelumnya
2. `reading == LOW` → dan nilai baru itu adalah LOW (kondisi ditekan)

**Masalahnya**: begitu kedua syarat ini terpenuhi, program LANGSUNG mempercayainya sebagai penekanan asli — tidak ada jeda waktu untuk menunggu apakah sinyal ini stabil atau cuma gejolak sesaat (bounce). Kalau kontak mekanik tombol bergejolak 3-4 kali dalam hitungan milidetik, SETIAP gejolak yang kebetulan hasilnya LOW akan lolos syarat ini dan dianggap sebagai penekanan terpisah.

```cpp
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    Serial.println("toggle");
  }
```
Aksi toggle LED — persis sama seperti versi Step 2, TAPI di sini bisa terpicu berkali-kali untuk SATU kali tekan fisik.

```cpp
  lastReading = reading;
}
```
Simpan nilai mentah kali ini untuk pembanding di iterasi berikutnya.

## Narasi

Selama idle (tombol tidak ditekan), pin GPIO19 berada di level tegangan HIGH, karena tersambung ke 3.3V melalui internal pull-up resistor yang diaktifkan lewat `INPUT_PULLUP`. Berbeda dari versi Step 2, di sini hanya ada satu variabel state: `lastReading`, yang dideklarasikan dengan nilai awal HIGH, selaras dengan kondisi idle pin saat program pertama kali dijalankan.

Ketika tombol ditekan, pin terputus dari 3.3V dan tersambung langsung ke GND, sehingga `digitalRead()` membaca LOW. Di sinilah letak masalahnya: begitu program mendeteksi bahwa `reading` berbeda dari `lastReading` dan nilainya LOW, program langsung mempercayainya sebagai penekanan asli, tanpa ada jeda waktu untuk memastikan sinyal ini benar-benar stabil. Padahal secara fisik, kontak mekanik tombol bergejolak (bounce) beberapa kali dalam hitungan milidetik sebelum benar-benar menetap — dan karena `loop()` berjalan jauh lebih cepat dari kecepatan bounce tersebut, program melihat setiap gejolak sebagai perubahan sinyal yang terpisah.

Akibatnya, kode ini tetap bisa di-compile dan LED memang menyala-mati saat tombol disentuh — dalam artian ia "bekerja" — tapi ia tidak "benar", karena jumlah baris "toggle" yang tercatat di Serial Monitor tidak proporsional dengan jumlah penekanan fisik yang sebenarnya dilakukan. Inilah bukti nyata bahwa **working code** dan **correct code** adalah dua hal yang berbeda.

## Tugas modul

Tekan tombol persis 10 kali, hitung berapa banyak baris "toggle" yang muncul di Serial Monitor, dan rekam sebagai bukti bahwa bounce itu nyata — sebelum lanjut ke Step 2 yang memperbaikinya dengan `debounceDelay` + `buttonState`.
