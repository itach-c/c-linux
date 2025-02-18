#include "../source/tcpserver.h"

class EchoServer
{

private:
    TcpServer server_;

public:
    EchoServer(const std::string &ip, uint32_t port)
        : server_(ip, port, true)

    {

        server_.SetOnConnectionCallBack(std::bind(&EchoServer::OnConnection, this, std::placeholders::_1));

        server_.SetOnMessageCallBack(std::bind(&EchoServer::OnMessage, this, std::placeholders::_1, std::placeholders::_2));

        server_.SetOnCloseCallBack(std::bind(&EchoServer::OnClose, this, std::placeholders::_1));


       // server_.SetOnEventsCallBack(std::bind(&EchoServer::OnEvent, this, std::placeholders::_1));
    }

    void start()
    {
        server_.SetThreadNum(0);
    
        server_.Start();

    }

private:
    void OnConnection(Connection::PtrConnection conn)
    {
        std::cout << "get a new connection" << std::endl;
    
        LOG_INFO("conn address %p", conn.get());
    }
    void OnMessage(Connection::PtrConnection conn, Buffer *buffer)
    {
       
        std::string str = buffer->ReadAsString(buffer->ReadAbleBytes());
        std::cout << str.c_str() << std::endl;
        std::string data = "今天天气不错!";
        conn->Send(data.c_str(), data.size());
        //conn->ShutDown();
    }
    // void OnEvent(Connection::PtrConnection)
    // {

    //    // std::cout << "有一个事件" << std::endl;
    // }
    void OnClose(Connection::PtrConnection conn)
    {
        LOG_INFO("连接关闭");
    }
};


int main()
{
    EchoServer server("127.0.0.1",8888);
    server.start();



    return 0;
}