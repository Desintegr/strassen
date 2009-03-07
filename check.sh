#!/bin/sh

rm -f 1.mat
rm -f 2.mat
rm -f strassen.mat
rm -f mult.mat

if [ -n "$1" ]
then
    if [ $1 -gt 0 ]
    then
        ./generate $1 1.mat
        ./generate $1 2.mat
        ./strassen 1.mat 2.mat strassen.mat
        ./mult 1.mat 2.mat mult.mat 
        ./compare strassen.mat mult.mat
        echo
    else
        echo "Error: size must be greater than 0"
    fi
else
    echo "Usage: $0 size"
fi