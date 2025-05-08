#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // For std::reverse, std::min
#include <tuple>     // For std::tuple, std::make_tuple, std::get
#include <sys/resource.h>
#include <sys/time.h>
#include <cstring>   // For perror

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
        default: return -1; // Should not happen with valid inputs
    }
}

// Generate string according to the rules specified
// Generate string according to the rules specified
std::string generateString(const std::string& baseString, const std::vector<int>& indices) {
    std::string result = baseString;
    
    for (int index : indices) {
        // Insert a copy of the current string after position 'index'
        result.insert(index + 1, result);
    }
    
    return result;
}

// Computes scores for the last row of DP table (Needleman-Wunsch) using O(N) space
// Used by Hirschberg to find the optimal split point in Y
std::vector<int> NWScore(const std::string& X, const std::string& Y) {
    int m = X.length();
    int n = Y.length();

    std::vector<int> prev_dp_row(n + 1);
    std::vector<int> curr_dp_row(n + 1);

    for (int j = 0; j <= n; ++j) {
        prev_dp_row[j] = j * DELTA;
    }

    for (int i = 1; i <= m; ++i) {
        curr_dp_row[0] = i * DELTA;
        for (int j = 1; j <= n; ++j) {
            int charX_idx = charToIndex(X[i-1]);
            int charY_idx = charToIndex(Y[j-1]);
            
            // Assuming charToIndex returns valid indices (0-3) due to input constraints
            int score_match_mismatch = prev_dp_row[j-1] + ALPHA[charX_idx][charY_idx];
            int score_gap_in_Y = prev_dp_row[j] + DELTA; // X[i-1] vs '-'
            int score_gap_in_X = curr_dp_row[j-1] + DELTA; // '-' vs Y[j-1]
            
            curr_dp_row[j] = std::min({score_match_mismatch, score_gap_in_Y, score_gap_in_X});
        }
        prev_dp_row = curr_dp_row;
    }
    return prev_dp_row; // This is the last row of scores
}

// Standard Needleman-Wunsch algorithm for base cases of Hirschberg
// Returns aligned strings and computes the cost
std::pair<std::string, std::string> standardNeedlemanWunsch(const std::string& X, const std::string& Y, int& cost) {
    int m = X.length();
    int n = Y.length();
    
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    
    for (int i = 0; i <= m; i++) dp[i][0] = i * DELTA;
    for (int j = 0; j <= n; j++) dp[0][j] = j * DELTA;
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int charX_idx = charToIndex(X[i-1]);
            int charY_idx = charToIndex(Y[j-1]);
            // Assuming valid indices from charToIndex due to input constraints
            int mismatch_penalty = ALPHA[charX_idx][charY_idx];

            int score_match = dp[i-1][j-1] + mismatch_penalty;
            int score_deleteX = dp[i-1][j] + DELTA; // Gap in Y
            int score_deleteY = dp[i][j-1] + DELTA; // Gap in X
            dp[i][j] = std::min({score_match, score_deleteX, score_deleteY});
        }
    }
    
    cost = dp[m][n];
    
    std::string alignX_rev, alignY_rev;
    int i = m, j = n;
    
    while (i > 0 || j > 0) {
        if (i > 0 && j > 0) {
            int charX_idx = charToIndex(X[i-1]);
            int charY_idx = charToIndex(Y[j-1]);
            int mismatch_penalty = ALPHA[charX_idx][charY_idx];
            if (dp[i][j] == dp[i-1][j-1] + mismatch_penalty) {
                alignX_rev += X[i-1];
                alignY_rev += Y[j-1];
                i--; j--;
                continue;
            }
        }
        if (i > 0 && dp[i][j] == dp[i-1][j] + DELTA) {
            alignX_rev += X[i-1];
            alignY_rev += '-';
            i--;
        } else { // j > 0 && dp[i][j] == dp[i][j-1] + DELTA (or only option left)
            alignX_rev += '-';
            alignY_rev += Y[j-1];
            j--;
        }
    }
    
    std::reverse(alignX_rev.begin(), alignX_rev.end());
    std::reverse(alignY_rev.begin(), alignY_rev.end());
    return {alignX_rev, alignY_rev};
}

