#ifndef TNT_IMGUI_LIB_HPP
#define TNT_IMGUI_LIB_HPP

#include "core/Window.hpp"
#include "math/Vector.hpp"
#include "utils/BitFlags.hpp"

// TODO: more widgets:
// image, radio, text field, selection box, context menu, icon(button with image)
// TODO: load from config file.
// TODO: themes and same color for widgets like button, checkbox, etc
// TODO: add support for multiple windows.
// TODO: make sure no widget (except list_item) is drawed between BeginList() and EndList() calls.
// TODO: add support for handling more than one context (multiple game windows).
// TODO: sameline()
// TODO: add support for drawing text on the left of the widget and switching from the left to the right of the widget.
// TODO: add support for calling window inside a window (ex. creating window on button click)
// TODO: make a draw function to draw all widgets the way you want.
// TODO: draw menu even if there is no ImGui window. In that case, draw the menu on the "top" of the window.
// TODO: reduce new/delete calls (maybe) eleminate them totally

// TODO(maybe):
// move slider's modified value before min/max ??
// slider_double, slider_uint8 ??
// keyboard navigation ??
// move update_context() stuff to Begin() ??
// use "text" instead of "list_item" for adding items to list.

namespace tnt
{
    namespace ImGui
    {
        /// @brief An enum representing the flags that an ImGui window (not a
        /// game window) can have.
        /// @sa @ref tnt::ImGui::Begin()
        /// @note This enum is not fully functional yet. This means that you should
        /// leave the default (last) argument of @c tnt::ImGui::Begin() as is.
        enum class WindowFlags : std::uint8_t
        {
            Collapsible = (1 << 0),      ///< with a collapse button
            Closable = (1 << 1),         ///< with a close button
            Resizable = (1 << 2),        ///< with a resize button
            Movable = (1 << 3),          ///< can be moved
            WithTitleBar = (1 << 4),     ///< has a title bar
            OpaqueBackground = (1 << 5), ///< draw text and widgets, and the background
            WidgetThenText = (1 << 6),   ///< draw the widget on the left and it's text on the right

            Default = Collapsible | Closable | Resizable | Movable | WithTitleBar | OpaqueBackground | WidgetThenText ///< has all the enum's values OR-ed
        };
    } // namespace ImGui

    template <>
    struct enable_bit_mask<ImGui::WindowFlags> : std::true_type
    {
    };

    namespace ImGui
    {
        /// @brief Creates a new context and makes ImGui ready to be used.
        /// @note This is automatically called by @c tnt_imgui_begin().
        TNT_API void make_context() noexcept;

        /// @brief Draw a new ImGui window on @c win at @c (x,y) and handle events related to it.
        /// Widgets that don't require position parameters are drawn on this window.
        /// @param win The window where the drawing should happen.
        /// @param name The desired title for the window.
        /// @param x The desired x position.
        /// @param y The desired y position.
        /// @param flags The flags the window should have.
        /// @return bool
        /// @sa @ref tnt::ImGui::WindowFlags
        /// @note @c tnt::ImGui::End() should be called for each @c tnt::ImGui::Begin().
        /// @note For the moment, you CANNOT call @c tnt::ImGui::Begin() inside another
        /// @c tnt::ImGui::Begin() / @c tnt::ImGui::End() pair. That's the only case when
        /// this function returns false, but this is subject to change.
        TNT_API bool Begin(Window const &win, std::string_view name, int x, int y,
                           WindowFlags flags = WindowFlags::Default) noexcept;

        /// @brief Stop drawing widgets on the last window where @c tnt::ImGui::Begin()
        /// is called. Also update some context-related data.
        /// @sa @ref tnt::ImGui::Begin()
        TNT_API void End() noexcept;

        /// @brief Draw a "section" on the current ImGui window, which contains widgets and can be collapsed.
        /// @param win The game window where the drawing should happen.
        /// @param text The text to be displayed on the section.
        /// @return bool
        /// @sa @ref tnt::ImGui::EndSection()
        TNT_API bool BeginSection(Window const &win, std::string_view text) noexcept;

