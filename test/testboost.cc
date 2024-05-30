#include <iostream>
#include <string>
#include <fstream>
#include "WebScraper.hpp"
using namespace std;

int main()
{
    WebScraper ws("https://www.boost.org/");
    ws.Launch();

    return 0;
}

// size_t write_callback(void *contents, size_t size, size_t nmemb, std::string *buffer)
// {
//     size_t realsize = size * nmemb;
//     buffer->append((char *)contents, realsize);
//     return realsize;
// }

// int main()
// {
//     CURL *curl;
//     CURLcode res;
//     std::string url = "https://www.boost.org/";
//     std::string htmlContent;

//     curl = curl_easy_init();
//     if (curl)
//     {
//         curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
//         curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);

//         res = curl_easy_perform(curl);

//         if (res != CURLE_OK)
//         {
//             std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
//         }
//         else
//         {
//             std::cout << "HTML content of the website: " << std::endl;
//             //std::cout << htmlContent << std::endl;
//         }

//         curl_easy_cleanup(curl);
//     }
//     else
//     {
//         std::cerr << "Could not initialize cURL!" << std::endl;
//     }

//     string ret;
//     size_t tmp = htmlContent.find("/users/history/version");
//     size_t lpos = htmlContent.find("_", tmp);
//     size_t rpos = htmlContent.find(".html" ,lpos);
//     ret = htmlContent.substr(lpos, rpos - lpos);
//     cout << ret << endl;
//     return 0;
// }
