#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "csv.h"
#include "fileLister.h"

struct OptionData {
    std::string quoteDate;
    int dte;
    double cGamma;
    double pGamma;
    double strike;
};

int main() {
    // ... (Existing code remains unchanged)

    std::string folderPath = "data/option/qqq";
    std::vector<std::string> allFiles = listFilesInDirectory(folderPath);
    std::vector<std::vector<std::string>> ct;  
    read_csv_from_file_list(ct, allFiles);

    // Use a map to store the maximum gamma and corresponding strike for each quote date and DTE
    std::map<std::tuple<std::string, int>, std::tuple<double, double, double>> maxGammaMap;

    for (const auto& v : ct) {
        OptionData optionData;
        optionData.quoteDate = v[2]; // QUOTE_DATE
        optionData.dte = std::stoi(v[7]); // DTE
        optionData.cGamma = std::stod(v[9]); // C_GAMMA
        optionData.pGamma = std::stod(v[25]); // P_GAMMA
        optionData.strike = std::stod(v[19]); // STRIKE

        auto key = std::make_tuple(optionData.quoteDate, optionData.dte);

        // Check if the current gamma is greater than the stored maximum gamma
        if (maxGammaMap.find(key) == maxGammaMap.end() ||
            optionData.cGamma + optionData.pGamma > std::get<0>(maxGammaMap[key]) + std::get<1>(maxGammaMap[key])) {
            maxGammaMap[key] = std::make_tuple(optionData.cGamma, optionData.pGamma, optionData.strike);
        }
    }

    // Write the highest C_GAMMA + P_GAMMA values and corresponding strike to CSV
    std::ofstream outputFile("data/gamma.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    // Write CSV header
    outputFile << "DATE,DTE,C_GAMMA_P_GAMMA,STRIKE\n";

    // Write data to CSV
    for (const auto& entry : maxGammaMap) {
        outputFile << std::get<0>(entry.first) << ","
                   << std::get<1>(entry.first) << ","
                   << std::get<0>(entry.second) + std::get<1>(entry.second) << ","
                   << std::get<2>(entry.second) << "\n";
    }

    outputFile.close();

    return 0;
}
