// ==========================
// File: main.cpp
// ==========================
#include <iostream>
#include "data_loader.h"
#include "analytics.h"
#include "formatter.h"


int main() {
    // Title / description (SDG mention)
    std::cout << "Sistem Monitoring Perubahan Unit Usaha Mikro–Kecil Tiap KBLI"
    << " -- Mendukung SDG 9 (Industri, Inovasi, Infrastruktur)\n\n";


    // filename: make sure this matches the CSV file in repo (case-sensitive)
    std::string filename = "data_umkm.csv"; // <-- change to your file name (e.g. "Data ALPROG.csv")


    auto rows = loadCSV(filename);
    if (rows.empty()) {
        std::cerr << "Gagal memuat data, periksa nama file dan format CSV.\n";
    return 1;
}
    printHeader();
    printSummary(rows);
    printYearlyDetails(rows);
    printYearSummary(rows);
    printGrowthReport(rows);
    printCategoryReport(rows);
    printSeparator();
    std::cout << " PROSES ANALISIS SELESAI\n";
    printSeparator();

    return 0;
}
