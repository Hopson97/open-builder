#pragma once

#include <cassert>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <zlib.h>

#include <iostream>

/**
 * Open-Builder Tag Format (otf)
 * The format is similar to json (key-value structure), but gets
 * stored in a binary format (which can be compressed)
 **/

#define CHUNK 16384

class Tag {
  public:
    virtual ~Tag();
    virtual void write(std::ostream &stream) = 0;
    static Tag *read(std::istream &stream);
};

class ShortTag : public Tag {
  public:
    ~ShortTag();
    ShortTag(uint16_t value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    uint16_t get();
    friend std::ostream &operator<<(std::ostream &str, const ShortTag *v);

  private:
    uint16_t value;
};

class IntTag : public Tag {
  public:
    ~IntTag();
    IntTag(uint32_t value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    uint32_t get();
    friend std::ostream &operator<<(std::ostream &str, const IntTag *v);

  private:
    uint32_t value;
};

class LongTag : public Tag {
  public:
    ~LongTag();
    LongTag(uint64_t value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    uint64_t get();
    friend std::ostream &operator<<(std::ostream &str, const LongTag *v);

  private:
    uint64_t value;
};

class FloatTag : public Tag {
  public:
    ~FloatTag();
    FloatTag(float value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    float get();
    friend std::ostream &operator<<(std::ostream &str, const FloatTag *v);

  private:
    float value;
};

class DoubleTag : public Tag {
  public:
    ~DoubleTag();
    DoubleTag(double value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    double get();
    friend std::ostream &operator<<(std::ostream &str, const DoubleTag *v);

  private:
    double value;
};

class StringTag : public Tag {
  public:
    ~StringTag();
    StringTag(std::string value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    std::string get();
    friend std::ostream &operator<<(std::ostream &str, const StringTag *v);

  private:
    std::string value;
};

class MapTag : public Tag {
  public:
    ~MapTag();
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    void set(std::string key, Tag *value);
    Tag *get(std::string key);
    uint16_t getShort(std::string key);
    uint32_t getInt(std::string key);
    uint64_t getLong(std::string key);
    float getFloat(std::string key);
    double getDouble(std::string key);
    std::string getString(std::string key);
    std::vector<Tag *> getVector(std::string key);
    MapTag *getMap(std::string key);

    void setShort(std::string key, uint16_t value);
    void setInt(std::string key, uint32_t value);
    void setLong(std::string key, uint64_t value);
    void setFloat(std::string key, float value);
    void setDouble(std::string key, double value);
    void setString(std::string key, std::string value);
    void setVector(std::string key, std::vector<Tag *> value);
    friend std::ostream &operator<<(std::ostream &str, const MapTag *v);

  private:
    std::map<std::string, Tag *> data;
};

class VectorTag : public Tag {
  public:
    ~VectorTag();
    VectorTag(std::vector<Tag *> value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    std::vector<Tag *> get();
    friend std::ostream &operator<<(std::ostream &str, const VectorTag *v);

  private:
    std::vector<Tag *> data;
};

Tag *readFile(std::string filename, bool compression = true);

void writeFile(std::string filename, Tag *tag, bool compression = true);