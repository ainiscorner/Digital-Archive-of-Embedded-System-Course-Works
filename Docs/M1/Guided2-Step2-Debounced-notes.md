# Notes — Guided Example 2, Step 2: Debounce It Correctly

## Anotasi baris-per-baris

```cpp
const int ledPin = 13;
const int buttonPin = 19;
```
Sama seperti Step 1.

```cpp
const unsigned long debounceDelay = 50; // ms
```
Ambang waktu (dalam ms) yang harus dilewati sebelum sebuah pembacaan pin dianggap "stabil" dan boleh dipercaya. 50ms dipilih karena bounce mekanik biasanya cuma berlangsung 1-10ms, jadi 50ms jauh lebih dari cukup untuk menunggu bounce selesai, tapi masih jauh lebih cepat dari kecepatan jari manusia.

```cpp
bool ledState = LOW;
```
Status LED saat ini, mulai dari OFF.

```cpp
int lastReading = HIGH;
```
Nilai MENTAH (raw) dari pin tombol pada iterasi `loop()` SEBELUMNYA. Variabel ini HANYA dipakai untuk mendeteksi "apakah pin berubah dari pass sebelumnya ke pass sekarang" — bukan nilai yang dipercaya sebagai kondisi tombol.

```cpp
int buttonState = HIGH;
```
Nilai yang SUDAH DIPERCAYA sebagai kondisi tombol sesungguhnya, setelah lolos proses debounce. Ini beda dari `lastReading`:
- `lastReading` = "apa yang barusan dibaca mentah-mentah"
- `buttonState` = "apa yang kita yakini benar-benar terjadi setelah stabil"

```cpp
unsigned long lastChangeTime = 0;
```
Mencatat kapan terakhir kali sinyal MENTAH (`reading`) berubah. Ini jadi titik referensi untuk menghitung "sudah berapa lama sinyal diam/stabil".

```cpp
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}
```
Set pin tombol sebagai INPUT, dengan internal pull-up resistor diaktifkan. Pull-up ini membuat pin idle di HIGH (ditarik ke 3.3V) saat tombol TIDAK ditekan. Saat tombol DITEKAN, pin disambungkan langsung ke GND, jadi terbaca LOW. Tanpa pull-up ini, pin akan "floating" (mengambang) saat tidak ditekan, artinya nilainya jadi acak/noise, bukan benar-benar 0 volt yang stabil.

```cpp
void loop() {
  int reading = digitalRead(buttonPin);
```
Baca nilai MENTAH pin tombol, dilakukan setiap kali `loop()` berjalan.

```cpp
  if (reading != lastReading) {
    lastChangeTime = millis();
  }
```
JIKA nilai mentah barusan BEDA dari nilai mentah pass sebelumnya, berarti sinyal SEDANG berubah/bergejolak (baik itu bounce asli ATAU penekanan/pelepasan tombol yang sesungguhnya — di titik ini kita BELUM TAHU mana). Maka kita reset "jam penantian" — anggap sinyal ini baru saja goyang, mulai hitung ulang dari nol apakah nanti dia akan diam.

```cpp
  if (millis() - lastChangeTime > debounceDelay) {
```
JIKA sudah lebih dari 50ms berlalu SEJAK sinyal terakhir kali berubah, artinya sinyal sudah "diam" cukup lama untuk dipercaya — bounce sudah selesai.

```cpp
    if (reading != buttonState) {
      buttonState = reading;
```
DAN jika nilai yang sudah stabil ini ternyata BEDA dari `buttonState` yang kita percaya sebelumnya, berarti ini perubahan yang GENUINE (bukan cuma bounce sesaat) — update `buttonState` ke nilai baru ini.

```cpp
      if (buttonState == LOW) {
```
HANYA bereaksi kalau state baru yang genuine ini adalah LOW, yaitu momen tombol BARU SAJA ditekan (bukan dilepas). Ini disebut **falling edge** — momen transisi dari HIGH ke LOW. Kenapa bukan reaksi di saat dilepas (rising edge, HIGH)? Karena secara konvensi interaksi manusia, aksi tombol biasanya dianggap terjadi saat DITEKAN, dan supaya tiap 1 penekanan cuma menghasilkan 1 aksi (bukan 2 aksi, 1 saat tekan + 1 saat lepas).

