#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Done: Return this process's ID
int Process::Pid() { return pid_; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() {return cpu_; }

// Done: Return the command that generated this process
string Process::Command() { return command_; }

// Done: Return this process's memory utilization
string Process::Ram() { return ram_; }

// Done: Return the user (name) that generated this process
string Process::User() { return user_; }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Done: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const 
{
    //return stoi(this->ram_) > stoi(a.ram_);
    return this->cpu_ > a.cpu_;
}