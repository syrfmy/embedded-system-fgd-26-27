# Proyek Latex FGD

## Usefull command

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

## Struktur file 

```text
sections/
  ├── A_pendahuluan.tex
  ├── B_konsep_mikrokontroler.tex
  ├── C_prinsip_kerja.tex
  ├── D_eksperimen_proyek.tex
  └── E_referensi.tex
    ├── components
    ├── dc_motor.tex
    ├── dht22.tex
    ├── hc-sr501.tex
    ├── komponen_template.tex
    ├── mq_gas_sensors.tex
    ├── power_relay.tex
    └── sg90.tex
```
