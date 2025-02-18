/*大文件传输测试，给服务器上传一个大文件，服务器将文件保存下来，观察处理结果*/
/*
    上传的文件，和服务器保存的文件一致
*/
#include "../../source/tcp/tcpserver.h"
#include "/home/itach/code/project/mymuduo/source/http/util.hpp"
int main()
{
    Socket cli_sock;
    std::string body;
    util::ReadFile("./hello.txt", &body);

    std::cout << "read over " << std::endl;
    cli_sock.CreateClient("127.0.0.1", 8888);
    std::cout << "conn success" << std::endl;
    std::string req = "PUT /putfile HTTP/1.1\r\nConnection: keep-alive\r\n";
    req += "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n";
    cli_sock.Send(req.c_str(), req.size());
    cli_sock.Send(body.c_str(), body.size());
    char buf[1024] = {0};

    cli_sock.Recv(buf, 1023);
    printf("%s", buf);
   while(1)
   {
    sleep(1);
   }
    cli_sock.Close();
    return 0;
}