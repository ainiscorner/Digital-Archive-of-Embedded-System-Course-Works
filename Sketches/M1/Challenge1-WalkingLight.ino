// CHALLENGE 1 - Walking light

const int ledPin[] = {32, 33, 25, 26, 27, 14, 12, 13}; 
// array 8 pin LED, urutan sesuai posisi fisik di breadboard

const int buttonPin = 4; 
// pin tombol, sama peran seperti di Guided 2

const unsigned long debounceDelay = 50; 
// ambang waktu stabil sebelum sinyal dipercaya - sama seperti Guided 2

int ledNumber = 0; 
// index LED yang sedang menyala (pengganti "currentLed" - nama ini kita 
// pakai konsisten sejak pseudocode Challenge 1)

int lastReading = HIGH; 
// nilai MENTAH pin, diupdate TANPA SYARAT tiap loop - sama peran seperti Guided 2

int buttonState = HIGH; 
// nilai yang SUDAH DIPERCAYA setelah lolos debounce - variabel TERPISAH 
// dari lastReading, sama seperti Guided 2. INILAH yang tadinya hilang 
// di kode "kakak"

unsigned long lastChangeTime = 0; 
// kapan lastReading terakhir kali berubah - sama peran seperti Guided 2

void setup() {
  Serial.begin(115200);

  for (int pin : ledPin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);   // LOW = mati (wiring normal)
  }

  pinMode(buttonPin, INPUT_PULLUP);

  digitalWrite(ledPin[0], HIGH); // LED pertama nyala sebelum loop() mulai
}

void loop() {
  int reading = digitalRead(buttonPin);

  if (reading != lastReading) { 
    lastChangeTime = millis(); 
  }

  if (millis() - lastChangeTime > debounceDelay) {
    if (reading != buttonState) { 
      buttonState = reading;

      if (buttonState == LOW) { 
        digitalWrite(ledPin[ledNumber], LOW);   // matikan LED lama

        ledNumber++;
        if (ledNumber >= 8) { 
          ledNumber = 0; 
        }

        digitalWrite(ledPin[ledNumber], HIGH);  // nyalakan LED baru

        Serial.print("Toggle | Index LED aktif: ");
        Serial.println(ledNumber);
      }
    }
  }

  lastReading = reading; 
}
