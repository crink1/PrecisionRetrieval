#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <curl.h>

class WebScraper
{
private:
    CURL *_curl;
    CURLcode _res;
    std::string _url;
    std::string _htmlContent;

private:
    size_t write_callback(void *contents, size_t size, size_t nmemb, std::string *buffer)
    {
        size_t realsize = size * nmemb;
        buffer->append((char *)contents, realsize);
        return realsize;
    }

public:
    WebScraper(std::string url)
        : _curl(curl_easy_init()), _res(curl_easy_perform(_curl)), _url(url)
    {
    }
    ~WebScraper()
    {
        if (_curl)
        {
            curl_easy_cleanup(_curl);
        }
    }

    void Gethtml()
    {
        if (_curl)
        {
            curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str());
            curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, std::bind(&WebScraper::write_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_htmlContent);

            if (_res != CURLE_OK)
            {
                std::cerr << "Error: " << curl_easy_strerror(_res) << std::endl;
            }
            else
            {
                // std::cout << "HTML content of the website: " << std::endl;
                // std::cout << htmlContent << std::endl;
            }
        }
        else
        {
            std::cerr << "Could not initialize cURL!" << std::endl;
        }
    }

    std::string Extract()
    {
        std::string ret;
        size_t tmp = _htmlContent.find("/users/history/version");
        size_t lpos = _htmlContent.find("_", tmp);
        size_t rpos = _htmlContent.find(".html", lpos);
        ret = _htmlContent.substr(lpos, rpos - lpos);
    }

    void Launch()
    {
        Gethtml();
        std::string ret = Extract();
        std::cout << ret << std::endl;
    }
};
