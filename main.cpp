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
    // std::ifstream file("data/option/qqq/qqq_eod_202103.txt");
    // if (!file.is_open()) {
    //     std::cerr << "Error opening file." << std::endl;
    //     return 1;
    // }

    // // Skip the header line
    // std::string header;
    // std::getline(file, header);

    // std::map<std::pair<std::string, int>, OptionData> dataMap;

    // std::string line;
    // while (std::getline(file, line)) {
    //     std::istringstream iss(line);
    //     std::string token;

    //     // Read values from CSV line
    //     std::vector<std::string> v;
    //     while (std::getline(iss, token, ',')) {
    //         v.push_back(token);
    //     }

    //     if (v.size() >= 32) { // Assuming there are at least 32 columns
    //         OptionData optionData;
    //         optionData.quoteDate = trim(v[2]); // QUOTE_DATE
    //         optionData.dte = std::stoi(v[7]); // DTE
    //         optionData.cGamma = std::stod(v[9]); // C_GAMMA
    //         optionData.pGamma = std::stod(v[25]); // P_GAMMA
    //         optionData.strike = std::stod(v[19]); // STRIKE

    //         // Update the map if a higher value is found
    //         auto key = std::make_pair(optionData.quoteDate, optionData.dte);
    //         if (dataMap.find(key) == dataMap.end() || dataMap[key].cGamma + dataMap[key].pGamma < optionData.cGamma + optionData.pGamma) {
    //             dataMap[key] = optionData;
    //         }
    //     }
    // }

    // file.close();

    std::string folderPath = "data/option/qqq";
    std::vector<std::string> allFiles = listFilesInDirectory(folderPath);
    std::vector<std::vector<std::string>> ct;  
    read_csv_from_file_list(ct, allFiles);
    std::map<std::pair<std::string, int>, OptionData> dataMap;
    for (const auto& v : ct) {
        OptionData optionData;
        optionData.quoteDate = v[2]; // QUOTE_DATE
        optionData.dte = std::stoi(v[7]); // DTE
        optionData.cGamma = std::stod(v[9]); // C_GAMMA
        optionData.pGamma = std::stod(v[25]); // P_GAMMA
        optionData.strike = std::stod(v[19]); // STRIKE

        // Update the map if a higher value is found
        auto key = std::make_pair(optionData.quoteDate, optionData.dte);
        if (dataMap.find(key) == dataMap.end() || dataMap[key].cGamma + dataMap[key].pGamma < optionData.cGamma + optionData.pGamma) {
            dataMap[key] = optionData;
        }
    }

    // Write the highest C_GAMMA + P_GAMMA values and corresponding strike to CSV
    std::ofstream outputFile("data/gamma.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    // Write CSV header
    outputFile << "QUOTE_DATE,DTE,C_GAMMA_P_GAMMA,STRIKE\n";

    // Write data to CSV
    for (const auto& entry : dataMap) {
        outputFile << entry.second.quoteDate << ","
                   << entry.second.dte << ","
                   << entry.second.cGamma + entry.second.pGamma << ","
                   << entry.second.strike << "\n";
    }

    outputFile.close();

    return 0;
}
