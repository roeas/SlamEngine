#pragma once

#include "Core/Log.h"

#include <nameof/nameof.hpp>

#include <concepts>
#include <filesystem>
#include <fstream>
#include <optional>
#include <span>
#include <string>
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

    static std::string ReadString(std::string_view path)
    {
        std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", path.data());
            return "";
        }

        std::string content;
        content.resize(file.tellg());

        file.seekg(0, std::ios::beg);
        file.read(content.data(), content.size());
        if (!file)
        {
            SL_LOG_ERROR("Failed to read file \"{}\"", path.data());
            return "";
        }

        return content;
    }

    static void WriteString(std::string_view path, std::string_view content, bool append = false)
    {
        auto mode = std::ios::binary | (append ? std::ios::app : std::ios::trunc);
        std::ofstream file(path.data(), mode);
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", path.data());
            return;
        }

        file.write(content.data(), content.size());
        if (!file)
        {
            SL_LOG_ERROR("Failed to write file \"{}\"", path.data());
        }
    }

    template<typename T = uint8_t> requires std::is_trivial_v<T>
    static std::vector<T> ReadBinary(std::string_view path)
    {
        std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", path.data());
            return {};
        }

        size_t fileSize = file.tellg();
        if (fileSize % sizeof(T) != 0)
        {
            SL_LOG_ERROR("File size of {} is not divisible by sizeof {} ({})", path.data(), nameof::nameof_type<T>(), sizeof(T));
            return {};
        }

        std::vector<T> buffer(fileSize / sizeof(T));
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char *>(buffer.data()), fileSize);
        if (!file)
        {
            SL_LOG_ERROR("Failed to read file \"{}\"", path.data());
            return {};
        }

        return buffer;
    }

    static std::tuple<void *, size_t> ReadBinaryRaw(std::string_view path)
    {
        std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", path.data());
            return { nullptr, 0 };
        }

        size_t fileSize = file.tellg();
        char *pData = new char[fileSize];

        file.seekg(0, std::ios::beg);
        file.read(pData, fileSize);
        if (!file)
        {
            SL_LOG_ERROR("Failed to read file \"{}\"", path.data());
            delete[] pData;
            return { nullptr, 0 };
        }

        return { pData, fileSize };
    }

    template<typename T = uint8_t> requires std::is_trivial_v<T>
    static void WriteBinary(std::string_view path, std::span<const T> data, bool append = false)
    {
        auto mode = std::ios::binary | (append ? std::ios::app : std::ios::trunc);
        std::ofstream file(path.data(), mode);
        if (!file.is_open())
        {
            SL_LOG_ERROR("Failed to open file \"{}\"", path.data());
            return;
        }

        file.write(reinterpret_cast<const char *>(data.data()), data.size_bytes());
        if (!file)
        {
            SL_LOG_ERROR("Failed to write file \"{}\"", path.data());
        }
    }
};

} // namespace sl
