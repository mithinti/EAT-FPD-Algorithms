#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " input_file.txt" << std::endl;
        return 1;
    }

    // Open the input file
    std::ifstream input_file(argv[1]);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    int n, m;
    input_file >> n >> m;

    // Create a vector to store the out degree of each vertex
    std::vector<int> outDegree(n, 0);

    // Read and process each edge
    for (int i = 0; i < m; ++i) {
        int u, v, t, l;
        input_file >> u >> v >> t >> l;

        // Increment the out degree of vertex u
        outDegree[u]++;
    }

    // Find the maximum out degree
    int maxOutDegree = 0;
    for (int i = 0; i < n; ++i) {
        if (outDegree[i] > maxOutDegree) {
            maxOutDegree = outDegree[i];
        }
    }

    // Calculate the average out degree
    int totalOutDegree = 0;
    for (int i = 0; i < n; ++i) {
        totalOutDegree += outDegree[i];
    }
    double averageOutDegree = static_cast<double>(totalOutDegree) / n;

    // Output the results
    std::cout << "Maximum Out-Degree: " << maxOutDegree << std::endl;
    std::cout << "Average Out-Degree: " << averageOutDegree << std::endl;

    // Close the input file
    input_file.close();

    return 0;
}
