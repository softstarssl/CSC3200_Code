#!/bin/sh

if command -v clang++ >/dev/null 2>&1; then
  CXX=clang++
elif command -v g++ >/dev/null 2>&1; then
  CXX=g++
else
  echo "No suitable C++ compiler found (need clang++ or g++)."
  exit 1
fi

TOTAL="0"
CPP_OK=0

if "$CXX" test_vector_interface.cpp -o interface_test; then
  CPP_OK=1
  rm interface_test
fi

if [ "$CPP_OK" -eq 1 ]; then
  echo "Interface test compiles:\t\t\t\t\t\t1.0 / 1.0"
  TOTAL="$(awk -v t="$TOTAL" 'BEGIN{print t + 1.0}')"
else
  echo "Interface does not compile:                0 / 1.0"
fi

echo ""
echo ""
echo "Run test_vector"

trap 'rm -f test_vector' EXIT HUP INT TERM

if "$CXX" test_vector.cpp -o test_vector; then
  OUT="$(./test_vector)"
  echo "$OUT"
  echo ""
  echo ""
  NUM="$(printf '%s\n' "$OUT" | awk '
    /^test_vector.cpp subtotal:/ {
      for (i=1; i<=NF; i++) if ($i ~ /^[0-9]+(\.0|\.5)?$/) { print $i; exit }
    }')"
  if [ -n "$NUM" ]; then
    TOTAL="$(awk -v t="$TOTAL" -v n="$NUM" 'BEGIN{print t + n}')"
  fi
fi
printf "Total:\t\t\t\t\t\t\t\t\t%s / 3.5\n" "$(awk -v t="$TOTAL" 'BEGIN{printf "%.1f", t}')"

