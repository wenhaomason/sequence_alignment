#!/bin/bash
g++ efficient.cpp -o efficient

for i in {1..11}
do
  ./efficient "./SampleTestCases/input$i.txt" "./test_efficient/out$i.txt"
done