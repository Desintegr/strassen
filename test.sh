#!/bin/sh

if [ -z "$1" ]
then
    echo "Usage: $0 size"
    exit
fi

if [ $1 -le 0 ]
then
    echo "Error: size must be greater than 0"
    exit
fi

rm -f 1.mat
rm -f 2.mat
rm -f strassen.mat
rm -f mult.mat

echo -n "Generating $1-sized matrix 1..." &&
./generate $1 1.mat &&
echo " Ok" &&
echo -n "Generating $1-sized matrix 2..." &&
./generate $1 2.mat &&
echo " Ok" &&
echo -n "Computing strassen algorithm..." &&
./strassen 1.mat 2.mat strassen.mat &&
echo " Ok" &&
echo -n "Computing normal algorithm..." &&
./mult 1.mat 2.mat mult.mat &&
echo " Ok" &&
echo -n "Comparing results... " &&
./compare strassen.mat mult.mat &&
echo