#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <string>
#include "Inventaris.h"

using namespace std;

// =========================================================================
// FUNGSI VALIDATOR ASLI (DARI main.cpp)
// =========================================================================
int ambilInputIntBerbatas(string labelPrompt, int minVal, int maxVal, bool bolehKosong = false, int defaultVal = 0) {
    string input;
    getline(cin, input);
    bool hanyaSpasi = true;
    for (char c : input) { if (c != ' ' && c != '\t') { hanyaSpasi = false; break; } }
    if ((bolehKosong && (input.empty() || hanyaSpasi))) return defaultVal;
    if (input.empty() || hanyaSpasi) return -1;
    bool valid = true;
    size_t start = (input[0] == '-') ? 1 : 0;
    if (start == input.size()) valid = false;
    for (size_t i = start; i < input.size(); i++) { if (!isdigit((unsigned char)input[i])) { valid = false; break; } }
    if (!valid || input.size() > 11) return -1; 
    try {
        long long ll = stoll(input);
        if (ll < minVal || ll > maxVal) return -1;
        return (int)ll;
    } catch (...) { return -1; }
}

double ambilInputHargaBerbatas(string labelPrompt, double minVal, double maxVal) {
    string input;
    getline(cin, input);
    if (input.empty()) return -1;
    bool valid = true;
    int titikCount = 0;
    size_t start = (input[0] == '-') ? 1 : 0;
    if (start == input.size()) valid = false;
    for (size_t i = start; i < input.size(); i++) {
        if (input[i] == '.') { titikCount++; if (titikCount > 1) { valid = false; break; } }
        else if (!isdigit((unsigned char)input[i])) { valid = false; break; }
    }
    if (!valid) return -1;
    try {
        double nilai = stod(input);
        if ((maxVal > 0 && nilai > maxVal) || nilai < minVal) return -1;
        return nilai;
    } catch (...) { return -1; }
}

// =========================================================================
// HELPER FOR DATABASE STATE-VERIFICATION
// =========================================================================
int dapatkanStokAktualDariCSV(string kodeCari) {
    ifstream file("inventaris.csv");
    if (!file.is_open()) return -1;
    string line;
    getline(file, line); // Lewati Header
    while (getline(file, line)) {
        if (line.empty()) continue;
        if (line.back() == '\r') line.pop_back();
        stringstream ss(line);
        string kode, nama, stokStr;
        getline(ss, kode, ';');
        getline(ss, nama, ';');
        getline(ss, stokStr, ';');
        if (kode == kodeCari) return stoi(stokStr);
    }
    return -1;
}

// =========================================================================
// MOTOR PENGUJI DINAMIS (Fungsi-Fungsi yang Membuat Testing Fleksibel)
// =========================================================================

// 1. Fungsi Fleksibel untuk Menguji Error Handling Angka Bulat (Integer)
void ujiValidatorIntDinamis(string inputSimulasi, int min, int max, int ekspektasi, string namaKasus) {
    streambuf* orig_cin = cin.rdbuf();
    stringstream ss(inputSimulasi + "\n");
    cin.rdbuf(ss.rdbuf()); // Alihkan input

    int hasil = ambilInputIntBerbatas("Prompt: ", min, max);
    
    cin.rdbuf(orig_cin); // Kembalikan input ke keyboard

    cout << "[" << namaKasus << "] Input: \"" << inputSimulasi << "\" -> Hasil Sistem: " << hasil;
    if (hasil == ekspektasi) cout << " | STATUS: [PASSED]\n";
    else cout << " | STATUS: [FAILED] (Harusnya " << ekspektasi << ")\n";
}

// 2. Fungsi Fleksibel untuk Menguji Error Handling Desimal/Harga (Double)
void ujiValidatorHargaDinamis(string inputSimulasi, double min, double max, double ekspektasi, string namaKasus) {
    streambuf* orig_cin = cin.rdbuf();
    stringstream ss(inputSimulasi + "\n");
    cin.rdbuf(ss.rdbuf());

    double hasil = ambilInputHargaBerbatas("Prompt: ", min, max);

    cin.rdbuf(orig_cin);

    cout << "[" << namaKasus << "] Input: \"" << inputSimulasi << "\" -> Hasil Sistem: " << hasil;
    if (hasil == ekspektasi) cout << " | STATUS: [PASSED]\n";
    else cout << " | STATUS: [FAILED] (Harusnya " << ekspektasi << ")\n";
}

