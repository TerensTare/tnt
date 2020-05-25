#ifndef TNT_IMGUI_LIB_HPP
#define TNT_IMGUI_LIB_HPP

#include <string_view>

// TODO: more widgets:
// image, radio, text field, selection box, context menu, icon(button with image)
// TODO: separate menu() into BeginMenu()/EndMenu() and menu_item() (whatever name)
// TODO: load from config file.
// TODO: themes and same color for widgets like button, checkbox, etc
// TODO: cheaper and faster text handling.
// TODO: add support for collapsing and multiple windows.
// TODO: make sure no widget (except list_item) is drawed between BeginList() and EndList() calls.
// TODO: add support for handling more than one context (multiple game windows).
// TODO: sameline()
// TODO: add support for drawing text on the left of the widget and switching from the left to the right of the widget.
// TODO: add support for calling window inside a window (ex. creating window on button click)
// TODO: remove some of the config functions and members.
// TODO: resize the hslider's width in case the window is resized.
// TODO: make a draw function to draw all widgets the way you want.

// TODO(maybe):
// remove window_config ??
// move slider's modified value before min/max ??
// slider_double, slider_uint8 ??
// draw ImGui widgets straight from tnt::Window ??
// keyboard navigation ??
// move update_context() stuff to Begin() ??

namespace tnt
{
    class Window;
    struct Vector;

    namespace ImGui
    {

        /// @brief An enum representing the flags that an ImGui window (not a
        /// game window) can have.
        /// @sa tnt::ImGui::Begin()
        /// @note This enum is not fully functional yet. This means that you should
        /// leave the default (last) argument of @c tnt::ImGui::Begin() as is.
        enum class WindowFlags : int
        {
            Default = 0,              ///< doesn't have any of the flags above
            NonCollapsible = 1,       ///< without a collapse button
            NonClosable = 2,          ///< without a close button
            NonResizable = 3,         ///< without a resize button
            NonMovable = 4,           ///< can't be moved
            NoTitleBar = 5,           ///< doesn't have a title bar
            WithoutMenu = 6,          ///< doesn't have a menu
            TransparentBackground = 7 ///< draw only text and widgets, without background
        };

        //////////////////////////
        // operator overloading //
        //////////////////////////

        /// @brief bit_or for @c WindowFlags.
        /// @param lhs The left @c WindowFlag.
        /// @param rhs The right @c WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags operator|(WindowFlags const &lhs, WindowFlags const &rhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(static_cast<type>(lhs) | static_cast<type>(rhs));
        }

        /// @brief bit_or equal for @c WindowFlags. Shorthand for lhs = (lhs | rhs).
        /// @param lhs The left @c WindowFlag.
        /// @param rhs The right @c WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags &operator|=(WindowFlags &lhs, WindowFlags rhs) noexcept
        {
            lhs = lhs | rhs;
            return lhs;
        }

