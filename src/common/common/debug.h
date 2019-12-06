#pragma once

#include <SFML/System/Vector3.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <mutex>
#include <ostream>
#include <vector>

#define DEBUG_MODE_

#ifdef DEBUG_MODE_

class DebugLogger {
  public:
    static DebugLogger &get()
    {
        std::unique_lock<std::mutex> lock(mu);
        static DebugLogger logger;
        return logger;
    }

    template <typename T> 
	void log(const char* file, int line, const T &item)
    {
        std::cout << file << ":" << line << " - " << item << std::endl;
	}

    template <typename T, typename Var>
	void log(const char *file, int line, const T &title, const Var& var)
	{
        std::cout << file << ":" << line << " - " << title << " " << var
                  << std::endl;
	}

  private:
	DebugLogger() = default;
    inline static std::mutex mu;
};

#define LOGVAR(title, var)  \
	DebugLogger::get().log( __FILE__, __LINE__, title, var);

#define LOG(item) \
	DebugLogger::get().log(__FILE__, __LINE__, item);

#else

#define LOGVAR(title, var)
#define LOG(item)

#endif

template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &data)
{
    for (const auto &entry : data) {
        stream << entry << '\n';
    }
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const glm::vec3 &vec);
std::ostream &operator<<(std::ostream &stream, const glm::vec2 &vec);

std::ostream &operator<<(std::ostream &stream, const sf::Vector3i &vec);
// std::ostream &operator<<(std::ostream &stream, const ChunkPosition &vec);
