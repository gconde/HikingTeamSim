#include "HikingSimException.h"

namespace TorchAndBridge
{

BadConfigException::BadConfigException(const char* msg)
{
    msg_.assign(msg);
}

char const * BadConfigException::what() const noexcept
{
    return msg_.c_str();
}

BadParameterException::BadParameterException(const char* msg)
{
    msg_.assign(msg);
}

char const * BadParameterException::what() const noexcept
{
    return msg_.c_str();
}

} // TorchAndBridge
