#!/bin/bash

efficient_dir="./results/efficient"
basic_dir="./results/basic"

g++ efficient.cpp -o efficient

g++ basic.cpp -o basic

for i in {1..15}
do
  ./efficient "./datapoints/in$i.txt" "${efficient_dir}/out$i.txt"
done

for i in {1..15}
do
  ./basic "./datapoints/in$i.txt" "${basic_dir}/out$i.txt"
done

echo "Base:"

time1=()
mem1=()
for i in {1..15}; do
  time1+=("$(sed -n '4p' "${basic_dir}/out$i.txt")")
  mem1+=("$(sed -n '5p' "${basic_dir}/out$i.txt")")
done
(IFS=,; echo "${time1[*]}")
(IFS=,; echo "${mem1[*]}")

echo "Efficient:"

time2=()
mem2=()
for i in {1..15}; do
  time2+=("$(sed -n '4p' "${efficient_dir}/out$i.txt")")
  mem2+=("$(sed -n '5p' "${efficient_dir}/out$i.txt")")
done
(IFS=,; echo "${time2[*]}")
(IFS=,; echo "${mem2[*]}")


echo "\\nComparing first lines of test_basic outputs with SampleTestCases outputs:"
for i in {1..15}; do
  basic_out_file="${efficient_dir}/out$i.txt"
  sample_out_file="${basic_dir}/out$i.txt"

  if [ ! -f "$basic_out_file" ]; then
    echo "File ${i} comparison: $basic_out_file not found."
    continue
  fi
  if [ ! -f "$sample_out_file" ]; then
    echo "File ${i} comparison: $sample_out_file not found."
    continue
  fi

  first_line_basic=$(head -n 1 "$basic_out_file")
  first_line_sample=$(head -n 1 "$sample_out_file")

  if [ "$first_line_basic" == "$first_line_sample" ]; then
    echo "File ${i} ($basic_out_file vs $sample_out_file): First lines are equal."
  else
    echo "File ${i} ($basic_out_file vs $sample_out_file): First lines are NOT equal."
    echo "  Basic: '$first_line_basic'"
    echo "  Sample: '$first_line_sample'"
  fi
done