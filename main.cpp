#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

// Struct untuk menyimpan data KBLI
struct KBLI_Data {
    std::string kbli_code;
    std::vector<int> counts; 
    double avg_growth; 
    double total_change; 
};

// Fungsi untuk membaca data dari CSV (ekspor dari Excel)
std::vector<KBLI_Data> readCSV(const std::string& filename) {
    std::vector<KBLI_Data> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return data;
    }

    std::string line;
    bool isFirstLine = true;
    while (std::getline(file, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue; // skip header
        }

        std::stringstream ss(line);
        std::string token;
        KBLI_Data entry;
        bool firstToken = true;

        while (std::getline(ss, token, ',')) {
            if (firstToken) {
                entry.kbli_code = token;
                firstToken = false;
            } else {
                entry.counts.push_back(std::stoi(token));
            }
        }
        data.push_back(entry);
    }
    file.close();
    return data;
}

// Fallback input manual
std::vector<KBLI_Data> inputManual() {
    std::vector<KBLI_Data> data;
    int n;
    std::cout << "Masukkan jumlah KBLI: ";
    std::cin >> n;

    for (int i = 0; i < n; ++i) {
        KBLI_Data entry;
        std::cout << "KBLI code: ";
        std::cin >> entry.kbli_code;

        int years;
        std::cout << "Jumlah tahun: ";
        std::cin >> years;

        for (int j = 0; j < years; ++j) {
            int count;
            std::cout << "Tahun " << j + 1 << ": ";
            std::cin >> count;
            entry.counts.push_back(count);
        }
        data.push_back(entry);
    }
    return data;
}
