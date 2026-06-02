#pragma once
#include <string>
#include <vector>

using namespace std;

struct LogTransaksi {
    int idLog;
    string waktu;
    string kode_barang;
    string jenisAksi;
    string detail;
};

struct Barang {
    string kode_barang;
    string nama_barang;
    int stok;
    double harga;
    bool statusAktif;
};