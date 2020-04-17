#ifndef TNT_IMGUI_LIB_HPP
#define TNT_IMGUI_LIB_HPP

#define IMGEN_WIDGET_ID(N) (((__LINE__ << 8) | ((N)&0xFF)) ^ ((long long)&__FILE__))

#define IMGUI_ID IMGEN_WIDGET_ID(1)

#include <string_view>

// TODO: more widgets:
// image, checkbox, radio, text field, horizontal slider
// selection box
// window with Begin()/End() to replace tnt_imgui_begin()/finish()
// vertical and horizontal slider etc...
// TODO: better widget id generator. (maybe) by using hash(win_title + widget text)
// TODO: load from config file.
// TODO: add support for moving and collapsing windows.

// TODO(maybe):
// move slider's modified value before min/max ??
// slider_double, slider_uint8 ??
// draw ImGui widgets straight from tnt::Window ??
// keyboard navigation ??

namespace tnt
{
    class Window;

    namespace ImGui
    {
        // warning: this enum is not fully functional yet.
        enum class WindowFlags : int
        {
            Default               = 0, // doesn't have any of the flags above
            NonCollapsible        = 1, // without a collapse button
            NonClosable           = 2, // without a close button
            NonResizable          = 3, // without a resize button
            NonMovable            = 4, // can't be moved
            NoTitleBar            = 5, // doesn't have a title bar
            WithoutMenu           = 6, // doesn't have a menu
            TransparentBackground = 7  // draw only text and widgets, without background
        };

        //////////////////////////
        // operator overloading //
        //////////////////////////

        inline WindowFlags operator|(WindowFlags lhs, WindowFlags rhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(static_cast<type>(lhs) | static_cast<type>(rhs));
        }

        inline WindowFlags &operator|=(WindowFlags &lhs, WindowFlags rhs) noexcept
        {
            lhs = lhs | rhs;
            return lhs;
        }

        inline WindowFlags operator&(WindowFlags lhs, WindowFlags rhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(static_cast<type>(lhs) & static_cast<type>(rhs));
        }

        inline WindowFlags &operator&=(WindowFlags &lhs, WindowFlags rhs) noexcept
        {
            lhs = lhs & rhs;
            return lhs;
        }

        inline WindowFlags operator^(WindowFlags lhs, WindowFlags rhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(static_cast<type>(lhs) ^ static_cast<type>(rhs));
        }

        inline WindowFlags &operator^=(WindowFlags &lhs, WindowFlags rhs) noexcept
        {
            lhs = lhs ^ rhs;
            return lhs;
        }

        inline WindowFlags operator~(WindowFlags const &lhs) noexcept
        {
            using type = std::underlying_type_t<WindowFlags>;
            return static_cast<WindowFlags>(~static_cast<type>(lhs));
        }

        /////////////
        // context //
        /////////////

        void make_context() noexcept;
        void update_context() noexcept;
        void destroy_context() noexcept;

        /////////////
        // widgets //
        /////////////

        bool Begin(Window *win, std::string_view name, int x, int y,
                   WindowFlags flags = WindowFlags::Default) noexcept;
        void End() noexcept;

        bool button(Window *win, long long id, std::string_view text, int x, int y) noexcept;

        bool slider_int(Window *win, long long id, int x, int y, int min_, int max_,
                        int *value) noexcept;
        bool slider_float(Window *win, long long id, int x, int y, float min_, float max_,
                          float *value) noexcept;

        bool hslider_int(Window *win, long long id, int x, int y, int min_, int max_,
                         int *value) noexcept;
        bool hslider_float(Window *win, long long id, int x, int y, float min_, float max_,
                           float *value) noexcept;

        int menu(Window *win, long long id, int x, int y, std::string *first,
                 std::string *last) noexcept;

        // (maybe) return 1 if *value == max_
        void progress_bar(Window *win, long long id, int x, int y, int min_, int max_,
                          int *value) noexcept;

    } // namespace ImGui
} // namespace tnt

void tnt_imgui_init(tnt::Window *win) noexcept;
void tnt_imgui_close() noexcept;

#endif //! TNT_IMGUI_LIB_HPP