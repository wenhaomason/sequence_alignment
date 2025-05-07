#!/bin/bash
./test_efficient.sh
./test_basic.sh

cost1=()
time1=()
mem1=()
for i in {1..11}; do
  cost1+=("$(head -n 1 "test_basic/out${i}.txt")")
  time1+=("$(sed -n '4p' "test_basic/out${i}.txt")")
  mem1+=("$(sed -n '5p' "test_basic/out${i}.txt")")
done

echo "Base:"
(IFS=,; echo "${cost1[*]}")
(IFS=,; echo "${time1[*]}")
(IFS=,; echo "${mem1[*]}")

echo "Efficient:"

cost2=()
time2=()
mem2=()
for i in {1..11}; do
  cost2+=("$(head -n 1 "test_efficient/out${i}.txt")")
  time2+=("$(sed -n '4p' "test_efficient/out${i}.txt")")
  mem2+=("$(sed -n '5p' "test_efficient/out${i}.txt")")
done
(IFS=,; echo "${cost2[*]}")
(IFS=,; echo "${time2[*]}")
(IFS=,; echo "${mem2[*]}")


echo "\\nComparing first lines of test_basic outputs with SampleTestCases outputs:"
for i in {1..11}; do
  basic_out_file="test_basic/out${i}.txt"
  sample_out_file="SampleTestCases/output${i}.txt"

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

echo "\\nComparing first lines of test_basic outputs with SampleTestCases outputs:"
for i in {1..11}; do
  basic_out_file="test_efficient/out${i}.txt"
  sample_out_file="SampleTestCases/output${i}.txt"

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

