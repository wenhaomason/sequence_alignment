#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/resource.h>
#include <sys/time.h>
#include <cstring>

// Function to get current memory usage in KB
long getTotalMemory() {
    struct rusage usage;
    int returnCode = getrusage(RUSAGE_SELF, &usage);
    if (returnCode == 0) {
        return usage.ru_maxrss; // In KB on Linux
    } else {
        perror("getrusage");
        return -1;
    }
}

// Mismatch penalty lookup table
const int ALPHA[4][4] = {
    {0, 110, 48, 94},    // A
    {110, 0, 118, 48},   // C
    {48, 118, 0, 110},   // G
    {94, 48, 110, 0}     // T
};

// Gap penalty
const int DELTA = 30;

// Function to convert character to index for ALPHA table
int charToIndex(char c) {
    switch (c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default: return -1;
    }
}

// Generate string according to the rules specified
std::string generateString(const std::string& baseString, const std::vector<int>& indices) {
    std::string result = baseString;
    
    for (int index : indices) {
        // Insert a copy of the current string after position 'index'
        result.insert(index + 1, result);
    }
    
    return result;
}

// Sequence alignment using dynamic programming
std::pair<std::string, std::string> basicSequenceAlignment(const std::string& X, const std::string& Y, int& cost) {
    int m = X.length();
    int n = Y.length();
    
    // Create DP table (m+1) x (n+1)
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
    // Initialize the DP table
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i * DELTA;
    }
    
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j * DELTA;
    }
    
    // Fill the DP table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int match = dp[i-1][j-1] + ALPHA[charToIndex(X[i-1])][charToIndex(Y[j-1])];
            int deleteX = dp[i-1][j] + DELTA;
            int deleteY = dp[i][j-1] + DELTA;
            
            dp[i][j] = std::min(std::min(match, deleteX), deleteY);
        }
    }
    
    // Backtrack to find the alignment
    std::string alignX, alignY;
    int i = m, j = n;
    
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && dp[i][j] == dp[i-1][j-1] + ALPHA[charToIndex(X[i-1])][charToIndex(Y[j-1])]) {
            // Match or mismatch
            alignX = X[i-1] + alignX;
            alignY = Y[j-1] + alignY;
            i--; j--;
        } else if (i > 0 && dp[i][j] == dp[i-1][j] + DELTA) {
            // Gap in Y
            alignX = X[i-1] + alignX;
            alignY = '_' + alignY;
            i--;
        } else {
            // Gap in X
            alignX = '_' + alignX;
            alignY = Y[j-1] + alignY;
            j--;
        }
    }
    
    // Set the cost
    cost = dp[m][n];
    
    return {alignX, alignY};
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }
    
    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];
    
    std::ifstream inputFile(inputFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }
    
    // Read input
    std::string baseString1, baseString2;
    std::vector<int> indices1, indices2;
    std::vector<int> *i = &indices1;
    std::string line;

    // Read first base string
    inputFile >> baseString1;

    while (inputFile >> line) {
        if (isalpha(line[0])) {
            baseString2 = line;
            i = &indices2;
        } else if (isdigit(line[0])) {
            i->push_back(std::stoi(line));
        }
    }

    int j = indices1.size();
    int k = indices2.size();
    
    inputFile.close();

    int expectedLen1 = baseString1.length() * (1 << j);
    int expectedLen2 = baseString2.length() * (1 << k);

    std::string string1 = baseString1;
    std::string string2 = baseString2;
    
    // Generate strings
    if (!indices1.empty())
        string1 = generateString(baseString1, indices1);
    if (!indices2.empty())
        string2 = generateString(baseString2, indices2);

    if (string1.length() != expectedLen1 || string2.length() != expectedLen2) {
        std::cerr << "Error: Generated strings have incorrect lengths." << std::endl;
        return 1;
    }
    
    // Measure time and memory
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    
    // Perform sequence alignment
    int cost;
    std::pair<std::string, std::string> alignment = basicSequenceAlignment(string1, string2, cost);
    
    // Get memory usage
    long memoryUsage = getTotalMemory();
    
    gettimeofday(&end, 0);
    
    // Calculate time in milliseconds
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double timeMs = seconds * 1000.0 + microseconds / 1000.0;
    
    // Write results to output file
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return 1;
    }
    
    outputFile << cost << std::endl;
    outputFile << alignment.first << std::endl;
    outputFile << alignment.second << std::endl;
    outputFile << timeMs << std::endl;
    outputFile << memoryUsage << std::endl;
    
    outputFile.close();
    
    return 0;
} 