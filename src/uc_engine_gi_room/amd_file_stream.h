#pragma once

#include <exception>
#include "amd_base_types.h"

namespace AdvancedMicroDevices
{
    namespace GraphicDemos
    {
        class StreamReadException : public std::exception
        {

        };

        class SimpleReadFileStream
        {

        public:

            SimpleReadFileStream(const wchar_t* file_name);
            ~SimpleReadFileStream();
            void ReadBuffer(void* buffer, size_t buffer_size);

            void Seek(size_t offset);
            void SeekRelative(size_t offset);

        private:

            SimpleReadFileStream(const SimpleReadFileStream &) = delete;
            SimpleReadFileStream& operator=(const SimpleReadFileStream &) = delete;

            HANDLE m_handle;
        };

        template <typename t> void Read( SimpleReadFileStream& r, t& b)
        {
            r.ReadBuffer(&b, sizeof(t));
        }

    }
}

