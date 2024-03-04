//
// Created by Work on 01/03/2024.
//

#include "HelpingCalculations.h"


void calculateAdditionalFeatures(Data &data) {
    for (auto &record: data.records) {
        record.spread = record.ask - record.bid;
        record.dollarValue = record.price * record.volume;
    }
};