#include "pch.h"

#include "uc_uwp_librocket_system_interface.h"

namespace uc
{
    namespace uwp
    {
        namespace librocket
        {
            system_interface::system_interface()
            {

            }

            system_interface::~system_interface()
            {

            }

            /// Get the number of seconds elapsed since the start of the application.
            /// @return Elapsed time, in seconds.
            float system_interface::GetElapsedTime()
            {
                return m_frame_time;
            }

            /// Translate the input string into the translated string.
            // @param[out] translated Translated string ready for display.
            /// @param[in] input String as received from XML.
            /// @return Number of translations that occured.
            int system_interface::TranslateString(Rocket::Core::String& translated, const Rocket::Core::String& input)
            {
                return base::TranslateString(translated, input);
            }


            /// Joins the path of an RML or RCSS file with the path of a resource specified within the file.
            /// @param[out] translated_path The joined path.
            /// @param[in] document_path The path of the source document (including the file name).
            /// @param[in] path The path of the resource specified in the document.
            void system_interface::JoinPath(Rocket::Core::String& translated_path, const Rocket::Core::String& document_path, const Rocket::Core::String& path)
            {
                return base::JoinPath(translated_path, document_path, path);
            }

            /// Log the specified message.
            /// @param[in] type Type of log message, ERROR, WARNING, etc.
            /// @param[in] message Message to log.
            /// @return True to continue execution, false to break into the debugger.
            bool system_interface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
            {
                message;
                if (type == Rocket::Core::Log::LT_ASSERT)
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }

            // Activate keyboard (for touchscreen devices)
            void system_interface::ActivateKeyboard()
            {

            }

            /// Deactivate keyboard (for touchscreen devices)
            void system_interface::DeactivateKeyboard()
            {

            }

            /// Called when this system interface is no longer required.
            void system_interface::Release()
            {
                base::Release();
            }

            void system_interface::set_frame_time(float time)
            {
                m_frame_time = time;
            }
        }
    }
}
