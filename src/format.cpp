#include "format.h"

#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string h;
  string m;
  string s;

  // Thanks for the simplier calc-suggestions here!
  // hours
  long hours = seconds / 3600;

  if (hours == 0) {
    h = "00";
  } else if (hours < 10) {
    h = "0" + std::to_string(hours);
  } else {
    h = std::to_string(hours);
  }

  // minutes
  seconds = seconds % 3600;
  long minutes = seconds / 60;

  if (minutes == 0) {
    m = "00";
  } else if (minutes < 10) {
    m = "0" + std::to_string(minutes);
  } else {
    m = std::to_string(minutes);
  }

  // seconds
  seconds = seconds % 60;

  if (seconds == 0) {
    s = "00";
  } else if (seconds < 10) {
    s = "0" + std::to_string(seconds);
  } else {
    s = std::to_string(seconds);
  }

  return h + ":" + m + ":" + s;
}

// Thanks for this approach!
/*
string Format::ElapsedTime(long s) {
  std::chrono::seconds seconds{s};

  // return std::chrono::format("%T", seconds); // in C++20 :-)

  std::chrono::hours hours =
      std::chrono::duration_cast<std::chrono::hours>(seconds);

  seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);

  std::chrono::minutes minutes =
      std::chrono::duration_cast<std::chrono::minutes>(seconds);

  seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

  std::stringstream ss{};

  ss << std::setw(2) << std::setfill('0') << hours.count()     // HH
     << std::setw(1) << ":"                                    // :
     << std::setw(2) << std::setfill('0') << minutes.count()   // MM
     << std::setw(1) << ":"                                    // :
     << std::setw(2) << std::setfill('0') << seconds.count();  // SS

  return ss.str();
}
*/