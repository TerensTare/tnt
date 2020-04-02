#ifndef TNT_IMGUI_LIB_HPP
#define TNT_IMGUI_LIB_HPP

#define IMGEN_WIDGET_ID(N) (((__LINE__ << 8) | ((N)&0xFFFF)) ^ ((long long)&__FILE__))

namespace tnt
{
class Window;

namespace ImGui
{
/////////////
// context //
/////////////

struct context_t;

context_t *make_context(int w, int h) noexcept;
void update_context(context_t *ctx) noexcept;
void destroy_context(context_t *ctx) noexcept;

/////////////
// widgets //
/////////////

int button(context_t *ctx, Window *win, int id, int x, int y) noexcept;

} // namespace ImGui
} // namespace tnt

void tnt_imgui_begin(tnt::ImGui::context_t *ctx) noexcept;
void tnt_imgui_finish(tnt::ImGui::context_t *ctx) noexcept;

#endif //!TNT_IMGUI_LIB_HPP