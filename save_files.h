#pragma once

#include <fstream>
#include <map>
#include <string>
#include <vector>

/**
 * The save file format is similar to json (key-value structure), but gets
 *stored in a binary format (which can be compressed)
 **/

class Tag {
  public:
    virtual void write(std::ostream &stream) = 0;
    static Tag *read(std::istream &stream);
};

class ShortTag : public Tag {
  public:
    ShortTag(uint16_t value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    uint16_t get();

  private:
    uint16_t value;
};

class IntTag : public Tag {
  public:
    IntTag(uint32_t value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    uint32_t get();

  private:
    uint32_t value;
};

class LongTag : public Tag {
  public:
    LongTag(uint64_t value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    uint64_t get();

  private:
    uint64_t value;
};

class FloatTag : public Tag {
  public:
    FloatTag(float value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    float get();

  private:
    float value;
};

class DoubleTag : public Tag {
  public:
    DoubleTag(double value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    double get();

  private:
    double value;
};

class StringTag : public Tag {
  public:
    StringTag(std::string value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    std::string get();

  private:
    std::string value;
};

class MapTag : public Tag {
  public:
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    void set(std::string key, Tag *value);
    Tag *get(std::string key);

  private:
    std::map<std::string, Tag *> data;
};

class VectorTag : public Tag {
  public:
    VectorTag(std::vector<Tag *> value);
    void write(std::ostream &stream);
    static Tag *read(std::istream &stream);
    std::vector<Tag *> get();

  private:
    std::vector<Tag *> data;
};

Tag *readFile(std::string filename)
{
    std::ifstream file(filename, std::ios::binary);
    Tag *tag = Tag::read(file);
    file.close();
    return tag;
}

void readFile(std::string filename, Tag *tag)
{
    std::ofstream file(filename, std::ios::binary);
    tag->write(file);
    file.close();
}