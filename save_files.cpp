#include "save_files.h"
#include <fstream>
#include <iostream>

void writeShort(std::ostream &stream, uint16_t value)
{
    stream.put((value >> 8) & 0xFF);
    stream.put(value & 0xFF);
}

void writeInt(std::ostream &stream, uint32_t value)
{
    stream.put((value >> 24) & 0xFF);
    stream.put((value >> 16) & 0xFF);
    stream.put((value >> 8) & 0xFF);
    stream.put(value & 0xFF);
}

void writeLong(std::ostream &stream, uint64_t value)
{
    stream.put((value >> 56) & 0xFF);
    stream.put((value >> 48) & 0xFF);
    stream.put((value >> 40) & 0xFF);
    stream.put((value >> 32) & 0xFF);
    stream.put((value >> 24) & 0xFF);
    stream.put((value >> 16) & 0xFF);
    stream.put((value >> 8) & 0xFF);
    stream.put(value & 0xFF);
}

void writeFloat(std::ostream &stream, float value)
{
    uint32_t *data = reinterpret_cast<uint32_t *>(&value);
    writeInt(stream, *data);
}

void writeDouble(std::ostream &stream, double value)
{
    uint64_t *data = reinterpret_cast<uint64_t *>(&value);
    writeLong(stream, *data);
}

uint16_t readShort(std::istream &stream)
{
    uint16_t value = stream.get() << 8;
    value |= stream.get();
    return value;
}

uint32_t readInt(std::istream &stream)
{
    uint32_t value = stream.get() << 24;
    value |= stream.get() << 16;
    value |= stream.get() << 8;
    value |= stream.get();
    return value;
}

uint64_t readLong(std::istream &stream)
{
    uint64_t value = (uint64_t) stream.get() << 56;
    value |= (uint64_t) stream.get() << 48;
    value |= (uint64_t) stream.get() << 40;
    value |= (uint64_t) stream.get() << 32;
    value |= (uint64_t) stream.get() << 24;
    value |= (uint64_t) stream.get() << 16;
    value |= (uint64_t) stream.get() << 8;
    value |= stream.get();
    return value;
}

float readFloat(std::istream &stream)
{
    uint32_t data = readInt(stream);
    return *reinterpret_cast<float *>(&data);
}

double readDouble(std::istream &stream)
{
    uint64_t data = readLong(stream);
    return *reinterpret_cast<double *>(&data);
}

void writeString(std::ostream &stream, std::string value)
{
    int size = value.size() + 1;
    writeInt(stream, size);
    stream.write(value.c_str(), size);
}

std::string readString(std::istream &stream)
{
    uint32_t size = readInt(stream);
    std::cout << size << std::endl;
    char data[size];
    stream.read(data, size);

    std::string result;
    result += data;

    return result;
}

Tag *Tag::read(std::istream &stream)
{
    char ident = stream.get();
    switch (ident) {
        case 'I':
            return IntTag::read(stream);
        case 'K':
            return ShortTag::read(stream);
        case 'J':
            return LongTag::read(stream);
        case 'F':
            return FloatTag::read(stream);
        case 'D':
            return DoubleTag::read(stream);
        case 'S':
            return StringTag::read(stream);
        case 'M':
            return MapTag::read(stream);
        case 'V':
            return VectorTag::read(stream);
        default:
            return nullptr;
    }
}

ShortTag::ShortTag(uint16_t in_value)
{
    this->value = in_value;
}

void ShortTag::write(std::ostream &stream)
{
    stream.put('K');
    writeShort(stream, this->value);
}

Tag *ShortTag::read(std::istream &stream)
{
    return new ShortTag(readShort(stream));
}

uint16_t ShortTag::get()
{
    return this->value;
}

IntTag::IntTag(uint32_t in_value)
{
    this->value = in_value;
}

void IntTag::write(std::ostream &stream)
{
    stream.put('I');
    writeInt(stream, this->value);
}

Tag *IntTag::read(std::istream &stream)
{
    return new IntTag(readInt(stream));
}

uint32_t IntTag::get()
{
    return this->value;
}

LongTag::LongTag(uint64_t in_value)
{
    this->value = in_value;
}

void LongTag::write(std::ostream &stream)
{
    stream.put('J');
    writeLong(stream, this->value);
}

Tag *LongTag::read(std::istream &stream)
{
    return new LongTag(readLong(stream));
}

uint64_t LongTag::get()
{
    return this->value;
}

FloatTag::FloatTag(float in_value)
{
    this->value = in_value;
}

void FloatTag::write(std::ostream &stream)
{
    stream.put('F');
    writeFloat(stream, this->value);
}

Tag *FloatTag::read(std::istream &stream)
{
    return new FloatTag(readFloat(stream));
}

float FloatTag::get()
{
    return this->value;
}

DoubleTag::DoubleTag(double in_value)
{
    this->value = in_value;
}

void DoubleTag::write(std::ostream &stream)
{
    stream.put('D');
    writeDouble(stream, this->value);
}

Tag *DoubleTag::read(std::istream &stream)
{
    return new DoubleTag(readDouble(stream));
}

double DoubleTag::get()
{
    return this->value;
}

StringTag::StringTag(std::string in_value)
{
    this->value = in_value;
}

void StringTag::write(std::ostream &stream)
{
    stream.put('S');
    writeString(stream, this->value);
}

Tag *StringTag::read(std::istream &stream)
{
    return new StringTag(readString(stream));
}

std::string StringTag::get()
{
    return this->value;
}

Tag *MapTag::get(std::string key)
{
    return this->data[key];
}

void MapTag::set(std::string key, Tag *value)
{
    this->data[key] = value;
}

void MapTag::write(std::ostream &stream)
{
    stream.put('M');
    writeInt(stream, this->data.size());
    for (auto key : this->data) {
        writeString(stream, key.first);
        key.second->write(stream);
    }
}

Tag *MapTag::read(std::istream &stream)
{
    MapTag *tag = new MapTag();
    int size = readInt(stream);
    for (int i = 0; i < size; i++) {
        std::string key = readString(stream);
        Tag *value = Tag::read(stream);
        tag->set(key, value);
    }
    return tag;
}

VectorTag::VectorTag(std::vector<Tag *> values)
{
    this->data = values;
}

void VectorTag::write(std::ostream &stream)
{
    stream.put('V');
    writeInt(stream, this->data.size());
    for (auto tag : this->data) {
        tag->write(stream);
    }
}

Tag *VectorTag::read(std::istream &stream)
{
    int size = readInt(stream);
    std::vector<Tag *> tags(size);
    for (int i = 0; i < size; i++) {
        Tag *value = Tag::read(stream);
        tags[i] = value;
    }
    return new VectorTag(tags);
}

std::vector<Tag *> VectorTag::get()
{
    return this->data;
}

int main(int argc, char const *argv[])
{
    Tag *tag = new VectorTag(std::vector<Tag *>{new ShortTag((1 << 15) - 1),
                                                new IntTag((1 << 31) - 1),
                                                new LongTag(((uint64_t) 1 << 63) - 1),
                                                new FloatTag(0.367485),
                                                new DoubleTag(9243875602746.4646)});

    std::ofstream my_file("test.tag", std::ios::binary);
    tag->write(my_file);
    my_file.close();

    std::ifstream my_file2("test.tag", std::ios::binary);
    Tag *tag2 = Tag::read(my_file2);
    for (auto tag : ((VectorTag *)tag2)->get()) {
        std::cout << ((LongTag *)tag)->get() << std::endl;
    }
    my_file2.close();
    return 0;
}