        /// @brief Stop drawing widgets on the current "section".
        /// @sa @ref tnt::ImGui::BeginSection()
        TNT_API void EndSection() noexcept;

        /// @brief Prepare the current ImGui window for drawing a list on it.
        /// Calling @c BeginList() inside another @c BeginList() / @c EndList() pair
        /// indents the second list. This is useful for making sublists.
        /// @sa @ref tnt::ImGui::EndList()
        /// @sa @ref tnt::ImGui::list_item()
        /// @note @c tnt::ImGui::EndList() should be called for each @c tnt::ImGui::BeginList().
        TNT_API void BeginList(bool indexed) noexcept;

        /// @brief End the current list of items. If this is a sublist (list inside another list)
        /// indentation is decreased.
        /// @sa @ref tnt::ImGui::BeginList()
        /// @sa @ref tnt::ImGui::list_item()
        TNT_API void EndList() noexcept;

        /// @brief Prepare the last ImGui window to draw the menu bar on it.
        /// @sa @ref tnt::ImGui::EndMenuBar()
        /// @sa @ref tnt::ImGui::menu_button()
        /// @sa @ref tnt::ImGui::menu_item()
        TNT_API void BeginMenuBar() noexcept;

        /// @brief Do the cleanup of the menu bar stuff on the last ImGui window.
        /// @sa @ref tnt::ImGui::BeginMenuBar()
        /// @sa @ref tnt::ImGui::menu_button()
        /// @sa @ref tnt::ImGui::menu_item()
        TNT_API void EndMenuBar() noexcept;

        /// @brief Draw a button on the current ImGui window, with @c text on it.
        /// Returns true if the button is pressed.
        /// @param win The game window where the button should be drawed.
        /// @param text The text that should be drawn on the button.
        /// @return bool
        TNT_API bool button(Window const &win, std::string_view text) noexcept;

        /// @brief Draw a vertical slider that modifies an @c int value on the current ImGui window.
        /// The widget returns @c true if the value has been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        TNT_API bool slider_int(Window const &win, int min_, int max_, int *value) noexcept;

        /// @brief Draw a vertical slider that modifies an @c float value on the current ImGui window.
        /// The widget returns @c true if the value has been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        TNT_API bool slider_float(Window const &win, float min_, float max_,
                                  float *value) noexcept;

