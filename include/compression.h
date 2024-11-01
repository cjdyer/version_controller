#ifndef __COMPRESSION_H__
#define __COMPRESSION_H__

#include <cstdint>
#include <string>
#include <vector>

class Compression
{
   public:
    virtual std::vector<uint8_t> compress(const std::string &data) = 0;
    virtual std::string decompress(const std::vector<uint8_t> &data) = 0;
};

class LZ77Compression : public Compression
{
   public:
    std::vector<uint8_t> compress(const std::string &data) override;
    std::string decompress(const std::vector<uint8_t> &data) override;

   private:
    struct LZ77Token {
        uint8_t offset;
        uint8_t length;
        uint8_t next;
    };

    std::vector<LZ77Token> token_compress(const std::string &input);
    std::string token_decompress(const std::vector<LZ77Token> &tokens);

    static constexpr uint8_t WINDOW_SIZE = 255;
    static constexpr uint8_t BUFFER_SIZE = 15;
};

class RLECompression : public Compression
{
   public:
    std::vector<uint8_t> compress(const std::string &data) override;
    std::string decompress(const std::vector<uint8_t> &data) override;
};

#endif  // __COMPRESSION_H__