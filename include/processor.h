#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  int lastCpuUsageJiffies{0};  // last values to calculate
  int lastCpuIdleJiffies{0};   // the cpu utilization for one iteration
  int lastCpuTotalJiffies{0};
};

#endif