        /// @brief Draw a horizontal slider that modifies an @c int value on the current ImGui window.
        /// The widget returns @c true if the value has been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param text The text that should be drawn next to the slider.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        TNT_API bool hslider_int(Window const &win, std::string_view text, int min_, int max_,
                                 int *value) noexcept;

        /// @brief Draw a horizontal slider that modifies an @c float value on the current ImGui window.
        /// The widget returns @c true if the value has been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param text The text that should be drawn next to the slider.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        TNT_API bool hslider_float(Window const &win, std::string_view text, float min_, float max_,
                                   float *value) noexcept;

        /// @brief Draw two aligned horizontal sliders that modify 2 @c int values on the current ImGui window.
        /// The widget returns @c true if any of the values have been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param text The text that should be drawn next to the sliders.
        /// @param min_ The minimun value the widget can set for the first @c int.
        /// @param max_ The maximum value the widget can set for the first @c int.
        /// @param value1 The first value that the widget can modify.
        /// @param value2 The second value that the widget can modify.
        /// @return bool
        TNT_API bool hslider_int2(Window const &win, std::string_view text,
                                  int min_, int max_, int *value1, int *value2) noexcept;

        /// @brief Draw two aligned horizontal sliders that modify 2 @c float values on the current ImGui window.
        /// The widget returns @c true if any of the values have been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param text The text that should be drawn next to the sliders.
        /// @param min_ The minimun value the widget can set for the first @c float.
        /// @param max_ The maximum value the widget can set for the first @c float.
        /// @param value1 The first value that the widget can modify.
        /// @param value2 The second value that the widget can modify.
        /// @return bool
        TNT_API bool hslider_float2(Window const &win, std::string_view text,
                                    float min_, float max_, float *value1, float *value2) noexcept;

        /// @brief Draw two aligned horizontal sliders that modify a @c tnt::Vector's value on the current ImGui window.
        /// The widget returns @c true if any of the values have been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param text The text that should be drawn next to the sliders.
        /// @param min1 The minimun value the widget can set for the first @c float of the @c tnt::Vector.
        /// @param max1 The maximum value the widget can set for the first @c float of the @c tnt::Vector.
        /// @param min2 The minimun value the widget can set for the second @c float of the @c tnt::Vector.
        /// @param max2 The maximum value the widget can set for the second @c float of the @c tnt::Vector.
        /// @param value The @c tnt::Vector that the widget can modify.
        /// @return bool
        TNT_API bool hslider_vec(Window const &win, std::string_view text,
                                 float min1, float max1, float min2, float max2,
                                 tnt::Vector *value) noexcept;

        /// @brief Draw a checkbox with @c text on the side of it.
        /// Returns @c true if modified, @c false otherwise.
        /// @param win The game window where the checkbox should be drawed.
        /// @param text The text that should be displayed next to the checkbox.
        /// @param value The value representing the state of the checkbox.
        /// @return bool
        TNT_API bool checkbox(Window const &win, std::string_view text, bool *value) noexcept;

        /// @brief Draw a menu button on the current window.
        /// @param win The game window where the menu button will be drawed.
        /// @param text The name of the menu button.
        /// @return bool
        /// @sa @ref tnt::ImGui::BeginMenuBar()
        /// @sa @ref tnt::ImGui::EndMenuBar()
        /// @sa @ref tnt::ImGui::menu_item()
        TNT_API bool menu_button(Window const &win, std::string_view text) noexcept;

        /// @brief Draw a menu bar with options from the @c options array.
        /// Returns the index of the pressed menu button (if any), or -1 otherwise.
        /// @param win The game window where the menu should be drawed.
        /// @param text The text to be shown on the item's button.
        /// @return bool
        /// @sa @ref tnt::ImGui::BeginMenuBar()
        /// @sa @ref tnt::ImGui::EndMenuBar()
        /// @sa @ref tnt::ImGui::menu_button()
        TNT_API bool menu_item(Window const &win, std::string_view text) noexcept;

        /// @brief Draw a progress bar with text on the side of it.
        /// @param win The game window where the progress bar should be drawed.
        /// @param text The text that should be displayed next to the progress bar.
        /// @param min_ The minimum value of the progress bar.
        /// @param max_ The maximum value of the progress bar.
        /// @param value The value that shows the current progress bar index.
        /// @todo (maybe) return 1 if *value == max_
        TNT_API void progress_bar(Window const &win, std::string_view text, int min_, int max_,
                                  int *value) noexcept;

        /// @brief Add an empty line on the current window.
        TNT_API void newline() noexcept;

        /// @brief Draw @c text on the current window.
        /// @param win The game window where the text should be drawed.
        /// @param text The text that should be drawed.
        TNT_API void text(Window const &win, std::string_view text) noexcept;

        /// @brief Draw @c text with the specified color.
        /// @param win The game window where the text should be drawed.
        /// @param text The text that should be drawed.
        /// @param r The red "component" of the text color.
        /// @param g The green "component" of the text color.
        /// @param b The blue "component" of the text color.
        /// @param a The transparent "component" of the text color.
        TNT_API void colored_text(Window const &win, std::string_view text,
                                  unsigned char r, unsigned char g,
                                  unsigned char b, unsigned char a) noexcept;

        /// @brief Add a new item to the current list.
        /// @param win The game window where the list item should be drawed.
        /// @param text The text of the list item.
        /// @sa @ref tnt::ImGui::BeginList()
        /// @sa @ref tnt::ImGui::EndList()
        TNT_API void list_item(Window const &win, std::string_view text) noexcept;
    } // namespace ImGui
} // namespace tnt

/// @brief Initialize all needed stuff to use ImGui on @c win.
/// @param win The target game window.
TNT_API void tnt_imgui_init(tnt::Window const &win) noexcept;

#endif //! TNT_IMGUI_LIB_HPP