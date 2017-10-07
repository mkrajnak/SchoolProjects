#!/bin/sh

# test projektu 2 IZP - 2014
# autor: ihynek

if [ "$1" = "" ] ; then
  path="./"
else
  path=$1
fi

echo "gcc -std=c99 -pedantic -Wall -Wextra -g -o "$path"proj2 "$path"proj2.c -lm"
gcc -std=c99 -pedantic -Wall -Wextra -g -o "$path"proj2 "$path"proj2.c -lm
echo "---------------------
"

echo ""$path"proj2 --help"
"$path"proj2 --help

echo "---------------------
"

echo ""$path"proj2 --tan .785398163 10 10"
result=`timeout 5s "$path"proj2 --tan .785398163 10 10`
if [ "$result" = "10 1.000000e+00 9.999992e-01 8.095039e-07 1.000000e+00 1.110223e-16" ] ; then
  echo "OK"
else 
  echo "$result"
  echo "ERR - EXPECTED:"
  echo "10 1.000000e+00 9.999992e-01 8.095039e-07 1.000000e+00 1.110223e-16"
fi
echo "---------------------
"

echo ""$path"proj2 -m 0.3 0.9"
result=`timeout 5s "$path"proj2 -m 0.3 0.9`
if [ "$result" = "4.8490922156e+00
7.6106234032e+00" ] ; then
  echo "OK"
else 
  echo "$result"
  echo "ERR - EXPECTED:"
  echo "4.8490922156e+00
7.6106234032e+00"
fi
echo "---------------------
"

echo ""$path"proj2 -c 1.7 -m 0.15 1.3"
result=`timeout 5s "$path"proj2 -c 1.7 -m 0.15 1.3`
if [ "$result" = "1.1248205560e+01
4.2217188781e+01" ] ; then
  echo "OK"
else 
  echo "$result"
  echo "ERR - EXPECTED:"
  echo "1.1248205560e+01
4.2217188781e+01"
fi
