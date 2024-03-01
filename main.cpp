#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>
#include "DataStructures.h"
#include "HelpingCalculations.h"


int main() {
    // createFile();
    Data data;
    const std::string fileName{"/Users/work/CLionProjects/quant/IVE_tickbidask_sample.txt"};
    size_t tickInterval = 5;
    size_t tickVolumeBar = 10000;
    size_t tickDollarAmount = 100000;

    // Read fileName and process the filetext
    if (std::ifstream ifs{fileName}; ifs) {
        // Read and parse complete data
        ifs >> data;
    } else std::cerr << "\n*** Error: Could not open '" << fileName << "' for reading\n\n";

    calculateAdditionalFeatures(data);


    TickBar tickBar = createTickBar(data, tickInterval);
    VolumeBar volumeBar = createVolumeBar(data, tickVolumeBar);
    std::vector<DollarTick> dollarTicks = createDollarBar(data, tickDollarAmount);

    // Some debug output to verify the calculation
    std::cout << "\n\nNumber of read records:  " << data.records.size() << "\n\n";
    for (int k = 0; k < std::min(static_cast<size_t>(10), data.records.size()); ++k) {
        std::cout << "Printing record " << k << std::endl;
        std::cout << "Normal records: " << data.records[k] << '\n';
        std::cout << "Tick record: " << tickBar.ticks[k] << '\n';
        std::cout << "Volume tick record: " << volumeBar.tickVolume[k] << '\n';
    }
    for (size_t i = 0; i < std::min(dollarTicks.size(), static_cast<size_t>(10)); ++i) {
        std::cout << "DollarBar " << i << " - Date: " << dollarTicks[i].date
                  << ", Time: " << dollarTicks[i].time << ", Price: " << dollarTicks[i].price
                  << ", Bid: " << dollarTicks[i].bid << ", Ask: " << dollarTicks[i].ask
                  << ", Spread: " << dollarTicks[i].spread << ", Volume: " << dollarTicks[i].volume
                  << ", Dollar Volume: " << dollarTicks[i].dollarVolume << '\n';
    }
}