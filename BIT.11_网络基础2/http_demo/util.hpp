#include <iostream>

class Util
{
public:
    // inbuffer -> XXXX XXX XXX \r\n
    static std::string GetFirstline(std::string &inbuffer, const std::string sep)
    {
        auto sep_pos = inbuffer.find(sep);

        if (std::string::npos == sep_pos)
        {
            return "";
        }

        std::string line_string = inbuffer.substr(0, sep_pos);

        inbuffer.erase(0, sep_pos + sep.size());

        return line_string;
    }
};