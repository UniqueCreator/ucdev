#include "pch.h"

#include "uc_uwp_librocket_file_interface.h"

#include <fstream>
#include <memory>
#include <assert.h>

namespace uc
{
    namespace uwp
    {
        namespace librocket
        {
            namespace details
            {
                struct file_interface_handle
                {
                    std::experimental::filesystem::path m_file_path;
                    std::fstream                        m_stream;
                };


                inline file_interface_handle* handle_to_stream(Rocket::Core::FileHandle h)
                {
                    if (h == 0)
                    {
                        return nullptr;
                    }
                    else
                    {
                        return reinterpret_cast<file_interface_handle*> (h);
                    }
                }

                inline std::ios_base::seekdir origin_to_way(int origin)
                {
                    switch (origin)
                    {
                        case SEEK_SET: return std::ios_base::ios_base::beg;
                        case SEEK_END: return std::ios_base::ios_base::end;
                        case SEEK_CUR: return std::ios_base::ios_base::cur;

                        default:
                                assert(false);
                                return std::ios_base::ios_base::beg;
                    }
                }
            }
            /// Opens a file.
            /// @param file The file handle to write to.
            /// @return A valid file handle, or NULL on failure
            Rocket::Core::FileHandle file_interface::Open(const Rocket::Core::String& path)
            {
                /*
                std::unique_ptr< details::file_interface_handle > f(new details::file_interface_handle );
                
                f->m_stream.open(path.CString(), std::ios_base::in | std::ios_base::binary);

                if (f->m_stream.good())
                {
                    f->m_file_path = std::experimental::filesystem::path(path.CString());
                    return reinterpret_cast<Rocket::Core::FileHandle> (f.release());
                }
                else
                {
                     return 0;
                }
                */

                // Attempt to open the file relative to the application's root.
                FILE* fp = fopen(path.CString(), "rb");
                if (fp != NULL)
                    return (Rocket::Core::FileHandle) fp;

                // Attempt to open the file relative to the current working directory.
                fp = fopen(path.CString(), "rb");
                return (Rocket::Core::FileHandle) fp;
            }

            /// Closes a previously opened file.
            /// @param file The file handle previously opened through Open().
            void file_interface::Close( Rocket::Core::FileHandle file )
            {
                /*
                auto h = details::handle_to_stream(file);
                if (h)
                {
                    h->m_stream.close();
                    delete h;
                }*/

                fclose((FILE*)file);
            }

            /// Reads data from a previously opened file.
            /// @param buffer The buffer to be read into.
            /// @param size The number of bytes to read into the buffer.
            /// @param file The handle of the file.
            /// @return The total number of bytes read into the buffer.
            size_t file_interface::Read( void* buffer, size_t size, Rocket::Core::FileHandle file )
            {
                /*
                auto h = details::handle_to_stream(file);

                if (h)
                {
                    h->m_stream.read(reinterpret_cast<char*>(buffer), size);
                    if ( (h->m_stream.rdstate() & (std::ios_base::eofbit | std::ios_base::failbit)) != 0)
                    {
                        auto s = h->m_stream.gcount();
                        return s;
                    }
                    else
                    {
                        return size;
                    }
                }
                else
                {
                    return 0;
                }
                */

                return fread(buffer, 1, size, (FILE*)file);
            }

            /// Seeks to a point in a previously opened file.
            /// @param file The handle of the file to seek.
            /// @param offset The number of bytes to seek.
            /// @param origin One of either SEEK_SET (seek from the beginning of the file), SEEK_END (seek from the end of the file) or SEEK_CUR (seek from the current file position).
            /// @return True if the operation completed successfully, false otherwise.
            bool file_interface::Seek( Rocket::Core::FileHandle file, long offset, int origin )
            {
                return fseek((FILE*)file, offset, origin) == 0;
                /*
                auto h = details::handle_to_stream(file);

                if (h)
                {
                    h->m_stream.seekg(offset, details::origin_to_way(origin));
                }
                else
                {
                    return false;
                }
                */
            }
            /// Returns the current position of the file pointer.
            /// @param file The handle of the file to be queried.
            /// @return The number of bytes from the origin of the file.
            size_t file_interface::Tell(Rocket::Core::FileHandle file)
            {
                return ftell((FILE*)file);
                /*
                auto h = details::handle_to_stream(file);

                if (h)
                {
                    h->m_stream.tellg();
                }
                else
                {
                    return 0;
                }
                */
                
            }

            /// Returns the length of the file.
            /// The default implementation uses Seek & Tell.
            /// @param file The handle of the file to be queried.
            /// @return The length of the file in bytes.
            size_t file_interface::Length(Rocket::Core::FileHandle file)
            {
                return base::Length(file);
                /*
                auto h = details::handle_to_stream(file);

                if (h)
                {
                    std::experimental::filesystem::file_size(h->m_file_path);
                }
                else
                {
                    return 0;
                }
                */
            }

            /// Called when this file interface is released.
            void file_interface::Release()
            {
                base::Release();
            }
        }
    }
}
