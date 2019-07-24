#ifndef __HIKINGSIMULATOREXCEPTION_H__
#define __HIKINGSIMULATOREXCEPTION_H__

#include <exception>
#include <string>

#ifdef _WIN32
#ifdef SIMULATOR_EXPORTS
#define SIMULATOR_API __declspec(dllexport)
#else
#define SIMULATOR_API __declspec(dllimport)
#endif
#else
#define SIMULATOR_API
#endif

class SIMULATOR_API BadConfigException : public std::exception
{
public:
    BadConfigException(const char*);
    virtual char const * what() const;
private:
    std::string msg_;
};

class SIMULATOR_API BadParameterException : public std::exception
{
public:
    BadParameterException(const char*);
    virtual char const * what() const;
private:
    std::string msg_;
};


#endif //__HIKINGSIMULATOREXCEPTION_H__