// 3. Fungsi Fleksibel untuk Menguji Mutasi Logika Bisnis & Database CSV
void ujiTambahStokDinamis(SistemInventaris &gudang, string kodeBarang, int jumlahTambah) {
    int stokSebelum = dapatkanStokAktualDariCSV(kodeBarang);
    gudang.tambahStok(kodeBarang, jumlahTambah);
    int stokSesudah = dapatkanStokAktualDariCSV(kodeBarang);
    
    cout << "[Logika Bisnis] Tambah +" << jumlahTambah << " pada \"" << kodeBarang << "\" (Sebelum: " << stokSebelum << " | Sesudah: " << stokSesudah << ")\n";
    if (stokSesudah == stokSebelum + jumlahTambah) {
        cout << " >> STATUS MUTASI DATABASE: [SUKSES BERTAHADA]\n";
    } else if (stokSesudah == stokSebelum) {
        cout << " >> STATUS MUTASI DATABASE: [BATAL/TERKUNCI AMAN] (Melanggar Atas/Bawah Pagu)\n";
    } else {
        cout << " >> STATUS MUTASI DATABASE: [GAGAL/ANOMALI]\n";
    }
}

// =========================================================================
// TEMPAT UTAMA UNTUK KAMU UBAH-UBAH DATA DATA TEST SEPUASNYA
// =========================================================================
int main() {
    cout << "========================================================\n";
    cout << "    MASTER TESTING FRAMEWORK (ERROR HANDLING & LOGIC)   \n";
    cout << "========================================================\n\n";

    // -----------------------------------------------------------------
    // KELOMPOK A: UJI ERROR HANDLING INPUT (Kamu Bisa Ubah-Ubah Datanya di Sini!)
    // Format: ujiValidatorIntDinamis("input_kamu", min_batas, max_batas, ekspektasi_output, "Nama Kasus");
    // Catatan: Ekspektasi -1 berarti sistem mendeteksi error dan menolaknya.
    // -----------------------------------------------------------------
    cout << "--- MENGUJI KELOMPOK ERROR HANDLING & VALIDASI INPUT ---\n";
    
    // TC-01: Tes Injeksi Huruf alfabet pada Input Angka
    ujiValidatorIntDinamis("abc", 1, 100, -1, "TC-01: Huruf Alfabet"); 
    ujiValidatorIntDinamis("pilihan_lima", 1, 8, -1, "TC-01: Huruf pada Menu");

    // TC-02: Tes Input Nominal Harga Bernilai Minus (Negatif)
    ujiValidatorHargaDinamis("-50000", 1.0, 10000000.0, -1.0, "TC-02: Harga Minus");

    // TC-03: Tes Input Angka yang Sangat Besar (Mencegah Integer Overflow)
    ujiValidatorIntDinamis("999999999999", 0, 1000, -1, "TC-03: Integer Overflow");

    // Kamu mau iseng tambah tes angka valid? Tinggal tulis gini:
    ujiValidatorIntDinamis("25", 1, 100, 25, "TC-Aman: Angka Normal");


    // -----------------------------------------------------------------
    // KELOMPOK B: UJI MUTASI DATA LOGIK & DATABASE CSV
    // (Kamu juga bisa ubah angka stok sesuka hati!)
    // -----------------------------------------------------------------
    cout << "\n--- MENGUJI KELOMPOK MUTASI DATA & REAL-TIME CSV ---\n";
    SistemInventaris inventarisGudang;
    
    // Set data barang uji mula-mula: Stok Awal = 50 unit
    inventarisGudang.tambahBarangBaru("TST02", "Barang uwow", 5000.0, 100); 

    // Jalankan skenario tambah stok secara dinamis
    ujiTambahStokDinamis(inventarisGudang, "TST02", 50);   
    ujiTambahStokDinamis(inventarisGudang, "TST02", 30); 
    ujiTambahStokDinamis(inventarisGudang, "TST02", 5);   
    cout << "\n========================================================\n";
    cout << "              SELURUH PROSES TESTING SELESAI            \n";
    cout << "========================================================\n";
    return 0;
}