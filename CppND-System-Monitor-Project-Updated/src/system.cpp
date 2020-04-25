#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <unistd.h>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

#include <string>

using std::set;
using std::size_t;
using std::string;
using std::vector;

//Compute process utilization
float ComputeCpu(std::vector<std::string> cpu_data, long uptime)
{
    float utime = stoi(cpu_data.at(0));
    float stime = stoi(cpu_data.at(1));
    float cuttime = stoi(cpu_data.at(2));
    float cstime = stoi(cpu_data.at(3));
    float starttime = stoi(cpu_data.at(4));

    float total_time = (utime + stime + cuttime + cstime);

    float uptime_elapsed =  uptime - (starttime/sysconf(_SC_CLK_TCK));

    return ((total_time/sysconf(_SC_CLK_TCK)) / uptime_elapsed); 
} 
// Done: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Done: Return a container composed of the system's processes
vector<Process>& System::Processes() 
{
    std::vector<int> pids{LinuxParser::Pids()};
    std::set<int> pid_set;
    for(auto& process: processes_)
    {
        pid_set.insert(process.Pid());
    }
    for (int pid: pids)
    {
        Process process;
        //Check if pid already exists in processes:
        if (pid_set.find(pid) == pid_set.end())
        {
            process.setpid(pid);
            process.setuser(LinuxParser::User(pid));
            process.setuptime(LinuxParser::UpTime(pid));
            process.setcommand(LinuxParser::Command(pid));
            process.setram(LinuxParser::Ram(pid));
            std::vector<std::string> cpu_data = LinuxParser::CpuProcessUtilization(pid);
            process.setCpu(ComputeCpu(cpu_data, UpTime()));
            processes_.emplace_back(process);
        }
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

// Done: Return the system's kernel identifier (string)
std::string System::Kernel() 
{
    return LinuxParser::Kernel(); 
}

// Done: Return the system's memory utilization

float System::MemoryUtilization() 
{ 
    return LinuxParser::MemoryUtilization();
}

// Done: Return the operating system name
std::string System::OperatingSystem() 
{ 
    return LinuxParser::OperatingSystem();
}

// Done: Return the number of processes actively running on the system
int System::RunningProcesses() 
{
    running_processes_ = LinuxParser::RunningProcesses(); 
    return running_processes_;
}

// Done: Return the total number of processes on the system
int System::TotalProcesses() 
{
    return LinuxParser::TotalProcesses();
}

// Done: Return the number of seconds since the system started running
long int System::UpTime() 
{
    return LinuxParser::UpTime(); 
}