#ifndef TNT_IMGUI_LIB_HPP
#define TNT_IMGUI_LIB_HPP

#define IMGEN_WIDGET_ID(N) (((__LINE__ << 8) | ((N)&0xFFFF)) ^ ((long long)&__FILE__))

// TODO: seems that slider_* doesn't support setting a minimum. Fix that.
// TODO: more widgets:
// image, checkbox, radio, text field, horizontal slider
// selection box
// window with Begin() and End()
// vertical and horizontal slider
// etc...
// TODO: better widget id generator.
// TODO: load from config file.
// TODO: keyboard navigation.
// TODO: find a way to return the widgets color from the get_* functions.
// TODO: store the context on the header file so that there is no need to pass it on every function.

// TODO(maybe):
// move slider's modified value before min/max ??
// slider_double, slider_uint8 ??
// draw ImGui widgets straight from tnt::Window ??

namespace tnt
{
class Window;

namespace ImGui
{
/////////////
// context //
/////////////

void make_context(int w, int h) noexcept;
void update_context() noexcept;
void destroy_context() noexcept;

/////////////
// widgets //
/////////////

int button(Window *win, int id, int x, int y) noexcept;

int slider_int(Window *win, int id, int x, int y,
               int min, int max, int &value) noexcept;
int slider_float(Window *win, int id, int x, int y,
                 float min, float max, float &value) noexcept;
int slider_byte(Window *win, int id, int x, int y,
                unsigned char min, unsigned char max, unsigned char &value) noexcept;

// TODO: implement these
int hslider_int(Window *win, int id, int x, int y,
                int min, int max, int &value) noexcept;
int hslider_float(Window *win, int id, int x, int y,
                  float min, float max, float &value) noexcept;
int hslider_byte(Window *win, int id, int x, int y,
                 unsigned char min, unsigned char max, unsigned char &value) noexcept;

} // namespace ImGui
} // namespace tnt

void tnt_imgui_begin() noexcept;
void tnt_imgui_finish() noexcept;

#endif //!TNT_IMGUI_LIB_HPP