# Proyek Laporan Mikrokontroler — Template FGD

Struktur dokumen menempatkan eksplorasi komponen sebagai fokus utama FGD.
Proyek pada Bagian D digunakan sebagai eksperimen dan bukti pemahaman.

## Kompilasi Dokumen

Gunakan skrip kompilasi otomatis yang sudah dilengkapi dengan pemeriksaan dependensi:

```bash
# Kompilasi dokumen (otomatis memeriksa dependensi terlebih dahulu)
./compile.sh

# Hanya periksa ketersediaan alat dan paket LaTeX
./compile.sh --check

# Kompilasi dan bersihkan file temporer build
./compile.sh --clean

# Mode watch (otomatis mengompilasi ulang saat ada perubahan file)
./compile.sh --watch

# Kompilasi dan langsung buka hasil PDF
./compile.sh --open
```

## Struktur utama

- **A. Pendahuluan** — konteks, tujuan, ruang lingkup, dan daftar komponen.
- **B. Konsep Mikrokontroler** — konsep dasar MCU yang diperlukan untuk memahami antarmuka komponen.
- **C. Prinsip Kerja Setiap Komponen** — bagian inti eksplorasi. Setiap komponen memiliki file `.tex` sendiri.
- **D. Proyek sebagai Eksperimen dan Bukti Pemahaman** — penerapan dan validasi pemahaman.

## Struktur file komponen

```text
sections/
├── A_pendahuluan.tex
├── B_konsep_mikrokontroler.tex
├── C_prinsip_kerja.tex
├── D_eksperimen_proyek.tex
└── components/
    ├── komponen_template.tex
    ├── komponen_01.tex
    ├── komponen_02.tex
    └── komponen_03.tex
```

### Menambahkan komponen baru

1. Salin `sections/components/komponen_template.tex`.
2. Beri nama sesuai komponen, misalnya `sensor_ldr.tex` atau `sensor_ultrasonik.tex`.
3. Isi seluruh bagian template dengan hasil eksplorasi.
4. Tambahkan `\input{sections/components/nama_file}` pada `C_prinsip_kerja.tex`.
5. Tambahkan komponen tersebut pada tabel perbandingan jika diperlukan.

Template mencakup identitas, prinsip kerja, alur internal, besaran, karakteristik
sinyal, pin dan catu daya, datasheet, hubungan dengan MCU, rangkaian,
perhitungan, eksperimen sederhana, program dasar, hasil, analisis, keterbatasan,
dan kesimpulan pemahaman.
