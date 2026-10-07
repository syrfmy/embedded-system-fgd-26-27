#!/usr/bin/env bash
# ==============================================================================
# Compile Script with Dependency Checking for FGD Report (LaTeX)
# ==============================================================================

set -euo pipefail

# Script directory is always the project root
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

MAIN_TEX="main.tex"
MAIN_PDF="main.pdf"

# ANSI Colors
if [ -t 1 ]; then
  BOLD="\033[1m"
  RESET="\033[0m"
  RED="\033[31m"
  GREEN="\033[32m"
  YELLOW="\033[33m"
  BLUE="\033[34m"
  CYAN="\033[36m"
else
  BOLD=""
  RESET=""
  RED=""
  GREEN=""
  YELLOW=""
  BLUE=""
  CYAN=""
fi

log_info()    { echo -e "${CYAN}[INFO]${RESET} $*"; }
log_ok()      { echo -e "${GREEN}[OK]${RESET} $*"; }
log_warn()    { echo -e "${YELLOW}[WARN]${RESET} $*"; }
log_error()   { echo -e "${RED}[ERROR]${RESET} $*"; }
log_header()  { echo -e "\n${BOLD}${BLUE}=== $* ===${RESET}"; }

show_help() {
  cat << EOF
Penggunaan: ./compile.sh [OPSI]

Opsi:
  -c, --check       Hanya lakukan pemeriksaan dependensi tanpa kompilasi
  --pdflatex        Paksa gunakan pdflatex (2 kali putaran kompilasi)
  --latexmk         Paksa gunakan latexmk
  -w, --watch       Mode watch (latexmk -pvc, otomatis recompile saat file diedit)
  --clean           Hapus file temporer build (.aux, .log, .toc, dll.)
  --clean-all       Hapus file temporer dan file output PDF
  --open            Buka file PDF setelah berhasil dikompilasi (xdg-open)
  -h, --help        Tampilkan bantuan ini

Contoh:
  ./compile.sh               # Periksa dependensi lalu kompilasi dokumen
  ./compile.sh --check       # Hanya periksa kelengkapan dependensi
  ./compile.sh --clean       # Bersihkan file temporer
  ./compile.sh --watch       # Mode pantau otomatis
EOF
}

clean_aux_files() {
  log_info "Membersihkan file temporer LaTeX..."
  rm -f main.aux main.bbl main.blg main.fdb_latexmk main.fls main.lof \
        main.log main.lot main.out main.toc main.synctex.gz
  log_ok "File temporer berhasil dibersihkan."
}

clean_all_files() {
  clean_aux_files
  rm -f "$MAIN_PDF"
  log_ok "File $MAIN_PDF dihapus."
}

