#include <iostream>
#include <limits>
#include <string>
#include "Inventaris.h"

using namespace std;

//data handling
string ambilInputString(string labelPrompt, bool bolehKosong) {
    string input;
    while (true) {
        cout << labelPrompt;
        getline(cin, input);
        if (!bolehKosong && input.empty()) {
            cout << "[⚠️ ERROR] Input tidak boleh kosong! Harap ketik teks dengan benar.\n";
            continue;
        }
        return input;
    }
}

// Data Handling
int ambilInputIntBerbatas(string labelPrompt, int minVal, int maxVal) {
    int nilai;
    while (true) {
        cout << labelPrompt;
        if (cin >> nilai) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan sisa enter
            if (nilai >= minVal && nilai <= maxVal) {
                return nilai;
            }
            cout << "[⚠️ ERROR] Batasan salah! Harap masukkan angka antara " << minVal << " sampai " << maxVal << ".\n";
        } else {
            cout << "[⚠️ ERROR] Salah input! Harap masukkan angka bulat yang valid.\n";
            cin.clear(); // loop handlinh
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        }
    }
}

// 3. data handlinh
double ambilInputHargaBerbatas(string labelPrompt, double minVal) {
    double nilai;
    while (true) {
        cout << labelPrompt;
        if (cin >> nilai) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (nilai >= minVal) {
                return nilai;
            }
            cout << "[⚠️ ERROR] Nominal tidak valid! Harga tidak boleh kurang dari " << minVal << ".\n";
        } else {
            cout << "[⚠️ ERROR] Salah input! Harap masukkan angka nominal harga yang valid.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void tampilkanMenu() {
    cout << "\n============================================\n";
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
                inventarisGudang.tampilkanSemuaBarang();
                break;
            case 2:
                cout << "\n--- Registrasi Barang Baru ---\n";
                kode = ambilInputString("Masukkan Kode Barang : ", false);
                nama = ambilInputString("Masukkan Nama Barang : ", false);
                hrg = ambilInputHargaBerbatas("Masukkan Harga Satuan: ", 1.0); 
                jml = ambilInputIntBerbatas("Masukkan Stok Awal   : ", 0, 1000); 
                inventarisGudang.tambahBarangBaru(kode, nama, hrg, jml);
                break;
            case 3:
                cout << "\n--- Tambah Kuantitas Stok ---\n";
                kode = ambilInputString("Masukkan Kode Barang : ", false);
                jml = ambilInputIntBerbatas("Jumlah Tambah Stok   : ", 1, 1000); 
                inventarisGudang.tambahStok(kode, jml);
                break;
            case 4:
                cout << "\n--- Kurangi Kuantitas Stok ---\n";
                kode = ambilInputString("Masukkan Kode Barang : ", false);
                jml = ambilInputIntBerbatas("Jumlah Kurang Stok   : ", 1, 1000); 
                inventarisGudang.hapusStok(kode, jml, false);
                break;
            case 5:
                cout << "\n--- Soft Delete Barang ---\n";
                kode = ambilInputString("Masukkan Kode Barang : ", false);
                inventarisGudang.hapusStok(kode, 0, true);
                break;
            case 6:
                cout << "\n--- Update Informasi Barang ---\n";
                kode = ambilInputString("Masukkan Kode Barang     : ", false);
                nama = ambilInputString("Nama Baru (Enter jika skip): ", true); 
                hrg = ambilInputHargaBerbatas("Harga Baru (0 jika skip)   : ", 0.0); 
                inventarisGudang.updateBarang(kode, nama, hrg);
                break;
            case 7:
                inventarisGudang.tampilkanLogAudit();
                inventarisGudang.tampilkanValuasiGudang();
                break;
            case 8:
                cout << "\nTerima kasih! Program Kelompok 6 Selesai.\n";
                break;
        }
    } while (pilihan != 8);

    return 0;
}