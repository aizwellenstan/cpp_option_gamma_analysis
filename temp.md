#include <iostream>
#include <string>
#include <sstream>
#include <ctime>

bool dateDifferenceGreaterThan5Days(const std::string& dateStr1, const std::string& dateStr2) {
    // Define a date format: YYYY-MM-DD
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

    return daysDifference > 5;
}

int main() {
    std::string date1 = "2023-10-20";
    std::string date2 = "2023-10-27";

    if (dateDifferenceGreaterThan5Days(date1, date2)) {
        std::cout << "The difference is greater than 5 days." << std::endl;
    } else {
        std::cout << "The difference is not greater than 5 days." << std::endl;
    }

    return 0;
}