# ------------------------------------------------------------------------------
# 1. Pemeriksaan Dependensi
# ------------------------------------------------------------------------------
check_dependencies() {
  log_header "Memeriksa Dependensi Sistem dan LaTeX"

  local missing_bins=()
  local missing_pkgs_req=()
  local missing_pkgs_opt=()

  # 1.1 Periksa CLI Binaries
  log_info "Memeriksa executable pendukung..."
  
  if command -v pdflatex >/dev/null 2>&1; then
    log_ok "pdflatex ditemukan: $(command -v pdflatex)"
  else
    log_error "pdflatex TIDAK ditemukan!"
    missing_bins+=("pdflatex (texlive-latex-base)")
  fi

  if command -v kpsewhich >/dev/null 2>&1; then
    log_ok "kpsewhich ditemukan: $(command -v kpsewhich)"
  else
    log_error "kpsewhich TIDAK ditemukan!"
    missing_bins+=("kpsewhich (texlive-binaries)")
  fi

  if command -v latexmk >/dev/null 2>&1; then
    log_ok "latexmk ditemukan (opsional, disukai): $(command -v latexmk)"
  else
    log_warn "latexmk tidak ditemukan. Kompilasi akan menggunakan pdflatex secara langsung."
  fi

  # 1.2 Periksa LaTeX Packages via kpsewhich (Wajib)
  if command -v kpsewhich >/dev/null 2>&1; then
    log_info "Memeriksa paket-paket LaTeX wajib..."

    local required_packages=(
      "inputenc.sty"
      "fontenc.sty"
      "geometry.sty"
      "graphicx.sty"
      "xcolor.sty"
      "booktabs.sty"
      "array.sty"
      "tabularx.sty"
      "amsmath.sty"
      "float.sty"
      "caption.sty"
      "fancyhdr.sty"
      "listings.sty"
      "tikz.sty"
      "pgfplots.sty"
      "circuitikz.sty"
      "setspace.sty"
      "tcolorbox.sty"
      "hyperref.sty"
    )

    for pkg in "${required_packages[@]}"; do
      if kpsewhich "$pkg" >/dev/null 2>&1; then
        echo -e "  ${GREEN}✔${RESET} $pkg"
      else
        echo -e "  ${RED}✘${RESET} $pkg (kurang)"
        missing_pkgs_req+=("$pkg")
      fi
    done

    # 1.3 Periksa Paket Opsional / Tambahan
    log_info "Memeriksa paket-paket LaTeX opsional..."

    # siunitx
    if kpsewhich "siunitx.sty" >/dev/null 2>&1; then
      echo -e "  ${GREEN}✔${RESET} siunitx.sty (paket asli aktif)"
    else
      echo -e "  ${YELLOW}!${RESET} siunitx.sty tidak ditemukan (fallback terpasang di main.tex)"
      missing_pkgs_opt+=("siunitx.sty (paket texlive-science)")
    fi

    # babel indonesian
    if kpsewhich "indonesian.ldf" >/dev/null 2>&1; then
      echo -e "  ${GREEN}✔${RESET} indonesian.ldf (tata bahasa Indonesia aktif)"
    else
      echo -e "  ${YELLOW}!${RESET} indonesian.ldf tidak ditemukan (fallback label manual di main.tex)"
      missing_pkgs_opt+=("indonesian.ldf (paket texlive-lang-other / texlive-lang-indonesian)")
    fi

    # lmodern
    if kpsewhich "lmodern.sty" >/dev/null 2>&1; then
      echo -e "  ${GREEN}✔${RESET} lmodern.sty (font Latin Modern)"
    else
      echo -e "  ${YELLOW}!${RESET} lmodern.sty tidak ditemukan"
      missing_pkgs_opt+=("lmodern.sty (paket texlive-fonts-recommended)")
    fi
  fi

  # 1.4 Hasil Pemeriksaan
  if [ ${#missing_bins[@]} -gt 0 ] || [ ${#missing_pkgs_req[@]} -gt 0 ]; then
    log_header "KESALAHAN DEPENDENSI"
    log_error "Beberapa dependensi wajib belum terpasang di sistem:"
    for item in "${missing_bins[@]}"; do
      echo -e "  - Binary: $item"
    done
    for item in "${missing_pkgs_req[@]}"; do
      echo -e "  - Paket LaTeX: $item"
    done
    echo ""
    log_info "Cara instalasi pada Ubuntu/Debian:"
    echo "  sudo apt update && sudo apt install -y texlive-latex-base texlive-latex-extra texlive-pictures texlive-science"
    echo ""
    log_info "Cara instalasi pada Arch Linux:"
    echo "  sudo pacman -S texlive-basic texlive-latex texlive-latexextra texlive-pictures texlive-science"
    echo ""
    return 1
  fi

  if [ ${#missing_pkgs_opt[@]} -gt 0 ]; then
    log_warn "Beberapa paket opsional belum ada (dokumen tetap dapat dikompilasi via mekanisme fallback):"
    for item in "${missing_pkgs_opt[@]}"; do
      echo -e "  - $item"
    done
    log_info "Untuk instalasi paket opsional lengkap di Ubuntu/Debian:"
    echo "  sudo apt install -y texlive-science texlive-lang-other texlive-fonts-recommended"
  fi

  log_ok "Pemeriksaan dependensi selesai. Semua kebutuhan utama terpenuhi!\n"
  return 0
}

# ------------------------------------------------------------------------------
# 2. Kompilasi LaTeX
# ------------------------------------------------------------------------------
compile_document() {
  local engine="$1"
  local watch_mode="$2"

  if [ ! -f "$MAIN_TEX" ]; then
    log_error "File utama '$MAIN_TEX' tidak ditemukan di $(pwd)"
    exit 1
  fi

  log_header "Memulai Kompilasi Dokumen ($MAIN_TEX)"

  local start_time
  start_time=$(date +%s)

  if [ "$watch_mode" = true ]; then
    if ! command -v latexmk >/dev/null 2>&1; then
      log_error "Mode watch memerlukan 'latexmk'. Silakan pasang latexmk terlebih dahulu."
      exit 1
    fi
    log_info "Menjalankan latexmk dalam mode watch (-pvc)... Tekan Ctrl+C untuk keluar."
    exec latexmk -pdf -pvc -interaction=nonstopmode "$MAIN_TEX"
  fi

  if [ "$engine" = "latexmk" ] && command -v latexmk >/dev/null 2>&1; then
    log_info "Mengompilasi menggunakan latexmk..."
    if ! latexmk -pdf -interaction=nonstopmode -halt-on-error "$MAIN_TEX"; then
      log_error "Kompilasi dengan latexmk gagal! Periksa main.log untuk detail."
      extract_errors
      exit 1
    fi
  else
    log_info "Mengompilasi menggunakan pdflatex (Putaran 1/2)..."
    if ! pdflatex -interaction=nonstopmode -halt-on-error "$MAIN_TEX" > /dev/null; then
      log_error "Putaran 1 gagal!"
      extract_errors
      exit 1
    fi

    log_info "Mengompilasi menggunakan pdflatex (Putaran 2/2 - sinkronisasi TOC & referensi)..."
    if ! pdflatex -interaction=nonstopmode -halt-on-error "$MAIN_TEX" > /dev/null; then
      log_error "Putaran 2 gagal!"
      extract_errors
      exit 1
    fi
  fi

  local end_time
  end_time=$(date +%s)
  local duration=$((end_time - start_time))

  if [ -f "$MAIN_PDF" ]; then
    local size
    size=$(du -h "$MAIN_PDF" | cut -f1)
    local pages="N/A"
    if command -v pdfinfo >/dev/null 2>&1; then
      pages=$(pdfinfo "$MAIN_PDF" | awk '/^Pages:/ {print $2}')
    elif [ -f "main.log" ]; then
      pages=$(grep -o "Output written on .* ([0-9]\+ pages" main.log | grep -o "[0-9]\+" || echo "N/A")
    fi

    log_header "Kompilasi Sukses"
    log_ok "File PDF berhasil dibuat: ${BOLD}${GREEN}${MAIN_PDF}${RESET}"
    echo "  - Ukuran file : $size"
    echo "  - Jumlah hal. : $pages halaman"
    echo "  - Waktu proses: ${duration} detik"
  else
    log_error "Kompilasi selesai namun file '$MAIN_PDF' tidak ditemukan."
    exit 1
  fi
}

extract_errors() {
  if [ -f "main.log" ]; then
    echo -e "\n${RED}--- Cuplikan Kesalahan dari main.log ---${RESET}"
    grep -A 2 -E "^! " main.log | head -n 30 || true
    echo -e "${RED}----------------------------------------${RESET}\n"
  fi
}

# ------------------------------------------------------------------------------
# 3. Main Logic & Arg Parsing
# ------------------------------------------------------------------------------
MODE_CHECK_ONLY=false
MODE_WATCH=false
OPEN_PDF=false
ENGINE="auto"

while [[ $# -gt 0 ]]; do
  case "$1" in
    -h|--help)
      show_help
      exit 0
      ;;
    -c|--check)
      MODE_CHECK_ONLY=true
      shift
      ;;
    --clean)
      clean_aux_files
      exit 0
      ;;
    --clean-all)
      clean_all_files
      exit 0
      ;;
    --pdflatex)
      ENGINE="pdflatex"
      shift
      ;;
    --latexmk)
      ENGINE="latexmk"
      shift
      ;;
    -w|--watch)
      MODE_WATCH=true
      shift
      ;;
    --open)
      OPEN_PDF=true
      shift
      ;;
    *)
      log_error "Opsi tidak dikenal: $1"
      show_help
      exit 1
      ;;
  esac
done

# Step 1: Pemeriksaan Dependensi
if ! check_dependencies; then
  log_error "Pemeriksaan dependensi gagal. Harap penuhi kebutuhan di atas sebelum mengompilasi."
  exit 1
fi

if [ "$MODE_CHECK_ONLY" = true ]; then
  exit 0
fi

# Tentukan engine jika auto
if [ "$ENGINE" = "auto" ]; then
  if command -v latexmk >/dev/null 2>&1; then
    ENGINE="latexmk"
  else
    ENGINE="pdflatex"
  fi
fi

# Step 2: Kompilasi
compile_document "$ENGINE" "$MODE_WATCH"

# Step 3: Buka PDF jika diminta
if [ "$OPEN_PDF" = true ] && [ -f "$MAIN_PDF" ]; then
  if command -v xdg-open >/dev/null 2>&1; then
    log_info "Membuka $MAIN_PDF dengan xdg-open..."
    xdg-open "$MAIN_PDF" >/dev/null 2>&1 &
  else
    log_warn "xdg-open tidak tersedia untuk membuka PDF secara otomatis."
  fi
fi
