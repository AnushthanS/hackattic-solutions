#include "password_generator.h"
#include <cassert>
#include <chrono>
#include <iostream>

using namespace std::chrono;

int main() {
    // 1. Initialize with a specific charset
    PasswordConverter converter("abcdefghijklmnopqrstuvwxyz0123456789");
    std::string filename = "test_dump.txt";

    std::cout << "--- Starting Password Toolkit Tests ---\n" << std::endl;

    // 2. Test: Integer to Password Conversion
    // val 0 should be 'aaaaa', val 1 should be 'aaaab' (depending on length)
    long test_val = 1000;
    int length = 4;
    std::string encoded = converter.numToPassword(test_val, length);
    std::cout << "[Test 1] intToPassword(" << test_val << "): " << encoded
              << std::endl;

    // 3. Test: Password to Integer (Round-trip)
    long decoded = converter.passwordToNum(encoded);
    std::cout << "[Test 2] passwordToInt(" << encoded << "): " << decoded
              << std::endl;

    if (test_val == decoded) {
        std::cout << " >> SUCCESS: Round-trip conversion matches." << std::endl;
    } else {
        std::cerr << " >> FAILURE: Round-trip conversion failed!" << std::endl;
    }

    // 4. Test: Password Increment Logic
    std::string overflow_test =
        "999"; // '6' is the last char in our custom charset
    std::cout << "\n[Test 3] Incrementing edge case: " << overflow_test
              << " -> ";
    converter.incrementPassword(overflow_test);
    std::cout << overflow_test << " (Expected: aaaa)" << std::endl;

    // 5. Test: File Management (Batch Writing)
    std::vector<std::string> batch = {"pass1", "pass2", "pass3", "pass4"};
    bool file_ok = PasswordFileManager::dumpToFile(batch, filename,
                                                   false); // false = overwrite

    if (file_ok) {
        std::cout << "\n[Test 4] dumpToFile: Success. Data written to "
                  << filename << std::endl;
    } else {
        std::cerr << "\n[Test 4] dumpToFile: Failed to open file!" << std::endl;
    }

    // 6. Mini Benchmark
    std::cout << "\n--- Starting Benchmark (100,000 generations) ---"
              << std::endl;
    auto t_start = high_resolution_clock::now();

    std::vector<std::string> bench_buffer;
    bench_buffer.reserve(100000);
    for (long i = 0; i < 100000; ++i) {
        bench_buffer.push_back(converter.numToPassword(i, 8));
    }

    auto t_end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(t_end - t_start);

    std::cout << "Generated 100k passwords in: " << duration.count() << "ms"
              << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    return 0;
}
