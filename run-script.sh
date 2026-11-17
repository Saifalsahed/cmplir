#!/bin/bash
source /home/ubuntu/Desktop/antlr4_venv/bin/activate
antlr4 -Dlanguage=Cpp -visitor -no-listener CSubset.g4
g++ -std=c++17 -w -I/usr/local/include/antlr4-runtime *.cpp -L/usr/local/lib/ -lantlr4-runtime -pthread -o compiler.out
LD_LIBRARY_PATH=/usr/local/lib ./compiler.out "$1"