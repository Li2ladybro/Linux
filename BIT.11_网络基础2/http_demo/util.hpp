#include <iostream>
#include <fstream>
#include <string>

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

    static bool ReadFile(std::string resource, std::string *body)
    {
        std::ifstream resc(resource, std::ios::in);
        std::string content;
        if (resc.is_open() == false)
        {
            return false;
        }

        while (std::getline(resc, content))
        {
            *body += content;
        }
        resc.close();
        return true;
    }

    static std::string SuffixToHeaderType(const std::string &suffix)
    {
        if (suffix == ".html")
        {
            return "Content-Type: text/html\r\n";
        }

        else if (suffix == ".png")
        {
            return "Content-Type: image/png\r\n";
        }

        else if (suffix == ".jpg")
        {
            return "Content-Type: image/jpeg\r\n";
        }

        else if (suffix == ".mp4")
        {
            return "Content-Type: video/mp4\r\n";
        }

        else if (suffix == ".mp3")
        {
            return "Content-Type: audio/mpeg\r\n";
        }

        else
        {
            return "Content-Type: text/html\r\n";
        }
    }

    static inline int hex2int(char c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;
        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;
        return -1;
    }

    static std::string UrlDecode(const std::string &src)
    {
        std::string dst;
        for (size_t i = 0; i < src.size(); ++i)
        {
            if (src[i] == '%' && i + 2 < src.size())
            {
                int h1 = hex2int(src[i + 1]);
                int h2 = hex2int(src[i + 2]);
                if (h1 != -1 && h2 != -1)
                {
                    dst.push_back(static_cast<char>(h1 * 16 + h2));
                    i += 2;
                    continue;
                }
            }
            else if (src[i] == '+')
            {
                dst.push_back(' ');
                continue;
            }
            dst.push_back(src[i]);
        }
        return dst;
    }
};