#include "worker.h"
#include <atomic>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <filesystem>
#include <algorithm>

#include <cpr/cpr.h>
#include <glaze/glaze.hpp>

struct Solution
{
    std::string secret;
};

template <>
struct glz::meta<Solution>
{
    using T = Solution;

    static constexpr auto value = object(
        "secret", &T::secret
    );
};

// Combination constants
const long COMBI_4L = 1679616;
const long COMBI_5L = 60466176;
const long COMBI_6L = 2176782336;

void solve(int length, long totalCombi, int numThreads, const std::string& charset, 
           std::string& secret, std::atomic<bool>& foundFlag, const std::string& zipPath) {
    
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<Worker>> workers;
    
    long chunkSize = totalCombi / numThreads;

    for (int i = 0; i < numThreads; ++i) {
        long start = i * chunkSize;
        // Ensure the last thread covers the remainder due to integer division
        long end = (i == numThreads - 1) ? (totalCombi - 1) : (start + chunkSize - 1);

        workers.push_back(std::make_unique<Worker>(start, end, i + 1, length, 
                          charset, secret, foundFlag, zipPath));
        
        threads.emplace_back(&Worker::run, workers.back().get());
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}

int main() {
    std::string charset = "abcdefghijklmnopqrstuvwxyz0123456789";
    std::atomic<bool> foundFlag(false);
    std::string secret = "";

    std::filesystem::path root = PROJECT_ROOT;
    std::filesystem::path zip = (root / "build" / "problem.zip").string();

    // Configuration for each length
    struct Level {
        int len;
        long combi;
        int threads;
    };

    std::vector<Level> levels = {
        {4, COMBI_4L, 2},
        {5, COMBI_5L, 4},
        {6, COMBI_6L, (int)std::thread::hardware_concurrency()}
    };

    for (const auto& level : levels) {
        if (foundFlag.load()) break;

        std::cout << "Attempting " << level.len << " letters with " 
                  << level.threads << " threads..." << std::endl;
        
        solve(level.len, level.combi, level.threads, charset, secret, foundFlag, zip);
    }

    if (foundFlag.load()) {
        std::cout << "Secret found: " << secret << std::endl;
    } else {
        std::cout << "Password not found." << std::endl;
    }

    Solution solution{.secret = secret};
    std::string body;
    auto err = glz::write_json(solution, body);

    if (err)
    {
        std::cerr << "MAIN:JSON parsing error: " << err << std::endl;
        return 1;
    }

    std::string access_token = std::string(std::getenv("ACCESS_TOKEN"));

    cpr::Response response = cpr::Post(
        cpr::Url(
            "https://hackattic.com/challenges/brute_force_zip/"
            "solve?access_token=" + access_token
        ),
        cpr::Header{
            {"Content-Type", "application/json"}
        },
        cpr::Body(body)
    );

    std::cout << "Response status: " << response.status_code << std::endl;
    std::cout << "Body: " << response.text << std::endl;

    return 0;
}