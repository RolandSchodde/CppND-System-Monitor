#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  int memTotal;
  int memFree;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    int i;
    for (i = 0; i < 5; i++) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = std::stoi(value);
      }
      if (key == "MemFree:") {
        memFree = std::stoi(value);
      }
    }
  }

  return (float)(memTotal - memFree) / memTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  std::string line;
  std::string uptime;
  std::string idle;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return (std::stol(uptime));
  }
  return -1;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::vector<std::string> cpuUtil = LinuxParser::CpuUtilization();
  long jiffies;
  if (!cpuUtil.empty()) {
    jiffies = std::stol(cpuUtil[kUser_]) + std::stol(cpuUtil[kNice_]) +
              std::stol(cpuUtil[kSystem_]) + std::stol(cpuUtil[kIdle_]) +
              std::stol(cpuUtil[kIOwait_]) + std::stol(cpuUtil[kIRQ_]) +
              std::stol(cpuUtil[kSoftIRQ_]) + std::stol(cpuUtil[kSteal_]);
  }
  return jiffies;
}

//  Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::string line, value;
  std::vector<string> processUtil;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  long activeJiffies;

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    while (linestream >> value) {
      processUtil.emplace_back(value);
    }
  }

  if (!processUtil.empty()) {
    activeJiffies =
        std::stol(processUtil[kUTime_]) + std::stol(processUtil[kSTime_]) +
        std::stol(processUtil[kCUTime_]) + std::stol(processUtil[kCSTime_]);
  }
  return activeJiffies;
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::vector<std::string> cpuUtil = LinuxParser::CpuUtilization();
  long activeJiffies;
  if (!cpuUtil.empty()) {
    activeJiffies = std::stol(cpuUtil[kUser_]) + std::stol(cpuUtil[kNice_]) +
                    std::stol(cpuUtil[kSystem_]) + std::stol(cpuUtil[kIRQ_]) +
                    std::stol(cpuUtil[kSoftIRQ_]) + std::stol(cpuUtil[kSteal_]);
  }
  return activeJiffies;
}

//  Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<std::string> cpuUtil = LinuxParser::CpuUtilization();
  return std::stoi(cpuUtil[kIdle_]) + std::stoi(cpuUtil[kIOwait_]);
}

//  Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  std::vector<std::string> cpuUtil;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    while (linestream >> value) {
      if (value != "cpu") {
        cpuUtil.emplace_back(value);
      }
    }
  }

  return cpuUtil;
}

//  Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  int totalProcesses;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      if (linestream >> key >> value) {
        if (key == "processes") {
          totalProcesses = std::stoi(value);
        }
      }
    }
  }
  return totalProcesses;
}

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  int runningProcesses;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      if (linestream >> key >> value) {
        if (key == "procs_running") {
          runningProcesses = std::stoi(value);
        }
      }
    }
  }
  return runningProcesses;
}

//  Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::string line = "-";

  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }

  return line;
}

//  Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::string line, value, key, ram;
  std::vector<string> processUtil;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        ram = value;
      }
    }
  }

  return ram;
}

//  Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::string line, value, key, uid;
  std::vector<string> processUtil;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        uid = value;
      }
    }
  }

  return uid;
}

//  Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::string line, uid, username;
  std::vector<string> processUtil;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::remove(line.begin(), line.end(), 'x');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> username >> uid;
      if (uid == Uid(pid)) {
        return username;
      }
    }
  }

  return "-";
}

//  Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::string line, value;
  std::vector<string> processStat;
  long overAllUptime = UpTime();
  std::ifstream filestream(kProcDirectory + std::to_string(pid) +
                           kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    while (linestream >> value) {
      processStat.emplace_back(value);
    }
    return overAllUptime -
           (std::stod(processStat[kProcessStartTime_]) / sysconf(_SC_CLK_TCK));
  }

  return 0;
}
