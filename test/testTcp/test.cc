#include <iostream>
#include <cstring>
#include <functional>
#include <cassert>
using namespace std;
namespace bit
{
    class string
    {
    public:
        typedef char *iterator;
        iterator begin()
        {
            return _str;
        }
        iterator end()
        {
            return _str + _size;
        }
        string(const char *str = "")
            : _size(strlen(str)), _capacity(_size)
        {
            // cout << "string(char* str)" << endl;
            cout << "string(const char *str)------构造函数" << std::endl;
            _str = new char[_capacity + 1];
            strcpy(_str, str);
        }
        // s1.swap(s2)
        void swap(string &s)
        {
            ::swap(_str, s._str);
            ::swap(_size, s._size);
            ::swap(_capacity, s._capacity);
        }
        // 拷贝构造
        string(const string &s)
            : _str(nullptr)
        {

            cout << "string(const string& s) -- 深拷贝" << endl;
            string tmp(s._str);
            swap(tmp);
        }
        // 赋值重载
        string &operator=(const string &s)
        {

            cout << "string& operator=(string s) -- 深拷贝" << endl;
            string tmp(s);
            swap(tmp);
            return *this;
        }
        // 移动构造
        string(string &&s)
            : _str(nullptr), _size(0), _capacity(0)
        {
            cout << "string(string&& s) -- 移动语义" << endl;
            swap(s);
        }
        // 移动赋值
        string &operator=(string &&s)
        {
            cout << "string& operator=(string&& s) -- 移动语义" << endl;
            swap(s);
            return *this;
        }
        ~string()
        {
            delete[] _str;
            _str = nullptr;
        }
        char &operator[](size_t pos)
        {
            assert(pos < _size);
            return _str[pos];
        }
        void reserve(size_t n)
        {
            if (n > _capacity)
            {
                char *tmp = new char[n + 1];
                strcpy(tmp, _str);
                delete[] _str;
                _str = tmp;
                _capacity = n;
            }
        }
        void push_back(char ch)
        {
            if (_size >= _capacity)
            {
                size_t newcapacity = _capacity == 0 ? 4 : _capacity * 2;
                reserve(newcapacity);
            }

            ++_size;
            _str[_size] = '\0';
        }
        // string operator+=(char ch)
        string &operator+=(char ch)
        {
            push_back(ch);
            return *this;
        }
        const char *c_str() const
        {
            return _str;
        }

    private:
        char *_str;
        size_t _size;
        size_t _capacity; // 不包含最后做标识的\0
    };
}

void SendIn(bit::string str)
{
    std::cout << str.c_str() << std::endl;
}
void Send(const char *data, int len)
{
    // bit::string str(data);
    // auto t = [str ]()
    // {
    //     SendIn(str);
    // };
    // t();

    // cout << endl
    //      << endl;

 auto s = std::bind(SendIn, std::move(bit::string(data)));
     s();
}

int main()
{
    const char *str = "今天风好大";

    Send(str, strlen(str));

    return 0;
}