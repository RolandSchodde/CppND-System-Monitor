#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid(pid) {}

// Return this process's ID
int Process::Pid() { return this->pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization; }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// Return this process's memory utilization
string Process::Ram() {
  std::string ram = LinuxParser::Ram(Pid());
  // ram should be not empty for calculation;
  if (!ram.empty()) {
    // KB -> MB
    ram = std::to_string(stol(ram) / 1000);
  }

  return ram;
}

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

//  Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

//  Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return this->cpuUtilization < a.cpuUtilization;
}

// Updates the CPU Utilization
void Process::updateCpuUtilization() {
  long activeJiffies = LinuxParser::ActiveJiffies(Pid());
  long upTime = LinuxParser::UpTime(Pid());
  int activeJiffiesOneIt{0};
  int upTimeOneIt{0};

  if (lastUpTime != 0) {
    // Jiffies for one iteration
    activeJiffiesOneIt = activeJiffies - lastActiveJiffies;
    upTimeOneIt = upTime - lastUpTime;
    // calculate  the the Utilization
    cpuUtilization =
        ((double)activeJiffiesOneIt / sysconf(_SC_CLK_TCK)) / upTimeOneIt;
  }

  lastActiveJiffies = activeJiffies;
  lastUpTime = upTime;
}