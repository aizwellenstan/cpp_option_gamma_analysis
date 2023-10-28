#include "csv.h"
#include "fileLister.h"
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    std::vector<std::vector<std::string>> ct;
    std::string in_csv = "data/treasuryRisk.csv";

    read_csv(ct, in_csv);

    std::vector<std::string> dateArr;
    for (std::size_t i = 0; i < ct.size(); i++) {  // Start from the second row (index 1)
        std::vector<std::string>& v = ct[i];
        if (v.size() > 1) {  // Ensure the row has at least two columns
            dateArr.push_back(v[0]);  // Add the second column to dateArr
        }
    }

    // Now, dateArr contains the second element of each row (excluding the first row)

    ct.clear();

    std::string folderPath = "data/option";
    // Get a list of all files in the folder
    std::vector<std::string> allFiles = listFilesInDirectory(folderPath);
    read_csv_from_file_list(ct, allFiles);

    // Create a new vector to store values from the second column that match dateArr
    std::vector<std::vector<std::string>> matchingValues;
    // Check the second column of ct and append matching values to matchingValues
    for (std::size_t i = 0; i < ct.size(); i++) {
        std::vector<std::string>& v = ct[i];
        if (v.size() > 1) {  // Ensure the row has at least two columns
            std::string value = v[2];
            if (std::find(dateArr.begin(), dateArr.end(), value) != dateArr.end()) {
                matchingValues.push_back(v);
            }
        }
    }

    // print_csv(matchingValues);
    write_csv(matchingValues,"data/combined.csv");
    
    // for (std::size_t i = 0; i < dateArr.size(); i++) {
    //     std::cout << dateArr.at(i) << std::endl;
    // }
    return 0;
}
