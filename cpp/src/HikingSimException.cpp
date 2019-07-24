#include "HikingSimException.h"

BadConfigException::BadConfigException(const char* msg)
{
    msg_.assign(msg);
}

char const * BadConfigException::what() const
{
    return msg_.c_str();
}

BadParameterException::BadParameterException(const char* msg)
{
    msg_.assign(msg);
}

char const * BadParameterException::what() const
{
    return msg_.c_str();
}

