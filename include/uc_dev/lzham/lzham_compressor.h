#pragma once

#include <memory>
#include <vector>

#include "loader.h"

namespace uc
{
    namespace lzham
    {
        namespace details
        {
            inline std::shared_ptr<ilzham> make_lzham()
            {
                auto compressor = std::make_shared< loader >();
                if (!compressor->load())
                {
                    throw std::exception("cannot initialize lzham");
                }
                return compressor;
            }
        }

        inline std::shared_ptr<ilzham> make_compressor()
        {
            static std::shared_ptr<ilzham> compressor = details::make_lzham();
            return compressor;
        }

        inline std::shared_ptr<ilzham> make_decompressor()
        {
            static std::shared_ptr<ilzham> decompressor = details::make_lzham();
            return decompressor;
        }

        inline std::vector<uint8_t>  compress_buffer(const std::vector<uint8_t>& buffer)
        {
            auto c = make_compressor();

            lzham_compress_params params = {};

            params.m_struct_size = sizeof(lzham_compress_params);
            params.m_dict_size_log2 = 18;
            params.m_level = LZHAM_COMP_LEVEL_BETTER;
            params.m_max_helper_threads = -1;

            c->lzham_compress_init(&params);
            uint32_t adler = 0;
            std::vector<uint8_t> result(buffer.size());

            size_t result_size = buffer.size();;

            auto state_compression = c->lzham_compress_memory(&params, &result[0], &result_size, &buffer[0], buffer.size(), &adler);

            if (state_compression != LZHAM_COMP_STATUS_SUCCESS)
            {
                throw std::exception("cannot compress");
            }

            result.resize(result_size);
            return result;
        }

        inline std::vector<uint8_t>  decompress_buffer(const uint8_t* buffer, uint64_t compressed_size, uint64_t decompressed_size)
        {
            auto c = make_compressor();

            lzham_decompress_params params = {};

            params.m_struct_size = sizeof(lzham_decompress_params);
            params.m_dict_size_log2 = 18;

            c->lzham_decompress_init(&params);
            uint32_t adler = 0;

            std::vector<uint8_t> result(static_cast<size_t>(decompressed_size));

            auto result_size = static_cast<size_t>(decompressed_size);
            auto state = c->lzham_decompress_memory(&params, &result[0], &result_size, &buffer[0], static_cast<size_t>(compressed_size), &adler);
            if (state != LZHAM_DECOMP_STATUS_SUCCESS)
            {
                throw std::exception("cannot decompress");
            }
            result.resize(result_size);
            return result;
        }

        inline std::vector<uint8_t>  decompress_buffer(const uint8_t* buffer, uint32_t compressed_size, uint32_t decompressed_size)
        {
            return decompress_buffer(buffer, static_cast<uint64_t>(compressed_size), static_cast<uint64_t>(decompressed_size));
        }
    }
}