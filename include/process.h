#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

  void updateCpuUtilization();
  bool ComparePIDIncreasing(Process const& a) const;

 private:
  int pid{0};  // Process ID
  float cpuUtilization{0};
  long lastActiveJiffies{0};  // last values to calculate
  long lastUpTime{0};         // the cpuUtilization for one iteration
};

#endif