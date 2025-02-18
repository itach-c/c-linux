#include "../../source/tcp/tcpserver.h"
int main()
{

    Socket client;
    client.CreateClient("127.0.0.1", 8888);

    int i = 5;
    while (i--)
    {
        char buf[1024];
        std::string str = "傻逼王瑞";
        int n = write(client.Fd(), str.c_str(), str.size());
        std::cout << "发送字节数" << std::endl;
        int ret = read(client.Fd(), buf, sizeof(buf));
        if (ret > 0)
        {

            buf[ret] = '\0';
        }
        sleep(1);

        std::cout << buf << std::endl;
    }
    while (1)
    {
    }

    return 0;
}

