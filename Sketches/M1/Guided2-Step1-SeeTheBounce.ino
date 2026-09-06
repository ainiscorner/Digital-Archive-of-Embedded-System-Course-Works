// GUIDED EXAMPLE 2, STEP 1 - See the problem

const int ledPin = 13; 
// deklarasi pin LED = GPIO 13, sama persis seperti Guided 1 dan Step 2

const int buttonPin = 19; 
// deklarasi pin tombol = GPIO 19, dipakai sebagai INPUT (baca sinyal)

bool ledState = LOW; 
// status LED saat ini, mulai dari OFF

int lastReading = HIGH; 
// nilai MENTAH pin tombol dari iterasi loop() sebelumnya.
// HIGH karena idle state (belum ditekan) = HIGH akibat INPUT_PULLUP.
// CATATAN PENTING: di versi ini HANYA ADA SATU variabel state - 
// tidak ada buttonState terpisah seperti di Step 2. Artinya tidak ada 
// pembeda antara "nilai yang barusan dibaca" dan "nilai yang benar-benar dipercaya"

void setup() {
  Serial.begin(115200); 
  // buka komunikasi serial di 115200 baud

  pinMode(ledPin, OUTPUT); 
  // set pin LED sebagai output

  pinMode(buttonPin, INPUT_PULLUP); 
  // idle HIGH, pressed LOW - pull-up internal diaktifkan supaya pin 
  // tidak floating saat tombol tidak ditekan
}

void loop() {
  int reading = digitalRead(buttonPin); 
  // baca nilai MENTAH pin tombol, dilakukan setiap kali loop() berjalan 
  // (ribuan kali per detik)

  if (reading != lastReading && reading == LOW) { 
    // dua syarat digabung sekaligus dalam SATU pengecekan:
    // 1. reading != lastReading -> nilai barusan BEDA dari pass sebelumnya
    // 2. reading == LOW -> dan nilai baru itu adalah LOW (kondisi ditekan)
    // 
    // MASALAHNYA: begitu kedua syarat ini terpenuhi, program LANGSUNG 
    // mempercayainya sebagai penekanan asli - TIDAK ADA jeda waktu untuk 
    // menunggu apakah sinyal ini stabil atau cuma gejolak sesaat (bounce).
    // Kalau kontak mekanik tombol bergejolak 3-4 kali dalam hitungan 
    // milidetik, SETIAP gejolak yang kebetulan hasilnya LOW akan lolos 
    // syarat ini dan dianggap sebagai penekanan terpisah

    ledState = !ledState; 
    digitalWrite(ledPin, ledState); 
    Serial.println("toggle"); 
    // aksi toggle LED - persis sama seperti versi Step 2, 
    // TAPI di sini bisa terpicu berkali-kali untuk SATU kali tekan fisik
  }

  lastReading = reading; 
  // simpan nilai mentah kali ini untuk pembanding di iterasi berikutnya
}
