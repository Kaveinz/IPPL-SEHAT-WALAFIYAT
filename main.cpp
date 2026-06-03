#include <iostream>
#include <limits>
#include <string>
#include <cstdlib>
#include <cctype>
#include <climits>
#include "Inventaris.h"

using namespace std;

// mode: "" = bebas, "kode" = alfanumerik tanpa spasi maks 10 char,
//        "nama" = huruf/angka/spasi maks 50 char (tidak boleh ada titik koma)
string ambilInputString(string labelPrompt, bool bolehKosong,
                        int maxLen = 0, string mode = "") {
    string input;
    while (true) {
        cout << labelPrompt;
        getline(cin, input);

        // Cek apakah hanya spasi/tab
        bool hanyaSpasi = true;
        for (char c : input) {
            if (c != ' ' && c != '\t') { hanyaSpasi = false; break; }
        }

        // Boleh kosong dan memang kosong: langsung return
        if (bolehKosong && (input.empty() || hanyaSpasi)) return input;

        // Tidak boleh kosong
        if (!bolehKosong && (input.empty() || hanyaSpasi)) {
            cout << "[ERROR] Input tidak boleh kosong atau hanya spasi.\n";
            continue;
        }

        // Validasi panjang maksimal
        if (maxLen > 0 && (int)input.size() > maxLen) {
            cout << "[ERROR] Input terlalu panjang! Maksimal " << maxLen << " karakter.\n";
            continue;
        }

        // Validasi karakter sesuai mode
        if (mode == "kode") {
            // Hanya huruf (A-Z, a-z) dan angka (0-9), tanpa spasi
            bool valid = true;
            for (char &c : input) {
                if (!isalnum((unsigned char)c)) { valid = false; break; }
                c = toupper((unsigned char)c); // OTOMATIS UPPERCASE
            }
            if (!valid) {
                cout << "[ERROR] Kode barang hanya boleh berisi huruf (A-Z, a-z) dan angka (0-9),"
                     << " tanpa spasi atau simbol.\n";
                continue;
            }
        } else if (mode == "nama") {
            // Huruf, angka, dan spasi saja; titik koma dilarang (merusak CSV)
            bool valid = true;
            for (char c : input) {
                if (!isalnum((unsigned char)c) && c != ' ') { valid = false; break; }
            }
            if (!valid) {
                cout << "[ERROR] Nama barang hanya boleh berisi huruf, angka, dan spasi.\n";
                continue;
            }
        }

        // Trim spasi di awal dan akhir
        size_t first = input.find_first_not_of(" \t");
        if (first != string::npos) {
            size_t last = input.find_last_not_of(" \t");
            input = input.substr(first, (last - first + 1));
        }

        return input;
    }
}

// Data Handling — baca sebagai string dulu untuk mencegah integer overflow
int ambilInputIntBerbatas(string labelPrompt, int minVal, int maxVal, bool bolehKosong = false, int defaultVal = 0) {
    string input;
    while (true) {
        cout << labelPrompt;
        getline(cin, input);

        // Cek apakah hanya spasi/tab
        bool hanyaSpasi = true;
        for (char c : input) {
            if (c != ' ' && c != '\t') { hanyaSpasi = false; break; }
        }

        // Jika boleh kosong dan memang kosong (atau hanya spasi), return defaultVal
        if (bolehKosong && (input.empty() || hanyaSpasi)) {
            return defaultVal;
        }

        // Pastikan tidak kosong (jika tidak boleh kosong)
        if (input.empty() || hanyaSpasi) {
            cout << "[ERROR] Input tidak boleh kosong.\n";
            continue;
        }
        bool valid = true;
        size_t start = (input[0] == '-') ? 1 : 0;
        if (start == input.size()) valid = false; // hanya tanda minus
        for (size_t i = start; i < input.size(); i++) {
            if (!isdigit((unsigned char)input[i])) { valid = false; break; }
        }
        if (!valid) {
            cout << "[ERROR] Input tidak valid! Harap masukkan angka bulat (contoh: 5, 100).\n";
            continue;
        }

        // Cegah overflow: panjang string melebihi panjang INT_MAX (10 digit)
        if (input.size() > 11) { // 11 = 10 digit + kemungkinan minus
            cout << "[ERROR] Angka terlalu besar! Masukkan nilai antara " << minVal << " sampai " << maxVal << ".\n";
            continue;
        }

        // Parse dengan long long untuk deteksi overflow sebelum cast ke int
        try {
            long long ll = stoll(input);
            if (ll < minVal || ll > maxVal) {
                cout << "[ERROR] Input di luar batas! Harap masukkan angka antara "
                     << minVal << " sampai " << maxVal << ".\n";
                continue;
            }
            return (int)ll;
        } catch (...) {
            cout << "[ERROR] Angka melebihi batas yang diizinkan. Masukkan nilai antara "
                 << minVal << " sampai " << maxVal << ".\n";
        }
    }
}

