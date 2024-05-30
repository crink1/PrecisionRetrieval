#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <unistd.h>
#include <sys/wait.h>
#include "../curl/curl.h"

#define DEFAULT_DESC_PATH "./data/input/"

class WebScraper
{
private:
    CURL *_curl;
    std::string _url;
    std::string _htmlContent;
    std::string _tar;
    std::string _filename;
    const std::string _local_path;
    std::string _download_url;
    const std::string _mvdest_path;

private:
    static size_t write_callback(void *contents, size_t size, size_t nmemb, std::string *buffer)
    {
        size_t realsize = size * nmemb;
        buffer->append((char *)contents, realsize);
        return realsize;
    }

public:
    WebScraper(std::string url, const std::string &local_path = "", const std::string &mvdest_path = DEFAULT_DESC_PATH)
        : _url(url), _local_path(local_path), _mvdest_path(mvdest_path)
    {
    }

    void Gethtml()
    {
        CURLcode _res;
        _curl = curl_easy_init();
        if (_curl)
        {
            curl_easy_setopt(_curl, CURLOPT_URL, _url.c_str());
            curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &_htmlContent);

            _res = curl_easy_perform(_curl);
            if (_res != CURLE_OK)
            {
                std::cerr << "Error: " << curl_easy_strerror(_res) << std::endl;
            }
            else
            {
                // std::cout << "HTML content of the website: " << std::endl;
                // std::cout << _htmlContent << std::endl;
            }
        }
        else
        {
            std::cerr << "Could not initialize cURL!" << std::endl;
        }
        curl_easy_cleanup(_curl);
    }

    std::string Extract()
    {
        std::string ret;
        size_t tmp = _htmlContent.find("/users/history/version");
        if (tmp == std::string::npos)
        {
            return "NoSercher";
        }
        size_t lpos = _htmlContent.find("_", tmp);
        size_t rpos = _htmlContent.find(".html", lpos);
        ret = _htmlContent.substr(lpos, rpos - lpos);
        return ret;
    }

    void HandlerDurlAndName()
    {
        std::string name = _tar;
        size_t pos = 0;
        while ((pos = name.find("_")) != std::string::npos)
        {
            name.replace(pos, 1, ".");
        }
        _download_url = "https://boostorg.jfrog.io/artifactory/main/release/";
        if (name[0] == '.')
        {
            _download_url += name.substr(1);
        }
        else
        {
            _download_url += name;
        }
        _download_url += "/source/boost";
        _download_url += _tar;
        _download_url += ".tar.gz";

        _filename = "boost" + _tar;
        _tar += ".tar.gz";
    }

    bool Download()
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            // 失败处理
            std::cerr << "Failed to fork" << std::endl;
            return false;
        }
        else if (pid == 0)
        {
            // 子进程中执行wget命令
            const char *wget_args[] = {"wget", _download_url.c_str(), nullptr};

            // 调用exec函数族中的其中一个函数来执行wget命令
            execvp("wget", const_cast<char *const *>(wget_args));

            // 如果execvp返回，说明出错
            std::cerr << "Failed to execute wget" << std::endl;
            return false;
        }
        else
        {
            // 等待子进程结束
            int status;
            waitpid(pid, &status, 0);

            // 检查子进程的退出状态
            if (WIFEXITED(status))
            {
                std::cout << "wget exited with status " << WEXITSTATUS(status) << std::endl;
            }
            else
            {
                std::cerr << "wget terminated abnormally" << std::endl;
            }
        }

        // std::string command = "wget " + _download_url;
        // if (!_local_path.empty())
        // {
        //     command += " -P ";
        //     command += _local_path;
        // }
        // int result = system(command.c_str());

        // if (result == 0)
        // {
        //     std::cout << "File downloaded successfully\n";
        // }
        // else
        // {
        //     std::cerr << "Failed to download file\n";
        // }
        return true;
    }

    bool unzip()
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            // 失败处理
            std::cerr << "Failed to fork" << std::endl;
            return false;
        }
        else if (pid == 0)
        {
            // 子进程中执行tar命令解压文件
            if (!_local_path.empty())
            {
                const char *tar_args[] = {"tar", "-xvf", _filename.c_str(), "-P", _local_path.c_str(), nullptr};
                // 调用exec函数族中的其中一个函数来执行tar命令
                execvp("tar", const_cast<char *const *>(tar_args));
            }
            else
            {
                const char *tar_args[] = {"tar", "-xvf", _filename.c_str(), nullptr};
                // 调用exec函数族中的其中一个函数来执行tar命令
                execvp("tar", const_cast<char *const *>(tar_args));
            }

            // 如果execvp返回，说明出错
            std::cerr << "Failed to execute tar" << std::endl;
            return false;
        }
        else
        {
            // 等待子进程结束
            int status;
            waitpid(pid, &status, 0);

            // 检查子进程的退出状态
            if (WIFEXITED(status))
            {
                std::cout << "tar exited with status " << WEXITSTATUS(status) << std::endl;
            }
            else
            {
                std::cerr << "tar terminated abnormally" << std::endl;
            }
        }
        return true;
    }

    bool RmOld()
    {
         pid_t pid = fork();

        if (pid == -1)
        {
            // 失败处理
            std::cerr << "Failed to fork" << std::endl;
            return false;
        }
        else if (pid == 0)
        {
            
            const char *mv_args[] = {"rm", "-rf", _mvdest_path.c_str(), nullptr};

            // 调用exec函数族中的其中一个函数来执行wget命令
            execvp("rm", const_cast<char *const *>(mv_args));

            // 如果execvp返回，说明出错
            std::cerr << "Failed to execute rm" << std::endl;
            return false;
        }
        else
        {
            // 等待子进程结束
            int status;
            waitpid(pid, &status, 0);

            // 检查子进程的退出状态
            if (WIFEXITED(status))
            {
                std::cout << "rm exited with status " << WEXITSTATUS(status) << std::endl;
            }
            else
            {
                std::cerr << "rm terminated abnormally" << std::endl;
            }
        }
        return true;
    }

    bool Move()
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            // 失败处理
            std::cerr << "Failed to fork" << std::endl;
            return false;
        }
        else if (pid == 0)
        {
            // 子进程中执行wget命令
            std::string src = "./";
            src += _filename;
            src += "/doc/html";
            std::cout << src << " " << _mvdest_path << std::endl;
            const char *mv_args[] = {"cp", "-rf",src.c_str(), _mvdest_path.c_str(), nullptr};

            // 调用exec函数族中的其中一个函数来执行wget命令
            execvp("cp", const_cast<char *const *>(mv_args));

            // 如果execvp返回，说明出错
            std::cerr << "Failed to execute cp" << std::endl;
            return false;
        }
        else
        {
            // 等待子进程结束
            int status;
            waitpid(pid, &status, 0);

            // 检查子进程的退出状态
            if (WIFEXITED(status))
            {
                std::cout << "cp exited with status " << WEXITSTATUS(status) << std::endl;
            }
            else
            {
                std::cerr << "cp terminated abnormally" << std::endl;
            }
        }
        return true;
    }


    

    void Launch()
    {
        Gethtml();
        _tar = Extract();
        HandlerDurlAndName();
        // bool flag = Download();
        // if (!flag)
        // {
        //     std::cerr << "Download error" << std::endl;
        // }
        // flag = unzip();
        // if (!flag)
        // {
        //     std::cerr << "unzip error" << std::endl;
        // }
        RmOld();
        Move();
    }
};
