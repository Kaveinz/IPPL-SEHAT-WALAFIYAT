#pragma once
#include "model.h"
#include <string>
#include <vector>

using namespace std;

class SistemInventaris {
private:
    vector<Barang> gudang;
    vector<LogTransaksi> riwayatAktivitas;
    int logCounter;
    const int KAPASITAS_MAKSIMAL = 1000;

    string getWaktuSekarang();
    void catatLog(string kode_brg, string aksi, string detailPerubahan);

public:
    SistemInventaris();
    void inisialisasiDataAwal();
    void tambahBarangBaru(string kode_brg, string nama_brg, double harga_brg, int stok_awal);
    void tambahStok(string kode_barang, int jumlah_tambah);
    void hapusStok(string kode_barang, int jumlah_hapus, bool hapusPermanen);
    void updateBarang(string kode_barang, string nama_barang_baru, double harga_baru);
    void tampilkanSemuaBarang();
    void tampilkanLogAudit();
    void tampilkanValuasiGudang();
};