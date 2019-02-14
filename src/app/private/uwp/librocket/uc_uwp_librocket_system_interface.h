#pragma once

#include <Rocket/Core.h>

namespace uc
{
    namespace uwp
    {
        namespace librocket
        {
            class system_interface : public Rocket::Core::SystemInterface
            {
                using base = Rocket::Core::SystemInterface;
               
                public:

                system_interface();
                virtual ~system_interface();

                /// Get the number of seconds elapsed since the start of the application.
                /// @return Elapsed time, in seconds.
                float GetElapsedTime() override;

                /// Translate the input string into the translated string.
                // @param[out] translated Translated string ready for display.
                /// @param[in] input String as received from XML.
                /// @return Number of translations that occured.
                int TranslateString(Rocket::Core::String& translated, const Rocket::Core::String& input) override;

                /// Joins the path of an RML or RCSS file with the path of a resource specified within the file.
                /// @param[out] translated_path The joined path.
                /// @param[in] document_path The path of the source document (including the file name).
                /// @param[in] path The path of the resource specified in the document.
                void JoinPath(Rocket::Core::String& translated_path, const Rocket::Core::String& document_path, const Rocket::Core::String& path) override;

                /// Log the specified message.
                /// @param[in] type Type of log message, ERROR, WARNING, etc.
                /// @param[in] message Message to log.
                /// @return True to continue execution, false to break into the debugger.
                bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message) override;

                // Activate keyboard (for touchscreen devices)
                void ActivateKeyboard() override;

                /// Deactivate keyboard (for touchscreen devices)
                void DeactivateKeyboard() override;
    
                /// Called when this system interface is no longer required.
                void Release() override;


                void set_frame_time(float time);

                private:

                float m_frame_time = 0.0f;

            };
         
        }
    }
}
