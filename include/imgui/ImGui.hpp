#ifndef TNT_IMGUI_LIB_HPP
#define TNT_IMGUI_LIB_HPP

#include <string_view>

/// @brief Macro that generates an id for an ImGui widget.
/// @sa IMGUI_ID
/// @note Use IMGUI_ID instead.
/// @note Both this and IMGUI_ID will be removed in the future.
#define IMGEN_WIDGET_ID(N) (((__LINE__ << 8) | ((N)&0xFF)) ^ ((std::size_t)&__FILE__))

/// @brief Short macro that generates an id for ImGui widgets.
/// @note This will be removed in future.
#define IMGUI_ID IMGEN_WIDGET_ID(1)

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

    namespace ImGui
    {

        /// @brief An enum representing the flags that an ImGui window (not a
        /// game window) can have.
        /// @sa tnt::ImGui::Begin()
        /// @note This enum is not fully functional yet. This means that you should
        /// leave the default (last) argument of @em tnt::ImGui::Begin() as is.
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

        /// @brief bit_or for @em WindowFlags.
        /// @param lhs The left @em WindowFlag.
        /// @param rhs The right @em WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags operator|(WindowFlags const &lhs, WindowFlags const &rhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(static_cast<type>(lhs) | static_cast<type>(rhs));
        }

        /// @brief bit_or equal for @em WindowFlags. Shorthand for lhs = (lhs | rhs).
        /// @param lhs The left @em WindowFlag.
        /// @param rhs The right @em WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags &operator|=(WindowFlags &lhs, WindowFlags rhs) noexcept
        {
            lhs = lhs | rhs;
            return lhs;
        }

        /// @brief bit_and for @em WindowFlags.
        /// @param lhs The left @em WindowFlag.
        /// @param rhs The right @em WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags operator&(WindowFlags const &lhs, WindowFlags const &rhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(static_cast<type>(lhs) & static_cast<type>(rhs));
        }

        /// @brief bit_and equal for @em WindowFlags. Shorthand for lhs = (lhs & rhs).
        /// @param lhs The left @em WindowFlag.
        /// @param rhs The right @em WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags &operator&=(WindowFlags &lhs, WindowFlags rhs) noexcept
        {
            lhs = lhs & rhs;
            return lhs;
        }

        /// @brief bit_xor for @em WindowFlags.
        /// @param lhs The left @em WindowFlag.
        /// @param rhs The right @em WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags operator^(WindowFlags const &lhs, WindowFlags const &rhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(static_cast<type>(lhs) ^ static_cast<type>(rhs));
        }

        /// @brief bit_xor equal for @em WindowFlags. Shorthand for lhs = (lhs ^ rhs).
        /// @param lhs The left @em WindowFlag.
        /// @param rhs The right @em WindowFlag.
        /// @return tnt::ImGui::WindowFlags
        inline WindowFlags &operator^=(WindowFlags &lhs, WindowFlags rhs) noexcept
        {
            lhs = lhs ^ rhs;
            return lhs;
        }

        /// @brief bitwise complement for @em WindowFlags.
        /// @param lhs The left @em WindowFlag.
        /// @param rhs The right @em WindowFlag.
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
        /// @note This is automatically called by @em tnt_imgui_begin().
        void make_context() noexcept;

        /// @brief Updates ImGui's context data.
        void update_context() noexcept;

        /// @brief Destroys the ImGui context and makes ImGui unusable.
        /// @note This is automatically called by @em tnt_imgui_close().
        void destroy_context() noexcept;

        /////////////
        // widgets //
        /////////////

        /// @brief Draw a new ImGui window on @em win at @em (x,y) and handle events related to it.
        /// Widgets that don't require position parameters are drawn on this window.
        /// @param win The window where the drawing should happen.
        /// @param name The desired title for the window.
        /// @param x The desired x position.
        /// @param y The desired y position.
        /// @param flags The flags the window should have.
        /// @return bool
        /// @sa tnt::ImGui::WindowFlags
        /// @note @em tnt::ImGui::End() should be called for each @em tnt::ImGui::Begin().
        /// @note For the moment, you CANNOT call @em tnt::ImGui::Begin() inside another
        /// @em tnt::ImGui::Begin() / @em tnt::ImGui::End() pair. That's the only case when
        /// this function returns false, but this is subject to change.
        bool Begin(Window *win, std::string_view name, int x, int y,
                   WindowFlags flags = WindowFlags::Default) noexcept;

        /// @brief Stop drawing widgets on the last window where @em tnt::ImGui::Begin()
        /// is called. Also update some context-related data.
        /// @sa tnt::ImGui::Begin()
        void End() noexcept;

        /// @brief Prepare the current ImGui window for drawing a list on it.
        /// Calling @em BeginList() inside another @em BeginList() / @em EndList() pair
        /// indents the second list. This is useful for making sublists.
        /// @sa tnt::ImGui::EndList()
        /// @sa tnt::ImGui::list_item()
        /// @note @em tnt::ImGui::EndList() should be called for each @em tnt::ImGui::BeginList().
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

        /// @brief Draw a button on the current ImGui window, with @em text on it.
        /// Returns true if the button is pressed.
        /// @param win The game window where the button should be drawed.
        /// @param text The text that should be drawn on the button.
        /// @return bool
        bool button(Window *win, std::string_view text) noexcept;

        /// @brief Draw a vertical slider that modifies an @em int value on the current ImGui window.
        /// The widget returns @em true if the value has been modified, @em false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        bool slider_int(Window *win, int min_, int max_,
                        int *value) noexcept;

        /// @brief Draw a vertical slider that modifies an @em float value on the current ImGui window.
        /// The widget returns @em true if the value has been modified, @em false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        bool slider_float(Window *win, float min_, float max_,
                          float *value) noexcept;

        /// @brief Draw a horizontal slider that modifies an @em int value on the current ImGui window.
        /// The widget returns @em true if the value has been modified, @em false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param text The text that should be drawn next to the slider.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        bool hslider_int(Window *win, std::string_view text, int min_, int max_,
                         int *value) noexcept;

        /// @brief Draw a horizontal slider that modifies an @em float value on the current ImGui window.
        /// The widget returns @em true if the value has been modified, @em false otherwise.
        /// @param win The game window where the slider should be drawed.
        /// @param text The text that should be drawn next to the slider.
        /// @param min_ The minimun value the widget can set.
        /// @param max_ The maximum value the widget can set.
        /// @param value The value that the widget can modify.
        /// @return bool
        bool hslider_float(Window *win, std::string_view text, float min_, float max_,
                           float *value) noexcept;

        /// @brief Draw a checkbox with @em text on the side of it.
        /// Returns @em true if modified, @em false otherwise.
        /// @param win The game window where the checkbox should be drawed.
        /// @param text The text that should be displayed next to the checkbox.
        /// @param value The value representing the state of the checkbox.
        /// @return bool
        bool checkbox(Window *win, std::string_view text, bool *value) noexcept;

        /// @brief Draw a menu button on the current window.
        /// @param text The name of the menu button.
        /// @return bool
        /// @sa tnt::ImGui::BeginMenuBar()
        /// @sa tnt::ImGui::EndMenuBar()
        /// @sa tnt::ImGui::menu_item()
        bool menu_button(Window *win, std::string_view text) noexcept;

        /// @brief Draw a menu bar with options from the @em options array.
        /// Returns the index of the pressed menu button (if any), or -1 otherwise.
        /// @param win The game window where the menu should be drawed.
        /// @param text The text to be shown on the item's button.
        /// @return bool
        /// @sa tnt::ImGui::BeginMenuBar()
        /// @sa tnt::ImGui::EndMenuBar()
        /// @sa tnt::ImGui::menu_button()
        bool menu_item(Window *win, std::string_view text) noexcept;

        // (maybe) return 1 if *value == max_

        /// @brief Draw a progress bar with text on the side of it.
        /// @param win The game window where the progress bar should be drawed.
        /// @param text The text that should be displayed next to the progress bar.
        /// @param min_ The minimum value of the progress bar.
        /// @param max_ The maximum value of the progress bar.
        /// @param value The value that shows the current progress bar index.
        void progress_bar(Window *win, std::string_view text, int min_, int max_,
                          int *value) noexcept;

        /// @brief Add an empty line on the current window.
        void newline() noexcept;

        /// @brief Draw @em text on the current window.
        /// @param win The game window where the text should be drawed.
        /// @param text The text that should be drawed.
        void text(Window *win, std::string_view text) noexcept;

        /// @brief Draw @em text with the specified color.
        /// @param win The game window where the text should be drawed.
        /// @param text The text that should be drawed.
        /// @param r The red "component" of the text color.
        /// @param g The green "component" of the text color.
        /// @param b The blue "component" of the text color.
        /// @param a The transparent "component" of the text color.
        void colored_text(Window *win, std::string_view text,
                          unsigned char r, unsigned char g,
                          unsigned char b, unsigned char a) noexcept;

        /// @brief Add a new item to the current list.
        /// @param win The game window where the list item should be drawed.
        /// @param text The text of the list item.
        /// @sa tnt::ImGui::BeginList()
        /// @sa tnt::ImGui::EndList()
        void list_item(Window *win, std::string_view text) noexcept;
    } // namespace ImGui
} // namespace tnt

/// @brief Initialize all needed stuff to use ImGui on @em win.
/// @param win The target game window.
void tnt_imgui_init(tnt::Window *win) noexcept;

/// @brief Cleans up all ImGui-related stuff.
void tnt_imgui_close() noexcept;

#endif //! TNT_IMGUI_LIB_HPP