#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>


template <typename T> std::string toStr(T tmp)
{
    std::stringstream stream;
    stream << tmp;

    std::string str;
    stream >> str;

    return str;
}

class Logger
{
    enum Mode { ASSERT, ERROR, LOG, INFO, DEBUG, VERBOSE, WARN };

    mutable bool space;

    const Logger& log(const Mode& mode, const std::string& tag = "")
    {
        space = true;
        *this << e2s(mode) << std::string("\t") << tag.substr(0, 6) << std::string("\t");
        return *this;
    }

public:
    Logger() : space(true) {}

    inline const Logger& i(const std::string& tag = "")
    {
        return log(INFO, tag);
    }

    inline const Logger& e(const std::string& tag = "")
    {
        return log(ERROR, tag);
    }

    inline const Logger& a(const std::string& tag = "")
    {
        return log(ASSERT, tag);
    }

    inline const Logger& l(const std::string& tag = "")
    {
        return log(LOG, tag);
    }

    inline const Logger& d(const std::string& tag = "")
    {
        return log(DEBUG, tag);
    }

    inline const Logger& v(const std::string& tag = "")
    {
        return log(VERBOSE, tag);
    }

    inline const Logger& w(const std::string& tag = "")
    {
        return log(WARN, tag);
    }

    inline friend
    const Logger& operator<<(const Logger& logger, const char* message)
    {
        return (logger << std::string(message));
    }

    inline friend
    const Logger& operator<<(const Logger& logger, const std::string& message)
    {
        std::string _space = (message.length() == 0 ? "" : " ");

        std::cout << (logger.space ? "\n" : _space) << message;
        std::cout.flush();
        logger.space ? logger.space = false : 0;
        return logger;
    }

    template <typename T>
    inline friend const Logger& operator<<(const Logger& logger, const T& obj)
    {
//        logger << toStr(obj);
        std::stringstream ss;
        ss << obj;
        std::string str;
        ss >> str;

        logger << str;
        return logger;
    }

    static
    const Logger& newline(Logger& logger)
    {
        return logger.l() << "";
    }

    inline friend
    const Logger& operator<<(const Logger& logger, const Logger& (*__func)(Logger&))
    {
        return __func((Logger&)logger);
    }

    static const std::string e2s(const Mode& mode)
    {
        switch(mode)
        {
        case ERROR:
            return "ERROR:";
        case INFO:
            return "INFO:";
        case DEBUG:
            return "DEBUG:";
        case ASSERT:
            return "ASSER:";
        case VERBOSE:
            return "VERB:";
        case WARN:
            return "WARN:";
        case LOG:
        default:
            return "LOG:";

        }
    }
};

static Logger klog;

// usage
//friend Logger& operator<<(Logger& log, const Maniac& m)
//{
//    log.i("tag") << m.field;
//    return log;
//}

#endif // LOGGER_H