        /// @brief bit_and for @c WindowFlags.
        /// @param lhs The left @c WindowFlag.
        /// @param rhs The right @c WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags operator&(WindowFlags const &lhs, WindowFlags const &rhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(static_cast<type>(lhs) & static_cast<type>(rhs));
        }

        /// @brief bit_and equal for @c WindowFlags. Shorthand for lhs = (lhs & rhs).
        /// @param lhs The left @c WindowFlag.
        /// @param rhs The right @c WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags &operator&=(WindowFlags &lhs, WindowFlags rhs) noexcept
        {
            lhs = lhs & rhs;
            return lhs;
        }

        /// @brief bit_xor for @c WindowFlags.
        /// @param lhs The left @c WindowFlag.
        /// @param rhs The right @c WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags operator^(WindowFlags const &lhs, WindowFlags const &rhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(static_cast<type>(lhs) ^ static_cast<type>(rhs));
        }

        /// @brief bit_xor equal for @c WindowFlags. Shorthand for lhs = (lhs ^ rhs).
        /// @param lhs The left @c WindowFlag.
        /// @param rhs The right @c WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags &operator^=(WindowFlags &lhs, WindowFlags rhs) noexcept
        {
            lhs = lhs ^ rhs;
            return lhs;
        }

        /// @brief bitwise complement for @c WindowFlags.
        /// @param lhs The left @c WindowFlag.
        /// @param rhs The right @c WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags operator~(WindowFlags const &lhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(~static_cast<type>(lhs));
        }

        /////////////
        // context //
        /////////////

        /// @brief Creates a new context and makes ImGui ready to be used.
        /// @note This is automatically called by @c tnt_imgui_begin().
        void make_context() noexcept;

        /// @brief Updates ImGui's context data.
        void update_context() noexcept;

        /// @brief Destroys the ImGui context and makes ImGui unusable.
        /// @note This is automatically called by @c tnt_imgui_close().
        void destroy_context() noexcept;

        /////////////
        // widgets //
        /////////////

        /// @brief Draw a new ImGui window on @c win at @c (x,y) and handle events related to it.
        /// Widgets that don't require position parameters are drawn on this window.
        /// @param win The window where the drawing should happen.
        /// @param name The desired title for the window.
        /// @param x The desired x position.
        /// @param y The desired y position.
        /// @param flags The flags the window should have.
        /// @return bool
        /// @sa tnt::ImGui::WindowFlags
        /// @note @c tnt::ImGui::End() should be called for each @c tnt::ImGui::Begin().
        /// @note For the moment, you CANNOT call @c tnt::ImGui::Begin() inside another
        /// @c tnt::ImGui::Begin() / @c tnt::ImGui::End() pair. That's the only case when
        /// this function returns false, but this is subject to change.
        bool Begin(Window const *win, std::string_view name, int x, int y,
                   WindowFlags flags = WindowFlags::Default) noexcept;

        /// @brief Stop drawing widgets on the last window where @c tnt::ImGui::Begin()
        /// is called. Also update some context-related data.
        /// @sa tnt::ImGui::Begin()
        void End() noexcept;

        /// @brief Prepare the current ImGui window for drawing a list on it.
        /// Calling @c BeginList() inside another @c BeginList() / @c EndList() pair
        /// indents the second list. This is useful for making sublists.
        /// @sa tnt::ImGui::EndList()
        /// @sa tnt::ImGui::list_item()
        /// @note @c tnt::ImGui::EndList() should be called for each @c tnt::ImGui::BeginList().
        void BeginList(bool indexed) noexcept;

        /// @brief End the current list of items. If this is a sublist (list inside another list)
        /// indentation is decreased.
        /// @sa tnt::ImGui::BeginList()
        /// @sa tnt::ImGui::list_item()
        void EndList() noexcept;

        /// @brief Prepare the last ImGui window to draw the menu bar on it.
        /// @sa tnt::ImGui::EndMenuBar()
        /// @sa tnt::ImGui::menu_button()
        /// @sa tnt::ImGui::menu_item()
        void BeginMenuBar() noexcept;

        /// @brief Do the cleanup of the menu bar stuff on the last ImGui window.
        /// @sa tnt::ImGui::BeginMenuBar()
        /// @sa tnt::ImGui::menu_button()
        /// @sa tnt::ImGui::menu_item()
        void EndMenuBar() noexcept;

        /// @brief Draw a button on the current ImGui window, with @c text on it.
        /// Returns true if the button is pressed.
        /// @param win The game window where the button should be drawed.
        /// @param text The text that should be drawn on the button.
        /// @return bool
        bool button(Window const *win, std::string_view text) noexcept;

        /// @brief Draw a vertical slider that modifies an @c int value on the current ImGui window.
        /// The widget returns @c true if the value has been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        bool slider_int(Window const *win, int min_, int max_,
                        int *value) noexcept;

        /// @brief Draw a vertical slider that modifies an @c float value on the current ImGui window.
        /// The widget returns @c true if the value has been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        bool slider_float(Window const *win, float min_, float max_,
                          float *value) noexcept;

        /// @brief Draw a horizontal slider that modifies an @c int value on the current ImGui window.
        /// The widget returns @c true if the value has been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param text The text that should be drawn next to the slider.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        bool hslider_int(Window const *win, std::string_view text, int min_, int max_,
                         int *value) noexcept;

        /// @brief Draw a horizontal slider that modifies an @c float value on the current ImGui window.
        /// The widget returns @c true if the value has been modified, @c false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param text The text that should be drawn next to the slider.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        bool hslider_float(Window const *win, std::string_view text, float min_, float max_,
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
        bool hslider_int2(Window const *win, std::string_view text,
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
        bool hslider_float2(Window const *win, std::string_view text,
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
        bool hslider_vec(Window const *win, std::string_view text,
                         float min1, float max1, float min2, float max2,
                         tnt::Vector *value) noexcept;

        /// @brief Draw a checkbox with @c text on the side of it.
        /// Returns @c true if modified, @c false otherwise.
        /// @param win The game window where the checkbox should be drawed.
        /// @param text The text that should be displayed next to the checkbox.
        /// @param value The value representing the state of the checkbox.
        /// @return bool
        bool checkbox(Window const *win, std::string_view text, bool *value) noexcept;

        /// @brief Draw a menu button on the current window.
        /// @param text The name of the menu button.
        /// @return bool
        /// @sa tnt::ImGui::BeginMenuBar()
        /// @sa tnt::ImGui::EndMenuBar()
        /// @sa tnt::ImGui::menu_item()
        bool menu_button(Window const *win, std::string_view text) noexcept;

        /// @brief Draw a menu bar with options from the @c options array.
        /// Returns the index of the pressed menu button (if any), or -1 otherwise.
        /// @param win The game window where the menu should be drawed.
        /// @param text The text to be shown on the item's button.
        /// @return bool
        /// @sa tnt::ImGui::BeginMenuBar()
        /// @sa tnt::ImGui::EndMenuBar()
        /// @sa tnt::ImGui::menu_button()
        bool menu_item(Window const *win, std::string_view text) noexcept;

        // (maybe) return 1 if *value == max_

        /// @brief Draw a progress bar with text on the side of it.
        /// @param win The game window where the progress bar should be drawed.
        /// @param text The text that should be displayed next to the progress bar.
        /// @param min_ The minimum value of the progress bar.
        /// @param max_ The maximum value of the progress bar.
        /// @param value The value that shows the current progress bar index.
        void progress_bar(Window const *win, std::string_view text, int min_, int max_,
                          int *value) noexcept;

        /// @brief Add an empty line on the current window.
        void newline() noexcept;

        /// @brief Draw @c text on the current window.
        /// @param win The game window where the text should be drawed.
        /// @param text The text that should be drawed.
        void text(Window const *win, std::string_view text) noexcept;

        /// @brief Draw @c text with the specified color.
        /// @param win The game window where the text should be drawed.
        /// @param text The text that should be drawed.
        /// @param r The red "component" of the text color.
        /// @param g The green "component" of the text color.
        /// @param b The blue "component" of the text color.
        /// @param a The transparent "component" of the text color.
        void colored_text(Window const *win, std::string_view text,
                          unsigned char r, unsigned char g,
                          unsigned char b, unsigned char a) noexcept;

        /// @brief Add a new item to the current list.
        /// @param win The game window where the list item should be drawed.
        /// @param text The text of the list item.
        /// @sa tnt::ImGui::BeginList()
        /// @sa tnt::ImGui::EndList()
        void list_item(Window const *win, std::string_view text) noexcept;
    } // namespace ImGui
} // namespace tnt

/// @brief Initialize all needed stuff to use ImGui on @c win.
/// @param win The target game window.
void tnt_imgui_init(tnt::Window const *win) noexcept;

/// @brief Cleans up all ImGui-related stuff.
void tnt_imgui_close() noexcept;

#endif //! TNT_IMGUI_LIB_HPP