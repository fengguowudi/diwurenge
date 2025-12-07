#include <unistd.h>
#include "my_imgui.h"
#include "imgui_internal.h"
namespace ImGui {
ImFont* SystemFont = NULL;
bool My_Android_LoadSystemFont(float SizePixels)
{
const char *dirs[] = {
"/system/fonts",
"/system/font",
"/data/fonts"
};
const char *fonts[] = {
"NotoSansCJK-Regular.ttc",
"NotoSerifCJK-Regular.ttc",
"DroidSansFallback.ttf",
"NotoSansSC-Regular.otf",
"NotoSansTC-Regular.otf"
};
char path[256];
for (auto dir : dirs)
{
if (access(dir, R_OK) != 0) continue;          // 目录不可读 → 下个目录
for (auto file : fonts)
{
snprintf(path, sizeof(path), "%s/%s", dir, file);
if (access(path, R_OK) != 0) continue;   // 文件不可读 → 下个字体
// 文件可读 → 直接加载
ImFontConfig config;
config.FontDataOwnedByAtlas = false;
config.SizePixels           = SizePixels;
config.OversampleH          = 1;
// 关键：加载失败返回 nullptr，就继续下一个字体
ImFont *tryFont = My_AddFontFromFileTTF(path, 0, &config, nullptr);
if (tryFont)
{
SystemFont = tryFont;
return true;
}
// 加载失败 → 自动继续循环
}
}
// 所有目录+字体都试过且失败
IM_ASSERT_USER_ERROR(0, "All system fonts failed to load!");
return false;
}
ImFont* My_AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges) {
ImGuiIO &io = ImGui::GetIO();
IM_ASSERT(!Locked && "Cannot modify a locked ImFontAtlas between NewFrame() and EndFrame/Render()!");
size_t data_size = 0;
void* data = ImFileLoadToMemory(filename, "rb", &data_size, 0);
if (!data) {
IM_ASSERT_USER_ERROR(0, "Could not load font file!");
return NULL;
}
ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig();
if (font_cfg.Name[0] == '\0') {
// Store a short copy of filename into into the font name for convenience
const char* p;
for (p = filename + strlen(filename); p > filename && p[-1] != '/' && p[-1] != '\\'; p--) {}
ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "%s, %.0fpx", p, size_pixels);
}
ImFont *local_Font = io.Fonts->AddFontFromMemoryTTF(data, (int)data_size, size_pixels, &font_cfg, glyph_ranges);
free(data);
data = NULL;
return local_Font;
}
}