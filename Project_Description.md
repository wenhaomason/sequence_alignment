# CSCI-570 Spring 2025 Analysis of Algorithms Final Project

**Due: May 12, 2025**

## 1 Guidelines

You can work on this project in groups of no more than 3 people.

## 2 Project Description

Implement the basic Dynamic Programming solution to the Sequence Alignment problem. Run the test set provided and show your results.

### 2.1 Algorithm Description

Suppose we are given two strings X and Y, where X consists of the sequence of symbols x<sub>1</sub>, x<sub>2</sub>, ..., x<sub>m</sub> and Y consists of the sequence of symbols y<sub>1</sub>, y<sub>2</sub>, ..., y<sub>n</sub>.

Consider the sets {1, 2, ..., m} and {1, 2, ..., n} as representing the different positions in the strings X and Y, and consider a matching of these sets; Recall that a matching is a set of ordered pairs with the property that each item occurs in at most one pair. We say that a matching M of these two sets is an **alignment** if there are no "crossing” pairs: if (i, j), (i', j') ∈ M and i < i', then j < j'. Intuitively, an alignment gives a way of lining up the two strings, by telling us which pairs of positions will be lined up with one another.

Our definition of similarity will be based on finding the optimal alignment between X and Y, according to the following criteria. Suppose M is a given alignment between X and Y:

1.  First, there is a parameter δ > 0 that defines a gap penalty. For each position of X or Y that is not matched in M - it is a gap – we incur a cost of δ.
2.  Second, for each pair of letters p, q in our alphabet, there is a mismatch cost of α<sub>pq</sub> for lining up p with q. Thus, for each (i, j) ∈ M, we pay the appropriate mismatch cost α<sub>x<sub>i</sub>y<sub>j</sub></sub> for lining up x<sub>i</sub> with y<sub>j</sub>. One generally assumes that α<sub>pp</sub> = 0 for each letter p – there is no mismatch cost to line up a letter with another copy of itself - although this will not be necessary in anything that follows.
3.  The cost of M is the sum of its gap and mismatch costs, and we seek an alignment of minimum cost.

### 2.2 Input string Generator

The input to the program would be a text file containing the following information:

1.  First base string (s<sub>1</sub>)
2.  Next j lines consist of indices after which the copy of the previous string needs to be inserted in the cumulative string. (eg given below)
3.  Second base string (s<sub>2</sub>)
4.  Next k lines consist of indices after which the copy of the previous string needs to be inserted in the cumulative string. (eg given below)

This information would help generate 2 strings from the original 2 base strings. This file could be used as an input to your program, and your program could use the base strings and the rules to generate the actual strings. Also note that the numbers j and k correspond to the first and the second string, respectively. Make sure you validate the length of the first and the second string to be 2<sup>j</sup> \* len(s<sub>1</sub>) and 2<sup>k</sup> \* len(s<sub>2</sub>). Please note that the base strings need not be of equal length and similarly, j need not be equal to k.

Example:
```
ACTG
3
6
1
TACG
1
2
9
```
Using the above numbers, the generated strings would be `ACACTGACTACTGACTGGTGACTACTGACTGG` and `TATTATACGCTATTATACGCGACGCGGACGCG`.

Following is the step by step process on how the above strings are generated:
ACTG
ACTG**ACTG**
ACTG**ACTG**ACTG**ACTG**
AC**ACTGACTACTGACTGG**TGACTACTGACTGG

TACG
TA**TACG**CGCG
TATTA**TACGCGA**CGCG**TACGCG**ACGCG
TATTATACGC**TATTATACGCGACGCGGACGCG**GACGCG

### 2.3 Values for Delta and Alphas

Values for α's are as follows. δ<sub>e</sub> is equal to 30.

|   | A   | C   | G   | T   |
|---|-----|-----|-----|-----|
| A | 0   | 110 | 48  | 94  |
| C | 110 | 0   | 118 | 48  |
| G | 48  | 118 | 0   | 110 |
| T | 94  | 48  | 110 | 0   |

### 2.4 Programming/Scripting Languages

Following are the list of languages which could be used:
1.  C
2.  C++
3.  Java
4.  Python
5.  Python3

### 2.5 Bounds

