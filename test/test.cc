#include <iostream>
#include <cstdlib>

// int main() {
//     std::string url = "https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/boost_1_84_0.zip";
//     std::string command = "wget " + url;
//     command += " -P ../data";

//     int result = system(command.c_str());

//     if (result == 0) {
//         std::cout << "File downloaded successfully\n";
//     } else {
//         std::cerr << "Failed to download file\n";
//     }

//     return 0;
// }


// #include <iostream>
// #include <filesystem>

// namespace fs = std::filesystem;

// int main() {
//     std::string sourcePath = "source/file.zip";
//     std::string targetPath = "target/file.zip";

//     try {
//         fs::path source = fs::path(sourcePath);
//         fs::path target = fs::path(targetPath);

//         fs::rename(source, target);

//         std::cout << "File moved successfully" << std::endl;
//     } catch (const std::exception& e) {
//         std::cerr << "Error moving file: " << e.what() << std::endl;
//     }

//     return 0;
// }
