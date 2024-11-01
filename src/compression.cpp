#include "compression.h"

#include <iostream>
#include <stdexcept>

std::vector<uint8_t> LZ77Compression::compress(const std::string &data)
{
    auto compressed_tokens = token_compress(data);

    std::vector<uint8_t> compressed_data;
    for (const LZ77Token &token : compressed_tokens) {
        compressed_data.push_back(token.offset);
        compressed_data.push_back(token.length);
        compressed_data.push_back(token.next);
    }

    return compressed_data;
}

std::string LZ77Compression::decompress(const std::vector<uint8_t> &data)
{
    std::vector<LZ77Token> compressed_tokens;

    for (size_t i = 0; i < data.size(); i += 3) {
        LZ77Token token;
        token.offset = data[i];
        token.length = data[i + 1];
        token.next = data[i + 2];
        compressed_tokens.push_back(token);
    }

    return token_decompress(compressed_tokens);
}

std::vector<LZ77Compression::LZ77Token> LZ77Compression::token_compress(const std::string &input)
{
    std::vector<LZ77Token> compressed;

    uint8_t i = 0;
    while (i < input.size()) {
        uint8_t best_offset = 0;
        uint8_t best_length = 0;

        uint8_t max_offset = std::min(i, WINDOW_SIZE);
        uint8_t max_length = std::min((uint8_t)(input.size() - i), BUFFER_SIZE);

        for (size_t offset = 1; offset <= max_offset; ++offset) {
            for (size_t length = 1; length <= max_length; ++length) {
                if (input.substr(i - offset, length) == input.substr(i, length) &&
                    length > best_length) {
                    best_offset = offset;
                    best_length = length;
                }
            }
        }

        LZ77Token token;
        token.offset = best_offset;
        token.length = best_length;
        token.next = i + best_length < input.size() ? input[i + best_length] : 0;

        compressed.push_back(token);

        i += best_length + 1;
    }

    return compressed;
}

std::string LZ77Compression::token_decompress(const std::vector<LZ77Token> &tokens)
{
    std::string output;

    for (const LZ77Token &token : tokens) {
        if (token.length == 0) {
            output.push_back(token.next);
            continue;
        }

        for (size_t i = 0; i < token.length; ++i) {
            output.push_back(output[output.size() - token.offset]);
        }

        if (token.next != 0) {
            output.push_back(token.next);
        }
    }

    return output;
}

std::vector<uint8_t> RLECompression::compress(const std::string &data)
{
    std::vector<uint8_t> compressed_data;

    for (size_t i = 0; i < data.size(); ++i) {
        char current_char = data[i];
        uint8_t count = 1;

        while (i + 1 < data.size() && data[i + 1] == current_char && count < 255) {
            ++count;
            ++i;
        }

        compressed_data.push_back(static_cast<uint8_t>(current_char));
        compressed_data.push_back(count);
    }

    return compressed_data;
}

std::string RLECompression::decompress(const std::vector<uint8_t> &data)
{
    std::string decompressed_data;

    for (size_t i = 0; i < data.size(); i += 2) {
        if (i + 1 >= data.size()) {
            throw std::runtime_error("Invalid RLE compressed data format.");
        }

        char current_char = static_cast<char>(data[i]);
        uint8_t count = data[i + 1];

        decompressed_data.append(count, current_char);
    }

    return decompressed_data;
}
