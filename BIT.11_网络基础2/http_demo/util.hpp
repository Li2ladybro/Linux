#include <iostream>
#include <fstream>

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
            return "Content-Type: text/css\r\n";
        }

        else if (suffix == ".mp3")
        {
            return "Content-Type: audio/mpeg\r\n";
        }
        
        else
        {
            return "Content-Type: application/octet-stream\r\n";
        }
    }
};