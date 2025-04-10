#pragma once

#include <string>

namespace sl
{

struct TagComponent
{
    void Reset()
    {
        m_name = "New Entity";
    }

    std::string m_name;
};

} // namespace sl
