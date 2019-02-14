#pragma once

#include <imgui.h>

namespace uc
{
    namespace uwp
    {
        namespace overlay
        {
            // Usage:
            //  static ExampleAppLog my_log;
            //  my_log.AddLog("Hello %d world\n", 123);
            //  my_log.Draw("title");
            struct ExampleAppLog
            {
                ImGuiTextBuffer     Buf;
                ImGuiTextFilter     Filter;
                ImVector<int>       LineOffsets;        // Index to lines offset
                bool                ScrollToBottom;

                void    clear() { Buf.clear(); LineOffsets.clear(); }

                void    add_log( const char* fmt, ...) IM_PRINTFARGS(2)
                {
                    int old_size = Buf.size();
                    va_list args;
                    va_start(args, fmt);
                    Buf.appendv(fmt, args);
                    va_end(args);
                    for (int new_size = Buf.size(); old_size < new_size; old_size++)
                        if (Buf[old_size] == '\n')
                            LineOffsets.push_back(old_size);
                    ScrollToBottom = true;
                }

                void    draw(const char* title, bool* p_open = NULL)
                {
                    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiSetCond_FirstUseEver);
                    ImGui::Begin(title, p_open);
                    if (ImGui::Button("Clear")) clear();
                    ImGui::SameLine();
                    bool copy = ImGui::Button("Copy");
                    ImGui::SameLine();
                    Filter.Draw("Filter", -100.0f);
                    ImGui::Separator();
                    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
                    if (copy) ImGui::LogToClipboard();

                    if (Filter.IsActive())
                    {
                        const char* buf_begin = Buf.begin();
                        const char* line = buf_begin;
                        for (int line_no = 0; line != NULL; line_no++)
                        {
                            const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
                            if (Filter.PassFilter(line, line_end))
                                ImGui::TextUnformatted(line, line_end);
                            line = line_end && line_end[1] ? line_end + 1 : NULL;
                        }
                    }
                    else
                    {
                        ImGui::TextUnformatted(Buf.begin());
                    }

                    if (ScrollToBottom)
                        ImGui::SetScrollHere(1.0f);
                    ScrollToBottom = false;
                    ImGui::EndChild();
                    ImGui::End();
                }
            };
        }
    }
}