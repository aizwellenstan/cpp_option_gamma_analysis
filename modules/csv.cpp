#include "csv.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void read_csv(std::vector<std::vector<std::string>>& ct, const std::string& filename)
{
    std::string str;
    std::ifstream ifs(filename);
    bool skipHeader = true;

    while (std::getline(ifs, str)) {
        if (skipHeader) {
            skipHeader = false;
            continue;
        }

        std::istringstream is(str);
        std::vector<std::string> v;
        std::string buf;  // Reuse the same buffer

        while (std::getline(is >> std::ws, buf, ',')) {
            v.push_back(std::move(buf));  // Use move semantics to save memory
        }
        ct.push_back(std::move(v));  // Use move semantics to save memory
    }
}

void read_csv_from_file_list(std::vector<std::vector<std::string>>& combinedData, const std::vector<std::string>& fileNames)
{
    for (const std::string& filename : fileNames) {
        std::string str;
        std::ifstream ifs(filename);
        bool skipHeader = true;

        while (std::getline(ifs, str)) {
            if (skipHeader) {
                skipHeader = false;
                continue;
            }

            std::istringstream is(str);
            std::vector<std::string> v;
            std::string buf;  // Reuse the same buffer

            while (std::getline(is >> std::ws, buf, ',')) {
                v.push_back(std::move(buf));  // Use move semantics to save memory
            }
            combinedData.push_back(std::move(v));  // Use move semantics to save memory
        }
    }
}

void write_csv(std::vector<std::vector<std::string>>& ct, const std::string& filename)
{
    std::ofstream ofs(filename);
    for (std::vector<std::string>& v : ct) {
        for (std::size_t i = 0; i < v.size(); i++) {
            ofs << v.at(i);
            if (i < v.size() - 1) {
                ofs << ",";
            }
        }
        ofs << std::endl;
    }
}