1.  **Basic Algorithm**
    *   0 ≤ j, k ≤ 10
    *   1 ≤ len(s<sub>1</sub>), len(s<sub>2</sub>) ≤ 2000
    *   1 ≤ 2<sup>j</sup> \* len(s<sub>1</sub>), 2<sup>k</sup> \* len(s<sub>2</sub>) ≤ 2000

2.  **Memory Efficient Algorithm**
    *   0 ≤ j, k ≤ 20
    *   1 ≤ len(s<sub>1</sub>), len(s<sub>2</sub>) ≤ 20000
    *   1 ≤ 2<sup>j</sup> \* len(s<sub>1</sub>), 2<sup>k</sup> \* len(s<sub>2</sub>) ≤ 20000

## 3 Goals

Following are the goals to achieve for your project:

### 3.1
Your program should take 2 arguments:
1.  Input file path
2.  Output file path (If path is valid and file not found, your program should create it)

Examples:
```
python2 basic_2.py input.txt output.txt
java Basic input.txt output.txt
python3 basic_3.py input.txt output.txt
```
Note: As mentioned in 2.2, input file will have data to generate strings. Since Gap penalty (δ<sub>e</sub>) and Mismatch penalty (α<sub>pq</sub>) are FIXED, you have to hardcode them in your program.
You are not allowed to use any libraries.

### 3.2
Implement the Dynamic Programming algorithm. Your program should print the following information at the respective lines in the output file:
1.  Cost of the alignment (Integer)
2.  First string alignment ( Consists of A, C, T, G, - (gap) characters)
3.  Second string alignment ( Consists of A, C, T, G, - (gap) characters)
4.  Time in Milliseconds (Float)
5.  Memory in Kilobytes (Float)

Note: There can be multiple alignments that have the same cost. You can print ANY alignment generated by your program. The only condition is it should have a minimum cost.
e.g. For strings s<sub>1</sub>: A and s<sub>2</sub>: C, alignments A, \_C and \_A, C both have alignment cost 60 which is minimum. You can print any one of them.

### 3.3
Implement the memory-efficient version of this solution and repeat the tests in 3.2.

### 3.4
Plot the results of 3.2 and 3.3 using a line graph.
**Please use the provided input files in the 'datapoints' folder for generating the data points to plot the graph.**
1.  Single plot of CPU time vs problem size for the two solutions.
2.  Single plot of Memory usage vs problem size for the two solutions.

Units: CPU time - milliseconds, Memory in KB, problem size m+n

## 4 Submission

### 4.1
You should submit the ZIP file containing the following files:
a.  Basic algorithm file
    Name of the program file should be ‘basic.c’ / ‘basic.cpp’ / ‘Basic.java’ / ‘basic_2.py’ (Python 2.7) / ‘basic_3.py’ (Python 3)
b.  Memory efficient algorithm file
    Name of the program file should be ‘efficient.c’ / ‘efficient.cpp’ / ‘Efficient.java’ / ‘efficient_2.py’(Python 2.7) / ‘efficient_3.py’ (Python 3)
c.  Summary.pdf
    It must contain the following details:
    1.  Datapoints output table (which are generated from provided input files)
    2.  Two graphs and Insights
    3.  Contribution from each group member e.g. coding, testing, report preparation, etc. if everybody did not have equal contribution (Please use the provided Summary.docx file, fill in the details and upload it as PDF)
d.  2 Shell files ‘basic.sh’ and ‘efficient.sh’ with the commands to compile and run your basic and efficient version. These are needed to provide you with flexibility in passing any additional compiler/run arguments that your programs might need. See More Hints (VII part E for more details)
    Example: `basic.sh`
    ```shell
    javac Basic.java
    java Basic "$1" "$2"
    ```
    Execution: `./basic.sh input.txt output.txt`
    `./efficient.sh input.txt output.txt`

### 4.2
The name of your zip file should have the USC IDs (not email ids) of everyone in your group separated by an underscore. e.g.
*   `1234567890_1234567890_1234567890.zip`
*   Contents (example):
    ```
    1234567890_1234567890_1234567890
        basic_2.py
        efficient_2.py
        Summary.pdf
        basic.sh
        efficient.sh
    ```