```cpp
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
        Serial.println("toggle");
      }
    }
  }

  lastReading = reading;
}
```
Aksi sesungguhnya: balik status LED dan tulis ke pin. Di AKHIR tiap iterasi `loop()`, simpan nilai mentah kali ini untuk jadi pembanding di iterasi berikutnya.

## Narasi

Selama idle, pin GPIO19 berada di level tegangan HIGH, karena tersambung ke 3.3V melalui internal pull-up resistor. `lastReading` dan `buttonState` dideklarasikan dengan nilai awal HIGH, selaras dengan kondisi idle pin saat program pertama kali dijalankan.

Ketika tombol ditekan, pin terputus dari 3.3V dan tersambung langsung ke GND, sehingga `digitalRead()` membaca LOW. Namun program tidak langsung mempercayai pembacaan ini — karena kontak mekanik tombol bisa bergejolak (bounce) sesaat sebelum benar-benar stabil. Program baru mempercayai bahwa pembacaan ini genuine setelah nilai `reading` tetap sama selama lebih dari 50ms (`debounceDelay`) — barulah `buttonState` di-update menjadi LOW, yang memicu perintah untuk membalik status LED.

Ketika tombol dilepaskan, pin kembali tersambung ke 3.3V lewat pull-up, sehingga pembacaan otomatis kembali ke HIGH — ini murni konsekuensi elektrikal dari pull-up, bukan hasil dari suatu "keputusan" yang diambil program. Namun karena blok kondisi di dalam kode hanya mengecek `if (buttonState == LOW)`, transisi ke HIGH ini tidak memicu aksi apapun — LED tetap berada di status terakhirnya, sampai penekanan berikutnya terdeteksi.

## Mengapa dibutuhkan DUA variabel state, bukan satu

Ini poin paling krusial dari seluruh Guided 2. Kalau cuma pakai satu variabel (`lastReading` saja, seperti Step 1), variabel itu akan "mengejar" nilai `reading` dalam hitungan mikrodetik — jauh lebih cepat dari 50ms yang ditunggu. Begitu 50ms terlewati, variabel pembanding itu sudah lama berubah mengikuti reading, dan perbandingan "apakah ini berubah dari kondisi lama" jadi selalu gagal.

Solusinya butuh satu variabel yang sengaja "lambat" (`buttonState`) — cuma berubah setelah dikonfirmasi stabil, bukan setiap saat.

## Kesalahan paling umum (dari PPT)

Kalau baris `if (reading != buttonState)` dihapus, kode appears to work — sampai tombol ditahan lebih dari 50ms. Karena `lastChangeTime` berhenti diupdate ketika reading sudah stabil, kondisi `millis() - lastChangeTime > debounceDelay` akan terus TRUE selama tombol ditahan, menyebabkan LED toggle berulang-ulang setiap loop pass, bukan cuma sekali per penekanan.

## Pertanyaan modul (untuk laporan)

1. **Apa yang dikembalikan `millis()`, dan kenapa membandingkan selisih waktu lebih baik daripada menghitung loop pass?** — `millis()` mengembalikan jumlah milidetik sejak board boot. Menghitung loop pass tidak reliable karena kecepatan `loop()` tidak konstan — tergantung berapa banyak kode lain yang dijalankan tiap iterasi.
2. **Metode debounce hardware, dan kapan lebih dipilih?** — Misalnya RC filter atau Schmitt trigger. Lebih dipilih ketika edge sinyal itu sendiri yang men-drive interrupt atau fungsi keamanan (safety-critical), karena hardware debouncing tidak memakan waktu CPU dan bereaksi instan.
3. **Kenapa bereaksi di falling edge (LOW), bukan saat dilepas?** — Supaya satu penekanan menghasilkan tepat satu aksi, dan sesuai konvensi umum bahwa "aksi tombol" terjadi saat ditekan, bukan dilepas.
