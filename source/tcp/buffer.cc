#include "buffer.h"

Buffer::Buffer()
    : buffer_(kCheapPrepend + kInitialSize), read_idx_(kCheapPrepend), write_idx_(kCheapPrepend)
{
}

Buffer::~Buffer()
{
}

// 返回初始化可读位置
char *Buffer::begin()
{
    return &(*buffer_.begin());
}

char *Buffer::ReadPosition()
{
    return begin() + read_idx_;
}

// 返回写位置
char *Buffer::WritePosition()
{
    return begin() + write_idx_;
}

// 可读元素个数
uint64_t Buffer::ReadAbleBytes()
{
    return write_idx_ - read_idx_;
}

// 可写元素个数
uint64_t Buffer::WriteAbleBytes()
{
    return buffer_.size() - write_idx_;
}

// 头部空闲元素个数
uint64_t Buffer::HeadIdleBytes()
{
    return read_idx_ - kCheapPrepend;
}

// 尾部空闲元素个数
uint64_t Buffer::TailIdleBytes()
{
    return buffer_.size() - write_idx_;
}

void Buffer::MoveReadOffset(uint64_t len)
{
    assert(len + read_idx_ <= write_idx_);
    read_idx_ += len;
}

void Buffer::MoveWriteOffset(uint64_t len)
{
    assert(len <= TailIdleBytes());
    write_idx_ += len;
}

// 确保写空间足够
void Buffer::EnsureWriteSpace(uint64_t len)
{
    if (TailIdleBytes() >= len)
    {
        return;
    }
    else if (TailIdleBytes() + HeadIdleBytes() >= len)
    {
        uint64_t readsize = ReadAbleBytes();
        std::copy(ReadPosition(), WritePosition(), begin());
        read_idx_ = kCheapPrepend;
        write_idx_ = read_idx_ + readsize;
    }
    else
    {
        
        buffer_.resize(len + write_idx_);
       
    }
}

void Buffer::Write(const void *data, uint64_t len)
{
    if(len == 0) return;
    EnsureWriteSpace(len);
    const char *d = (const char *)data;
    std::copy(d, d + len, WritePosition());
    MoveWriteOffset(len);
}

void Buffer::WriteString(const std::string &str)
{
    Write(str.c_str(), str.size());
}

void Buffer::WriteBuffer(Buffer &data)
{
    Write(data.ReadPosition(), data.ReadAbleBytes());
}

void Buffer::Read(char *buf, uint64_t len)
{
    assert(len <= ReadAbleBytes());
    std::copy(ReadPosition(), ReadPosition() + len, buf);
    MoveReadOffset(len);
}

std::string Buffer::ReadAsString(uint64_t len)
{
    assert(len <= ReadAbleBytes());
    std::string str;
    str.resize(len);
    Read(&str[0], len);
    return str;
}

void Buffer::Clear()
{
    read_idx_ = write_idx_ = kCheapPrepend;
}

char* Buffer::FindCrlf()
{
    char* s = (char*)memchr(ReadPosition(), '\n', ReadAbleBytes());
    return s;
}

std::string Buffer::GetLine()
{
    char* f = FindCrlf();
    if (f == nullptr)
    {
        return "";
    }
    return ReadAsString(f - ReadPosition() + 1);
}
