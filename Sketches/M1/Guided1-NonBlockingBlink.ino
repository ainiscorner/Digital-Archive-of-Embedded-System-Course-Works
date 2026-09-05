// GUIDED EXAMPLE 1 - Non-blocking blink

const int ledPin = 13; 
// deklarasi pin LED = GPIO 13

const unsigned long interval = 500; 
// interval adalah nama variabel, isinya 500 (dalam milidetik) = jeda antar blink.
// unsigned = tipe data ini hanya bisa menyimpan angka positif (tidak ada negatif)
// long = tipe data untuk angka bulat berukuran besar (minimal 32-bit)

unsigned long previousMillis = 0; 
// variabel yang menyimpan KAPAN TEPATNYA (dalam milidetik sejak boot) 
// LED terakhir kali berganti status. Dipakai sebagai TITIK REFERENSI 
// untuk perbandingan waktu di loop(), BUKAN untuk menyimpan durasi yang sudah lewat

bool ledState = LOW; 
// status LED saat ini, mulai dari OFF (tidak ada tegangan dikirim ke LED)

void setup() { 
  Serial.begin(115200); 
  // set kecepatan komunikasi serial ke 115200 baud, supaya Arduino 
  // bisa berkirim data ke Serial Monitor

  pinMode(ledPin, OUTPUT); 
  // menentukan ARAH pin, bukan menentukan tegangannya.
  // OUTPUT berarti pin ini akan MENGIRIM sinyal keluar (bukan membaca sinyal masuk).
  // Nilai tegangan sesungguhnya (HIGH/LOW) baru ditentukan nanti oleh digitalWrite()

  Serial.println("Module 1: non-blocking blink running.");
  // cetak pesan sekali di awal untuk konfirmasi program mulai jalan
}

void loop() {
  unsigned long now = millis(); 
  // ambil waktu SEKARANG (dalam milidetik sejak board menyala/reset), 
  // dilakukan setiap kali loop() berjalan

  if (now - previousMillis >= interval) { 
    // cek: apakah selisih antara waktu SEKARANG dan waktu TERAKHIR TOGGLE 
    // sudah mencapai atau melewati 500ms?
    // Kalau BELUM, blok ini dilewati, program lanjut ke baris paling bawah 
    // tanpa melakukan apapun terkait LED - loop() tetap berjalan terus, 
    // TIDAK berhenti menunggu (beda dengan delay())

    previousMillis = now; 
    // JIKA sudah waktunya: catat ulang titik referensi waktu ke SEKARANG,
    // supaya perhitungan 500ms berikutnya dihitung mulai dari titik ini, 
    // bukan dari titik toggle yang lebih lama

    ledState = !ledState; 
    // balik status LED dari kondisi SEBELUMNYA - kalau tadinya LOW jadi HIGH, 
    // kalau tadinya HIGH jadi LOW. Baris ini dieksekusi BERULANG-ULANG 
    // setiap 500ms, bukan cuma sekali di awal - itulah yang menciptakan 
        efek berkedip terus-menerus

    digitalWrite(ledPin, ledState); 
    // TULIS status yang baru saja dibalik ke pin fisik - baris inilah yang 
    // benar-benar mengubah tegangan di GPIO13 (bukan baris di atasnya)

    Serial.print("LED -> ");
    Serial.println(ledState ? "ON" : "OFF");
    // cetak status LED ke Serial Monitor untuk konfirmasi visual/debug
  }

  // The loop keeps running. Anything else you add still gets to execute.
  // catatan penting: karena tidak ada delay() di manapun, loop() ini 
  // menyelesaikan satu putaran dalam waktu SANGAT singkat (jauh di bawah 1ms), 
  // sehingga kalau kamu tambahkan kode lain di bawah blok if ini nanti, 
  // kode itu tetap akan sempat dieksekusi ribuan kali per detik - 
  // tidak pernah "diblokir" menunggu LED
}