## 5 Grading
Please read the following instructions to understand how your submission will be evaluated.

### 5.1 Correctness of algorithms - 70 points
1.  Both programs (basic/ efficient) are correctly outputting files having all 5 lines in the correct order: 15 points
2.  Basic Algorithm: 25 points
3.  Memory Efficient Algorithm: 30 points

Note: Graders will execute your program on the ‘Linux’ OS. The goal of this part is to check correctness. The program should output a valid alignment having a minimum cost. **We will check whether the answer is valid, the cost is optimal and aligned with the answer.** Memory and Time will be evaluated in the next part.

### 5.2 Plots, analysis of results, insights and observations: 30 points
1.  Your program will be run on the input files (provided by us in the ‘data points’ folder) to generate output files. The memory and time in the output files should be in a “close range” to what is given by you in the Summary.pdf. Memory usage and time should follow some certain patterns, but they are no longer required to be within the range of the sample files.
2.  Correctness of the graph
3.  Correctness of Analysis/ Insights about the graph and the algorithms.

Note: Unlike 5.1, the evaluation of Part B is subjective so it will be done manually. So it is alright if your graphs/data points have ‘some’ outliers.

## 6 What is provided to you in the zip file?
A. SampleTestCases folder containing sample input and output files
B. Datapoint folder containing input files to generate graph data points.
C. Summary.docx file for reference

## 7 HINTS, NOTES, and FAQs

### 7.1 Regarding Input and string generation
1.  We will never give an invalid input to your program. Input strings will always contain A, C, G, T only.
2.  The length of the final input string should be equal to 2<sup>number of lines</sup> \* len(base string), as mentioned in the document.
3.  The string generation mechanism is the same irrespective of the basic or the efficient version of the algorithm.
4.  The entire program (string generation, solution, write output) should be written in a single file. You may break those functions into different classes to make the code modular, but there should be only one file. Your program won’t be evaluated based on how modular it is.

### 7.2 Regarding Algorithm and output
1.  DO NOT REFER TO THE PSEUDOCODE PROVIDED IN KLEINBERG AND TARDOS.
2.  DO NOT USE ANY LIBRARIES FOR WRITING YOUR ALGORITHMS.
3.  Samples for time and memory calculation are provided. **Please use them for consistency. You can refer to them. However, their memory reporting may not be accurate, which is acceptable. While more techniques exist for obtaining precise memory readings, code simplicity is prioritized to ensure easy execution without complex environment setup.**
4.  Your solutions for the regular and memory-efficient algorithms should be in two different programs.
5.  There can be multiple valid sequences with the same optimal cost; you can output any of those. All of them are valid.
6.  You should code both the basic version and memory-efficient algorithm. Even though the memory-efficient version will pass all the bounds of the simple version, you must not use the memory-efficient version in both sub-problems.
7.  Your program should not print anything when it runs. It should only write to the output file.
8.  There is no specific requirement for the precision of Time and Memory float values.
9.  Time and Memory depend on so many factors such as CPU, Operating System, etc. So there might be differences in the output. Therefore, it will be evaluated subjectively. There must be a clear distinction in behavior between programs whose Time/ Memory complexity is O(n) vs O(n<sup>2</sup>) vs O(log n).

### 7.3 Regarding the plot
1.  Both the graphs are line graphs. The X-axis represents problem size as m+n, where m and n are the lengths of the generated string, and the Y-axis of the Memory plot represents memory in KB, and the Y-axis of the Time Plot represents time in milliseconds. The 2 lines in the graph will represent stats of basic and memory-efficient algorithms.
2.  You can use any libraries/packages in any language to plot the graphs.
3.  You do not have to provide code for generating the plots. Only add images in the Summary.pdf.

### 7.4 Regarding Submission
1.  Only 1 person in the group needs to submit the project. We’ll get the USC IDs of all the other team members from the filenames.
2.  To allow for grading the whole class in a reasonable amount of time, we’ll kill your program if it runs for more than a minute on a single input file.

### 7.5 Regarding Shell File
To make the evaluation seamless on our end, please make sure you also have a shell script named ‘basic.sh’ and ‘efficient.sh’ with the commands required to run your program. For example, the contents of this file can be one of the following:

