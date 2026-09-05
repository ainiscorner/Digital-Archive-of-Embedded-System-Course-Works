const int ledPin = 2; 
// deklarasi pin LED = GPIO 2. BEDA dari Guided 1 (GPIO 13) - ini karena 
// sketch ini menyalakan LED BAWAAN board (onboard LED), bukan LED eksternal 
// di breadboard. Kebanyakan dev board ESP32 mengkabelkan LED bawaannya ke GPIO 2

const unsigned long interval = 500; 
// jarak waktu antar toggle LED, 500ms - sama peran seperti Guided 1

unsigned long previousMillis = 0; 
// titik referensi waktu toggle terakhir - sama peran seperti Guided 1

bool ledState = LOW; 
// status LED, mulai dari OFF - sama seperti Guided 1

void setup() {
  Serial.begin(115200); 
  // buka komunikasi serial di 115200 baud

  delay(200); 
  // PENGECUALIAN KHUSUS: satu-satunya delay() yang dipakai di seluruh 
  // modul-modul kita, dan ini SENGAJA, bukan pelanggaran prinsip non-blocking.
  // Alasannya: USB-to-serial bridge butuh waktu sesaat untuk "settle" 
  // (siap menerima data) setelah koneksi baru terbentuk saat board reset/upload.
  // Tanpa delay ini, baris print pertama bisa hilang/tidak sempat terkirim 
  // sebelum port serial benar-benar siap. Ini terjadi SEKALI di setup(), 
  // bukan di loop(), jadi tidak melanggar prinsip "jangan blocking di loop()"

  pinMode(ledPin, OUTPUT); 
  // set pin LED sebagai output - sama seperti Guided 1

  Serial.println(); 
  // cetak baris kosong, cuma buat pemisah visual di Serial Monitor

  Serial.println("=== ENBM605026 setup check ==="); 
  // header/judul cetakan, penanda mulai laporan diagnostik

  Serial.printf("Chip model     : %s\n", ESP.getChipModel()); 
  // BARU: pakai Serial.printf(), bukan Serial.print()/println() seperti 
  // sebelumnya - printf memungkinkan format string dengan placeholder (%s, %d, dst),
  // lebih ringkas untuk mencetak label+nilai dalam satu baris.
  // ESP.getChipModel() adalah fungsi bawaan ESP32 yang membaca nama chip 
  // langsung dari hardware (misal "ESP32-D0WD-V3")

  Serial.printf("Chip revision  : %d\n", ESP.getChipRevision()); 
  // %d untuk angka (integer) - versi revisi chip

  Serial.printf("CPU cores      : %d\n", ESP.getChipCores()); 
  // jumlah core CPU (biasanya 2 untuk ESP32 dual-core)

  Serial.printf("CPU frequency  : %lu MHz\n", (unsigned long)getCpuFrequencyMhz()); 
  // %lu untuk unsigned long - kecepatan CPU dalam MHz. Ada (unsigned long) 
  // di depan fungsinya - ini disebut CASTING, memaksa hasil fungsi diperlakukan 
  // sebagai tipe unsigned long, supaya cocok dengan placeholder %lu yang dipakai

  Serial.printf("Flash size     : %lu bytes\n", (unsigned long)ESP.getFlashChipSize()); 
  // ukuran memori flash (penyimpanan program) dalam bytes, di-cast juga

  Serial.printf("Free heap      : %lu bytes\n", (unsigned long)ESP.getFreeHeap()); 
  // sisa memori RAM yang masih bebas dipakai saat ini

  Serial.printf("Core version   : %s\n", ESP_ARDUINO_VERSION_STR); 
  // versi board package Arduino-ESP32 yang terinstall - INI YANG PALING PENTING 
  // dicek: harus diawali angka 3 (core 3.x), bukan 2.x, karena modul-modul 
  // berikutnya (2, 4, 7) pakai API yang cuma ada di versi 3.x

  Serial.println("If you can read this, your toolchain works."); 
  // pesan konfirmasi akhir - kalau baris ini muncul di Serial Monitor, 
  // artinya seluruh rantai toolchain (board package, driver, upload, 
  // komunikasi serial) semuanya berfungsi
}

void loop() {
  unsigned long now = millis(); 
  // sama persis pola Guided 1

  if (now - previousMillis >= interval) { 
    previousMillis = now;
    ledState = !ledState;
    digitalWrite(ledPin, ledState); 
    // toggle LED bawaan - TIDAK ADA Serial.print() di sini, beda dari 
    // Guided 1 yang mencetak status tiap toggle. Sketch ini cuma perlu 
    // BUKTI VISUAL (LED berkedip), laporan detailnya sudah selesai 
    // dicetak sekali di setup()
  }
}
