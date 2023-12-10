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
    std::map<std::tuple<std::string, int, double>, OptionData> dataMap;  // Use std::tuple as the key type
    for (const auto& v : ct) {
        OptionData optionData;
        optionData.quoteDate = v[2]; // QUOTE_DATE
        optionData.dte = std::stoi(v[7]); // DTE
        optionData.cGamma = std::stod(v[9]); // C_GAMMA
        optionData.pGamma = std::stod(v[25]); // P_GAMMA
        optionData.strike = std::stod(v[19]); // STRIKE

        // Update the map if a higher value is found
        auto key = std::make_tuple(optionData.quoteDate, optionData.dte, optionData.strike);
        dataMap[key] = optionData;
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
    for (const auto& entry : dataMap) {
        outputFile << std::get<0>(entry.first) << ","
                   << std::get<1>(entry.first) << ","
                   << entry.second.cGamma + entry.second.pGamma << ","
                   << std::get<2>(entry.first) << "\n";
    }

    outputFile.close();

    return 0;
}
