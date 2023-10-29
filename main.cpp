#include "csv.h"
#include "fileLister.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <ctime>
#include <set>
#include <algorithm>

std::string formatDate(const std::string& inputDate) {
    int year, month, day;

    // Extract year, month, and day from the input string
    if (sscanf(inputDate.c_str(), "%d-%d-%d", &year, &month, &day) == 3) {
        // Format the date as "mm/dd/yyyy"
        return std::to_string(month) + "/" + std::to_string(day) + "/" + std::to_string(year);
    } else {
        return "Invalid date format";
    }
}

struct DteData {
    float average;
    float pDelta;
};


void writeToCSV(const std::string& filename, const std::vector<std::tuple<int, int, int, float, float>>& data) {
    std::ofstream csvFile(filename);

    if (csvFile.is_open()) {
        csvFile << "DTE1,DTE2,Count,Average,pDelta\n";

        for (const auto& entry : data) {
            int dte1, dte2, count;
            float average, pDelta;
            std::tie(dte1, dte2, count, average, pDelta) = entry;

            csvFile << dte1 << "," << dte2 << "," << count << "," << average << "," << pDelta << "\n";
        }

        csvFile.close();
        std::cout << "CSV file written: " << filename << std::endl;
    } else {
        std::cerr << "Error: Unable to open the CSV file for writing." << std::endl;
    }
}

