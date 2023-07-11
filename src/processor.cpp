#include "processor.h"

float Processor::Utilization() {
    
// TODO: Return the aggregate CPU utilization
    std::vector<std::string> cpu_utilization = LinuxParser::CpuUtilization();
    float user = std::stof(cpu_utilization[0]);
    float nice = std::stof(cpu_utilization[1]);
    float system = std::stof(cpu_utilization[2]);
    float idle = std::stof(cpu_utilization[3]);
    float iowait = std::stof(cpu_utilization[4]);
    float irq = std::stof(cpu_utilization[5]);
    float softirq = std::stof(cpu_utilization[6]);
    float steal = std::stof(cpu_utilization[7]);
    float guest = std::stof(cpu_utilization[8]);
    float guest_nice = std::stof(cpu_utilization[9]);
    float idlealltime = idle + iowait;
    float systemalltime = system + irq + softirq;
    float virtalltime = guest + guest_nice;
    float totaltime = user + nice + systemalltime + idlealltime + steal + virtalltime;
    float totald = totaltime - prevtotaltime;
    float idled = idlealltime - previdlealltime;
    float CPU_Percentage = (totald - idled)/totald;
    prevtotaltime = totaltime;
    previdlealltime = idlealltime;
    return CPU_Percentage;
}