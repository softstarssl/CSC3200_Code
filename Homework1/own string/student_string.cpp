#include"student_string.h"

namespace
{
    char is_alpha(char c) {return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')); }

    char low_to_up(char c)
    {
        if(c >= 'a' && c <= 'z') return (char)(c + 'A' - 'a');
        return c;
    }

    char up_to_low(char c)
    {
        if(c >= 'A' && c <= 'Z') return (char)(c + 'a' - 'A');
        return c;
    }

    int copy_cstr(char* tmp,const char* need)
    {
        if(tmp == nullptr) return 0;
        int i = 0;
        if(need)
        {
            while(i < MAXLEN - 1 && need[i] != '\0')
            {
                tmp[i] = need[i];
                ++i;
            }
        }
        tmp[i] = '\0';
        return i;
    }
}
namespace student_std
{
    string::string() : strlen(0) { c_str[0] = '\0'; } //初始化
    string::string(const char* str) //C风格的建立字符串
    {
        strlen = copy_cstr(c_str,str); //用复制的办法来实现初始化
    }

    string::string(string const& other) // 深拷贝 other 的数据到本对象
    {
        //c++风格建立字符串
        int n = other.strlen;
        if(n > MAXLEN - 1) n = MAXLEN - 1; //防止溢出,-1是因为最后一个是\0

        for(int i = 0;i < n;++i) c_str[i] = other.c_str[i];

        strlen = n;
        c_str[strlen] = '\0';
    }

    string::~string() {} // 无动态资源需要释放（固定数组随对象自动销毁）

    int string::get_strlen() const { return strlen; }
    const char* string::get_c_str() const {return c_str; }
    
    void string::strcat(string const& other) //在末尾添加other
    {
        int space = MAXLEN - 1 - strlen; //剩余多少个空字符可以加上去
        int num = other.strlen; //需要拷贝几个
        if(num > space) num = space;

        for(int i = 0;i < num;++i) c_str[i + strlen] = other.c_str[i];
        strlen += num;
        c_str[strlen] = '\0';
    }

    void string::strcopy(string const& other)
    {
        int n = other.strlen;
        if(n > MAXLEN - 1) n = MAXLEN - 1;//防止溢出

        for(int i = 0;i < n;++i) c_str[i] = other.c_str[i];

        strlen = n;
        c_str[strlen] = '\0';
    }

    string& string::operator=(string const& other)
    {
        if(this == &other) return *this;
        this -> strcopy(other); //这个函数在后面的功能函数中可以实现
        return *this;
    }

    string& string::operator+=(string const& other)
    {
        this->strcat(other);
        return *this;
    }

    char& string::operator[](int id)
    {
        if(strlen == 0) return c_str[0];
        if(id < 0) id = 0;
        if(id >= strlen) id = strlen - 1;
        return c_str[id];
    }

    const char& string::operator[](int id) const
    {
         if(strlen == 0) return c_str[0];
        if(id < 0) id = 0;
        if(id >= strlen) id = strlen - 1;
        return c_str[id];
    }

    void string::to_upper()
    {
        for(int i = 0;i < strlen;++i) c_str[i] = low_to_up(c_str[i]);
    }

    void string::to_lower()
    {
        for(int i = 0;i < strlen;++i) c_str[i] = up_to_low(c_str[i]);
    }

    bool string::equals(string const& other) const
    {
        if(strlen != other.strlen) return false;
        for(int i = 0;i < other.strlen;++i)
        {
            if(c_str[i] != other.c_str[i]) return false;
        }
        return true;
    }

    bool string::equals_ignore_case(string const& other) const
    {
        if (strlen != other.strlen) return false;

        for (int i = 0; i < strlen; ++i) 
        {
            char a = c_str[i];
            char b = other.c_str[i];

            // 仅当两者都是字母时忽略大小写；
            // 只要有一个不是字母，就必须严格相等。
            if (is_alpha(a) && is_alpha(b)) 
            {
                if (up_to_low(a) != up_to_low(b)) return false;
            }
            else
            {
                if (a != b) return false;
            }
        }
        return true;
    }
    void string::trim()
    {
        int l = 0,r = strlen - 1;
        while(l < strlen && c_str[l] == ' ') ++l;
        while(r >= l && c_str[r] == ' ') --r;

        int nowlen = r - l + 1;
        if(nowlen < 0) nowlen = 0;

        for(int i = 0;i < nowlen;++i) c_str[i]  = c_str[l + i];
        strlen = nowlen;
        c_str[strlen] = '\0'; 
    }

    bool string::is_alphabetic() const
    {
        for(int i = 0;i < strlen;++i)
        {
            if(!is_alpha(c_str[i])) return false;
        }
        return true;
    }
} // namespace student_std
