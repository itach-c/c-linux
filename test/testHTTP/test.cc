// #include <iostream>
// #include <sys/stat.h>
// #include <unistd.h>
// #include "util.hpp"
// // static std::string urlEnCode(const std::string &url, bool space_to_plus)
// // {
// //     std::string res;

// //     for (auto &u : url)
// //     {
// //         if (u == '.' || u == '~' || u == '_' || isalnum(u))
// //         {
// //             res += u;
// //             continue;
// //         }
// //         else if (u == ' ' && space_to_plus == true)
// //         {
// //             res += '+';
// //         }
// //         else
// //         {

// //             char tmp[4] = {0};

// //             sprintf(tmp, "%%%02X", u);
// //             res += tmp;
// //         }
// //     }
// //     return res;
// // }

// // // url解码
// // static char HexToi(char c)
// // {
// //     if (c >= '0' && c <= '9')
// //     {
// //         return c - '0';
// //     }

// //     else if (c >= 'a' && c <= 'z')
// //     {

// //         return c - 'a' + 10;
// //     }
// //     else if (c >= 'A' && c <= 'Z')
// //     {

// //         return c - 'A' + 10;
// //     }
// //     else
// //     {

// //         return -1;
// //     }
// // }
// // static std::string urlDeCode(const std::string &url, bool space_to_plus)
// // {

// //     std::string res;

// //     for (int i = 0; i < url.size(); i++)
// //     {

// //         if (url[i] == '%')
// //         {
// //             char n1 = url[i + 1];
// //             char n2 = url[i + 2];
// //             char result = (HexToi(n1) << 4) + HexToi(n2);
// //             res += result;
// //             i += 2;
// //         }
// //         else if (url[i] == '+' && space_to_plus == true)
// //         {
// //             res += ' ';
// //         }
// //         else
// //         {
// //             res += url[i];
// //         }
// //     }

// //     return res;
// // }

// //    static bool isDirectory(const std::string &path)
// //     {
// //         struct stat st;
// //         int ret = stat(path.c_str(), &st);
// //         if (ret < 0)
// //         {
// //             return false;
// //         }

// //         return S_ISDIR(st.st_mode);
// //     }

// //     // 是否是常规文件
// //     static bool isRegular(const std::string &path)
// //     {
// //         struct stat st;
// //         int ret = stat(path.c_str(), &st);
// //         if (ret < 0)
// //         {
// //             return false;
// //         }

// //         return S_ISREG(st.st_mode);
// //     }

// int main()
// {

//     // // https://www.example.com/search?query=c++&page=1
//     // // https%3A%2F%2Fwww.example.com%2Fsearch%3Fquery%3Dc%2B%2B%26page%3D1

//     // std::string c = "https://www.example.com/search?query=c++&page=1";
//     // std::cout << c << std::endl;

//     // std::string ret = urlEnCode(c, true);

//     // std::cout << ret << std::endl;

//     // ret = urlDeCode(ret, true);

//     // std::cout << ret << std::endl;
//     // // https%3A%2F%2Fwww.example.com%2Fsearch%3Fquery%3Dc%2B%2B%26page%3D1

//     //  std::cout << utile::isDirectory("./test.cc") << std::endl;
//     //      std::cout <<  utiisDirectory("../http") << std::endl;

//     //    std::cout << isRegular("./test.cc") << std::endl;

//    // std::cout << utile::isValidPath("/index.html/../index.html");

// //     return 0;
// // }

#include "httpRequest.hpp"
#include "httpRequest.hpp"
#include "httpContext.hpp"
int main()
{

    std::string http_request =
        "POST /submit_form HTTP/1.0\r\n"
        "Host: www.example.com\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36\r\n"
        "Content-Type: application/x-www-form-urlencoded\r\n"
        "Accept: text/html\r\n"
        "Content-Length: 10\r\n"
        "Connection: keep-alive\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "DNT: 1\r\n"
        "Cache-Control: no-cache\r\n"
        "\r\n"
        "hell";

    Buffer buffer;
    buffer.WriteString(http_request.c_str());

    HttpContext context;

    while (context.GetRcvStatue() != HttpContext::RCV_OVER)
    {

        context.Rcv_Context_HttpReq(&buffer);
        buffer.WriteString("abcdefasdasdasdasdasdasdasd");
    }

    HttpRequest req = context.GetHttp();

    std::cout << req.method_ << " " << req.request_path_ << " " << req.version_ << std::endl;

    for (auto &head : req.headers_)
    {
        printf("%s: %s\n", head.first.c_str(), head.second.c_str());
    }
    for (auto &search : req.search_params_)
    {
        printf("%s=%s\n", search.first.c_str(), search.second.c_str());
    }
    std::cout << req.content_ << std::endl;
    std::cout << context.GetRcvStatue() << std::endl;

    return 0;
}