// Harga: baca sebagai string lalu parse dengan stod agar konsisten dengan getline
// maxVal = 0 berarti tidak ada batas atas (digunakan saat skip update)
double ambilInputHargaBerbatas(string labelPrompt, double minVal, double maxVal) {
    string input;
    while (true) {
        cout << labelPrompt;
        getline(cin, input);

        if (input.empty()) {
            cout << "[ERROR] Input tidak boleh kosong.\n";
            continue;
        }

        // Validasi: hanya digit, opsional satu titik desimal
        bool valid = true;
        int titikCount = 0;
        size_t start = (input[0] == '-') ? 1 : 0;
        if (start == input.size()) valid = false;
        for (size_t i = start; i < input.size(); i++) {
            if (input[i] == '.') { titikCount++; if (titikCount > 1) { valid = false; break; } }
            else if (!isdigit((unsigned char)input[i])) { valid = false; break; }
        }
        if (!valid) {
            cout << "[ERROR] Input tidak valid! Harap masukkan angka (contoh: 50000 atau 99500.50).\n";
            continue;
        }

        try {
            double nilai = stod(input);
            if (maxVal > 0 && nilai > maxVal) {
                cout << "[ERROR] Harga terlalu besar! Maksimal harga adalah Rp" << (long long)maxVal << ".\n";
                continue;
            }
            if (nilai < minVal) {
                cout << "[ERROR] Nominal tidak valid! Harga minimal Rp" << (long long)minVal << ".\n";
                continue;
            }
            return nilai;
        } catch (...) {
            cout << "[ERROR] Angka tidak dapat dibaca. Harap masukkan nilai yang wajar.\n";
        }
    }
}

void jeda() {
    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.get();
}

void tampilkanMenu() {
    system("cls");
    cout << "============================================\n";
    cout << "   SISTEM INVENTARIS BARANG - KELOMPOK 6\n";
    cout << "============================================\n";
    cout << "1. Lihat Daftar Stok Barang\n";
    cout << "2. Daftarkan Barang Baru\n";
    cout << "3. Tambah Kuantitas Stok (Inbound)\n";
    cout << "4. Kurangi Kuantitas Stok (Outbound)\n";
    cout << "5. Hapus Barang dari Sistem (Soft Delete)\n";
    cout << "6. Update Atribut Informasi Barang\n";
    cout << "7. Lihat Log Audit Trail & Valuasi Aset\n";
    cout << "8. Keluar dari Aplikasi\n";
    cout << "============================================\n";
}

