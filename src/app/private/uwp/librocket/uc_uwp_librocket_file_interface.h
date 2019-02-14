#pragma once

#include <Rocket/Core.h>

#include <filesystem>

namespace uc
{
    namespace uwp
    {
        namespace librocket
        {
            class file_interface : public Rocket::Core::FileInterface
            {
                using base = Rocket::Core::FileInterface;

                public:

                /// Opens a file.
                /// @param file The file handle to write to.
                /// @return A valid file handle, or NULL on failure
                Rocket::Core::FileHandle Open(const Rocket::Core::String& path) override;
                /// Closes a previously opened file.
                /// @param file The file handle previously opened through Open().
                void Close(Rocket::Core::FileHandle file) override;

                /// Reads data from a previously opened file.
                /// @param buffer The buffer to be read into.
                /// @param size The number of bytes to read into the buffer.
                /// @param file The handle of the file.
                /// @return The total number of bytes read into the buffer.
                size_t Read(void* buffer, size_t size, Rocket::Core::FileHandle file) override;
                /// Seeks to a point in a previously opened file.
                /// @param file The handle of the file to seek.
                /// @param offset The number of bytes to seek.
                /// @param origin One of either SEEK_SET (seek from the beginning of the file), SEEK_END (seek from the end of the file) or SEEK_CUR (seek from the current file position).
                /// @return True if the operation completed successfully, false otherwise.
                bool Seek(Rocket::Core::FileHandle file, long offset, int origin) override;
                /// Returns the current position of the file pointer.
                /// @param file The handle of the file to be queried.
                /// @return The number of bytes from the origin of the file.
                size_t Tell(Rocket::Core::FileHandle file)  override;

                /// Returns the length of the file.
                /// The default implementation uses Seek & Tell.
                /// @param file The handle of the file to be queried.
                /// @return The length of the file in bytes.
                size_t Length(Rocket::Core::FileHandle file) override;

                /// Called when this file interface is released.
                void Release() override;

                private:

            };
        }
     }
}
