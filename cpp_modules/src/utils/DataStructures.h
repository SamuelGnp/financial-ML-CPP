//
// Created by Work on 01/03/2024.
//



#ifndef QUANT_DATASTRUCTURES_H
#define QUANT_DATASTRUCTURES_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

constexpr size_t MaxLines = 600'000u;
constexpr size_t NumberOfLines = 500'000u;
constexpr size_t ifStreamBufferSize = 100'000u;
static char buffer[ifStreamBufferSize];

// Object oriented Model. Class for one record
struct Record {
    // Data
    std::string date{};
    std::string time{};
    std::tm datetime{};;
    double price{};
    double bid{};
    double ask{};
    double spread{};
    int volume{};
    double dollarValue{};

    // Methods
    // Extractor operator
    friend std::istream &operator>>(std::istream &is, Record &r) {
        // Read one complete line
        if (std::string line; std::getline(is, line)) {
            // Here we will store the parts of the line after the split
            std::vector<std::string> parts{};
            // Convert line to istringstream for further extraction of line parts
            std::istringstream iss{line};
            // One part of a line
            std::string part{};
            bool wrongData = false;
            // Split
            while (std::getline(iss, part, ',')) {
                // Check fpor error
                if (part[0] == '#') {
                    is.setstate(std::ios::failbit);
                    break;
                }
                // add part
                parts.push_back(std::move(part));
            }
            // If all was OK
            if (is) {
                // If we have enough parts
                if (parts.size() == 6) {
                    r.date = parts[0];
                    r.time = parts[1];
                    r.price = std::stod(parts[2]);
                    r.bid = std::stod(parts[3]);
                    r.ask = std::stod(parts[4]);
                    r.volume = std::stoi(parts[5]);
                } else is.setstate(std::ios::failbit);
            }
        }
        return is;
    }

    // Simple inserter function
    friend std::ostream &operator<<(std::ostream &os, const Record &r) {
        return os << r.date << " "
                  #pragma warning(suppress : 4996)
                  << r.time << " " << r.price << " " << r.bid << " " << r.ask << " " << r.volume << " " << r.spread;
    }
};

struct Data {
    // Data part
    std::vector<Record> records{};

    // Constructor will reserve space to avoid reallocation
    Data() { records.reserve(MaxLines); }

    // Simple extractor. Will call Record's extractor
    friend std::istream &operator>>(std::istream &is, Data &d) {
        // Set bigger file buffer. This is a time saver
        is.rdbuf()->pubsetbuf(buffer, ifStreamBufferSize);
        std::copy(std::istream_iterator<Record>(is), {}, std::back_inserter(d.records));
        return is;
    }

    // Simple inserter
    friend std::ostream &operator>>(std::ostream &os, const Data &d) {
        std::copy(d.records.begin(), d.records.end(), std::ostream_iterator<Record>(os, "\n"));
        return os;
    }
};

//we create a new structure that is the same as out Record structure
struct TickBar {
    std::vector<Record> ticks;
};

TickBar createTickBar(const Data &data, size_t tickInterval);

struct VolumeBar {
    std::vector<Record> tickVolume;
};

VolumeBar createVolumeBar(const Data &data, size_t volumeThreshold);

struct DollarTick {
    std::string date{};
    std::string time{};
    double price{};
    double bid{};
    double ask{};
    double spread{};
    int volume{};
    double dollarVolume{};
};

std::vector<DollarTick> createDollarBar(const Data &data, int tickDollarBar);

#endif //QUANT_DATASTRUCTURES_H
