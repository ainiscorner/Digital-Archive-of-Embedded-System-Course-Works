# Notes — Guided Example 1: Non-blocking Blink

## Anotasi baris-per-baris

```cpp
const int ledPin = 13;
```
Deklarasi pin LED = GPIO 13.

```cpp
const unsigned long interval = 500; // ms
```
`interval` adalah nama variabel, isinya 500 (dalam milidetik) = jeda antar blink. `unsigned` = tipe data ini hanya bisa menyimpan angka positif (tidak ada negatif). `long` = tipe data untuk angka bulat berukuran besar (minimal 32-bit).

```cpp
unsigned long previousMillis = 0;
```
Variabel yang menyimpan **kapan tepatnya** (dalam milidetik sejak boot) LED terakhir kali berganti status. Dipakai sebagai **titik referensi** untuk perbandingan waktu di `loop()`, bukan untuk menyimpan durasi yang sudah lewat.

```cpp
bool ledState = LOW;
```
Status LED saat ini, mulai dari OFF (tidak ada tegangan dikirim ke LED).

```cpp
void setup() {
  Serial.begin(115200);
```
Set kecepatan komunikasi serial ke 115200 baud, supaya Arduino bisa berkirim data ke Serial Monitor.

```cpp
  pinMode(ledPin, OUTPUT);
```
Menentukan **arah** pin, bukan menentukan tegangannya. `OUTPUT` berarti pin ini akan **mengirim** sinyal keluar (bukan membaca sinyal masuk). Nilai tegangan sesungguhnya (HIGH/LOW) baru ditentukan nanti oleh `digitalWrite()`.

```cpp
  Serial.println("Module 1: non-blocking blink running.");
}
```
Cetak pesan sekali di awal untuk konfirmasi program mulai jalan.

```cpp
void loop() {
  unsigned long now = millis();
```
Ambil waktu SEKARANG (dalam milidetik sejak board menyala/reset), dilakukan setiap kali `loop()` berjalan.

```cpp
  if (now - previousMillis >= interval) {
```
Cek: apakah selisih antara waktu SEKARANG dan waktu TERAKHIR TOGGLE sudah mencapai atau melewati 500ms? Kalau **belum**, blok ini dilewati, program lanjut ke baris paling bawah tanpa melakukan apapun terkait LED — `loop()` tetap berjalan terus, tidak berhenti menunggu (beda dengan `delay()`).

```cpp
    previousMillis = now;
```
JIKA sudah waktunya: catat ulang titik referensi waktu ke SEKARANG, supaya perhitungan 500ms berikutnya dihitung mulai dari titik ini.

```cpp
    ledState = !ledState;
```
Balik status LED dari kondisi SEBELUMNYA — kalau tadinya LOW jadi HIGH, kalau tadinya HIGH jadi LOW. Baris ini dieksekusi berulang-ulang setiap 500ms, bukan cuma sekali di awal — itulah yang menciptakan efek berkedip terus-menerus.

```cpp
    digitalWrite(ledPin, ledState);
```
TULIS status yang baru saja dibalik ke pin fisik — baris inilah yang benar-benar mengubah tegangan di GPIO13 (bukan baris `ledState = !ledState` di atasnya yang cuma mengubah nilai variabel di memori).

```cpp
    Serial.print("LED -> ");
    Serial.println(ledState ? "ON" : "OFF");
  }
}
```
Cetak status LED ke Serial Monitor untuk konfirmasi visual/debug.

## Narasi

Saat program mulai (`setup()`), pin GPIO13 diatur ke mode OUTPUT — ini penting dipahami sebagai penentuan **arah** pin (bisa mengirim sinyal keluar), **bukan** penentuan tegangan. Tegangan sesungguhnya baru ditentukan nanti oleh `digitalWrite()`. `previousMillis` dimulai dari 0 dan `ledState` dimulai dari LOW (OFF), sebagai titik awal sebelum ada blink pertama terjadi.

Di dalam `loop()`, program membaca waktu sekarang lewat `millis()`, lalu membandingkan selisihnya dengan `previousMillis` — pertanyaannya selalu sama tiap putaran: *"sudah 500ms sejak toggle terakhir?"*. Kalau belum, blok `if` dilewati sepenuhnya dan `loop()` langsung selesai tanpa melakukan apa-apa terkait LED — ini bedanya yang fundamental dengan `delay()`: program tidak pernah berhenti menunggu, ia cuma mengecek berulang kali, sangat cepat, sampai kondisinya terpenuhi.

Ketika 500ms sudah terlewati, tiga hal terjadi berurutan: pertama, `previousMillis` di-reset ke waktu sekarang. Kedua, `ledState` dibalik dari kondisi sebelumnya lewat operator `!` — bukan diset ke satu nilai tetap, tapi ditoggle. Ketiga, `digitalWrite()` benar-benar menuliskan status baru itu ke pin fisik.

Karena tidak ada `delay()` di manapun, satu putaran `loop()` selesai dalam hitungan mikrodetik ketika kondisi `if` belum terpenuhi — inilah yang membuat pola ini disebut **non-blocking**.

## Pertanyaan modul (untuk laporan)

1. **Apa yang berubah kalau blok `if` diganti jadi `digitalWrite(HIGH); delay(500); digitalWrite(LOW); delay(500);`?** — Processor akan berhenti total selama 500ms setiap kali, tidak bisa membaca tombol, menjawab serial, atau melakukan tugas lain apapun selama periode itu.
2. **`previousMillis` bertipe `unsigned long`. Apa yang terjadi setelah ~49 hari?** — `millis()` overflow (wrap around) kembali ke 0 setelah sekitar 49.7 hari. Karena `now - previousMillis` adalah operasi aritmatika unsigned, hasilnya tetap benar secara matematis meski terjadi wraparound, sehingga perbandingan `>= interval` tetap valid tanpa perlu penanganan khusus.
