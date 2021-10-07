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

  // hours
  if (seconds < 3600) {
    h = "00";
  } else if (seconds < 10 * 3600) {
    h = "0" + std::to_string(seconds / 3600);
  } else {
    h = std::to_string(seconds / 3600);
  }

  // minutes
  if (seconds % 3600 < 60) {
    m = "00";
  } else if (seconds % 3600 < 10 * 60) {
    m = "0" + std::to_string((seconds % 3600) / 60);
  } else {
    m = std::to_string((seconds % 3600) / 60);
  }

  // seconds
  if ((seconds % 3600) % 60 == 0) {
    s = "00";
  } else if ((seconds % 3600) % 60 < 10) {
    s = "0" + std::to_string((seconds % 3600) % 60);
  } else {
    s = std::to_string((seconds % 3600) % 60);
  }
  return h + ":" + m + ":" + s;
}
string calcHours(long seconds) {}
