#pragma once

#include <SFML/System/Vector3.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <mutex>
#include <ostream>
#include <vector>

#define DEBUG_MODE_

#ifdef DEBUG_MODE_

class DebugLogger final {
  public:
    static DebugLogger& get()
    {
        static DebugLogger logger;
        return logger;
    }

    /*
    template <typename T> void log(const char *file, int line, const T &item)
    {
        std::cout << file << ": " << line << " - " << item << std::endl;
    }*/

    template <typename T>
    void log(const char* where, const T& item)
    {
        std::cout << where << ":\n" << item << "\n\n";
    }

    template <typename T, typename Var>
    void log(const char* where, const T& title, const Var& var)
    {
        std::cout << where << ":\n" << title << " " << var << "\n\n";
    }

  private:
    DebugLogger() = default;
};

#define LOGVAR(where, title, var) DebugLogger::get().log(where, title, var);
/*
#define LOGLINE(title, var)   \
    DebugLogger::get().log(where, title, var);
*/
#define LOG(where, item) DebugLogger::get().log(where, item);

#else

#define LOGVAR(where, title, var)
#define LOGLINE(title, var)
#define LOG(where, item)

#endif

template <typename T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& data)
{
    for (const auto& entry : data) {
        stream << entry << '\n';
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const glm::vec3& vec);
std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec);

std::ostream& operator<<(std::ostream& stream, const sf::Vector3i& vec);
// std::ostream &operator<<(std::ostream &stream, const ChunkPosition &vec);
