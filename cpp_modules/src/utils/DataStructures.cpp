#include "DataStructures.h"
#include <sstream>


std::vector<DollarTick> createDollarBar(const Data &data, int tickDollarBar) {
    std::vector<DollarTick> dollarTicks;
    DollarTick dollarTick;
    double dollarVolumeAcc = 0;
    for (const auto &record: data.records) {
        double dollarVolumeCalc = record.price * record.volume;
        dollarVolumeAcc += dollarVolumeCalc;
        if (dollarVolumeCalc > tickDollarBar) {
            DollarTick dollarTick{record.date, record.time, record.price, record.bid, record.ask,
                                  record.ask - record.bid, record.volume, dollarVolumeAcc};
            dollarVolumeAcc = 0;
            dollarTicks.push_back(dollarTick);
        }
    }

    return dollarTicks;
};

VolumeBar createVolumeBar(const Data &data, size_t tickVolumeBar) {
    VolumeBar volumeBar;
    int volumeAccumulated = 0;
    volumeBar.tickVolume.reserve(data.records.size() / tickVolumeBar);
    for (size_t i = 0; i < data.records.size(); i++) {
        volumeAccumulated += data.records[i].volume;
        if (volumeAccumulated > tickVolumeBar) {
            volumeAccumulated = 0;
            volumeBar.tickVolume.push_back(data.records[i]);
        }
    }
    return volumeBar;
}

TickBar createTickBar(const Data &data, size_t tickInterval) {
    TickBar tickBar;
    // Reserve space for efficiency if you know the number of elements in advance
    tickBar.ticks.reserve(data.records.size() / tickInterval);
    for (size_t i = 0; i < data.records.size(); i += tickInterval) {
        tickBar.ticks.push_back(data.records[i]);
    }
    return tickBar;
}