**C**
*Listing 1: C Shell Script*
`basic.sh`
```shell
gcc basic.c -o basic
./basic "$1" "$2"
```
`efficient.sh`
```shell
gcc efficient.c -o efficient
./efficient "$1" "$2"
```

**C++**
*Listing 2: C++ Shell Script*
`basic.sh`
```cpp
g++ basic.cpp -o basic
./basic "$1" "$2"```
`efficient.sh`
```cpp
g++ efficient.cpp -o efficient
./efficient "$1" "$2"
```

**Java**
*Listing 3: Java Shell Script*
`basic.sh`
```shell
javac Basic.java
java Basic "$1" "$2"
```
`efficient.sh`
```shell
javac Efficient.java
java Efficient "$1" "$2"
```

**Python 2.7**
*Listing 4: Python 2.7 Shell Script*
`basic.sh`
```shell
python2 basic_2.py "$1" "$2"
```
`efficient.sh`
```shell
python2 efficient_2.py "$1" "$2"
```

**Python 3**
*Listing 5: Python 3 Shell Script*
`basic.sh`
```shell
python3 basic_3.py "$1" "$2"
```
`efficient.sh`
```shell
python3 efficient_3.py "$1" "$2"
```
Note that the above are just examples. You can modify them according to your convenience. The goal is to have a language-independent mechanism to run your script to get your outputs. Also note that for python2 or python3 users, it’s important that they have 2 or 3 suffixes at the end.

### 7.6 Sample code for memory and time calculation

**Python**
*Listing 6: Python Time and Memory*
```python
import sys
from resource import *
import time
import psutil

def process_memory():
    process = psutil.Process()
    memory_info = process.memory_info()
    memory_consumed = int(memory_info.rss/1024) # in KB
    return memory_consumed

def time_wrapper():
    start_time = time.time()
    call_algorithm() # Replace with your algorithm function call
    end_time = time.time()
    time_taken = (end_time - start_time)*1000 # in milliseconds
    return time_taken
```

**Java**
*Listing 7: Java Time and Memory*
```java
// private static double getMemoryInKB() {
//     double total = Runtime.getRuntime().totalMemory();
//     return (total - Runtime.getRuntime().freeMemory()) / 1024;
// }

// private static double getTimeInMilliseconds() {
//     return System.nanoTime() / 1000000.0;
// }

// double beforeUsedMem = getMemoryInKB();
// double startTime = getTimeInMilliseconds();
// String[] alignment = basicSolution(firstString, secondString, delta, alpha);
// double afterUsedMem = getMemoryInKB();
// double endTime = getTimeInMilliseconds();
// double totalUsage = afterUsedMem - beforeUsedMem;
// double totalTime = endTime - startTime;

// For actual measurement in Java, it's often better to use:
// long beforeTime = System.nanoTime();
// // ... run algorithm ...
// long afterTime = System.nanoTime();
// double timeInMilliseconds = (afterTime - beforeTime) / 1000000.0;

// Runtime r = Runtime.getRuntime();
// r.gc(); // Suggest garbage collection to get a clearer memory picture
// long beforeMemory = r.totalMemory() - r.freeMemory();
// // ... run algorithm ...
// // Potentially, the peak memory usage is harder to capture this way without instrumentation
// // For a simple approach, measure after objects are created by algorithm.
// long afterMemory = r.totalMemory() - r.freeMemory();
// double memoryInKB = (afterMemory - beforeMemory) / 1024.0; // This measures delta, not necessarily peak.
// For peak RSS, external tools or JVMTI agents are more reliable in Java.
// The provided example code for Java memory is:
private static double getMemoryInKB() {
    double total = Runtime.getRuntime().totalMemory();
    // It seems the intention was to measure used memory, not free memory or a delta for specific objects.
    // A common way to get used memory:
    // return (Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1024.0;
    // However, the snippet continues with `double beforeUsedMem = getMemoryInKB();`
    // and `double totalUsage = afterUsedMem - beforeUsedMem;`
    // This suggests `getMemoryInKB()` is a snapshot. Let's use the structure given.
    // The sample shows `double totalUsage = afterUsedMem - beforeUsedMem;`
    // This implies `getMemoryInKB()` should be the total used memory at that point.
    return (Runtime.getRuntime().totalMemory() - Runtime.getRuntime().freeMemory()) / 1024.0;
}

private static double getTimeInMilliseconds() {
    return System.nanoTime() / 1000000.0;
}

// In your main/calling method:
// double beforeUsedMem = getMemoryInKB(); // Call before your algorithm starts consuming significant memory for its data structures
// double startTime = getTimeInMilliseconds();
// String[] alignment = basicSolution(firstString, secondString, delta, alpha); // Your algorithm
// double endTime = getTimeInMilliseconds();
// double afterUsedMem = getMemoryInKB(); // Call after algorithm execution and before cleanup
// double totalUsage = afterUsedMem - beforeUsedMem; // This might be negative if GC ran aggressively
// double totalTime = endTime - startTime;
// A better approach for memory for the project might be to capture max memory used by the process if possible,
// or ensure getMemoryInKB() is called after all primary data structures of the algorithm are allocated
// and before they are deallocated.
// The C/C++ getrusage is more direct for max RSS.
// For Java, the provided `(total - free) / 1024` is a common proxy for heap usage.
// `totalUsage = afterUsedMem - beforeUsedMem` is how it's shown to be used.
```

