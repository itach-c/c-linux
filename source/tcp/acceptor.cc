
#include "acceptor.h"
#include <fcntl.h>
#include <unistd.h>

int setNonBlocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1)
    {
        return -1; // 获取文件描述符状态失败
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void Acceptor::HandleRead()
{

    int newfd = listen_socket_.Accept();
    setNonBlocking(newfd);
    if (newfd < 0)
        return;
    if (accept_callback_)
        accept_callback_(newfd);
}

Acceptor::Acceptor(EventLoop *loop, const std::string &ip, uint32_t port, bool reusePort)
    : loop_(loop)
{

    bool ret = listen_socket_.CreatListenSocket(ip, port, true, reusePort);

    assert(ret);
    listen_channel_ = std::make_unique<Channel>(loop, listen_socket_.Fd());
    listen_channel_->SetReadCallBack(std::bind(&Acceptor::HandleRead, this));
}

void Acceptor::SetNewConnectionCallback(const NewConnectionCallback &cb)
{
    accept_callback_ = cb;
    listen_channel_->EnAbleRead();
}