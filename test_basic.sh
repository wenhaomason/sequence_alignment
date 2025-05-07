#!/bin/bash
g++ basic.cpp -o basic

for i in {1..11}
do
  ./basic "./SampleTestCases/input$i.txt" "./test_basic/out$i.txt"
done

 