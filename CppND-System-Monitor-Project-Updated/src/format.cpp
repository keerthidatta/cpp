#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
std::string Format::ElapsedTime(long seconds) 
{
    long hours = seconds/3600;
    long minutes = (seconds/60)%60;
    long secs = seconds % 60;

    //TODO: Verify results/cross check formula 
    std::string HH = hours < 10 ? "0" + std::to_string(hours) : std::to_string(hours);
    std::string MM = minutes < 10 ?  "0" + std::to_string(minutes) : std::to_string(minutes);
    std::string SS = secs < 10 ? "0" + std::to_string(secs) : std::to_string(secs);
    return HH+":"+MM+":"+SS; 
}