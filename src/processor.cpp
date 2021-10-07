#include "processor.h"

#include <iostream>
#include <string>

#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> cpuUtil = {};
  int cpuUsageJiffies = 0;
  int cpuIdleJiffies = 0;
  int cpuTotalJiffies = 0;
  int idleDiff = 0;
  int totalDiff = 0;
  float cpuPercentage = 0.0;

  // read the file data
  cpuUtil = LinuxParser::CpuUtilization();
  // only one fetch for the most accurate result - so I don't use
  // LinuxParser::ativeJiffies() and idle...

  // extract  usage, idle and total
  cpuUsageJiffies = std::stol(cpuUtil[LinuxParser::kUser_]) +
                    std::stol(cpuUtil[LinuxParser::kNice_]) +
                    std::stol(cpuUtil[LinuxParser::kSystem_]) +
                    std::stol(cpuUtil[LinuxParser::kIRQ_]) +
                    std::stol(cpuUtil[LinuxParser::kSoftIRQ_]) +
                    std::stol(cpuUtil[LinuxParser::kSteal_]);

  cpuIdleJiffies = std::stoi(cpuUtil[LinuxParser::kIdle_]) +
                   std::stoi(cpuUtil[LinuxParser::kIOwait_]);

  cpuTotalJiffies = cpuUsageJiffies + cpuIdleJiffies;

  // subtract the values of the iteration to get a cpu utilization for about 1
  // sec
  idleDiff = cpuIdleJiffies - lastCpuIdleJiffies;
  totalDiff = cpuTotalJiffies - lastCpuTotalJiffies;

  // calc percentage
  cpuPercentage = (float)(totalDiff - idleDiff) / totalDiff;

  // set current valcues to last_values
  lastCpuUsageJiffies = cpuUsageJiffies;
  lastCpuIdleJiffies = cpuIdleJiffies;
  lastCpuTotalJiffies = cpuTotalJiffies;

  return cpuPercentage;
}