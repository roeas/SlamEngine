#pragma once

#include "Core/Log.h"
#include "Utils/ProfilerCPU.h"

#include <nameof/nameof.hpp>

#include <fstream>
#include <span>
#include <tuple>

namespace sl
{

class FileIO final
{
public:
    FileIO() = delete;
    FileIO(const FileIO &) = delete;
    FileIO &operator=(const FileIO &) = delete;
    FileIO(FileIO &&) = delete;
    FileIO &operator=(FileIO &&) = delete;
    ~FileIO() = delete;

    static std::string ReadString(const char *pPath)
    {
        SL_PROFILE;
        SL_PROFILE_ADD_TEXT(pPath, strlen(pPath));

        std::ifstream file(pPath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", pPath);
            return "";
        }

        std::string buffer;
        buffer.resize(file.tellg());

        file.seekg(0, std::ios::beg);
        file.read(buffer.data(), buffer.size());
        if (!file)
        {
            SL_LOG_ERROR("Failed to read file \"{}\"", pPath);
            return "";
        }

        return buffer;
    }

    static void WriteString(const char *pPath, std::string_view buffer, bool append = false)
    {
        SL_PROFILE;
        SL_PROFILE_ADD_TEXT(pPath, strlen(pPath));

        std::ofstream file(pPath, std::ios::binary | (append ? std::ios::app : std::ios::trunc));
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", pPath);
            return;
        }

        file.write(buffer.data(), buffer.size());
        if (!file)
        {
            SL_LOG_ERROR("Failed to write file \"{}\"", pPath);
        }
    }

    template<typename T = unsigned char> requires std::is_trivial_v<T>
    static std::vector<T> ReadBinary(const char *pPath)
    {
        SL_PROFILE;
        SL_PROFILE_ADD_TEXT(pPath, strlen(pPath));

        std::ifstream file(pPath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", pPath);
            return {};
        }

        size_t fileSize = file.tellg();
        if (fileSize % sizeof(T) != 0)
        {
            SL_LOG_ERROR("File size of \"{}\" is not divisible by sizeof {} ({})", pPath, nameof::nameof_type<T>(), sizeof(T));
            return {};
        }

        std::vector<T> buffer(fileSize / sizeof(T));
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char *>(buffer.data()), fileSize);
        if (!file)
        {
            SL_LOG_ERROR("Failed to read file \"{}\"", pPath);
            return {};
        }

        return buffer;
    }

    static std::tuple<void *, size_t> ReadBinaryRaw(const char *pPath)
    {
        SL_PROFILE;
        SL_PROFILE_ADD_TEXT(pPath, strlen(pPath));

        std::ifstream file(pPath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", pPath);
            return { nullptr, 0 };
        }

        size_t fileSize = file.tellg();
        char *pData = new char[fileSize];

        file.seekg(0, std::ios::beg);
        file.read(pData, fileSize);
        if (!file)
        {
            SL_LOG_ERROR("Failed to read file \"{}\"", pPath);
            delete[] pData;
            return { nullptr, 0 };
        }

        return { pData, fileSize };
    }

    template<typename T = unsigned char> requires std::is_trivial_v<T>
    static void WriteBinary(const char *pPath, std::span<const T> buffer, bool append = false)
    {
        SL_PROFILE;
        SL_PROFILE_ADD_TEXT(pPath, strlen(pPath));

        std::ofstream file(pPath, std::ios::binary | (append ? std::ios::app : std::ios::trunc));
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", pPath);
            return;
        }

        file.write(reinterpret_cast<const char *>(buffer.data()), buffer.size_bytes());
        if (!file)
        {
            SL_LOG_ERROR("Failed to write file \"{}\"", pPath);
        }
    }
};

} // namespace sl