int main(int argc, char* argv[])
{
    std::vector<std::vector<std::string>> ct;
    std::string in_csv = "data/treasuryRisk.csv";

    read_csv(ct, in_csv);

    std::vector<std::string> dateArr;
    dateArr.reserve(ct.size());  // Reserve space

    for (const auto& v : ct) {
        if (v.size() > 1) {
            dateArr.emplace_back(v[0]);  // Use emplace_back
        }
    }

    ct.clear();

    std::string folderPath = "data/option/qqq";
    std::vector<std::string> allFiles = listFilesInDirectory(folderPath);
    read_csv_from_file_list(ct, allFiles);

    std::vector<std::vector<std::string>> matchingValues;
    matchingValues.reserve(ct.size());  // Reserve space

    for (const auto& v : ct) {
        if (v.size() > 1) {
            std::string value = v[2];
            if (std::find(dateArr.begin(), dateArr.end(), value) != dateArr.end()) {
                matchingValues.emplace_back(v);  // Use emplace_back
            }
        }
    }

    std::map<std::pair<std::string, std::string>, std::vector<std::vector<std::string>>> groupedMap;

    for (const auto& v : matchingValues) {
        if (v.size() > 1) {
            std::string key1 = v[5];
            std::string key2 = v[19];
            groupedMap[{key1, key2}].push_back(v);
        }
    }

    std::map<std::pair<std::string, std::string>, std::vector<std::vector<std::string>>>
        filteredMap;

    std::map<std::pair<int, int>, std::vector<DteData>> dteMap;

    for (auto it = groupedMap.begin(); it != groupedMap.end(); ++it) {
        std::vector<std::vector<std::string>>& group = it->second;

        if (group.size() < 2) {
            continue;
        }

        if (group[0][7] == "0.000000") continue;
        if (group[0][20] == "0.020000"
            || group[0][20] == "0.010000" 
            || group[0][20] == "0.000000"
        ) continue;

        bool profitable = false;
        for (int i = 0; i < group.size(); ++i) {
            if (profitable) break;
            float pBid = std::stof(group[i][20]);
            float pAsk = std::stof(group[i][21]);
            int dte1 = std::stoi(group[i][7]);

            // if (dte1 > 45) continue;

            float strike = std::stof(group[i][19]);
            float pDelta = std::stof(group[i][24]);

            // if (pDelta <= -0.470) continue;

            if (pBid > pAsk) continue;
            for (int j = i + 1; j < group.size(); ++j) {
                pAsk = std::stof(group[j][21]);
                int dte2 = std::stoi(group[j][7]);

                // if (dte2 < 21) continue;

                if (pBid > pAsk + 0.01) {
                    profitable = true;

                    // break;
                }
                dteMap[{dte1, dte2}].push_back({(pBid - pAsk) / strike / (dte1 - dte2), pDelta});
            }
        }
        // if (!profitable) continue;

        // bool dataMissing = false;
        // for (int k = 0; k < group.size()-1; ++k) {
        //     int dayDiff = dateDifference(group[k][2], group[k+1][2]);
        //     if (dayDiff >= 5)  {
        //         dataMissing = true;
        //         break;
        //     }
        // }

        // if (dataMissing) continue;

        filteredMap[it->first] = group;
    }

    groupedMap = filteredMap;

    for (const auto& pair : groupedMap) {
        std::pair<std::string, std::string> key = pair.first;
        std::vector<std::vector<std::string>> group = pair.second;

        // std::cout << "Key: (" << key.first << ", " << key.second << ")" << std::endl;
        // for (const auto& v : group) {
        //     for (const std::string& element : v) {
        //         std::cout << element << " ";
        //     }
        //     std::cout << std::endl;
        // }
    }

    std::vector<std::pair<std::string, std::string>> groupKeys;
    groupKeys.reserve(groupedMap.size());

    for (const auto& pair : groupedMap) {
        groupKeys.push_back(pair.first);
    }

    std::vector<std::vector<std::string>> filteredMatchingValues;
    filteredMatchingValues.reserve(matchingValues.size());

    for (const auto& v : matchingValues) {
        std::string key1 = v[5];
        std::string key2 = v[19];
        std::pair<std::string, std::string> keyPair = {key1, key2};
        
        if (std::find(groupKeys.begin(), groupKeys.end(), keyPair) != groupKeys.end()) {
            filteredMatchingValues.emplace_back(std::move(v));
            // std::string underlying = "QQQ";
            // // std::string underlying_last = v[4];
            // std::string underlying_last = v[24]; // pDelta
            // std::string option_type = "put";
            // std::string expiration = formatDate(v[5]);
            // std::string quotedate = formatDate(v[2]);
            // std::string strike = v[19];
            // std::string bid = v[20];
            // std::string ask = v[21];

            // std::vector<std::string> optionData;
            // optionData.push_back(underlying);
            // optionData.push_back(underlying_last);
            // optionData.push_back(option_type);
            // optionData.push_back(expiration);
            // optionData.push_back(quotedate);
            // optionData.push_back(strike);
            // optionData.push_back(bid);
            // optionData.push_back(ask);
            // filteredMatchingValues.emplace_back(optionData);
        }
    }

    // write_csv(filteredMatchingValues, "data/pythonCombinedDelta.csv");

    std::vector<std::tuple<int, int, int, float, float>> dataWithCountAverageAndPDelta;

    for (const auto& entry : dteMap) {
        const std::pair<int, int>& dtePair = entry.first;
        const std::vector<DteData>& dteDataList = entry.second;

        int count = dteDataList.size();
        if (count > 0) {
            float sumAverage = 0.0;
            float sumPDelta = 0.0;

            for (const DteData& dteData : dteDataList) {
                sumAverage += dteData.average;
                sumPDelta += dteData.pDelta;
            }

            float average = sumAverage / static_cast<float>(count);
            float pDelta = sumPDelta / static_cast<float>(count);

            dataWithCountAverageAndPDelta.push_back(std::make_tuple(dtePair.first, dtePair.second, count, average, pDelta));
        }
    }

    // Sort the data in descending order by the average
    std::sort(dataWithCountAverageAndPDelta.begin(), dataWithCountAverageAndPDelta.end(), [](const auto& a, const auto& b) {
        return std::get<3>(a) > std::get<3>(b);
    });

    // Write the sorted data to a CSV file
    writeToCSV("data/dte.csv", dataWithCountAverageAndPDelta);

    return 0;
}
