# Notes — Challenge 2: Bouncing Light with Direction

## Anotasi baris-per-baris

```cpp
const int ledPin[] = {32, 33, 25, 26, 27, 14, 12, 13};
```
Sama seperti Challenge 1.

```cpp
const int buttonStepPin = 4;
const int buttonDirectionPin = 23;
```
**Dua** pin tombol, menggantikan `buttonPin` tunggal di Guided 2/Challenge 1 — karena sekarang ada dua tombol dengan peran berbeda: satu menggeser posisi, satu membalik arah.

```cpp
const unsigned long debounceDelay = 50;
```
Dipakai bersama oleh kedua tombol — tidak perlu dua versi, karena ambang waktunya sama untuk semua tombol.

```cpp
int ledNumber = 0;
```
Sama peran seperti Challenge 1.

```cpp
int directionStep = 1;
```
**Variabel baru**: arah gerak saat ini. 1 = maju (index naik), -1 = mundur (index turun). Tidak ada di Challenge 1, karena Challenge 1 cuma bisa bergerak satu arah (selalu maju, wrap ke 0).

```cpp
int lastReadingStep = HIGH;
int buttonStateStep = HIGH;
unsigned long lastChangeTimeStep = 0;

int lastReadingDirection = HIGH;
int buttonStateDirection = HIGH;
unsigned long lastChangeTimeDirection = 0;
```
Dua **set lengkap** variabel debounce, satu untuk tombol Step, satu untuk tombol Direction. Nama dasarnya identik dengan Guided 2/Challenge 1 (`lastReading`, `buttonState`, `lastChangeTime`), cuma ditambah suffix untuk membedakan instance-nya.

```cpp
bool isButtonPressed(int pin, int &lastReading, int &buttonState, unsigned long &lastChangeTime) {
```
**Fungsi pembantu** yang membungkus SELURUH logika debounce tiga-lapis dari Guided 2 Step 2, supaya bisa dipakai ulang untuk pin manapun tanpa menulis ulang blok yang sama. Inilah jawaban untuk requirement modul: *"Structure your code so the debouncing logic is written once, not copied twice."*

Parameter `int &lastReading, int &buttonState, unsigned long &lastChangeTime` dilewatkan **by reference** (pakai `&`) — penting, karena tanpa `&`, perubahan yang terjadi di dalam fungsi tidak akan "menempel" balik ke variabel asli di luar fungsi.

```cpp
  int reading = digitalRead(pin);
  bool pressedEvent = false;

  if (reading != lastReading) {
    lastChangeTime = millis();
  }

  if ((millis() - lastChangeTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        pressedEvent = true;
      }
    }
  }

  lastReading = reading;
  return pressedEvent;
}
```
Struktur tiga-lapis debounce yang identik dengan Guided 2/Challenge 1. Bedanya: hasil akhirnya (`pressedEvent`) **dikembalikan** sebagai bool, bukan langsung memicu aksi di dalam fungsi — supaya fungsi ini tetap generik (tidak perlu tahu apa-apa soal LED atau arah).

```cpp
void setup() {
  Serial.begin(115200);
  for (int pin : ledPin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  pinMode(buttonStepPin, INPUT_PULLUP);
  pinMode(buttonDirectionPin, INPUT_PULLUP);
  digitalWrite(ledPin[0], HIGH);
}
```
Sama seperti Challenge 1, tapi set KEDUA pin tombol sebagai input dengan pull-up internal.

```cpp
void loop() {
  if (isButtonPressed(buttonStepPin, lastReadingStep, buttonStateStep, lastChangeTimeStep)) {
    digitalWrite(ledPin[ledNumber], LOW);
```
Matikan LED lama — sama seperti Challenge 1.

```cpp
    ledNumber += directionStep;
```
Geser index **mengikuti arah saat ini** — beda dari Challenge 1 yang selalu `ledNumber++` (cuma maju).

```cpp
    if (ledNumber > 7) {
      ledNumber = 6;
      directionStep = -1;
    } else if (ledNumber < 0) {
      ledNumber = 1;
      directionStep = 1;
    }
```
Sudah melewati batas atas: koreksi index turun satu langkah, DAN balik arah jadi mundur — ini realisasi "reverse at the ends". Simetris untuk batas bawah.

