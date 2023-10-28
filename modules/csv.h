
#ifndef CSV_H
#define CSV_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

void read_csv(std::vector<std::vector<std::string>>& ct,
              const std::string& filename);

void print_csv(std::vector<std::vector<std::string>>& ct);

void write_csv(std::vector<std::vector<std::string>>& ct,
               const std::string& filename);

void read_csv_from_file_list(std::vector<std::vector<std::string>>& combinedData,
                             const std::vector<std::string>& fileNames);

#endif // CSV_H