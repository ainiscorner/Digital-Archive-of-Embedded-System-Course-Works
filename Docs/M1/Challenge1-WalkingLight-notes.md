# Notes — Challenge 1: Walking Light

## Anotasi baris-per-baris

```cpp
const int ledPin[] = {32, 33, 25, 26, 27, 14, 12, 13};
```
Array 8 pin LED. Urutan harus sesuai posisi fisik LED di breadboard (index 0 = LED 1, dst) — kalau urutan array tidak cocok dengan urutan fisik, LED tetap menyala bergantian, tapi kelihatan "loncat-loncat" tidak berurutan secara visual.

```cpp
const int buttonPin = 4;
```
Pin tombol.

```cpp
const unsigned long debounceDelay = 50;
```
Sama peran seperti Guided 2 Step 2.

```cpp
int lastReading = HIGH;
int buttonState = HIGH;
unsigned long lastChangeTime = 0;
```
Sama persis peran dan nama dengan Guided 2 Step 2.

```cpp
int ledNumber = 0;
```
Index LED yang SEDANG menyala saat ini (0-7). **Tidak boleh `const`** karena nilainya harus berubah tiap penekanan tombol yang valid. Nilai awal 0 berarti LED pertama (`ledPin[0]`) yang jadi starting point.

Ini **menggantikan** `ledState` (boolean) dari Guided 1/2 — karena Challenge 1 butuh merepresentasikan **posisi** di antara 8 pilihan, bukan sekadar dua kondisi (nyala/mati).

```cpp
void setup() {
  Serial.begin(115200);
  for (int pin : ledPin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);   // LOW = mati (wiring normal)
  }
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(ledPin[0], HIGH); // HIGH = nyala (wiring normal)
}
```
Set SEMUA 8 pin LED sebagai output sekaligus, pakai range-based for. Nyalakan LED PERTAMA sebelum `loop()` mulai berjalan sama sekali — ini "starting state" sesuai requirement modul ("starting at LED 1"). Tanpa baris ini, tidak akan ada LED yang menyala sebelum tombol ditekan.

**Penting**: nilai HIGH/LOW untuk "nyala"/"mati" bergantung pada polaritas wiring fisik — cek dulu apakah LED terpasang normal atau terbalik (inverted) sebelum menentukan nilai ini.

```cpp
void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastReading) {
    lastChangeTime = millis();
  }

  if (millis() - lastChangeTime > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
```
Struktur tiga-lapis debounce ini **identik** dengan Guided 2 Step 2 — deteksi perubahan, cek stabil, cek genuine.

```cpp
        digitalWrite(ledPin[ledNumber], LOW);
```
Matikan LED di posisi LAMA (`ledNumber` masih menunjuk index sebelumnya) — dilakukan SEBELUM index diubah.

```cpp
        ledNumber++;
        if (ledNumber >= 8) {
          ledNumber = 0;
        }
```
Pindah ke index berikutnya. Wraparound: kalau sudah lewat LED terakhir (index 7), balik ke LED pertama (index 0) — sesuai requirement "after LED 8, next press returns to LED 1".

```cpp
        digitalWrite(ledPin[ledNumber], HIGH);
```
Nyalakan LED di posisi BARU (`ledNumber` sudah ter-update).

```cpp
        Serial.println("toggle");
      }
    }
  }

  lastReading = reading;
}
```

## Narasi

Challenge 1 mewarisi seluruh struktur debounce dari Guided 2 Step 2 tanpa perubahan — deteksi perubahan sinyal mentah, cek stabil 50ms, cek apakah perubahan itu genuine, baru bereaksi di falling edge. Yang berubah hanyalah **aksi** yang dilakukan saat falling edge terdeteksi.

Perbedaan konsep paling penting dari Guided 1/2: di sana, aksi selalu "membalik dari kondisi sebelumnya" (`ledState = !ledState`), karena hanya ada dua kemungkinan (nyala/mati) dan programnya tidak pernah tahu pasti kondisi sekarang tanpa mengecek variabel state. Di Challenge 1, aksinya adalah **"pindah posisi nyala"** — bukan toggle, tapi **set eksplisit**: LED lama PASTI dimatikan, LED baru PASTI dinyalakan, tanpa perlu "menebak" arah baliknya.

Karena itu, `ledState` (boolean) sudah tidak relevan lagi di Challenge 1, digantikan oleh `ledNumber` (integer) yang menyimpan posisi, bukan status nyala/mati generik.

**Urutan operasi krusial**: matikan LED lama HARUS terjadi menggunakan `ledNumber` yang MASIH LAMA, SEBELUM `ledNumber` di-increment. Kalau urutannya terbalik (increment dulu baru matikan), program akan "kehilangan jejak" LED mana yang seharusnya dimatikan.

## Bug-bug yang ditemukan selama proses pengembangan (bahan "what did not work")

1. **`ledNumber` sempat dideklarasikan sebagai `const`** — menyebabkan compile error, karena nilainya perlu diubah di `loop()` tapi `const` melarang perubahan apapun setelah deklarasi.
2. **Loop pematian-semua-LED yang diletakkan di luar semua kondisi `if`** — pernah dicoba sebagai pendekatan awal, tapi ini salah: karena dijalankan setiap iterasi `loop()` (ribuan kali per detik) tanpa syarat, LED yang baru saja dinyalakan akan langsung dimatikan lagi di iterasi berikutnya sebelum sempat terlihat mata manusia.
3. **Pin mismatch antara kode dan wiring fisik** — kode sempat menunjuk ke pin tertentu sementara kabel tombol fisik terpasang di pin lain, menyebabkan tombol tidak terdeteksi sama sekali. Solusi: tambahkan `Serial.print(reading)` untuk diagnosa cepat, konfirmasi angka berubah atau tidak saat tombol ditekan.
4. **Toggle (`ledState = !ledState`) sempat dicoba dipakai untuk Challenge 1** — tapi ini keliru secara konsep, karena Challenge 1 butuh "set eksplisit ke posisi tertentu", bukan "membalik dari kondisi sebelumnya".
