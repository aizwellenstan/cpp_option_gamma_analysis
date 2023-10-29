#include "csv.h"
#include "fileLister.h"
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <ctime>
#include <set>

int dateDifference(const std::string& dateStr1, const std::string& dateStr2) {
    const char* dateFormat = "%Y-%m-%d";

    struct std::tm tm1 = {};
    struct std::tm tm2 = {};
    std::istringstream ss1(dateStr1);
    std::istringstream ss2(dateStr2);

    if (strptime(dateStr1.c_str(), dateFormat, &tm1) == nullptr ||
        strptime(dateStr2.c_str(), dateFormat, &tm2) == nullptr) {
        std::cerr << "Failed to parse date strings." << std::endl;
        return false;
    }

    // Calculate the difference in days by comparing the integer representations of the dates.
    time_t time1 = std::mktime(&tm1);
    time_t time2 = std::mktime(&tm2);
    int daysDifference = (time2 - time1) / (60 * 60 * 24);

    return daysDifference;
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

    std::string folderPath = "data/option";
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

    for (auto it = groupedMap.begin(); it != groupedMap.end(); ++it) {
        std::vector<std::vector<std::string>>& group = it->second;

        if (group.size() < 2) {
            continue;
        }

        if (group.size() == 2) {
            int dayDiff = dateDifference(group[0][2], group[1][2]);
            if (dayDiff <= 5) {
                filteredMap[it->first] = group;
            }
        } else {
            filteredMap[it->first] = group;
        }
    }

    groupedMap = filteredMap;

    for (const auto& pair : groupedMap) {
        std::pair<std::string, std::string> key = pair.first;
        std::vector<std::vector<std::string>> group = pair.second;

        std::cout << "Key: (" << key.first << ", " << key.second << ")" << std::endl;
        for (const auto& v : group) {
            for (const std::string& element : v) {
                std::cout << element << " ";
            }
            std::cout << std::endl;
        }
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
        }
    }

    write_csv(filteredMatchingValues, "data/combined.csv");

    return 0;
}
