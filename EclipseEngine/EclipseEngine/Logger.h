#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>

class Logger {
public:
    static void Init(const std::string& filename = "log.txt")
    {
        m_LogStream.open(filename, std::ios::out | std::ios::app);
    }

    static void Log(const std::string& message)
    {
        if (m_LogStream.is_open()) {
            m_LogStream << message << std::endl;
        }
        std::cout << message << std::endl; // Print to console as well
    }

    static void Close() 
    {
        if (m_LogStream.is_open()) {
            m_LogStream.close();
        }
    }

private:
    static std::ofstream m_LogStream;
};

#endif // LOGGER_H