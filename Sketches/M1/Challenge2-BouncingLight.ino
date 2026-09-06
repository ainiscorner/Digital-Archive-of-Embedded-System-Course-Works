// CHALLENGE 2 - Bouncing light with direction

const int ledPin[] = {32, 33, 25, 26, 27, 14, 12, 13}; 
// array 8 pin LED, urutan sesuai posisi fisik di breadboard - sama seperti Challenge 1

const int buttonStepPin = 4; 
// pin tombol untuk menggeser posisi LED (menggantikan "buttonPin" tunggal 
// di Challenge 1, karena sekarang ada 2 tombol dengan peran berbeda)

const int buttonDirectionPin = 23; 
// pin tombol untuk membalik arah gerak, kapan pun ditekan

const unsigned long debounceDelay = 50; 
// ambang waktu stabil sebelum sinyal dipercaya - sama seperti Guided 2 dan Challenge 1

int ledNumber = 0; 
// index LED yang sedang menyala (0-7) - sama peran seperti Challenge 1

int directionStep = 1; 
// arah gerak saat ini: 1 = maju (index naik), -1 = mundur (index turun).
// variabel BARU yang tidak ada di Challenge 1, karena Challenge 1 cuma 
// bisa bergerak satu arah (selalu maju, wrap ke 0)

// Variabel debounce untuk tombol STEP - namanya identik dengan Guided 2/Challenge 1,
// cuma diberi suffix "Step" karena sekarang ada 2 tombol yang butuh state terpisah
int lastReadingStep = HIGH; 
int buttonStateStep = HIGH; 
unsigned long lastChangeTimeStep = 0; 

// Variabel debounce untuk tombol DIRECTION - struktur identik, suffix "Direction"
int lastReadingDirection = HIGH; 
int buttonStateDirection = HIGH; 
unsigned long lastChangeTimeDirection = 0; 

bool isButtonPressed(int pin, int &lastReading, int &buttonState, unsigned long &lastChangeTime) {
  // fungsi pembantu yang membungkus SELURUH logika debounce tiga-lapis dari 
  // Guided 2 Step 2, supaya bisa dipakai ulang untuk pin manapun tanpa 
  // menulis ulang blok yang sama - inilah jawaban untuk requirement modul 
  // "debouncing logic written once, not copied twice"

  int reading = digitalRead(pin); 
  bool pressedEvent = false; 
  // pressedEvent menggantikan peran blok "if (buttonState == LOW)" di 
  // Guided 2/Challenge 1 - di sini hasilnya DIKEMBALIKAN sebagai bool, 
  // bukan langsung memicu aksi di dalam fungsi, supaya fungsi ini tetap 
  // generik (tidak perlu tahu LED atau arah apapun)

  if (reading != lastReading) { 
    lastChangeTime = millis(); 
  }
  // deteksi perubahan mentah, reset jam penantian - identik dengan Guided 2

  if (millis() - lastChangeTime > debounceDelay) {
    // cek stabil - identik dengan Guided 2

    if (reading != buttonState) { 
      buttonState = reading;
      // update state yang dipercaya HANYA setelah lolos stabil - inilah 
      // variabel "juru catat yang malas" yang jadi kunci debounce yang benar

      if (buttonState == LOW) { 
        pressedEvent = true; 
        // tandai bahwa ini penekanan genuine (falling edge), untuk 
        // dikembalikan ke pemanggil fungsi
      }
    }
  }

  lastReading = reading; 
  // update tanpa syarat di akhir - identik dengan Guided 2

  return pressedEvent;
}

void setup() {
  Serial.begin(115200); 
  // buka komunikasi serial - sama seperti semua kode sebelumnya

  for (int pin : ledPin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW); // LOW = mati (wiring normal)
  }
  // set semua 8 pin LED sebagai output dan mulai dalam kondisi mati - 
  // sama seperti Challenge 1

  pinMode(buttonStepPin, INPUT_PULLUP);
  pinMode(buttonDirectionPin, INPUT_PULLUP);
  // set KEDUA pin tombol sebagai input dengan pull-up internal

  digitalWrite(ledPin[0], HIGH); 
  // LED pertama menyala sebelum loop() mulai - "starting state" sesuai 
  // requirement modul, sama seperti Challenge 1
}

void loop() {
  // --- TOMBOL STEP: menggeser posisi LED ---
  if (isButtonPressed(buttonStepPin, lastReadingStep, buttonStateStep, lastChangeTimeStep)) {
    digitalWrite(ledPin[ledNumber], LOW); 
    // matikan LED lama - sama seperti Challenge 1

    ledNumber += directionStep; 
    // geser index MENGIKUTI ARAH SAAT INI - beda dari Challenge 1 yang 
    // selalu ledNumber++ (cuma maju)

    if (ledNumber > 7) {
      ledNumber = 6; 
      directionStep = -1; 
      // sudah melewati batas atas: koreksi index turun satu langkah, 
      // DAN balik arah jadi mundur - ini realisasi "reverse at the ends"
    } else if (ledNumber < 0) {
      ledNumber = 1; 
      directionStep = 1; 
      // sudah melewati batas bawah: koreksi index naik satu langkah, 
      // DAN balik arah jadi maju
    }

    digitalWrite(ledPin[ledNumber], HIGH); 
    // nyalakan LED baru - sama seperti Challenge 1

    Serial.print("Step | Index LED aktif: ");
    Serial.println(ledNumber);
  }

  // --- TOMBOL DIRECTION: membalik arah kapan pun, di posisi manapun ---
  if (isButtonPressed(buttonDirectionPin, lastReadingDirection, buttonStateDirection, lastChangeTimeDirection)) {
    directionStep = -directionStep; 
    // balik arah secara instan - tidak menyentuh ledNumber sama sekali, 
    // jadi LED tetap di posisi yang sama, cuma arah geraknya yang berubah

    Serial.print("Direction Reversed! Arah saat ini: ");
    Serial.println(directionStep == 1 ? "Maju (1)" : "Mundur (-1)");
  }
}
