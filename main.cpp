#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
using namespace std;

/* ============================================================
                     STRUCT DATA UMKM
   ============================================================ */
struct DataUMKM {
    int tahun;
    string kategori_kbli;
    string tipe_usaha;
    double jumlah;
};

/* ============================================================
                     FUNGSI PEMBACA CSV
   ============================================================ */
vector<DataUMKM> bacaCSV(const string& nama_file) {
    vector<DataUMKM> data;
    ifstream file(nama_file);

    if (!file.is_open()) {
        cout << "Gagal membuka file!" << endl;
        return data;
    }

    string baris;
    getline(file, baris); // lewati header

    while (getline(file, baris)) {
        stringstream ss(baris);
        vector<string> kolom;
        string temp;

        while (getline(ss, temp, ',')) {
            kolom.push_back(temp);
        }

        if (kolom.size() != 6) continue;

        DataUMKM d;
        d.kategori_kbli = kolom[2];
        d.tipe_usaha    = kolom[3];
        d.tahun         = stoi(kolom[4]);
        d.jumlah        = stod(kolom[5]);

        data.push_back(d);
    }

    return data;
}

/* ============================================================
                     FUNGSI ANALISIS
   ============================================================ */
double hitungPertumbuhan(double awal, double akhir) {
    if (awal == 0) return 0;
    return ((akhir - awal) / awal) * 100.0;
}

/* ============================================================
                     FUNGSI CETAK LAPORAN
   ============================================================ */

// Ringkasan Data
void cetakRingkasan(const vector<DataUMKM>& data) {
    int jumlahBaris = data.size();
    int tahunAwal = data.front().tahun;
    int tahunAkhir = data.back().tahun;

    vector<string> kategoriUnik;

    for (const auto& d : data) {
        bool ada = false;
        for (const auto& k : kategoriUnik) {
            if (k == d.kategori_kbli) { ada = true; break; }
        }
        if (!ada) kategoriUnik.push_back(d.kategori_kbli);
    }

    cout << "Jumlah baris data    : " << jumlahBaris << endl;
    cout << "Jumlah tahun dicatat : " 
         << tahunAwal << " - " << tahunAkhir 
         << " (" << (tahunAkhir - tahunAwal + 1) << " tahun)\n";
    cout << "Jumlah kategori KBLI : " << kategoriUnik.size() << endl;
    cout << "Jumlah tipe usaha    : 2 (Mikro dan Kecil)\n";
}

// Total per tahun
void cetakTotalTahunan(const map<int,double>& totalTahunan) {
    cout << "\n=== TOTAL UMKM PER TAHUN ===\n";
    for (auto& t : totalTahunan) {
        cout << t.first << " : " << t.second << endl;
    }
}

// Pertumbuhan total
void cetakPertumbuhan(double awal, double akhir) {
    double persen = hitungPertumbuhan(awal, akhir);
    double selisih = akhir - awal;

    cout << "Pertumbuhan : " << selisih 
         << " (" << persen << "%)\n";
}

// Kategori terbesar pada tahun terbaru
void cetakKategoriTerbesar(const vector<DataUMKM>& data) {
    int tahunTerbaru = data.back().tahun;

    vector<string> kategori;
    vector<double> total;

    for (const auto& d : data) {
        if (d.tahun != tahunTerbaru) continue;

        bool ada = false;
        for (int i = 0; i < kategori.size(); i++) {
            if (kategori[i] == d.kategori_kbli) {
                total[i] += d.jumlah;
                ada = true;
                break;
            }
        }

        if (!ada) {
            kategori.push_back(d.kategori_kbli);
            total.push_back(d.jumlah);
        }
    }

    // sorting manual
    for (int i = 0; i < kategori.size(); i++) {
        for (int j = i + 1; j < kategori.size(); j++) {
            if (total[j] > total[i]) {
                swap(total[i], total[j]);
                swap(kategori[i], kategori[j]);
            }
        }
    }

    cout << "\n=== KATEGORI TERBESAR TAHUN TERBARU ===\n";
    for (int i = 0; i < kategori.size(); i++) {
        cout << i+1 << ". " << kategori[i] 
             << " : " << total[i] << endl;
    }
}

// Pertumbuhan per KBLI
void cetakPertumbuhanKBLI(const vector<DataUMKM>& data) {
    vector<string> kbli;
    vector<double> awal;
    vector<double> akhir;

    for (const auto& d : data) {
        bool ada = false;

        for (int i = 0; i < kbli.size(); i++) {
            if (kbli[i] == d.kategori_kbli) {
                akhir[i] = d.jumlah;
                ada = true;
                break;
            }
        }

        if (!ada) {
            kbli.push_back(d.kategori_kbli);
            awal.push_back(d.jumlah);
            akhir.push_back(d.jumlah);
        }
    }

    int n = kbli.size();
    double* growth = new double[n];

    for (int i = 0; i < n; i++) {
        growth[i] = hitungPertumbuhan(awal[i], akhir[i]);
    }

    int idxMax = 0, idxMin = 0;
    for (int i = 1; i < n; i++) {
        if (growth[i] > growth[idxMax]) idxMax = i;
        if (growth[i] < growth[idxMin]) idxMin = i;
    }

    cout << "\n=== PERTUMBUHAN PER KBLI ===\n";

    cout << "\nKBLI Pertumbuhan Tertinggi :\n";
    cout << kbli[idxMax] << " : " << growth[idxMax] << "%\n";

    cout << "\nKBLI Pertumbuhan Terendah :\n";
    cout << kbli[idxMin] << " : " << growth[idxMin] << "%\n";

    delete[] growth;
}

/* ============================================================
                     MAIN PROGRAM
   ============================================================ */
int main() {

    cout << fixed << setprecision(0); // supaya tidak e+06

    vector<DataUMKM> data = bacaCSV("data_umkm.csv");

    if (data.empty()) {
        cout << "File kosong atau gagal dibaca.\n";
        return 0;
    }

    cout << "============================================================\n";
    cout << "      SISTEM MONITORING UMKM PER KBLI (INDONESIA)\n";
    cout << "============================================================\n\n";

    cetakRingkasan(data);

    map<int,double> totalTahunan;

    for (auto& d : data) {
        totalTahunan[d.tahun] += d.jumlah;
    }

    cout << "\n";
    cetakTotalTahunan(totalTahunan);

    double awal = totalTahunan.begin()->second;
    double akhir = totalTahunan.rbegin()->second;

    cout << "\n";
    cetakPertumbuhanKBLI(data);

    cout << "\n=== PERTUMBUHAN ===\n";
    cetakPertumbuhan(awal, akhir);

    cout << "\n";
    cetakKategoriTerbesar(data);

    return 0;
}
