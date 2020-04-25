#include "processor.h"
#include <vector>
#include <string>
#include "linux_parser.h"

// Done: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    float cpu_utilization = 0.0;
    std::vector<std::string> processor_data{LinuxParser::CpuUtilization()};
    
    float userF = std::stof(processor_data[0]);
    float niceF = std::stof(processor_data[1]);
    float systemF = std::stof(processor_data[2]);
    float idleF = std::stof(processor_data[3]);
    float iowaitF = std::stof(processor_data[4]);
    float irqF = std::stof(processor_data[5]);
    float softirqF = std::stof(processor_data[6]);
    float stealF = std::stof(processor_data[7]);

    float idle = idleF + iowaitF;
    float nonIdle = userF + niceF + systemF + irqF + softirqF + stealF;
    float total = idle + nonIdle;
    float prevTotal = prevIdle_ + prevNonIdle_;

    float deltaTotal = total - prevTotal;
    float deltaIdle = idle - prevIdle_;

    cpu_utilization = (deltaTotal - deltaIdle) / deltaTotal;

    prevIdle_ = idle;
    prevNonIdle_ = nonIdle;

    return cpu_utilization;
}