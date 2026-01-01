#!/bin/sh

if command -v clang++ >/dev/null 2>&1; then
  CXX=clang++
elif command -v g++ >/dev/null 2>&1; then
  CXX=g++
else
  echo "No suitable C++ compiler found (need clang++ or g++)."
  exit 1
fi

CXXFLAGS="-std=c++20 -I."
TOTAL="0"

TU_OK=0
CPP_OK=0

if printf '#include "student_string.h"\n' | "$CXX" $CXXFLAGS -x c++ - -fsyntax-only >/dev/null 2>&1; then
  TU_OK=1
fi

if "$CXX" $CXXFLAGS -c student_string.cpp -o /dev/null >/dev/null 2>&1; then
  CPP_OK=1
fi

if [ "$TU_OK" -eq 1 ] && [ "$CPP_OK" -eq 1 ]; then
  echo "Header and source file compile:                 0.5 / 0.5"
  TOTAL="$(awk -v t="$TOTAL" 'BEGIN{print t + 0.5}')"
else
  echo "Header and/or source file fail to compile:      0 / 0.5"
fi


once_norm="$(
  printf '#include "student_string.h"\n' \
    | "$CXX" -E -P -x c++ $CXXFLAGS - 2>/dev/null \
    | tr -d ' \t\r\n\f\v'
)"
twice_norm="$(
  printf '#include "student_string.h"\n#include "student_string.h"\n' \
    | "$CXX" -E -P -x c++ $CXXFLAGS - 2>/dev/null \
    | tr -d ' \t\r\n\f\v'
)"

if [ "$once_norm" = "$twice_norm" ]; then
  echo "Include guard check passed:                     0.5 / 0.5"
  TOTAL="$(awk -v t="$TOTAL" 'BEGIN{print t + 0.5}')"
else
  echo "Lack of include guard:                          0 / 0.5"
fi

echo ""
echo ""
echo "Run test_string"

trap 'rm -f test_string' EXIT HUP INT TERM

if "$CXX" $CXXFLAGS test_string.cpp student_string.cpp -o test_string >/dev/null 2>&1; then
  OUT="$(./test_string 2>/dev/null)"
  echo "$OUT"
  echo ""
  echo ""
  NUM="$(printf '%s\n' "$OUT" | awk '
    /\/[[:space:]]*2\.5/ {
      if (match($0, /([+-]?[0-9]+([.][0-9]+)?)[[:space:]]*\/[[:space:]]*2\.5/, m)) { print m[1]; exit }
      if (match($0, /([+-]?[0-9]*[.][0-9]+)[[:space:]]*\/[[:space:]]*2\.5/, m)) { print m[1]; exit }
    }')"
  if [ -n "$NUM" ]; then
    TOTAL="$(awk -v t="$TOTAL" -v n="$NUM" 'BEGIN{print t + n}')"
  fi
fi
printf "Total:\t\t\t\t\t\t%s / 3.5\n" "$(awk -v t="$TOTAL" 'BEGIN{printf "%.1f", t}')"

