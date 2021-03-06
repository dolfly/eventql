#pragma once

#include <stdint.h>
#include <eventql/util/duration.h>

class MonotonicTime {
public:
  constexpr MonotonicTime();
  constexpr MonotonicTime(const MonotonicTime& other);
  constexpr explicit MonotonicTime(uint64_t nanosecs);

  constexpr uint64_t seconds() const;
  constexpr uint64_t milliseconds() const;
  constexpr uint64_t microseconds() const;
  constexpr uint64_t nanoseconds() const;

  constexpr Duration operator-(const MonotonicTime& other) const;
  constexpr MonotonicTime operator+(const Duration& other) const;

  constexpr bool operator==(const MonotonicTime& other) const;
  constexpr bool operator!=(const MonotonicTime& other) const;
  constexpr bool operator<=(const MonotonicTime& other) const;
  constexpr bool operator>=(const MonotonicTime& other) const;
  constexpr bool operator<(const MonotonicTime& other) const;
  constexpr bool operator>(const MonotonicTime& other) const;

  constexpr bool operator!() const;
  
private:
  uint64_t nanosecs_;
};

std::string inspect(const MonotonicTime& value);

#include <eventql/util/MonotonicTime_impl.h>
