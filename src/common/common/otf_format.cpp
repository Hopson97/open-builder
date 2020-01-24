#include "otf_format.h"

int compress(std::istream &src, std::ostream &dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    do {
        src.read(reinterpret_cast<char *>(in), CHUNK);
        strm.avail_in = src.gcount();
        if (src.bad()) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = src.eof() ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);
            assert(ret != Z_STREAM_ERROR);
            have = CHUNK - strm.avail_out;
            dest.write(reinterpret_cast<char *>(out), have);
            if (dest.bad()) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);
    (void)deflateEnd(&strm);
    return Z_OK;
}

int decompress(std::istream &src, std::ostream &dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;
    do {
        src.read(reinterpret_cast<char *>(in), CHUNK);
        strm.avail_in = src.gcount();
        if (src.bad()) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);
            switch (ret) {
                case Z_NEED_DICT:
                    ret = Z_DATA_ERROR;
                    [[fallthrough]];
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    (void)inflateEnd(&strm);
                    return ret;
            }
            have = CHUNK - strm.avail_out;
            dest.write(reinterpret_cast<char *>(out), have);
            if (dest.bad()) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
    } while (ret != Z_STREAM_END);
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void zerr(int ret)
{
    switch (ret) {
        case Z_STREAM_ERROR:
            std::cerr << "invalid compression level" << std::endl;
            break;
        case Z_DATA_ERROR:
            std::cerr << "invalid or incomplete deflate data" << std::endl;
            break;
        case Z_MEM_ERROR:
            std::cerr << "out of memory" << std::endl;
            break;
        case Z_VERSION_ERROR:
            std::cerr << "zlib version mismatch!" << std::endl;
            break;
    }
}

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
    uint64_t value = (uint64_t)stream.get() << 56;
    value |= (uint64_t)stream.get() << 48;
    value |= (uint64_t)stream.get() << 40;
    value |= (uint64_t)stream.get() << 32;
    value |= (uint64_t)stream.get() << 24;
    value |= (uint64_t)stream.get() << 16;
    value |= (uint64_t)stream.get() << 8;
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
    char *data = new char[size];
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

std::ostream &operator<<(std::ostream &str, const Tag *v)
{
    Tag *tag = const_cast<Tag *>(v);
    const ShortTag *kv = dynamic_cast<ShortTag *>(tag);
    if (kv != nullptr) {
        str << kv;
        return str;
    }
    const IntTag *iv = dynamic_cast<IntTag *>(tag);
    if (iv != nullptr) {
        str << iv;
        return str;
    }
    const LongTag *lv = dynamic_cast<LongTag *>(tag);
    if (lv != nullptr) {
        str << lv;
        return str;
    }
    const FloatTag *fv = dynamic_cast<FloatTag *>(tag);
    if (fv != nullptr) {
        str << fv;
        return str;
    }
    const DoubleTag *dv = dynamic_cast<DoubleTag *>(tag);
    if (dv != nullptr) {
        str << dv;
        return str;
    }
    const StringTag *sv = dynamic_cast<StringTag *>(tag);
    if (sv != nullptr) {
        str << sv;
        return str;
    }
    const VectorTag *vv = dynamic_cast<VectorTag *>(tag);
    if (vv != nullptr) {
        str << vv;
        return str;
    }
    const MapTag *mv = dynamic_cast<MapTag *>(tag);
    if (mv != nullptr) {
        str << mv;
        return str;
    }
    return str;
}