**C/C++**
*Listing 8: C/C++ Time and Memory*
```cpp
#include <iostream>
#include <sys/resource.h>
#include <errno.h>
#include <stdio.h>
#include <sys/time.h>

extern int errno;

// getrusage() is available in Linux. Your code will be evaluated in Linux OS.
long getTotalMemory() { // Peak RSS
    struct rusage usage;
    int returnCode = getrusage(RUSAGE_SELF, &usage);
    if (returnCode == 0) {
        return usage.ru_maxrss; // In KB on Linux
    } else {
        // It should never occur. Check man getrusage for more info to debug.
        // printf("error %d", errno); // Use perror for error messages
        perror("getrusage");
        return -1;
    }
}

int main() { // Example usage
    struct timeval begin, end;

    gettimeofday(&begin, 0);

    // write your solution here
    // Example: call_your_solution_function();

    // Please call getTotalMemory() only after calling your solution function. It
    // calculates max memory used by the program.
    long totalmemory_kb = getTotalMemory(); // Changed to long and clearer name

    gettimeofday(&end, 0);

    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double totaltime_ms = seconds * 1000.0 + microseconds / 1000.0; // Corrected to double and ms

    printf("%f\n", totaltime_ms);
    printf("%ld\n", totalmemory_kb); // Changed to %ld for long

    return 0;
}
```

---

# SUMMARY

**USC ID/s:**

**Datapoints**

| M+N  | Time in MS (Basic) | Time in MS (Efficient) | Memory in KB (Basic) | Memory in KB (Efficient) |
|------|--------------------|------------------------|----------------------|--------------------------|
| 16   |                    |                        |                      |                          |
| 64   |                    |                        |                      |                          |
| 128  |                    |                        |                      |                          |
| 256  |                    |                        |                      |                          |
| 384  |                    |                        |                      |                          |
| 512  |                    |                        |                      |                          |
| 768  |                    |                        |                      |                          |
| 1024 |                    |                        |                      |                          |
| 1280 |                    |                        |                      |                          |
| 1536 |                    |                        |                      |                          |
| 2048 |                    |                        |                      |                          |
| 2560 |                    |                        |                      |                          |
| 3072 |                    |                        |                      |                          |
| 3584 |                    |                        |                      |                          |
| 3968 |                    |                        |                      |                          |

**Insights**

**Graph1 - Memory vs Problem Size (M+N)**

[Add Graph1 here]

*Nature of the Graph (Logarithmic/ Linear/ Polynomial/ Exponential)*
Basic:
Efficient:
Explanation:

**Graph2 - Time vs Problem Size (M+N)**

[Add Graph2 here]

*Nature of the Graph (Logarithmic/ Linear/ Polynomial/ Exponential)*
Basic:
Efficient:
Explanation:

**Contribution**
(Please mention what each member did if you think everyone in the group does not have an equal contribution, otherwise, write “Equal Contribution”)
<USC ID/s>: <Equal Contribution>
```