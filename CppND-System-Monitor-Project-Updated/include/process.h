#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
   Process(){}
   //Accessors
    int Pid();                               // TODO: See src/process.cpp
    std::string User();                      // TODO: See src/process.cpp
    std::string Command();                   // TODO: See src/process.cpp
    float CpuUtilization();                  // TODO: See src/process.cpp
    std::string Ram();                       // TODO: See src/process.cpp
    long int UpTime();                       // TODO: See src/process.cpp
    bool operator<(Process const& a) const;  // TODO: See src/process.cpp

    //Mutators
    void setpid (int pid){pid_ = pid;}
    void setuser(std::string user){user_=user;}
    void setCpu(float cpu){cpu_=cpu;}
    void setcommand(std::string command){command_=command;}
    void setuptime(long int uptime){uptime_=uptime;}
    void setram(std::string ram){ram_=ram;}
  // TODO: Declare any necessary private members
 private:
    int pid_;
    std::string user_;
    float cpu_;
    std::string ram_;
    long int uptime_;
    std::string command_;
};

#endif