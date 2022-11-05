#pragma once

#include <exception>
#include <sstream>
#include <string>

class Error : public std::exception {
public:
    const char* what() const noexcept
    {
        return _message.c_str();
    }

    template <class T>
    friend Error& operator<<(Error& error, const T& value)
    {
        auto stream = std::ostringstream{error._message};
        stream << value;
        error._message = stream.str();
        return error;
    }

    template <class T>
    friend Error operator<<(Error&& error, const T& value)
    {
        auto stream = std::ostringstream{error._message};
        stream << value;
        error._message = stream.str();
        return error;
    }

private:
    std::string _message;
};
