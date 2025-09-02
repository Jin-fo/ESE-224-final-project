#ifndef HEADER_CPP
#define HEADER_CPP
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

/******************************************************************************
 * Date Class Definition
 * Handles date and time operations with string parsing and comparisons
 ******************************************************************************/
class Date {
    //2022_04_23 04:47:30.977374
    private:
    int year;
    int month;
    int day;
    int hour;
    int minute;
    float second;
    // Format each component with leading zero if needed
    
    public:
    /******************************************************************************
     * Constructors and Initialization
     ******************************************************************************/
    Date() {
        year = 0;
        month = 0;
        day = 0;
        hour = 0;
        minute = 0;
        second = 0.0;
    }  // Default constructor
    Date(string date_str) {
        if (date_str.empty()) {
            throw runtime_error("Date string is empty");
        } 
        
        try {
            // Replace underscores and slashes with hyphens
            for (size_t i = 0; i < date_str.length(); i++) {
                if (date_str[i] == '_' || date_str[i] == '/') date_str[i] = '-';
            }
            
            // Parse date components
            stringstream ss(date_str);
            string part1, part2, part3;
            
            getline(ss, part1, '-');
            getline(ss, part2, '-');
            getline(ss, part3, ' '); // Stop at space if time follows
            
            // Determine format (YYYY-MM-DD vs MM-DD-YYYY)
            if (part1.length() == 4) {
                // YYYY-MM-DD format
                year = stoi(part1);
                month = stoi(part2);
                day = stoi(part3);
            } else {
                // MM-DD-YYYY format
                month = stoi(part1);
                day = stoi(part2);
                year = stoi(part3);
            }
            
            // Parse time if available
            if (date_str.find(':') != string::npos) {
                size_t time_pos = date_str.find(' ');
                if (time_pos != string::npos) {
                    string time_str = date_str.substr(time_pos + 1);
                    stringstream ts(time_str);
                    string hour_str, minute_str, second_str;
                    
                    getline(ts, hour_str, ':');
                    getline(ts, minute_str, ':');
                    getline(ts, second_str);
                    
                    hour = stoi(hour_str);
                    minute = stoi(minute_str);
                    second = stof(second_str);
                }
            } else {
                hour = minute = 0;
                second = 0;
            }
            
            // Validate
            if (!is_valid()) {
                throw runtime_error("Invalid date values in: " + date_str);
            }
        } catch (const exception& e) {
            throw runtime_error("Error parsing date '" + date_str + "': " + e.what());
        }
    }

    /******************************************************************************
     * Validation and Utility Functions
     ******************************************************************************/
    bool is_valid() const {
        // Check basic range validity
        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        
        // Days per month (not accounting for leap years in February)
        int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        
        // Adjust February for leap years
        if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))) {
            days_in_month[2] = 29;
        }
        
        if (day < 1 || day > days_in_month[month]) return false;
        if (hour < 0 || hour > 23) return false;
        if (minute < 0 || minute > 59) return false;
        if (second < 0 || second >= 60) return false;
        
        return true;
    }

    /******************************************************************************
     * Getters and Formatting
     ******************************************************************************/
    int get_year() {
        return year;
    }
    int get_month() {
        return month;
    }   
    int get_day() {
        return day;
    }
    int get_hour() {
        return hour;
    }
    int get_minute() {
        return minute;
    }
    float get_second() {
        return second;
    }
    string format(int n) {
        if (n < 10) {
            return "0" + to_string(n);
        }
        return to_string(n);
    }
    
    /******************************************************************************
     * Operators and Calculations
     ******************************************************************************/
    string operator*() {
        string result = to_string(year) + "_" + format(month) + "_" + format(day) + " " + 
                       format(hour) + ":" + format(minute) + ":" + format(int(second));
        return result;
    }
    Date operator-(Date date) {
        Date result;
        result.year = this->year - date.year;
        result.month = this->month - date.month;
        result.day = this->day - date.day;
        result.hour = this->hour - date.hour;
        result.minute = this->minute - date.minute;
        result.second = this->second - date.second;
        return result;
    }
    int to_minutes() {
        return (this->year * 365 + this->month * 30 + this->day) * 24 * 60 + this->hour * 60 + this->minute + this->second / 60;
    }
    bool operator>(Date date) {
        if (year != date.year) return year > date.year;
        if (month != date.month) return month > date.month;
        if (day != date.day) return day > date.day;
        if (hour != date.hour) return hour > date.hour;
        if (minute != date.minute) return minute > date.minute;
        return second > date.second;
    }
    bool operator<(Date date) {
        if (year != date.year) return year < date.year;
        if (month != date.month) return month < date.month;
        if (day != date.day) return day < date.day;
        if (hour != date.hour) return hour < date.hour;
        if (minute != date.minute) return minute < date.minute;
        return second < date.second;
    }
    bool operator==(Date date) {
        return year == date.year && month == date.month && day == date.day &&
               hour == date.hour && minute == date.minute && second == date.second;
    }
    friend class Document;
};

#endif // HEADER_CPP
