#pragma once

#include <memory>
#include <Windows.h>
#include <uc_dev/lzham/lzham_compressor.h>

#include "geo.h"

namespace uc
{
    namespace lip
    {
        namespace details
        {
            inline uint64_t get_file_size(const std::wstring& filename)
            {
                struct _stat64 stat_buf;
                int rc = _wstat64(filename.c_str(), &stat_buf);
                return rc == 0 ? stat_buf.st_size : -1;
            }

            struct file_handle_deleter
            {
                void operator() (void* f) const
                {
                    CloseHandle(f);
                }
            };
        }

        inline std::vector<uint8_t> read_lip_file(const std::wstring& filename)
        {
            auto size2 = details::get_file_size(filename);

            std::unique_ptr< void, details::file_handle_deleter> h(CreateFile2(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr), details::file_handle_deleter());

            std::vector<uint8_t> bytes;
            bytes.resize(static_cast<size_t>(size2));

            auto r = ReadFile(h.get(), &bytes[0], static_cast<DWORD>(size2), nullptr, nullptr);

            if (r)
            {
                return bytes;
            }
            else
            {
                //todo: error
                return bytes;
            }
        }

        template <typename t>
        inline lip::unique_lip_pointer<t> create_from_lip_file(const std::wstring& filename)
        {
            return lip::make_unique_lip_pointer< t >( read_lip_file(filename) );
        }

        inline std::vector<uint8_t> read_from_compressed_lip_file(const std::wstring& filename)
        {
            auto size2 = details::get_file_size(filename);

            std::unique_ptr< void, details::file_handle_deleter> h(CreateFile2(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr), details::file_handle_deleter());

            std::vector<uint8_t> bytes;

            assert(size2 < 0xFFFFFFFF );

            bytes.resize(static_cast<size_t>(size2));

            auto r = ReadFile(h.get(), &bytes[0], static_cast<DWORD>(size2), nullptr, nullptr);

            if (r)
            {
                size_t decompressed_size = static_cast<size_t>(*(uint64_t*)(&bytes[8]));
                return lzham::decompress_buffer(&bytes[16], bytes.size() - 16, decompressed_size);
            }
            else
            {
                //todo: error
                return bytes;
            }
        }

        template <typename t>
        inline lip::unique_lip_pointer<t> create_from_compressed_lip_file(const std::wstring& filename)
        {
            return lip::make_unique_lip_pointer< t >( read_from_compressed_lip_file(filename) );
        }
    }
}