Tag::~Tag()
{
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

std::ostream &operator<<(std::ostream &str, const ShortTag *v)
{
    str << v->value;
    return str;
}

ShortTag::~ShortTag()
{
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

std::ostream &operator<<(std::ostream &str, const IntTag *v)
{
    str << v->value;
    return str;
}

IntTag::~IntTag()
{
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

std::ostream &operator<<(std::ostream &str, const LongTag *v)
{
    str << v->value;
    return str;
}

LongTag::~LongTag()
{
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

std::ostream &operator<<(std::ostream &str, const FloatTag *v)
{
    str << v->value;
    return str;
}

FloatTag::~FloatTag()
{
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

std::ostream &operator<<(std::ostream &str, const DoubleTag *v)
{
    str << v->value;
    return str;
}

DoubleTag::~DoubleTag()
{
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

std::ostream &operator<<(std::ostream &str, const StringTag *v)
{
    str << "\"" << v->value << "\"";
    return str;
}

StringTag::~StringTag()
{
}

Tag *MapTag::get(std::string key)
{
    return this->data[key];
}

void MapTag::set(std::string key, Tag *value)
{
    this->data[key] = value;
}

uint16_t MapTag::getShort(std::string key)
{
    ShortTag *tag = dynamic_cast<ShortTag *>(this->get(key));
    if (tag != nullptr) {
        return tag->get();
    }
    else {
        throw "Unexpected tag type, expected ShortTag";
    }
}

uint32_t MapTag::getInt(std::string key)
{
    IntTag *tag = dynamic_cast<IntTag *>(this->get(key));
    if (tag != nullptr) {
        return tag->get();
    }
    else {
        throw "Unexpected tag type, expected IntTag";
    }
}

uint64_t MapTag::getLong(std::string key)
{
    LongTag *tag = dynamic_cast<LongTag *>(this->get(key));
    if (tag != nullptr) {
        return tag->get();
    }
    else {
        throw "Unexpected tag type, expected LongTag";
    }
}

float MapTag::getFloat(std::string key)
{
    FloatTag *tag = dynamic_cast<FloatTag *>(this->get(key));
    if (tag != nullptr) {
        return tag->get();
    }
    else {
        throw "Unexpected tag type, expected FloatTag";
    }
}

double MapTag::getDouble(std::string key)
{
    DoubleTag *tag = dynamic_cast<DoubleTag *>(this->get(key));
    if (tag != nullptr) {
        return tag->get();
    }
    else {
        throw "Unexpected tag type, expected DoubleTag";
    }
}

std::string MapTag::getString(std::string key)
{
    StringTag *tag = dynamic_cast<StringTag *>(this->get(key));
    if (tag != nullptr) {
        return tag->get();
    }
    else {
        throw "Unexpected tag type, expected StringTag";
    }
}

std::vector<Tag *> MapTag::getVector(std::string key)
{
    VectorTag *tag = dynamic_cast<VectorTag *>(this->get(key));
    if (tag != nullptr) {
        return tag->get();
    }
    else {
        throw "Unexpected tag type, expected VectorTag";
    }
}

MapTag *MapTag::getMap(std::string key)
{
    MapTag *tag = dynamic_cast<MapTag *>(this->get(key));
    if (tag != nullptr) {
        return tag;
    }
    else {
        throw "Unexpected tag type, expected MapTag";
    }
}

void MapTag::setShort(std::string key, uint16_t value)
{
    this->set(key, new ShortTag(value));
}

void MapTag::setInt(std::string key, uint32_t value)
{
    this->set(key, new IntTag(value));
}

void MapTag::setLong(std::string key, uint64_t value)
{
    this->set(key, new LongTag(value));
}

void MapTag::setFloat(std::string key, float value)
{
    this->set(key, new FloatTag(value));
}

void MapTag::setDouble(std::string key, double value)
{
    this->set(key, new DoubleTag(value));
}

void MapTag::setString(std::string key, std::string value)
{
    this->set(key, new StringTag(value));
}

void MapTag::setVector(std::string key, std::vector<Tag *> value)
{
    this->set(key, new VectorTag(value));
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

std::ostream &operator<<(std::ostream &str, const MapTag *v)
{
    str << "{ ";
    bool first = true;
    for (auto pair : v->data) {
        if (!first) {
            str << ", ";
        }
        str << "\"" << pair.first << "\"=" << pair.second;
        first = false;
    }
    str << " }";
    return str;
}

MapTag::~MapTag()
{
    for (auto pair : this->data) {
        delete pair.second;
    }
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

std::ostream &operator<<(std::ostream &str, const VectorTag *v)
{
    str << "[ " << v->data[0];
    for (uint32_t i = 1; i < v->data.size(); i++) {
        str << ", " << v->data[i];
    }
    str << " ]";
    return str;
}

VectorTag::~VectorTag()
{
    for (Tag *tag : this->data) {
        delete tag;
    }
}

Tag *readFile(std::string filename, bool compresion)
{
    std::ifstream file(filename, std::ios::binary);
    Tag *tag;
    if (!compresion) {
        tag = Tag::read(file);
    }
    else {
        std::stringstream stream;
        if (decompress(file, stream) != Z_OK) {
            abort();
        }

        stream.seekg(0, std::ios::beg);
        tag = Tag::read(stream);
    }
    file.close();
    return tag;
}

void writeFile(std::string filename, Tag *tag, bool compresion)
{
    std::ofstream file(filename, std::ios::binary);
    if (!compresion) {
        tag->write(file);
    }
    else {
        std::stringstream stream;
        tag->write(stream);

        stream.seekg(0, std::ios::beg);
        if (compress(stream, file, Z_DEFAULT_COMPRESSION) != Z_OK) {
            abort();
        }
    }
    file.close();
}