```cpp
    digitalWrite(ledPin[ledNumber], HIGH);
    Serial.print("Step | Index LED aktif: ");
    Serial.println(ledNumber);
  }

  if (isButtonPressed(buttonDirectionPin, lastReadingDirection, buttonStateDirection, lastChangeTimeDirection)) {
    directionStep = -directionStep;
    Serial.print("Direction Reversed! Arah saat ini: ");
    Serial.println(directionStep == 1 ? "Maju (1)" : "Mundur (-1)");
  }
}
```
Tombol direction membalik arah secara instan — tidak menyentuh `ledNumber` sama sekali, jadi LED tetap di posisi yang sama, cuma arah geraknya yang berubah.

## Narasi

Challenge 2 mewarisi seluruh logika debounce dan mekanisme pemindahan LED dari Challenge 1, dengan dua tambahan konsep: **arah gerak** (`directionStep`) dan **reusable debounce function**.

Ketika LED sedang di index 7 (ujung atas) dan sedang bergerak maju, `ledNumber + directionStep` akan menghasilkan 8 — nilai invalid (di luar batas array 0-7). Di titik inilah dua hal terjadi sekaligus: arah dibalik (dari maju jadi mundur), DAN index dikoreksi mundur satu langkah dari titik itu (ke 6) — bukan direset ke 0 seperti Challenge 1, dan bukan pula diam di 7. Ini memastikan urutan "8, 7, 6..." berjalan mulus tanpa lompatan atau jeda.

Perbedaan paling signifikan dari Challenge 1 adalah lahirnya fungsi `isButtonPressed()`. Alih-alih menulis ulang seluruh blok debounce tiga-lapis untuk setiap tombol, logika itu dibungkus jadi satu fungsi yang menerima pin dan referensi ke tiga variabel state sebagai parameter, lalu mengembalikan `true`/`false` sebagai jawaban atas pertanyaan "apakah pin ini baru saja ditekan secara valid?". Kedua tombol (Step dan Direction) memanggil fungsi yang sama, masing-masing dengan set variabel state-nya sendiri.

## Proses debugging dan pelajaran penting

### 1. Bug pin mismatch (berulang kali)
Sama seperti Challenge 1, kode ini beberapa kali mengalami ketidakcocokan antara nomor pin di kode dan wiring fisik sebenarnya (misalnya `buttonDirPin = 18` di kode tapi kabel tombol fisik ada di GPIO22/23). Gejalanya: tombol "tidak merespons sama sekali", bukan salah logika. Cara diagnosa: tambahkan `Serial.print()` untuk memantau nilai mentah pin, cek apakah ada perubahan sama sekali saat tombol ditekan.

### 2. Perbandingan dua pendekatan: fungsi vs duplikasi manual
Selama proses debugging, sempat dicoba versi dengan seluruh blok debounce **diduplikasi manual** dua kali (satu untuk tombol Step, satu untuk tombol Direction, tanpa fungsi pembungkus). Versi ini **bekerja secara fungsional**, tapi tidak memenuhi requirement modul yang secara eksplisit meminta debounce logic ditulis sekali, tidak diduplikasi.

Setelah pin yang benar teridentifikasi (lewat proses debugging versi duplikasi), versi fungsi (`isButtonPressed()`) dicoba ulang dengan pin yang sudah tervalidasi — dan berhasil. Ini menunjukkan bahwa kegagalan versi fungsi sebelumnya kemungkinan besar disebabkan oleh pin mismatch yang sama, bukan oleh cacat pada strukturnya fungsi itu sendiri.

### 3. Kenapa fungsi bisa menggantikan duplikasi tanpa merusak logika
Poin krusial: pembungkusan ke dalam fungsi HARUS mempertahankan struktur dua-variabel (`lastReading` vs `buttonState`) yang sudah terbukti benar di Guided 2 — kalau fungsi ini disederhanakan jadi satu variabel saja, bug yang sama seperti yang pernah dialami sebelumnya (variabel pembanding "mengejar" nilai reading terlalu cepat) akan muncul kembali, terlepas dari apakah kodenya dibungkus fungsi atau tidak.

## Requirement khusus yang dinilai (untuk laporan)

*"In your report, explain how you avoided duplicating the debounce code."*

Jawaban: logika debounce dibungkus dalam fungsi `isButtonPressed(pin, &lastReading, &buttonState, &lastChangeTime)` yang menerima pin dan referensi ke variabel state sebagai parameter. Fungsi ini dipanggil dua kali di `loop()` — sekali untuk `buttonStepPin`, sekali untuk `buttonDirectionPin` — masing-masing dengan set variabel state miliknya sendiri, sehingga logika debounce hanya ditulis satu kali namun dapat melayani kedua tombol secara independen.