// Recursive Hirschberg algorithm implementation
std::tuple<std::string, std::string, int> hirschberg_recursive(const std::string& X, const std::string& Y) {
    std::string alignX, alignY;
    int cost = 0;
    int m = X.length();
    int n = Y.length();

    if (m == 0) {
        for (int k = 0; k < n; ++k) { alignX += '-'; alignY += Y[k]; }
        cost = n * DELTA;
    } else if (n == 0) {
        for (int k = 0; k < m; ++k) { alignX += X[k]; alignY += '-'; }
        cost = m * DELTA;
    } else if (m <= 1 || n <= 1) { // Base case: Use standard Needleman-Wunsch for small strings
        std::pair<std::string, std::string> res_pair = standardNeedlemanWunsch(X, Y, cost);
        alignX = res_pair.first;
        alignY = res_pair.second;
    } else {
        int x_mid = m / 2;
        std::string X_left = X.substr(0, x_mid);
        std::string X_right = X.substr(x_mid);

        std::vector<int> scoreL = NWScore(X_left, Y);

        std::string X_right_rev = X_right; 
        std::reverse(X_right_rev.begin(), X_right_rev.end());
        std::string Y_rev = Y; 
        std::reverse(Y_rev.begin(), Y_rev.end());
        
        std::vector<int> scoreR_rev = NWScore(X_right_rev, Y_rev);
        
        std::vector<int> scoreR(n + 1);
        for(int j=0; j <=n; ++j) {
            scoreR[j] = scoreR_rev[n-j];
        }

        int y_split_idx = 0; // Optimal split point in Y
        int min_combined_score = scoreL[0] + scoreR[0]; // Initialize with splitting Y at the beginning

        for (int j = 0; j <= n; ++j) { // Find optimal y_split_idx
            if (scoreL[j] + scoreR[j] < min_combined_score) {
                min_combined_score = scoreL[j] + scoreR[j];
                y_split_idx = j;
            }
        }
        cost = min_combined_score;

        std::string Y_left = Y.substr(0, y_split_idx);
        std::string Y_right = Y.substr(y_split_idx);

        auto align_tuple_left = hirschberg_recursive(X_left, Y_left);
        auto align_tuple_right = hirschberg_recursive(X_right, Y_right);

        alignX = std::get<0>(align_tuple_left) + std::get<0>(align_tuple_right);
        alignY = std::get<1>(align_tuple_left) + std::get<1>(align_tuple_right);
        // Cost is min_combined_score. Sum of costs from subproblems should also yield this.
    }
    return std::make_tuple(alignX, alignY, cost);
}

// Wrapper for Hirschberg algorithm
std::tuple<std::string, std::string, int> efficientSequenceAlignment(const std::string& X, const std::string& Y) {
    if (X.empty() && Y.empty()) {
        return std::make_tuple("", "", 0);
    }
    // The recursive function handles empty X or Y correctly.
    return hirschberg_recursive(X, Y);
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
        std::cerr << "Error: Unable to open input file: " << inputFilePath << std::endl;
        return 1;
    }
    
    std::string baseString1, baseString2;
    std::vector<int> indices1, indices2;
    std::string line;
    bool reading_first_indices = true;

    if (!(inputFile >> baseString1)) {
        std::cerr << "Error: Could not read first base string." << std::endl;
        return 1;
    }

    while (inputFile >> line) {
        if (isalpha(line[0])) {
            baseString2 = line;
            reading_first_indices = false;
        } else {
            try {
                int index_val = std::stoi(line);
                if (reading_first_indices) {
                    indices1.push_back(index_val);
                } else {
                    indices2.push_back(index_val);
                }
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Error: Invalid integer format in input file: " << line << std::endl;
                return 1;
            } catch (const std::out_of_range& oor) {
                std::cerr << "Error: Integer out of range in input file: " << line << std::endl;
                return 1;
            }
        }
    }
    inputFile.close();
    
    std::string string1 = baseString1;
    std::string string2 = baseString2;
    
    // Generate strings
    if (!indices1.empty())
        string1 = generateString(baseString1, indices1);
    if (!indices2.empty())
        string2 = generateString(baseString2, indices2);

    struct timeval begin_time, end_time;
    gettimeofday(&begin_time, 0);
    
    std::tuple<std::string, std::string, int> alignment_result = efficientSequenceAlignment(string1, string2);
    
    long memoryUsage = getTotalMemory(); // Peak memory usage
    
    gettimeofday(&end_time, 0);
    
    long seconds = end_time.tv_sec - begin_time.tv_sec;
    long microseconds = end_time.tv_usec - begin_time.tv_usec;
    double timeMs = seconds * 1000.0 + microseconds / 1000.0;
    
    std::ofstream outputFile(outputFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file: " << outputFilePath << std::endl;
        return 1;
    }
    
    outputFile << std::get<2>(alignment_result) << std::endl;    // Cost
    outputFile << std::get<0>(alignment_result) << std::endl;    // Aligned String 1
    outputFile << std::get<1>(alignment_result) << std::endl;    // Aligned String 2
    outputFile << timeMs << std::endl;                           // Time in Milliseconds
    outputFile << memoryUsage << std::endl;                      // Memory in Kilobytes
    
    outputFile.close();
    
    return 0;
}
