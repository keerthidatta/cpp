#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <math.h>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel ;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{
  float utilization = 0.0;
  int MemTotal, MemFree, Buffers, Cached, Reclaimable, shmem;
  std::string line, word;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  if (stream.is_open())
  {
      while(std::getline(stream, line))
      {
        std::istringstream linestream(line);
        linestream >> word;
        if (word == "MemTotal:")
          linestream >> MemTotal;
        else if (word == "MemFree:")
          linestream >> MemFree;
        else if (word == "Buffers:")
          linestream >> Buffers;
        else if (word == "Cached:")
          linestream >> Cached;
        else if (word == "SReclaimable:")
          linestream >> Reclaimable;
        else if (word == "Shmem:")
          linestream >> shmem;
      }
      //TODO: Verify, Include refernce
      //https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
      utilization = static_cast<float>(MemTotal - MemFree + Reclaimable - shmem)/MemTotal;
      
      //Free memory
      //static_cast<float>(MemFree)/MemTotal;

  } 
  return utilization; 
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() 
{
  long uptime=0;
  std::string line, word;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime; 
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{
  std::vector<std::string> cpu_data;
  std::ifstream stream(kProcDirectory+kStatFilename);
  std::string line, cpu, cpu_info;
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    for(int count=0; count < 10; count++)
    {
      linestream >> cpu_info;
      cpu_data.emplace_back(cpu_info);
    }
  }
  return cpu_data;
}

std::vector<std::string> LinuxParser::CpuProcessUtilization(int pid)
{
  std::vector<std::string> cpu_data;
  std::ifstream stream(kProcDirectory+"/"+to_string(pid)+kStatFilename);
  std::string line, cpu_info;
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int count=1; count <= 22; count++)
    {
      linestream >> cpu_info;
      if (count == 14)
        cpu_data.emplace_back(cpu_info);
      else if (count == 15)
        cpu_data.emplace_back(cpu_info);
      else if (count == 16)
        cpu_data.emplace_back(cpu_info);
      else if (count == 17)
        cpu_data.emplace_back(cpu_info);
      else if (count == 22)
        cpu_data.emplace_back(cpu_info);
    }
  }
  return cpu_data;
} 

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{
  std::string line, word;
  int processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "processes")
      {
        linestream >> processes;
        return processes;
      }
    }
  }
  return 0; 
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  std::string line, word;
  int processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "procs_running")
      {
        linestream >> processes;
        return processes;
      }
    }
  }
  return 0;  
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
  std::string commandline;
  std::ifstream stream(kProcDirectory+"/"+to_string(pid)+kCmdlineFilename);
  if (stream.is_open())
  {
    std::getline(stream, commandline);
  }
  return commandline;
}

// Done: Read and return the memory used by a process

std::string LinuxParser::Ram(int pid) 
{ 
  std::string line, word;
  int ram;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "VmSize:")
      {
        linestream >> ram;
        return to_string(ram/1000); //int (MB)
      }
    }
  }
  return "0";
}

// Done: Read and return the user ID associated with a process
int LinuxParser::Uid(int pid)
{
  std::string line, word;
  int uid;
  std::ifstream stream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "Uid:")
      {
        linestream >> uid;
        return uid;
      }
    }
  }
  return -1; 
}

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) 
{  
  int uid {Uid(pid)};
  std::string line, user, x;
  int uid_stream;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid_stream;
      if (uid_stream == uid)
      {
        return user;
      }
    }
  }
  return ""; 
}

// Done: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) 
{
  std::string line;
  int uptime_val = 22;
  std::string uptime;
  std::ifstream stream(kProcDirectory + "/"+ to_string(pid)+ kStatFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int count=1; count<=uptime_val; count++)
    {
      linestream >> uptime;
    }
    return stol(uptime)/sysconf(_SC_CLK_TCK);
  } 
  return 0;
}


// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }
