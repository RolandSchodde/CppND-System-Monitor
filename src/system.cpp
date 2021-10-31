#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

//  Return the system's CPU
Processor& System::Cpu() { return cpu_; }

//  Return a container composed of the system's processes
vector<Process>& System::Processes() {
  std::vector<int> pIDs = LinuxParser::Pids();

  // only in the first iteration when processes is empty
  if (processes_.empty()) {
    for (int pID : pIDs) {
      processes_.push_back(Process(pID));
      processes_.at(processes_.size() - 1).updateCpuUtilization();
    }
  }

  // only after the first iteration when processes is not empty
  if (!lastPIDs_.empty()) {
    // sort the Precess IDs  and the Processes in increasing order by Process ID
    // to use only one loop for updating the processes (processes_).
    std::sort(pIDs.begin(), pIDs.end());
    std::sort(processes_.begin(), processes_.end(),
              [](Process a, Process b) { return a.Pid() < b.Pid(); });

    // 1. (LastPID == currentPID) at index i -> only update
    // 2. (lastPID < currentPID) at index i -> erase process at index i
    // 3. (lastPID > currentPID) at index i -> insert new process at index i
    // stop if on of lastPIDs or pIDs reaches their end.
    unsigned int i;
    for (i = 0; i < lastPIDs_.size() && i < pIDs.size();) {
      if (lastPIDs_.at(i) == pIDs.at(i)) {
        processes_.at(i).updateCpuUtilization();
        i++;
      } else if (lastPIDs_.at(i) < pIDs.at(i)) {
        processes_.erase(processes_.begin() + i);
        lastPIDs_.erase(lastPIDs_.begin() + i);
      } else {
        processes_.insert(processes_.begin() + i, Process(pIDs.at(i)));
        lastPIDs_.insert(lastPIDs_.begin() + i, pIDs.at(i));
        processes_.at(i).updateCpuUtilization();
        i++;
      }
    }
    // now we need to handle the leftovers if one vector has more elements.
    if (lastPIDs_.size() < pIDs.size()) {
      for (; i < pIDs.size(); i++) {
        // insert new process
        processes_.emplace_back(Process(pIDs.at(i)));
        processes_.at(i).updateCpuUtilization();
      }
    } else if (lastPIDs_.size() > pIDs.size()) {
      for (; i < lastPIDs_.size(); i++) {
        // delete process
        processes_.pop_back();
      }
    }
  }
  // sort by cpu util
  std::sort(processes_.rbegin(), processes_.rend());

  lastPIDs_ = pIDs;
  return processes_;
}

//  Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

//  Return the system's memory utiliz
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

//  Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

//  Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

//  Return the total number of processes on the systemation
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

//  Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }