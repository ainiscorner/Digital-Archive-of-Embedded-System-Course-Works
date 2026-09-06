// GUIDED EXAMPLE 2, STEP 2 - Debounce it correctly

const int ledPin = 13; 
// deklarasi pin LED = GPIO 13, sama seperti Guided Example 1

const int buttonPin = 19; 
// deklarasi pin tombol = GPIO 19, ini pin INPUT (baca sinyal), bukan OUTPUT

const unsigned long debounceDelay = 50; 
// ambang waktu (dalam ms) yang harus dilewati sebelum sebuah pembacaan pin 
// dianggap "stabil" dan boleh dipercaya. 50ms dipilih karena bounce mekanik 
// biasanya cuma berlangsung 1-10ms, jadi 50ms jauh lebih dari cukup untuk 
// menunggu bounce selesai, tapi masih jauh lebih cepat dari kecepatan jari manusia

bool ledState = LOW; 
// status LED saat ini, mulai dari OFF (sama seperti Guided 1)

int lastReading = HIGH; 
// nilai MENTAH (raw) dari pin tombol pada iterasi loop() SEBELUMNYA.
// HIGH karena idle state tombol (belum ditekan) = HIGH akibat INPUT_PULLUP.
// variabel ini HANYA dipakai untuk mendeteksi "apakah pin berubah dari 
// pass sebelumnya ke pass sekarang" - bukan nilai yang dipercaya sebagai kondisi tombol

int buttonState = HIGH; 
// nilai yang SUDAH DIPERCAYA sebagai kondisi tombol sesungguhnya, 
// setelah lolos proses debounce. Ini beda dari lastReading:
// lastReading = "apa yang barusan dibaca mentah-mentah"
// buttonState = "apa yang kita yakini benar-benar terjadi setelah stabil"

unsigned long lastChangeTime = 0; 
// mencatat kapan terakhir kali sinyal MENTAH (reading) berubah. 
// Ini jadi titik referensi untuk menghitung "sudah berapa lama sinyal diam/stabil"

void setup() {
  Serial.begin(115200); 
  // sama seperti sebelumnya, buka komunikasi serial di 115200 baud

  pinMode(ledPin, OUTPUT); 
  // set pin LED sebagai output, sama seperti Guided 1

  pinMode(buttonPin, INPUT_PULLUP); 
  // set pin tombol sebagai INPUT, DENGAN internal pull-up resistor diaktifkan.
  // Pull-up ini membuat pin idle di HIGH (ditarik ke 3.3V) saat tombol TIDAK ditekan.
  // Saat tombol DITEKAN, pin disambungkan langsung ke GND, jadi terbaca LOW.
  // Tanpa pull-up ini, pin akan "floating" (mengambang) saat tidak ditekan, 
  // artinya nilainya jadi acak/noise, bukan benar-benar 0 volt yang stabil
}

void loop() {
  int reading = digitalRead(buttonPin); 
  // baca nilai MENTAH pin tombol saat ini juga, tiap kali loop() berjalan
  // (loop berjalan ribuan kali per detik, jauh lebih cepat dari bounce ataupun jari manusia)

  if (reading != lastReading) { 
    lastChangeTime = millis(); 
  }
  // JIKA nilai mentah barusan BEDA dari nilai mentah pass sebelumnya,
  // berarti sinyal SEDANG berubah/bergejolak (baik itu bounce asli ATAU 
  // penekanan/pelepasan tombol yang sesungguhnya - di titik ini kita BELUM TAHU mana).
  // Maka kita reset "jam penantian" - anggap sinyal ini baru saja goyang,
  // mulai hitung ulang dari nol apakah nanti dia akan diam

  if (millis() - lastChangeTime > debounceDelay) {
    // JIKA sudah lebih dari 50ms berlalu SEJAK sinyal terakhir kali berubah,
    // artinya sinyal sudah "diam" cukup lama untuk dipercaya - bounce sudah selesai

    if (reading != buttonState) { 
      buttonState = reading; 
      // DAN jika nilai yang sudah stabil ini ternyata BEDA dari buttonState 
      // yang kita percaya sebelumnya, berarti ini perubahan yang GENUINE 
      // (bukan cuma bounce sesaat) - update buttonState ke nilai baru ini

      if (buttonState == LOW) { 
        // HANYA bereaksi kalau state baru yang genuine ini adalah LOW,
        // yaitu momen tombol BARU SAJA ditekan (bukan dilepas).
        // Ini disebut "falling edge" - momen transisi dari HIGH ke LOW.
        // Kenapa bukan reaksi di saat dilepas (rising edge, HIGH)? 
        // Karena secara konvensi/kebiasaan interaksi manusia, aksi tombol 
        // biasanya dianggap terjadi saat DITEKAN, bukan saat dilepas -
        // dan supaya tiap 1 penekanan cuma menghasilkan 1 aksi (bukan 2 aksi, 
        // 1 saat tekan + 1 saat lepas)

        ledState = !ledState; 
        digitalWrite(ledPin, ledState); 
        Serial.println("toggle"); 
        // aksi sesungguhnya: balik status LED dan tulis ke pin, 
        // sama seperti mekanisme toggle di Guided 1
      }
    }
  }

  lastReading = reading; 
  // di AKHIR tiap iterasi loop(), simpan nilai mentah kali ini 
  // untuk jadi pembanding di iterasi loop() BERIKUTNYA (dipakai di baris paling atas)
}
