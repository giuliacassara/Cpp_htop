#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

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
    linestream >> os >> version >> kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float memTotal = 0.0;
  float memFree = 0.0;
  float memUtilization = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if(key == "MemTotal:"){
          memTotal = std::stof(value);
        }
        if(key == "MemFree:"){
          memFree = std::stof(value);
        }
      }
    }
  }

  return memUtilization = (memTotal - memFree) / memTotal;
  
}


// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string uptime;
  string idleTime;
  long upTime = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idleTime;
    upTime = std::stol(uptime);
  }
  return upTime;
  
  }

// TODO: Read and return the number of jiffies for the system
// The very first "cpu" line aggregates the numbers in all of the other "cpuN" lines.
/*
The meanings of the columns are as follows, from left to right:

user: normal processes executing in user mode
nice: niced processes executing in user mode
system: processes executing in kernel mode
idle: twiddling thumbs
iowait: waiting for I/O to complete
irq: servicing interrupts
softirq: servicing softirqs
steal: involuntary wait
guest: running a normal guest
guest_nice: running a niced guest
*/

long LinuxParser::Jiffies() { 
  string line;
  string cpu;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guest_nice;
  long jiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  // there are multiple cpu lines in the file, we need to read the first one
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    jiffies = std::stol(user) + std::stol(nice) + std::stol(system) + std::stol(idle) + std::stol(iowait) + std::stol(irq) + std::stol(softirq) + std::stol(steal) + std::stol(guest) + std::stol(guest_nice);
  }
  return jiffies;
  
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string value;
  long activeJiffies = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for(int i = 0; i < 18; i++){
      linestream >> value;
      // #14 utime - CPU time spent in user code, measured in clock ticks
      // #15 stime - CPU time spent in kernel code, measured in clock ticks
      if(i == 13 || i == 14){
        activeJiffies += std::stol(value);
      }
      //#16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
      //#17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
      // total_time = total_time + cutime + cstime
      if(i == 15 || i == 16){
        activeJiffies += std::stol(value);
      }
    }

  }
  return activeJiffies;
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  string cpu;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guest_nice;
  long activeJiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  // there are multiple cpu lines in the file, we need to read the first one
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    activeJiffies = std::stol(system); 
  }
  return activeJiffies;
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  string cpu;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guest_nice;
  long idleJiffies = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  // there are multiple cpu lines in the file, we need to read the first one
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    idleJiffies = std::stol(idle) + std::stol(iowait);
  }
  return idleJiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

  string line;
  string cpu;
  string user;
  string nice;
  string system;
  string idle;
  string iowait;
  string irq;
  string softirq;
  string steal;
  string guest;
  string guest_nice;
  vector<string> cpuUtilization;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  // there are multiple cpu lines in the file, we need to read the first one
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    cpuUtilization.push_back(user);
    cpuUtilization.push_back(nice);
    cpuUtilization.push_back(system);
    cpuUtilization.push_back(idle);
    cpuUtilization.push_back(iowait);
    cpuUtilization.push_back(irq);
    cpuUtilization.push_back(softirq);
    cpuUtilization.push_back(steal);
    cpuUtilization.push_back(guest);
    cpuUtilization.push_back(guest_nice);
  }
  return cpuUtilization;
  
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int totalProcesses = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "processes"){
        totalProcesses = std::stoi(value);
      }
    }
  }
  return totalProcesses;

}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int runningProcesses = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "procs_running"){
        runningProcesses = std::stoi(value);
      }
    }
  }
  return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  string ram;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "VmSize:"){
        ram = std::to_string(std::stoi(value)/1000);
      }
    }
  
  }
  return ram; 
  }


// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
    
    string line;
    string key;
    string value;
    string uid;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
    if(filestream.is_open()){
      while(std::getline(filestream, line)){
        std::istringstream linestream(line);
        linestream >> key >> value;
        if(key == "Uid:"){
          uid = value;
        }
      }
    
    }
    return uid; 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
// /etc/passwd contains the information necessary to match the UID to a username.
string LinuxParser::User(int pid) {

  string line;
  string key;
  string value;
  string uid = LinuxParser::Uid(pid);
  string user;
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
    while(std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> key >> value;
      if(value == uid){
        return user;
      }
    }
  }
  return user; 

}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  
    string line;
    string value;
    long uptime = 0;
    long starttime = 0;
    long seconds = 0;
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if(filestream.is_open()){
      std::getline(filestream, line);
      std::istringstream linestream(line);
      for(int i = 0; i < 22; i++){
        linestream >> value;
      }
    }
    starttime = std::stol(value);
    std::ifstream filestream2(kProcDirectory + kUptimeFilename);
    if(filestream2.is_open()){
      std::getline(filestream2, line);
      std::istringstream linestream2(line);
      linestream2 >> value;
    }
    uptime = std::stol(value);
      // Next we get the total elapsed time in seconds since the process started:

      // seconds = uptime - (starttime / Hertz)
    seconds = uptime - (starttime / sysconf(_SC_CLK_TCK));
      // Read /proc/uptime and take parameter #1 uptime of the system (seconds)

    
    return seconds;

 }
