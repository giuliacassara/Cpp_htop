#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


// TODO: Return the system's CPU
Processor& System::Cpu() { 

    return cpu_;
    
    //return the object of Processor class
    
    }

// TODO: Return a container composed of the system's processes

std::vector<Process>& System::Processes() {
  
    processes_.clear(); // Clear the existing processes_
    vector<int> pids = LinuxParser::Pids();
    for (int pid : pids) {
        Process process(pid);
        processes_.push_back(process); 
    }
    std::sort(processes_.begin(), processes_.end());
    //std::sort(pids.begin(), pids.end());

    return processes_;
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    string kernel_;
    kernel_ = LinuxParser::Kernel();
    return kernel_;
    }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
    float memory_utilization_;
    memory_utilization_ = LinuxParser::MemoryUtilization();
    return memory_utilization_; 
    }

// TODO: Return the operating system name
std::string System::OperatingSystem() { 

    string operating_system_;
    operating_system_ = LinuxParser::OperatingSystem();
    return operating_system_; 
    
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
    int running_processes_;
    running_processes_ = LinuxParser::RunningProcesses();
    return running_processes_; 
    }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
    int total_processes_;
    total_processes_ = LinuxParser::TotalProcesses();
    return total_processes_; 
    }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
    long int uptime_;
    uptime_ = LinuxParser::UpTime();
    return uptime_;
    }