int main() {
    SistemInventaris inventarisGudang;
    inventarisGudang.inisialisasiDataAwal();
    
    int pilihan;
    string kode, nama;
    int jml;
    double hrg;

    do {
        tampilkanMenu();
        
        pilihan = ambilInputIntBerbatas("Pilih opsi menu (1-8): ", 1, 8);

        switch (pilihan) {
            case 1:
                system("cls");
                inventarisGudang.tampilkanSemuaBarang();
                jeda();
                break;
            case 2:
                system("cls");
                cout << "--- Registrasi Barang Baru ---\n";
                inventarisGudang.tampilkanSemuaBarang();
                cout << "\n";
                kode = ambilInputString("Masukkan Kode Barang : ", false, 10, "kode");
                if (inventarisGudang.kodeSudahAda(kode)) {
                    cout << "\n[ERROR] Kode Barang sudah terdaftar di sistem!\n";
                    jeda();
                    break;
                }
                nama = ambilInputString("Masukkan Nama Barang : ", false, 25, "nama");
                hrg = ambilInputHargaBerbatas("Masukkan Harga Satuan: ", 1.0, 100000000.0); 
                jml = ambilInputIntBerbatas("Masukkan Stok Awal (Enter untuk 0): ", 0, 1000, true, 0); 
                inventarisGudang.tambahBarangBaru(kode, nama, hrg, jml);
                jeda();
                break;
            case 3:
                system("cls");
                cout << "--- Tambah Kuantitas Stok ---\n";
                inventarisGudang.tampilkanSemuaBarang();
                cout << "\n";
                kode = ambilInputString("Masukkan Kode Barang : ", false, 10, "kode");
                if (!inventarisGudang.barangAda(kode)) {
                    cout << "\n[ERROR] Barang dengan kode '" << kode << "' tidak ditemukan.\n";
                    jeda();
                    break;
                }
                jml = ambilInputIntBerbatas("Jumlah Tambah Stok   : ", 1, 1000); 
                inventarisGudang.tambahStok(kode, jml);
                jeda();
                break;
            case 4:
                system("cls");
                cout << "--- Kurangi Kuantitas Stok ---\n";
                inventarisGudang.tampilkanSemuaBarang();
                cout << "\n";
                kode = ambilInputString("Masukkan Kode Barang : ", false, 10, "kode");
                if (!inventarisGudang.barangAda(kode)) {
                    cout << "\n[ERROR] Barang dengan kode '" << kode << "' tidak ditemukan.\n";
                    jeda();
                    break;
                }
                jml = ambilInputIntBerbatas("Jumlah Kurang Stok   : ", 1, 1000); 
                inventarisGudang.hapusStok(kode, jml, false);
                jeda();
                break;
            case 5:
                system("cls");
                cout << "--- Soft Delete Barang ---\n";
                inventarisGudang.tampilkanSemuaBarang();
                cout << "\n";
                kode = ambilInputString("Masukkan Kode Barang : ", false, 10, "kode");
                if (!inventarisGudang.barangAda(kode)) {
                    cout << "\n[ERROR] Barang dengan kode '" << kode << "' tidak ditemukan.\n";
                    jeda();
                    break;
                }
                inventarisGudang.hapusStok(kode, 0, true);
                jeda();
                break;
            case 6:
                system("cls");
                cout << "--- Update Informasi Barang ---\n";
                inventarisGudang.tampilkanSemuaBarang();
                cout << "\n";
                kode = ambilInputString("Masukkan Kode Barang     : ", false, 10, "kode");
                nama = ambilInputString("Nama Baru (Enter jika skip): ", true, 25, "nama"); 
                hrg = ambilInputHargaBerbatas("Harga Baru (0 jika skip)   : ", 0.0, 100000000.0); 
                inventarisGudang.updateBarang(kode, nama, hrg);
                jeda();
                break;
            case 7:
                system("cls");
                inventarisGudang.tampilkanLogAudit();
                inventarisGudang.tampilkanValuasiGudang();
                jeda();
                break;
            case 8:
                system("cls");
                cout << "Terima kasih! Program Kelompok 6 Selesai.\n";
                break;
        }
    } while (pilihan != 8);

    return 0;
}