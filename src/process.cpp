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

// Processes


// TODO: Return this process's ID
int Process::Pid() {

    
    return pid_;
 }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    float cpu_utilization_ = 0;
    //use LinuxParser::ActiveJiffies(int pid) to get the active jiffies of the process
    //cpu_utilization_ = LinuxParser::ActiveJiffies(Pid());
    long seconds =LinuxParser::UpTime(Pid());
    long totaltime = LinuxParser::ActiveJiffies(Pid());
    cpu_utilization_ = float(totaltime) / float(seconds);
    return cpu_utilization_; 
}

// TODO: Return the command that generated this process
string Process::Command() {
    string command_ = "";
    //use LinuxParser::Command(int pid) to get the command
    command_ = LinuxParser::Command(Pid());
    return command_;     
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    string ram_ = "";
    //use LinuxParser::Ram(int pid) to get the ram
    ram_ = LinuxParser::Ram(Pid());
    return ram_;
 }

// TODO: Return the user (name) that generated this process
string Process::User() {
    string user_ = "";
    //use LinuxParser::User(int pid) to get the user
    user_ = LinuxParser::User(Pid());
    return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    long int uptime_ = 0;
    //use LinuxParser::UpTime(int pid) to get the uptime
    uptime_ = LinuxParser::UpTime(Pid()); 
    return uptime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process& a){ 
    //compare the cpu utilization of two processes
    return CpuUtilization() < a.CpuUtilization();
}

