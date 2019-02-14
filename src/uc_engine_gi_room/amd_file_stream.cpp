#include "pch.h"

#include "amd_file_stream.h"

namespace AdvancedMicroDevices
{
    namespace GraphicDemos
    {
        SimpleReadFileStream::SimpleReadFileStream(const wchar_t* file_name)
        {
            HANDLE h = CreateFile2(file_name, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr);

            if (h == INVALID_HANDLE_VALUE)
            {
                throw StreamReadException();
            }

            m_handle = h;
        }

        SimpleReadFileStream::~SimpleReadFileStream()
        {
            CloseHandle(m_handle);
        }

        void SimpleReadFileStream::ReadBuffer(void* buffer, size_t buffer_size)
        {
            DWORD read  = 0;

            if (!ReadFile(m_handle, buffer, static_cast<DWORD>(buffer_size), &read, nullptr))
            {
                throw StreamReadException();
            }

            if (read != buffer_size)
            {
                throw StreamReadException();
            }
        }

        void SimpleReadFileStream::Seek( size_t position )
        {
            auto r0 = static_cast<LONG>(position);
            auto r1 = static_cast<LONG>(position >> 32);

            if ( INVALID_SET_FILE_POINTER == SetFilePointer(m_handle, r0, &r1, FILE_BEGIN) )
            {
                throw StreamReadException();
            }
        }

        void SimpleReadFileStream::SeekRelative(size_t position)
        {
            auto r0 = static_cast<LONG>(position);
            auto r1 = static_cast<LONG>(position >> 32);

            if ( INVALID_SET_FILE_POINTER == SetFilePointer(m_handle, r0, &r1, FILE_CURRENT) )
            {
                throw StreamReadException();
            }
        }
    }
}


