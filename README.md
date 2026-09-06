# Digital Archive — Embedded System Course Works

Arsip pribadi sketch, dokumentasi, dan catatan belajar untuk mata kuliah **Sistem Embedded Biomedik dan Praktikum (ENBM605026)**, Semester Ganjil 2026/2027, Program Studi Teknik Biomedik, Universitas Indonesia.

## Struktur repo

```
Sketches/   → kode Arduino (.ino) per modul, siap upload
Docs/       → catatan belajar, anotasi baris-per-baris, wiring diagram, pseudocode
Reports/    → laporan praktikum resmi (PDF) yang dikumpulkan ke EMAS
Videos/     → link ke video demonstrasi (video asli disimpan di Google Drive, bukan di repo ini)
```

Setiap folder dipecah lagi per modul (`M0A`, `M1`, `M2`, dst), supaya file jenis yang sama tetap terkelompok tapi mudah ditelusuri per minggu.

## Progress

- [x] Modul 0A — Arduino IDE and ESP32 Setup
- [x] Modul 1 — ESP32 Toolchain and GPIO
- [ ] Modul 2 — Timing, Timers and Interrupts
- [ ] Modul 3 — ...
- [ ] Modul 4 — ...

## Catatan penggunaan

- File `.ino` di `Sketches/` sudah final dan teruji di hardware (ESP32 DevKitC).
- File `.md` di `Docs/` ditulis untuk belajar ulang materi secara mandiri — formatnya: anotasi baris-per-baris, lalu narasi penjelasan konsep, lalu (kalau relevan) daftar bug yang ditemukan selama proses debugging.
- Video demonstrasi tidak disimpan langsung di repo ini (batas ukuran file GitHub) — cek `Videos/<modul>/README.md` untuk link ke Google Drive/YouTube.

## Lisensi

Repo ini untuk keperluan belajar pribadi dan dokumentasi tugas kuliah.
