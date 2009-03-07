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

./generate $1 1.mat
./generate $1 2.mat
./strassen 1.mat 2.mat strassen.mat
./mult 1.mat 2.mat mult.mat
./compare strassen.mat mult.mat

echo