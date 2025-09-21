#ifndef SIGNAL_PROCESSING_BOOK_TEXT_HELPERS_H
#define SIGNAL_PROCESSING_BOOK_TEXT_HELPERS_H

#include "imgui.h"

/**
 * Line-wrapping version of ImGui::BulletText(). Kudos to https://github.com/ocornut/imgui/issues/3273
 * @param bullet The bullet format character, e.g. "-"
 * @param text User-displayed text
 */
#define TEXT_BULLET(bullet, text) ImGui::TextWrapped(bullet); ImGui::SameLine(); ImGui::TextWrapped(text);

#endif //SIGNAL_PROCESSING_BOOK_TEXT_HELPERS_H