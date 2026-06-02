#include "Inventaris.h"
#include <iostream>
#include <ctime>
#include <iomanip>

using namespace std;

SistemInventaris::SistemInventaris() {
    logCounter = 1;
}

string SistemInventaris::getWaktuSekarang() {
    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back();
    return dt;
}

void SistemInventaris::catatLog(string kode_brg, string aksi, string detailPerubahan) {
    LogTransaksi logBaru;
    logBaru.idLog = logCounter++;
    logBaru.waktu = getWaktuSekarang();
    logBaru.kode_barang = kode_brg;
    logBaru.jenisAksi = aksi;
    logBaru.detail = detailPerubahan;
    riwayatAktivitas.push_back(logBaru);
}

void SistemInventaris::inisialisasiDataAwal() {
    gudang.push_back({"B001", "Keyboard Mekanik", 50, 500000.0, true});
    gudang.push_back({"B002", "Mouse Gaming", 30, 250000.0, true});
}

void SistemInventaris::tambahBarangBaru(string kode_brg, string nama_brg, double harga_brg, int stok_awal) {
    for (const auto& b : gudang) {
        if (b.kode_barang == kode_brg) {
            cout << "\n[GAGAL] Kode Barang sudah terdaftar di sistem!\n";
            return;
        }
    }
    //error handling
    if (harga_brg <= 0 || stok_awal < 0 || stok_awal > KAPASITAS_MAKSIMAL) {
        cout << "\n[GAGAL] Aturan validasi dilanggar. Gagal mendaftarkan barang.\n";
        return;
    }
    gudang.push_back({kode_brg, nama_brg, stok_awal, harga_brg, true});
    catatLog(kode_brg, "REGISTRASI", "Mendaftarkan barang baru: " + nama_brg);
    cout << "\n[SUKSES] Barang baru berhasil didaftarkan.\n";
}

void SistemInventaris::tambahStok(string kode_barang, int jumlah_tambah) {
    if (jumlah_tambah <= 0) {
        cout << "\n[GAGAL] Jumlah penambahan harus lebih dari 0!\n";
        return;
    }

    for (auto& b : gudang) {
        if (b.kode_barang == kode_barang && b.statusAktif) {
            // error handling
            if (jumlah_tambah > (KAPASITAS_MAKSIMAL - b.stok)) {
                cout << "\n[GAGAL] Transaksi ditolak! Jumlah melebihi kapasitas gudang (" 
                     << KAPASITAS_MAKSIMAL << " unit).\n";
            } else {
                b.stok += jumlah_tambah;
                catatLog(b.kode_barang, "TAMBAH_STOK", "Menambah stok sebanyak " + to_string(jumlah_tambah) + " unit.");
                cout << "\n[SUKSES] Stok berhasil ditambahkan.\n";
            }
            return;
        }
    }
    cout << "\n[ERROR] Barang tidak ditemukan.\n";
}

void SistemInventaris::hapusStok(string kode_barang, int jumlah_hapus, bool hapusPermanen) {
    for (auto& b : gudang) {
        if (b.kode_barang == kode_barang && b.statusAktif) {
            if (hapusPermanen) {
                if (b.stok > 0) {
                    cout << "\n[GAGAL] Keamanan data: Barang masih memiliki stok fisik, tidak bisa dihapus!\n";
                } else {
                    b.statusAktif = false;
                    catatLog(b.kode_barang, "SOFT_DELETE", "Menghapus/menonaktifkan barang dari sistem.");
                    cout << "\n[SUKSES] Barang berhasil dinonaktifkan.\n";
                }
            } else {
                if (jumlah_hapus <= 0) {
                    cout << "\n[GAGAL] Jumlah pengurangan harus lebih dari 0!\n";
                    return;
                }

                int sisa_stok = b.stok - jumlah_hapus;
                if (sisa_stok < 0) {
                    cout << "\n[GAGAL] Pengurangan gagal! Stok gudang tidak mencukupi.\n";
                } else {
                    b.stok = sisa_stok;
                    catatLog(b.kode_barang, "KURANG_STOK", "Mengurangi stok sebanyak " + to_string(jumlah_hapus) + " unit.");
                    cout << "\n[SUKSES] Kuantitas stok berhasil dikurangi.\n";
                }
            }
            return;
        }
    }
    cout << "\n[ERROR] Barang tidak ditemukan.\n";
}

void SistemInventaris::updateBarang(string kode_barang, string nama_barang_baru, double harga_baru) {
    for (auto& b : gudang) {
        if (b.kode_barang == kode_barang && b.statusAktif) {
            string detail = "";
            if (b.nama_barang != nama_barang_baru && !nama_barang_baru.empty()) {
                detail += "Nama '" + b.nama_barang + "' -> '" + nama_barang_baru + "'. ";
                b.nama_barang = nama_barang_baru;
            }
            
            if (harga_baru > 0 && b.harga != harga_baru) {
                detail += "Harga Rp" + to_string((int)b.harga) + " -> Rp" + to_string((int)harga_baru) + ".";
                b.harga = harga_baru;
            }
            
            if (!detail.empty()) {
                catatLog(b.kode_barang, "UPDATE_INFO", detail);
                cout << "\n[SUKSES] Atribut informasi barang berhasil diperbarui.\n";
            } else {
                cout << "\n[INFO] Tidak ada data yang berubah.\n";
            }
            return;
        }
    }
    cout << "\n[ERROR] Barang tidak ditemukan.\n";
}

void SistemInventaris::tampilkanSemuaBarang() {
    cout << "\n=================================================================\n";
    cout << left << setw(10) << "KODE" << setw(25) << "NAMA BARANG" << setw(12) << "STOK" << "HARGA SATUAN\n";
    cout << "=================================================================\n";
    for (const auto& b : gudang) {
        if (b.statusAktif) {
            cout << left << setw(10) << b.kode_barang 
                 << setw(25) << b.nama_barang 
                 << setw(12) << b.stok 
                 << "Rp" << fixed << setprecision(0) << b.harga << "\n";
        }
    }
    cout << "=================================================================\n";
}

void SistemInventaris::tampilkanLogAudit() {
    cout << "\n============================================================================================\n";
    cout << "                        AUDIT TRAIL LOG TRACKING (AUTOMATIC DETECT)\n";
    cout << "============================================================================================\n";
    for (const auto& log : riwayatAktivitas) {
        cout << "[" << log.waktu << "] LOG #" << log.idLog << " | KODE: " << log.kode_barang 
             << " | AKSI: " << left << setw(13) << log.jenisAksi << " -> " << log.detail << "\n";
    }
    cout << "============================================================================================\n";
}

void SistemInventaris::tampilkanValuasiGudang() {
    double totalValuasi = 0;
    cout << "\n=== LAPORAN RINGKAS ASPEK FINANSIAL ===\n";
    for (const auto& b : gudang) {
        if (b.statusAktif) {
            double nilai = b.stok * b.harga;
            totalValuasi += nilai;
        }
    }
    cout << "Total Valuasi Nilai Aset Fisik Gudang Saat Ini: Rp" << fixed << setprecision(0) << totalValuasi << "\n";
}