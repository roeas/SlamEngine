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

    static std::optional<std::string> ReadString(std::string_view path)
    {
        std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            return std::nullopt;
        }

        std::string content;
        content.resize(file.tellg());

        file.seekg(0, std::ios::beg);
        file.read(content.data(), content.size());
        if (!file)
        {
            return std::nullopt;
        }

        return content;
    }

    static bool WriteString(std::string_view path, std::string_view content, bool append = false)
    {
        auto mode = std::ios::binary | (append ? std::ios::app : std::ios::trunc);
        std::ofstream file(path.data(), mode);
        if (!file.is_open())
        {
            return false;
        }

        file.write(content.data(), content.size());
        return file.good();
    }

    template<typename T = uint8_t> requires std::is_trivial_v<T>
    static std::optional<std::vector<T>> ReadBinary(std::string_view path)
    {
        std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            return std::nullopt;
        }

        size_t fileSize = file.tellg();
        if (fileSize % sizeof(T) != 0)
        {
            SL_LOG_ERROR("File size of {} is not divisible by sizeof {} ({})", path.data(), nameof::nameof_type<T>(), sizeof(T));
            return std::nullopt;
        }

        std::vector<T> buffer(fileSize / sizeof(T));
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char *>(buffer.data()), fileSize);
        if (!file)
        {
            return std::nullopt;
        }

        return buffer;
    }

    static std::tuple<void *, size_t> ReadBinaryRaw(std::string_view path)
    {
        std::ifstream file(path.data(), std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            return { nullptr, 0 };
        }

        size_t fileSize = file.tellg();
        char *pData = new char[fileSize];

        file.seekg(0, std::ios::beg);
        file.read(pData, fileSize);
        if (!file)
        {
            delete[] pData;
            return { nullptr, 0 };
        }

        return { pData, fileSize };
    }

    template<typename T = uint8_t> requires std::is_trivial_v<T>
    static bool WriteBinary(std::string_view path, std::span<const T> data, bool append = false)
    {
        auto mode = std::ios::binary | (append ? std::ios::app : std::ios::trunc);
        std::ofstream file(path.data(), mode);
        if (!file.is_open())
        {
            return false;
        }

        file.write(reinterpret_cast<const char *>(data.data()), data.size_bytes());
        return file.good();
    }
};

} // namespace sl
