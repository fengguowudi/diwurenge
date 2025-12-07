#include "draw.h"
#include <thread>
#include <cstdint>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <string>
#include <cmath>
#include <string_view>
#include <unordered_set>
#include <mutex>
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <ctime>
#include <cstdlib>
#include "Name.h"
#include "千叶.h"
#include "ThreadAffinity.h"
extern char extractedString[64];
extern std::atomic<int> pid;
static bool MemuSwitch = true;
static bool voice = true;
namespace GlobalMemory {
static uintptr_t libbase{};
static uintptr_t Arrayaddr{};
static long int Count{};
static uintptr_t Matrix{};
static uintptr_t 自身{};
static int 自身阵营{};
static int 数量{};
static uintptr_t 自身坐标{};
static long int MatrixOffset = 0;
static long int ArrayaddrOffset = 0;
static int 状态{};
static const char* libso = "libclient.so";
static long int ModulePagesCount = 0;
}
float 过滤矩阵[17]{};
float matrix[16]{};
static bool show_draw_MarktheSoul = true;
static bool show_draw_EnhancedFrame = true;
static bool show_draw_Line = false;
static bool show_draw_Camera = true;
static bool show_draw_QY = true;
static bool show_draw_sender = true;
static bool show_draw_Animal = true;
static bool show_draw_Name = true;
static bool show_draw_Distance = true;
static bool show_draw_Cellar = true;
static bool show_draw_Chair = false;
static bool show_draw_BANZI = false;
static bool show_draw_BoxItem = false;
static bool show_draw_Prop = true;
static bool show_draw_prophet = true;
static bool Debugging = false;
static bool 编号 = false;
static uintptr_t 红夫人{}, 红夫人镜像{}, 镜子{};
static bool redqueenmod = false;
static bool mirror_active = false;
static bool is_self_redqueen = false;
struct Vector3A {
float X{}, Y{}, Z{};
constexpr Vector3A() = default;
constexpr Vector3A(float x, float y, float z) noexcept : X(x), Y(y), Z(z) {}
};
static Vector3A 镜子_pos{};
inline ImVec2 operator-(const ImVec2& a, const ImVec2& b) noexcept {
return {a.x - b.x, a.y - b.y};
}
inline bool _skipClass(std::string_view cn) noexcept {
static const std::unordered_set<std::string_view> exact_filters = {
"buzz", "nvyao.gim", "creature"
};
if (exact_filters.count(cn) > 0) return true;
static const std::vector<std::string_view> fuzzy_filters = {
"ttds_hu.gim", "butterflyfx.gim",
"shuimu", "_box", "girl_page", "haitun", "feie", "huohuli",
"_ghost.gim", "happy_dm65_weapon_saw1.gim", "butcher_lod", "sender01a.gim"
};
for (const auto& keyword : fuzzy_filters) {
if (cn.find(keyword) != std::string_view::npos) return true;
}
return false;
}
static Vector3A 红夫人_pos{}, 红夫人镜像_pos{};
struct DataStruct {
uintptr_t obj{};
uintptr_t objcoor{};
int 阵营{};
char str[256]{};
char 类名[256]{};
};
struct AdvancedFilter {
int jxpd;
int a8;
float f150;
float x, y, z;
};
static ImVec2 circle_pos{-1.0f, -1.0f};
static ImFont* g_font_ui{};
static float circle_radius = 60.0f;
static float margin = 10.0f;
static std::string 过滤类名, 类名;
static char gwd1[25];
static const float 距离比例 = 11.886f;
static float xs_prime_mirror{}, ys_prime_mirror{}, xs_final{}, ys_final{};
template<typename T>
inline T SnapToPixel(T v) noexcept {
return static_cast<T>(std::lrint(v));
}
static uint32_t orientation = static_cast<uint32_t>(-1);
ANativeWindow* window{};
android::ANativeWindowCreator::DisplayInfo displayInfo{};
ImGuiWindow* g_window{};
int abs_ScreenX{}, abs_ScreenY{};
int native_window_screen_x{}, native_window_screen_y{};
std::unique_ptr<AndroidImgui> graphics{};
static ImFont* g_main_font{};
static ImFont* g_ui_font{};
static Vector3A D, Z, M;
static float z_x{}, z_y{}, z_z{}, d_x{}, d_y{}, d_z{}, camera, r_x{}, r_y{}, r_w{};
static float X1{}, Y1{}, X2{}, Y2{}, W{}, H{};
static int 距离{};
static char objtext[256]{};
static char 监管者预知[1024]{};
static float px{}, py{};
std::vector<DataStruct> data_buffers[2];
std::atomic<int> front_buffer_idx = 0;
std::mutex data_mutex;
std::unordered_map<uintptr_t, int> objectIdMapWithSelection;
static int 椅子距离 = 40;
static int 板子距离 = 40;
static int 箱子距离 = 30;
enum CoordOffsetType {
TYPE_50,
TYPE_E0
};
inline Vector3A getObjectCoordinates(uintptr_t coorBase, CoordOffsetType type, bool isProp) noexcept {
Vector3A pos{};
if (coorBase) {
if (type == TYPE_50) {
pos.X = getFloat(coorBase + 0x50);
pos.Z = getFloat(coorBase + 0x54);
pos.Y = getFloat(coorBase + 0x58);
} else {
pos.X = getFloat(coorBase + 0xe0);
pos.Z = getFloat(coorBase + 0xe4);
pos.Y = getFloat(coorBase + 0xe8);
}
if (isProp) {
pos.Z -= 8.5f;
}
}
return pos;
}
char libso[256] = {"libclient.so"};
int 数据获取状态{};
namespace FastMath {
inline float fastDistanceSquared(const Vector3A& a, const Vector3A& b) noexcept {
float dx = a.X - b.X;
float dy = a.Y - b.Y;
float dz = a.Z - b.Z;
return dx * dx + dy * dy + dz * dz;
}
inline float fastDistance(const Vector3A& a, const Vector3A& b) noexcept {
return std::sqrt(fastDistanceSquared(a, b));
}
}
inline void calculate_mirror_reflection(float x1, float y1, float x2, float y2, float xs, float ys, float& xs_prime, float& ys_prime) noexcept {
float xm = (x1 + x2) / 2.0;
float ym = (y1 + y2) / 2.0;
xs_prime = 2.0 * xm - xs;
ys_prime = 2.0 * ym - ys;
}
inline void calculate_line_reflection(float x1, float y1, float x2, float y2, float xs, float ys, float& xs_prime, float& ys_prime) noexcept {
const float A = y2 - y1;
const float B = x1 - x2;
const float C = x2 * y1 - x1 * y2;
const float D = A * xs + B * ys + C;
const float denom = A * A + B * B;
if (std::abs(denom) < 1e-6) return;
xs_prime = xs - 2.0f * A * D / denom;
ys_prime = ys - 2.0f * B * D / denom;
}
inline bool optimizedWorldToScreen(const Vector3A& worldPos, const float* matrix, float px, float py, float& screenX, float& screenY, float& screenW) noexcept {
const float w = matrix[3] * worldPos.X + matrix[7] * worldPos.Z + matrix[11] * worldPos.Y + matrix[15];
if (w <= 0.1f) return false;
const float invW = 1.0f / w;
screenX = px + (matrix[0] * worldPos.X + matrix[4] * worldPos.Z + matrix[8] * worldPos.Y + matrix[12]) * invW * px;
screenY = py - (matrix[1] * worldPos.X + matrix[5] * (worldPos.Z + 8.5f) + matrix[9] * worldPos.Y + matrix[13]) * invW * py;
screenW = py - (matrix[1] * worldPos.X + matrix[5] * (worldPos.Z + 28.5f) + matrix[9] * worldPos.Y + matrix[13]) * invW * py;
return true;
}
bool complete_self_identification() {
Z.X = getFloat(GlobalMemory::自身坐标);
Z.Z = getFloat(GlobalMemory::自身坐标 + 4) - 10;
Z.Y = getFloat(GlobalMemory::自身坐标 + 8);
const auto& current_data = data_buffers[front_buffer_idx.load()];
std::lock_guard<std::mutex> lock(data_mutex);
for (const auto& item : current_data) {
if (!item.objcoor) continue;
if (item.阵营 != 1 && item.阵营 != 2) {
continue;
}
float currentX = getFloat(item.objcoor + 0x50);
float currentY = getFloat(item.objcoor + 0x58);
float currentZ = getFloat(item.objcoor + 0x54);
if (std::abs(currentX - Z.X) < 0.1f && std::abs(currentY - Z.Y) < 0.1f && std::abs(currentZ - Z.Z) < 5.0f) {
GlobalMemory::自身 = item.obj;
GlobalMemory::自身阵营 = item.阵营;
return true;
}
}
return false;
}
static bool fonts_initialized = false;
void init_My_drawdata() {
if (fonts_initialized) return;
ImGuiIO& io = ImGui::GetIO();
const float base = std::min(abs_ScreenX, abs_ScreenY);
const float fontSize = std::sqrt(base) * 0.91f;
const float uiFontSize = fontSize * 1.32f;
if (!g_main_font) {
ImGui::My_Android_LoadSystemFont(fontSize);
g_main_font = io.Fonts->Fonts.back();
}
if (!g_ui_font) {
ImGui::My_Android_LoadSystemFont(uiFontSize);
g_ui_font = io.Fonts->Fonts.back();
}
g_font_ui = g_ui_font;
fonts_initialized = true;
}
void screen_config() {
static android::ANativeWindowCreator::DisplayInfo lastDisplayInfo{};
displayInfo = android::ANativeWindowCreator::GetDisplayInfo();
if (lastDisplayInfo.width != displayInfo.width || lastDisplayInfo.height != displayInfo.height || lastDisplayInfo.orientation != displayInfo.orientation) {
lastDisplayInfo = displayInfo;
fonts_initialized = false;
}
}
void drawBegin() {
if (orientation != displayInfo.orientation) {
orientation = displayInfo.orientation;
Touch::setOrientation(displayInfo.orientation);
if (g_window) {
const ImVec2 halfSize(g_window->Size.x * 0.5f, g_window->Size.y * 0.5f);
g_window->Pos.x = displayInfo.width * 0.5f - halfSize.x;
g_window->Pos.y = displayInfo.height * 0.5f - halfSize.y;
circle_pos.x = displayInfo.width * 0.5f;
circle_pos.y = displayInfo.height * 0.5f;
}
}
}
inline bool isValidCoordinate(const Vector3A& pos) noexcept {
return !(pos.X == 0.0f || pos.Y == 0.0f || pos.Z <= -300.0f || pos.Z > 1000.0f ||
std::fabs(pos.X) > 10000.0f || std::fabs(pos.Y) > 10000.0f);
}
inline bool isValidScreenPosition(float x, float y, float width, float height) noexcept {
return !(x < -width || x > displayInfo.width + width || y < -height || y > displayInfo.height + height ||
width <= 0.0f || height <= 0.0f || width > displayInfo.width * 2 || height > displayInfo.height * 2);
}
inline constexpr ImColor
红色(255,  50,  50, 255),  绿色( 50, 255,  50, 255), 蓝色( 50, 150, 255, 255),
黄色(255, 255,  50, 255),  紫色(200, 100, 255, 255), 黑色(  0,   0,   0, 255),
亮红色(255, 50, 50, 255),   白色(255, 255, 255, 255),
密码机色(255, 255, 100, 255),  板子色(255, 255, 255, 255), 箱子色(255, 255, 255, 255),
椅子色(255, 100, 100, 255),   地窖色(200,   0, 255, 255), 编号色(100, 150, 255, 255);
inline void DrawTriangle(ImDrawList* Draw, float centerX, float centerY, ImColor color, int distance, float thickness = 2.0f) noexcept {
if (color == 白色 && show_draw_MarktheSoul) {
float size = std::clamp(15.0f - (distance / 20.0f), 5.0f, 15.0f);
size = std::min(size, W * 0.3f);
ImVec2 p1 = {centerX, centerY - size};
ImVec2 p2 = {centerX - size * 0.866f, centerY + size * 0.5f};
ImVec2 p3 = {centerX + size * 0.866f, centerY + size * 0.5f};
Draw->AddTriangle(p1, p2, p3, color, thickness);
}
}
inline void DrawEnhancedFrame(ImDrawList* Draw, float x1, float y1, float x2, float y2, ImColor color, int distance) noexcept {
const float currentWidth = x2 - x1;
const float currentHeight = y2 - y1;
const float cornerRadius = std::clamp(std::min(currentWidth, currentHeight) * 0.15f, 2.0f, 8.0f);
const float thickness = std::clamp(2.5f - (distance / 100.0f), 1.2f, 2.5f);
Draw->AddRect({x1, y1}, {x2, y2}, color, cornerRadius, ImDrawFlags_RoundCornersAll, thickness);
}
struct ModuleBssInfo {
unsigned long addr{};
unsigned long taddr{};
};
ModuleBssInfo get_module_bss(int pid, const char* module_name) {
ModuleBssInfo info{};
char filename[64];
std::snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
std::unique_ptr<FILE, decltype(&fclose)> fp(std::fopen(filename, "r"), &fclose);
if (!fp) return info;
char line[1024]{};
bool found_module = false;
while (std::fgets(line, sizeof(line), fp.get())) {
if (std::strstr(line, module_name)) found_module = true;
if (found_module) {
long addr{}, taddr{};
if (std::sscanf(line, "%lx-%lx", &addr, &taddr) != 2) continue;
if (std::strstr(line, "rw") && std::strlen(line) < 86 && (taddr - addr) / 4096 >= 2800) {
char* words[10]{};
int numWords = 0;
char* tok = std::strtok(line, " ");
while (tok && numWords < 10) {
words[numWords++] = tok;
tok = std::strtok(nullptr, " ");
}
for (int i = 0; i < numWords; ++i) {
if (std::sscanf(words[i], "%lx-%lx", &info.addr, &info.taddr) == 2) {
return info;
}
}
return {};
}
}
}
return {};
}
ModuleBssInfo get_module_bssgjf(int pid, const char* module_name) {
ModuleBssInfo info{};
char filename[64];
std::snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
std::unique_ptr<FILE, decltype(&fclose)> fp(std::fopen(filename, "r"), &fclose);
if (!fp) return info;
char line[1024]{};
bool is = false;
while (std::fgets(line, sizeof(line), fp.get())) {
long addr{}, taddr{};
if (std::sscanf(line, "%lx-%lx", &addr, &taddr) != 2) continue;
if (std::strstr(line, module_name) && std::strstr(line, "r-xp") && (taddr - addr) == 114982912) is = true;
if (is && std::strstr(line, "rw") && !std::feof(fp.get()) && std::strlen(line) < 86) {
if ((taddr - addr) / 4096 <= 3000) continue;
if (std::sscanf(line, "%lx-%lx", &info.addr, &info.taddr) == 2) break;
return {};
}
}
return info;
}
int get_name_pid1(const char* packageName) {
int id = -1;
std::unique_ptr<DIR, decltype(&closedir)> dir(::opendir("/proc"), &closedir);
if (!dir) return -1;
struct dirent* entry;
char filename[64], cmdline[64];
while ((entry = ::readdir(dir.get()))) {
id = std::atoi(entry->d_name);
if (id <= 0) continue;
std::snprintf(filename, sizeof(filename), "/proc/%d/cmdline", id);
std::unique_ptr<FILE, decltype(&fclose)> fp(std::fopen(filename, "r"), &fclose);
if (!fp) continue;
if (std::fgets(cmdline, sizeof(cmdline), fp.get())) {
if ((std::strstr(cmdline, packageName) || std::strstr(cmdline, "com.netease.idv")) &&
std::strstr(cmdline, "com") && !std::strstr(cmdline, "PushService") && !std::strstr(cmdline, "gcsdk")) {
std::snprintf(extractedString, sizeof(extractedString), "%s", cmdline);
return id;
}
}
}
return -1;
}
long getModuleBasegjf(int pid, const char* module_name) {
char filename[64];
std::snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
std::unique_ptr<FILE, decltype(&fclose)> fp(std::fopen(filename, "r"), &fclose);
if (!fp) return 0;
char line[1024]{};
while (std::fgets(line, sizeof(line), fp.get())) {
if (std::strstr(line, "r-xp") && std::strstr(line, module_name)) {
long addr{}, taddr{};
if (std::sscanf(line, "%lx-%lx", &addr, &taddr) == 2 && (taddr - addr) == 114982912) {
fclose(fp.get());
return addr;
}
}
}
return 0;
}
void ProcessObjectWithFullDetails(ImDrawList* Draw, const DataStruct& item,
const Vector3A& raw, float r_x, float r_y,
float W, float H, int distance) {
if (Debugging && distance <= 30) {
char objtext[64];
std::snprintf(objtext, sizeof(objtext), "%lx", item.obj);
const int jxpd = getDword(item.obj + 0x78);
const int a8 = getDword(item.obj + 0xa8);
const float f150 = getFloat(item.obj + 0x150);
char debugText[256];
std::snprintf(debugText, sizeof(debugText), "[%d m] jxpd=%d A8=%d 150=%.1f 0x%s %s", distance, jxpd, a8, f150, objtext, item.类名);
const auto sz = ImGui::CalcTextSize(debugText, nullptr, 18);
Draw->AddText({r_x - sz.x * 0.5f, r_y - sz.y - 2}, ImColor(255, 255, 255), debugText);
}
auto optimizedDrawText = [&](const char* label, ImColor color, float offsetY = 0) {
const auto sz = ImGui::CalcTextSize(label, nullptr, 20);
const float txtX = SnapToPixel(r_x - sz.x * 0.5f);
const float txtY = SnapToPixel(r_y + offsetY);
Draw->AddText({txtX, txtY}, color, label);
};
if (std::strstr(item.类名, "camera") && distance < 38 &&
getDword(item.obj + 0xa8) != 256 && show_draw_Camera &&
std::strstr(监管者预知, "[约瑟夫]") != nullptr) {
optimizedDrawText("[摄影机]", ImColor(255, 255, 255));
}
else if (std::strstr(item.类名, "trap.gim") &&
getDword(item.obj + 0xa8) != 256 && show_draw_QY) {
optimizedDrawText("[陷阱]", ImColor(255, 50, 50));
}
else if ((std::strstr(item.类名, "sender") || std::strstr(item.类名, "dm65_scene_sender")) &&
!(getDword(item.obj + 0xa8) == 256 && getFloat(item.obj + 0x150) == 0.0f) &&
show_draw_sender && distance <= 100) {
optimizedDrawText("[密码机]", 密码机色);
char distText[32];
std::snprintf(distText, sizeof(distText), "%d m", distance);
optimizedDrawText(distText, 密码机色, ImGui::GetTextLineHeight());
}
else if (std::strstr(item.类名, "polun_jiazi.gim") &&
getDword(item.obj + 0xa8) != 256 && show_draw_QY) {
optimizedDrawText("[夹子]", ImColor(255, 50, 50));
}
else if (item.阵营 == 3) {
if (std::strstr(item.类名, "h55_sleepingtown3_jpcat01low") && distance < 38 && show_draw_Animal) {
optimizedDrawText("哈基米", ImColor(255, 50, 50));
} else if (std::strstr(item.类名, "h55_playground_lion") && distance < 38 && show_draw_Animal) {
optimizedDrawText("狮子", ImColor(255, 50, 50));
} else if (std::strstr(item.类名, "dm65_scene_prop_76") && show_draw_Cellar) {
char cellarText[32];
std::snprintf(cellarText, sizeof(cellarText), "[地窖] %d m", distance);
optimizedDrawText(cellarText, 地窖色);
} else if ((std::strstr(item.类名, "dm65_scene_prop_01") ||
std::strstr(item.类名, "christmasbox01") ||
std::strstr(item.类名, "halloweenbox01")) &&
show_draw_BoxItem && distance <= 箱子距离) {
char boxText[32];
std::snprintf(boxText, sizeof(boxText), "[道具箱] %d m", distance);
optimizedDrawText(boxText, 箱子色);
} else if ((std::strstr(item.类名, "dm65_scene_gallows") ||
std::strstr(item.类名, "dm65_scene_gallows_hx_low")) &&
show_draw_Chair && distance <= 椅子距离) {
char chairText[32];
std::snprintf(chairText, sizeof(chairText), "[椅子] %d m", distance);
optimizedDrawText(chairText, 椅子色);
} else if ((std::strstr(item.类名, "woodplane001") ||
std::strstr(item.类名, "woodplane01")) &&
show_draw_BANZI && distance <= 板子距离) {
char banziText[32];
std::snprintf(banziText, sizeof(banziText), "[板子] %d m", distance);
optimizedDrawText(banziText, 板子色);
}
}
else if (show_draw_Prop && item.阵营 == 4) {
const char* name = nullptr;
if (std::strstr(item.类名, "h55_pendant_inject")) name = "[镇静剂]";
else if (std::strstr(item.类名, "h55_pendant_moshubang")) name = "[魔术棒]";
else if (std::strstr(item.类名, "h55_pendant_flaregun")) name = "[信号枪]";
else if (std::strstr(item.类名, "h55_pendant_huzhou")) name = "[护肘]";
else if (std::strstr(item.类名, "h55_pendant_map")) name = "[地图]";
else if (std::strstr(item.类名, "h55_pendant_book")) name = "[书]";
else if (std::strstr(item.类名, "h55_pendant_gjx")) name = "[工具箱]";
else if (std::strstr(item.类名, "h55_pendant_glim")) name = "[手电筒]";
else if (std::strstr(item.类名, "h55_pendant_xiangshuiping")) name = "[忘忧之香]";
else if (std::strstr(item.类名, "h55_pendant_controller")) name = "[遥控器]";
else if (std::strstr(item.类名, "h55_pendant_football")) name = "[橄榄球]";
else if (std::strstr(item.类名, "h55_pendant_huaibiao")) name = "[怀表]";
else if (std::strstr(item.类名, "h55_pendant_puppet")) name = "[厂长傀儡]";
else if (std::strstr(item.类名, "h55_pendant_tower")) name = "[窥视者]";
else if (std::strstr(item.类名, "huojian")) name = "[哭丑的浮木]";
else if (std::strstr(item.类名, "h55_pendant_banqiu")) name = "[板球]";
else if (std::strstr(item.类名, "h55_pendant_pig")) name = "[野猪or虫群]";
else if (std::strstr(item.类名, "h55_pendant_maildog")) name = "哈基汪";
else if (std::strstr(item.类名, "h55_pendant_patro")) name = "[巡视者]";
else if (std::strstr(item.类名, "bianzi")) name = "牛鞭";
else if (std::strstr(item.类名, "h55_pendant_owl")) name = "鸟";
else if (std::strstr(item.类名, "h55_pendant_wushu_xiao")) name = "棍";
else if (std::strstr(item.类名, "h55_pendant_bow")) name = "弓";
else if (std::strstr(item.类名, "h55_prop_tieqiao")) name = "铁锹";
else if (std::strstr(item.类名, "h55_pendant_dxzh_toukui")) name = "头盔";
else if (std::strstr(item.类名, "h55_pendant_qx")) name = "气象瓶";
else if (std::strstr(item.类名, "h55_pendant_gouzhua_e")) name = "钩爪";
if (name) {
char propText[64];
std::snprintf(propText, sizeof(propText), "%s %d m", name, distance);
optimizedDrawText(propText, ImColor(255, 255, 0));
}
}
int zy = 0;
vm_readv(item.obj + 0xaa, &zy, 1);
if (getFloat(item.obj + 0x150) == 450.0f) {
if (std::strstr(item.类名, "h55_joseph_camera") ||
std::strstr(item.类名, "redqueen_mirror") ||
std::strstr(item.类名, "burke_console") ||
std::strstr(item.类名, "h55_survivor_w_shangren_tiaoban")) {
return;
}
if (GlobalMemory::自身 == item.obj) {
return;
}
if (item.阵营 == 1 || item.阵营 == 2) {
if (show_draw_Name) {
const auto sz = ImGui::CalcTextSize(item.str, nullptr, 20);
Draw->AddText({X1 + W * 0.5f - sz.x * 0.5f, Y1 - 35}, ImColor(255, 255, 255), item.str);
}
if (编号 && item.阵营 == 2) {
if (!objectIdMapWithSelection.contains(item.obj)) {
objectIdMapWithSelection[item.obj] = static_cast<int>(objectIdMapWithSelection.size()) + 1;
}
const std::string indexStr = std::to_string(objectIdMapWithSelection[item.obj]);
const auto sz = ImGui::CalcTextSize(indexStr.c_str(), nullptr, 20);
Draw->AddText({X1 + W * 0.5f - sz.x * 0.5f, Y1 - 55}, 编号色, indexStr.c_str());
}
const int jxpd = getDword(item.obj + 0x78);
const ImColor clr = (jxpd == 65150) ? 白色 : (item.阵营 == 1) ? 亮红色 : 绿色;
if (show_draw_EnhancedFrame) {
DrawEnhancedFrame(Draw, X1, Y1, X2, Y2, clr, distance);
DrawTriangle(Draw, X1 + W * 0.5f, Y1 + H * 0.5f, clr, distance);
}
if (show_draw_Distance) {
char distText[32];
std::snprintf(distText, sizeof(distText), "%d m", distance);
const auto sz = ImGui::CalcTextSize(distText, nullptr, 20);
Draw->AddText({X1 + W * 0.5f - sz.x * 0.5f, Y2 + 2}, ImColor(235, 235, 235), distText);
}
if (show_draw_Line) {
const float lineX = SnapToPixel(X1 + W * 0.5f);
const float lineY = SnapToPixel(Y1);
Draw->AddLine({px, 160.0f}, {lineX, lineY}, ImColor(255, 255, 255), 2);
}
}
}
}
void Draw_Main_Optimized(ImDrawList* Draw) {
complete_self_identification();
const uintptr_t camBase = getPtr64(GlobalMemory::libbase + GlobalMemory::MatrixOffset);
if (!camBase) return;
const uintptr_t camObj = getPtr64(camBase + 0x98);
if (!camObj) return;
GlobalMemory::Matrix = getPtr64(camObj + 0x380);
M.X = getFloat(GlobalMemory::Matrix - 0x290);
M.Z = getFloat(GlobalMemory::Matrix - 0x290 + 4);
M.Y = getFloat(GlobalMemory::Matrix - 0x290 + 8);
GlobalMemory::Matrix = getPtr64(getPtr64(getPtr64(GlobalMemory::libbase + GlobalMemory::MatrixOffset) + 0x98) + 0x10) + 0x180;
memset(matrix, 0, sizeof(matrix));
vm_readv(GlobalMemory::Matrix - 4, 过滤矩阵, 17 * sizeof(float));
if (过滤矩阵[0] == 1.0f) {
for (int i = 0; i < 16; ++i) {
matrix[i] = (i <= 3 && 过滤矩阵[i + 1] >= -2.0f && 过滤矩阵[i + 1] <= 2.0f) ?
过滤矩阵[i + 1] : 过滤矩阵[i + 1];
}
}
if (show_draw_prophet && 监管者预知[0] != '\0') {
const auto textSize = ImGui::CalcTextSize(监管者预知, nullptr, 25);
Draw->AddText({px - textSize.x * 0.5f, 130}, 红色, 监管者预知);
}
const auto& current_data = data_buffers[front_buffer_idx.load()];
const int maxDrawCount = static_cast<int>(current_data.size());
for (int i = 0; i < maxDrawCount; ++i) {
const auto& item = current_data[i];
if (_skipClass(item.类名)) continue;
const uintptr_t coorBase = item.objcoor;
if (!coorBase) continue;
const int jxpd_global = getDword(item.obj + 0x78);
if (jxpd_global == 65278) {
continue;
}
Vector3A raw;
CoordOffsetType offsetType = TYPE_E0;
bool isProp = false;
if (std::strstr(item.类名, "spjk") || std::strstr(item.类名, "huojian")) {
offsetType = TYPE_E0;
} else if (std::strstr(item.类名, "player") || std::strstr(item.类名, "boss")) {
offsetType = TYPE_50;
} else if (std::strstr(item.类名, "prop")) {
offsetType = TYPE_E0;
isProp = true;
} else if (std::strstr(item.类名, "mirror_model_obj") || std::strstr(item.类名, "redqueen_mirror")) {
offsetType = TYPE_E0;
}
raw = getObjectCoordinates(coorBase, offsetType, isProp);
if (!isValidCoordinate(raw)) continue;
const float distSquared = FastMath::fastDistanceSquared(raw, Z);
const float maxDist = 300.0f * 距离比例;
if (distSquared > maxDist * maxDist) continue;
距离 = static_cast<int>(FastMath::fastDistance(raw, Z) / 距离比例);
float r_x, r_y, r_w;
if (!optimizedWorldToScreen(raw, matrix, px, py, r_x, r_y, r_w)) continue;
W = (r_y - r_w) * 0.5f;
H = r_y - r_w;
X1 = SnapToPixel(r_x - W * 0.5f);
Y1 = SnapToPixel(r_y - H * 0.5f);
X2 = SnapToPixel(X1 + W);
Y2 = SnapToPixel(Y1 + H);
if (!isValidScreenPosition(r_x, r_y, W, H)) continue;
ProcessObjectWithFullDetails(Draw, item, raw, r_x, r_y, W, H, 距离);
}
if (redqueenmod && is_self_redqueen) {
bool hasRealMirror = mirror_active && 红夫人 && 红夫人镜像;
bool hasFakeMirror = 镜子 != 0;
if (!hasRealMirror && !hasFakeMirror) {
return;
}
for (int i = 0; i < maxDrawCount; ++i) {
const auto& item = current_data[i];
if (getFloat(item.obj + 0x150) != 450.0f || !std::strstr(item.类名, "player")) continue;
uintptr_t coordPtr = getPtr64(item.obj + 0x40);
if (!coordPtr) continue;
Vector3A raw = getObjectCoordinates(coordPtr, TYPE_50, false);
if (!isValidCoordinate(raw)) continue;
if (hasRealMirror) {
Vector3A reflected_pos = raw;
float prime_x, prime_y;
calculate_mirror_reflection(红夫人_pos.X, 红夫人_pos.Y, 红夫人镜像_pos.X, 红夫人镜像_pos.Y, reflected_pos.X, reflected_pos.Y, prime_x, prime_y);
calculate_line_reflection(红夫人_pos.X, 红夫人_pos.Y, 红夫人镜像_pos.X, 红夫人镜像_pos.Y, prime_x, prime_y, reflected_pos.X, reflected_pos.Y);
int reflected_distance = static_cast<int>(FastMath::fastDistance(reflected_pos, Z) / 距离比例);
float r_x, r_y, r_w;
if (!optimizedWorldToScreen(reflected_pos, matrix, px, py, r_x, r_y, r_w)) continue;
W = (r_y - r_w) * 0.5f; H = r_y - r_w;
X1 = SnapToPixel(r_x - W * 0.5f); Y1 = SnapToPixel(r_y - H * 0.5f);
X2 = SnapToPixel(X1 + W); Y2 = SnapToPixel(Y1 + H);
if (!isValidScreenPosition(r_x, r_y, W, H)) continue;
if (show_draw_Name) {
const auto sz = ImGui::CalcTextSize(item.str, nullptr, 20);
Draw->AddText({X1 + W * 0.5f - sz.x * 0.5f, Y1 - 35}, 白色, item.str);
}
const int jxpd = getDword(item.obj + 0x78);
const ImColor clr = (jxpd == 65150) ? 白色 : 紫色;
if (show_draw_EnhancedFrame) {
DrawEnhancedFrame(Draw, X1, Y1, X2, Y2, clr, reflected_distance);
DrawTriangle(Draw, X1 + W * 0.5f, Y1 + H * 0.5f, clr, reflected_distance);
}
if (show_draw_Distance) {
char distText[32];
std::snprintf(distText, sizeof(distText), "%d m", reflected_distance);
const auto sz = ImGui::CalcTextSize(distText, nullptr, 20);
Draw->AddText({X1 + W * 0.5f - sz.x * 0.5f, Y2 + 2}, 白色, distText);
}
if (show_draw_Line) {
const float lineX = SnapToPixel(X1 + W * 0.5f);
const float lineY = SnapToPixel(Y1);
Draw->AddLine({px, 160.0f}, {lineX, lineY}, 紫色, 2);
}
}
if (hasFakeMirror) {
Vector3A reflected_hunter_pos;
reflected_hunter_pos.X = 2.0f * 镜子_pos.X - Z.X;
reflected_hunter_pos.Y = 2.0f * 镜子_pos.Y - Z.Y;
Vector3A reflected_pos = raw;
float prime_x, prime_y;
calculate_mirror_reflection(Z.X, Z.Y, reflected_hunter_pos.X, reflected_hunter_pos.Y, reflected_pos.X, reflected_pos.Y, prime_x, prime_y);
calculate_line_reflection(Z.X, Z.Y, reflected_hunter_pos.X, reflected_hunter_pos.Y, prime_x, prime_y, reflected_pos.X, reflected_pos.Y);
int reflected_distance = static_cast<int>(FastMath::fastDistance(reflected_pos, Z) / 距离比例);
float r_x, r_y, r_w;
if (!optimizedWorldToScreen(reflected_pos, matrix, px, py, r_x, r_y, r_w)) continue;
W = (r_y - r_w) * 0.5f; H = r_y - r_w;
X1 = SnapToPixel(r_x - W * 0.5f); Y1 = SnapToPixel(r_y - H * 0.5f);
X2 = SnapToPixel(X1 + W); Y2 = SnapToPixel(Y1 + H);
if (!isValidScreenPosition(r_x, r_y, W, H)) continue;
if (show_draw_Name) {
const auto sz = ImGui::CalcTextSize(item.str, nullptr, 20);
Draw->AddText({X1 + W * 0.5f - sz.x * 0.5f, Y1 - 35}, 白色, item.str);
}
const int jxpd = getDword(item.obj + 0x78);
const ImColor clr = (jxpd == 65150) ? 白色 : 白色;
if (show_draw_EnhancedFrame) {
DrawEnhancedFrame(Draw, X1, Y1, X2, Y2, clr, reflected_distance);
DrawTriangle(Draw, X1 + W * 0.5f, Y1 + H * 0.5f, clr, reflected_distance);
}
if (show_draw_Distance) {
char distText[32];
std::snprintf(distText, sizeof(distText), "%d m", reflected_distance);
const auto sz = ImGui::CalcTextSize(distText, nullptr, 20);
Draw->AddText({X1 + W * 0.5f - sz.x * 0.5f, Y2 + 2}, 白色, distText);
}
if (show_draw_Line) {
const float lineX = SnapToPixel(X1 + W * 0.5f);
const float lineY = SnapToPixel(Y1);
Draw->AddLine({px, 160.0f}, {lineX, lineY}, 白色, 2);
}
}
}
}
}
struct ObjectPrimaryData {
uintptr_t objcoor_ptr;
char pad1[0x38];
int jxpd;
int a8;
char pad2[0x44];
float f150;
};
void read_thread(long int PD1, long int PD2, long int PD3) {
Timer data_timer("DataThread");
data_timer.BindCurrentThreadToCores(false, "DataThread");
pid = get_name_pid1("dwrg");
get_name_pid(extractedString);
ModuleBssInfo result;
if (std::strstr(extractedString, "com.netease.idv")) {
GlobalMemory::libbase = getModuleBasegjf(pid, ".");
result  = get_module_bssgjf(pid, ".");
} else {
GlobalMemory::libbase = getModuleBase(const_cast<char*>(GlobalMemory::libso));
result  = get_module_bss(pid, GlobalMemory::libso);
}
GlobalMemory::ModulePagesCount = (result.taddr - result.addr) / 4096;
std::vector<long> buff(512);
while (GlobalMemory::MatrixOffset == 0 || GlobalMemory::ArrayaddrOffset == 0) {
GlobalMemory::状态 = 1;
for (long int i = 0; i < GlobalMemory::ModulePagesCount; i++) {
vm_readv(result.addr + (i * 4096), buff.data(), 0x1000);
for (int ii = 0; ii < 512; ii += 1) {
if (buff[ii] == 16384) {
int tempsz = getDword(result.addr + 4096 * i + 8 * ii + 0x28);
int temps = getDword(result.addr + 4096 * i + 8 * ii + 0xA8);
if (temps == 257 && tempsz == 1 && getPtr64(result.addr + i * 4096 + ii * 8 + 0x70) == (result.addr + i * 4096 + ii * 8 + 0x78)) {
GlobalMemory::ArrayaddrOffset = result.addr - GlobalMemory::libbase + i * 4096 + ii * 8 + 0x88;
}
int tempszz = getDword(result.addr + 4096 * i + 8 * ii + 0x28);
if (tempszz == 1 && getFloat(result.addr + 4096 * i + 8 * ii + 0x30) == -1) {
GlobalMemory::自身坐标 = result.addr + 4096 * i + 8 * ii + 0x34;
}
}
if (buff[ii] == 4566433208965459695 &&
getDword(result.addr + 4096 * i + 8 * ii + 0xC) == 0 &&
getDword(result.addr + 4096 * i + 8 * ii + 0x20) == 257) {
uintptr_t ptr1 = getPtr64(result.addr + 4096 * i + 8 * ii + 0x10);
if (!ptr1) continue;
uintptr_t ptr2 = getPtr64(ptr1 + 0x98);
if (!ptr2) continue;
uintptr_t ptr3 = getPtr64(ptr2 + 0x10);
if (!ptr3) continue;
if (getFloat(ptr2 + 0x7C) == 1 &&
getFloat(ptr2 + 0xBC) == 1 &&
getFloat(ptr2 + 0x12C) == 1 &&
getFloat(ptr3 + 0x17C) == 1) {
GlobalMemory::MatrixOffset = result.addr - GlobalMemory::libbase + i * 4096 + ii * 8 + 0x10;
}
}
}
}
if (GlobalMemory::MatrixOffset != 0 && GlobalMemory::ArrayaddrOffset != 0) {
GlobalMemory::状态 = 2;
break;
}
}
int back_buffer_idx = 1;
while (true) {
auto& local_data = data_buffers[back_buffer_idx];
local_data.clear();
GlobalMemory::Arrayaddr = getPtr64(GlobalMemory::libbase + GlobalMemory::ArrayaddrOffset);
uintptr_t tail = getPtr64(GlobalMemory::libbase + GlobalMemory::ArrayaddrOffset + 8);
GlobalMemory::Count = (tail <= GlobalMemory::Arrayaddr || (tail - GlobalMemory::Arrayaddr) / 8 > 1000) ? 439 : (tail - GlobalMemory::Arrayaddr) / 8;
std::vector<uintptr_t> object_pointers(GlobalMemory::Count);
vm_readv(GlobalMemory::Arrayaddr, object_pointers.data(), GlobalMemory::Count * sizeof(uintptr_t));
std::vector<uintptr_t> valid_objects;
valid_objects.reserve(GlobalMemory::Count);
for(uintptr_t 对象 : object_pointers) {
if (对象 == 0) continue;
uintptr_t coorBase = getPtr64(对象 + 0x40);
if (!coorBase) continue;
AdvancedFilter filter{};
if (!vm_readv(对象 + 0x78, &filter.jxpd, 12)) continue;
filter.x = getFloat(coorBase + 0xe0);
filter.y = getFloat(coorBase + 0xe8);
filter.z = getFloat(coorBase + 0xe4);
if (filter.z <= -300.0f || filter.z > 1000.0f) continue;
valid_objects.push_back(对象);
}
bool found_hf_this_frame = false;
bool found_hfjx_this_frame = false;
bool found_fake_mirror_this_frame = false;
for (uintptr_t 对象 : valid_objects) {
uintptr_t namezfcz = 对象 + 0xe0;
namezfcz = getPtr64(namezfcz); if (!namezfcz) continue;
namezfcz = getPtr64(namezfcz); if (!namezfcz) continue;
namezfcz = getPtr64(namezfcz + 0x8);
namezfcz = getPtr64(namezfcz + 0x20);
namezfcz = getPtr64(namezfcz + 0x20);
namezfcz += 0x8;
int len = getDword(namezfcz + 0x8);
if (len >= 256) continue;
过滤类名.clear();
for (int i = 0; i < len; i += 24) {
vm_readv(getPtr64(namezfcz) + i, gwd1, 24);
gwd1[24] = '\0';
过滤类名 += gwd1;
}
if (_skipClass(过滤类名)) continue;
bool 过滤重复指针 = std::any_of(local_data.begin(), local_data.end(),
[&](const DataStruct& item) { return 对象 == item.obj; });
if (过滤重复指针) continue;
const char* cls = 过滤类名.c_str();
if (对象 == GlobalMemory::自身) {
if (std::strstr(cls, "boss") && std::strstr(cls, "redqueen") && !std::strstr(cls, "mirror")) {
is_self_redqueen = true;
} else {
is_self_redqueen = false;
}
}
if (redqueenmod && is_self_redqueen) {
if (getFloat(对象 + 0x150) == 450.0f && std::strstr(cls, "boss") && std::strstr(cls, "redqueen") && !std::strstr(cls, "mirror")) {
if (对象 == GlobalMemory::自身) {
红夫人 = 对象;
found_hf_this_frame = true;
} else {
红夫人镜像 = 对象;
found_hfjx_this_frame = true;
}
}
if (std::strstr(cls, "mirror_model_obj_001") ||
std::strstr(cls, "mirror_model_obj_007") ||
std::strstr(cls, "h55_guajian_rongguang") ||
std::strstr(cls, "mirror_model_obj_005") ||
std::strstr(cls, "mirror_model_obj_002") ||
std::strstr(cls, "mirror_model_obj_003") ||
std::strstr(cls, "mirror_model_obj_004") ||
std::strstr(cls, "mirror_model_obj_006") ||
std::strstr(cls, "redqueen_e_krm_head")) {
镜子 = 对象;
found_fake_mirror_this_frame = true;
}
}
if (show_draw_prophet) {
int a8_value = getDword(对象 + 0xa8);
if ((a8_value != 0 || std::strstr(cls, "boss")) &&
!std::strstr(cls, "burke_console") && !std::strstr(cls, "h55_joseph_camera") &&
!std::strstr(cls, "redqueen_e_heijin_yizi") && !std::strstr(cls, "chuanhuo") &&
std::strstr(cls, "boss")) {
std::string s = getboss(cls);
std::snprintf(监管者预知, sizeof(监管者预知), "%s", s.c_str());
}
}
float pd1 = getFloat(对象 + 0x150);
if (std::strstr(cls, "player") || std::strstr(cls, "boss") || pd1 == 450.0f ||
std::strstr(cls, "scene") || std::strstr(cls, "prop") || std::strstr(cls, "mirror") || Debugging) {
DataStruct item{};
item.obj = 对象;
item.objcoor = getPtr64(对象 + 0x40);
if(!item.objcoor) continue;
if (std::strstr(cls, "boss")) {
std::strcpy(item.str, getboss(cls));
item.阵营 = 1;
} else if (std::strstr(cls, "player") ||
std::strstr(cls, "npc_deluosi_dress_ghost") ||
std::strstr(cls, "h55_pendant_huojian")) {
std::strcpy(item.str, getplayer(cls));
item.阵营 = 2;
} else if (std::strstr(cls, "scene")) {
std::strcpy(item.str, getscene(cls));
item.阵营 = 3;
} else if (std::strstr(cls, "prop")) {
std::strcpy(item.str, getprop(cls));
item.阵营 = 4;
}
std::strcpy(item.类名, cls);
local_data.push_back(item);
}
}
if (redqueenmod && is_self_redqueen) {
if (!found_hf_this_frame) 红夫人 = 0;
if (!found_hfjx_this_frame) 红夫人镜像 = 0;
if (!found_fake_mirror_this_frame) 镜子 = 0;
if (红夫人 && 红夫人镜像) {
uintptr_t hf_coor = getPtr64(红夫人 + 0x40);
uintptr_t hfjx_coor = getPtr64(红夫人镜像 + 0x40);
if (hf_coor && hfjx_coor) {
红夫人_pos = getObjectCoordinates(hf_coor, TYPE_50, false);
红夫人镜像_pos = getObjectCoordinates(hfjx_coor, TYPE_50, false);
mirror_active = isValidCoordinate(红夫人_pos) && isValidCoordinate(红夫人镜像_pos);
} else {
mirror_active = false;
}
} else {
mirror_active = false;
}
if (镜子) {
uintptr_t fake_mirror_coor = getPtr64(镜子 + 0x40);
if (fake_mirror_coor) {
镜子_pos = getObjectCoordinates(fake_mirror_coor, TYPE_E0, false);
if (!isValidCoordinate(镜子_pos)) {
镜子 = 0;
}
} else {
镜子 = 0;
}
}
} else {
红夫人 = 0;
红夫人镜像 = 0;
镜子 = 0;
mirror_active = false;
}
GlobalMemory::数量 = local_data.size();
front_buffer_idx.store(back_buffer_idx);
back_buffer_idx = 1 - back_buffer_idx;
static Timer fpsTimer;
fpsTimer.SetFps(5);
fpsTimer.ControlFps();
}
}
void Draw_Main(ImDrawList* Draw) {
Draw_Main_Optimized(Draw);
}
int 数据() {
DIR* dir = opendir("/dev/input/");
if (dir == NULL) return -1;
struct dirent* ptr = NULL;
int count = 0;
while ((ptr = readdir(dir)) != NULL) {
if (strstr(ptr->d_name, "event"))
count++;
}
closedir(dir);
return count ? count : -1;
}
void 处理输入事件(struct input_event ev) {
if (ev.type == EV_KEY && ev.value == 1) {
// 上音量键显示UI，下音量键隐藏UI
if (ev.code == KEY_VOLUMEUP) {
MemuSwitch = true;
} else if (ev.code == KEY_VOLUMEDOWN) {
MemuSwitch = false;
}
}
}
void 音量() {
int EventCount = 数据();
if (EventCount < 0) {
printf("未找到输入设备\n");
return;
}
int* fdArray = (int*)malloc(EventCount * sizeof(int));
fd_set fds;
struct timeval tv;
int maxfd = 0;
for (int i = 0; i < EventCount; i++) {
char temp[128];
sprintf(temp, "/dev/input/event%d", i);
fdArray[i] = open(temp, O_RDONLY | O_NONBLOCK);
if (fdArray[i] > maxfd)
maxfd = fdArray[i];
}
struct input_event ev;
while (true) {
FD_ZERO(&fds);
for (int i = 0; i < EventCount; i++) {
if (fdArray[i] >= 0)
FD_SET(fdArray[i], &fds);
}
tv.tv_sec = 1;
tv.tv_usec = 0;
int ret = select(maxfd + 1, &fds, NULL, NULL, &tv);
if (ret > 0) {
for (int i = 0; i < EventCount; i++) {
if (fdArray[i] >= 0 && FD_ISSET(fdArray[i], &fds)) {
memset(&ev, 0, sizeof(ev));
if (read(fdArray[i], &ev, sizeof(ev)) == sizeof(ev)) {
处理输入事件(ev);
}
}
}
}
usleep(5000);
}
for (int i = 0; i < EventCount; i++) {
if (fdArray[i] >= 0)
close(fdArray[i]);
}
free(fdArray);
}
void Layout_tick_UI(bool* main_thread_flag) {
if (!ImGui::GetCurrentContext() || !g_font_ui || !g_font_ui->IsLoaded()) return;
px = static_cast<float>(displayInfo.width) * 0.5f;
py = static_cast<float>(displayInfo.height) * 0.5f;
screen_config();
drawBegin();
Draw_Main_Optimized(ImGui::GetForegroundDrawList());
static float ui_anim_scale = 0.0f;
const float anim_speed_fast = 0.15f;
const float anim_speed_slow = 0.17f;
float target_scale = MemuSwitch ? 1.0f : 0.0f;
float animSpeed = (ui_anim_scale < target_scale) ? anim_speed_fast : anim_speed_slow;
float deltaTimeScaled = ImGui::GetIO().DeltaTime / animSpeed;
ui_anim_scale = ImLerp(ui_anim_scale, target_scale, deltaTimeScaled);
if (ui_anim_scale > 0.01f) {
ImDrawList* fg_draw_list = ImGui::GetForegroundDrawList();
const char* watermark_text = "「千叶」- Internal";
float watermark_font_size = 45.0f;
ImVec2 watermark_pos = ImVec2(80.0f, displayInfo.height - (watermark_font_size + 40.0f));
fg_draw_list->AddText(g_font_ui, watermark_font_size, watermark_pos, IM_COL32(255, 0, 0, 255), watermark_text);
ImGuiStyle& s = ImGui::GetStyle();
const float bakW = s.WindowRounding, bakF = s.FrameRounding, bakI = s.ItemSpacing.y;
const ImVec2 bakP = s.WindowPadding;
const float base = static_cast<float>(std::min(displayInfo.width, displayInfo.height));
const float g_density = std::clamp(std::sqrt(base) * 0.0045f, 0.8f, 2.0f);
const float win_w_final = std::min(base * 0.51f * 1.618f, displayInfo.width  * 0.9f);
const float win_h_final = std::min(win_w_final / 1.618f, displayInfo.height * 0.9f);
float anim_win_w = win_w_final * ui_anim_scale;
float anim_win_h = win_h_final * ui_anim_scale;
ImVec2 anim_win_pos((displayInfo.width - anim_win_w) * 0.5f, (displayInfo.height - anim_win_h) * 0.5f);
s.WindowRounding = 10.0f * g_density;
s.FrameRounding  = 6.0f  * g_density;
s.ItemSpacing.y  = 6.0f  * g_density;
s.WindowPadding  = ImVec2(14 * g_density, 10 * g_density);
ImGui::StyleColorsDark();
ImGui::GetStyle().ScrollbarSize = ImGui::GetTextLineHeight() * 0.75f;
ImGui::SetNextWindowPos(anim_win_pos, ImGuiCond_Always);
ImGui::SetNextWindowSize(ImVec2(anim_win_w, anim_win_h), ImGuiCond_Always);
ImGui::SetNextWindowBgAlpha(ui_anim_scale);
ImGui::Begin("「Made in heaven」——神秘人千叶", main_thread_flag, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);
ImGui::PushFont(g_font_ui);
const ImVec2 window_pos2 = ImGui::GetWindowPos();
const ImVec2 window_size = ImGui::GetWindowSize();
ImDrawList* draw_list = ImGui::GetWindowDrawList();
const float titlebar_height = ImGui::GetTextLineHeight();
const float rounding = s.WindowRounding;
draw_list->AddRectFilled(window_pos2, ImVec2(window_pos2.x + window_size.x, window_pos2.y + titlebar_height), ImGui::GetColorU32(ImGuiCol_WindowBg, 0), rounding, ImDrawFlags_RoundCornersTop);
const ImVec2 text_size_title = ImGui::CalcTextSize("「Made in heaven」——神秘人千叶");
draw_list->AddText(ImVec2(window_pos2.x + (window_size.x - text_size_title.x) * 0.5f, window_pos2.y + (titlebar_height - text_size_title.y) * 0.5f), ImGui::GetColorU32(ImGuiCol_Text), "「Made in heaven」——神秘人千叶");
static int current_tab = 0;
const ImVec2 size_status  = ImGui::CalcTextSize("状态信息");
const ImVec2 size_setting = ImGui::CalcTextSize("功能设置");
const ImVec2 size_about   = ImGui::CalcTextSize("关于");
const ImVec2 size_exit    = ImGui::CalcTextSize("退出脚本");
const float button_padding = 20.0f * g_density;
const float button_width  = std::max({size_status.x, size_setting.x, size_about.x, size_exit.x}) + button_padding;
const float button_height = ImGui::GetTextLineHeightWithSpacing() + button_padding * 0.5f;
const float button_y_start = titlebar_height + 10.0f * g_density;
const float button_spacing = s.ItemSpacing.y;
ImGui::SetCursorPos(ImVec2(10.0f * g_density, button_y_start));
if (ImGui::Button("状态信息", ImVec2(button_width, button_height))) current_tab = 0;
ImGui::SetCursorPos(ImVec2(10.0f * g_density, button_y_start + (button_height + button_spacing) * 1));
if (ImGui::Button("功能设置", ImVec2(button_width, button_height))) current_tab = 1;
ImGui::SetCursorPos(ImVec2(10.0f * g_density, button_y_start + (button_height + button_spacing) * 2));
if (ImGui::Button("关于", ImVec2(button_width, button_height))) current_tab = 2;
ImGui::SetCursorPos(ImVec2(10.0f * g_density, button_y_start + (button_height + button_spacing) * 3));
if (ImGui::Button("退出脚本", ImVec2(button_width, button_height))) *main_thread_flag = false;
const float divider_x = 10.0f * g_density + button_width + 10.0f * g_density;
const float divider_y_start = button_y_start;
const float divider_y_end   = button_y_start + button_height * 4 + button_spacing * 3;
draw_list->AddLine(ImVec2(divider_x, divider_y_start), ImVec2(divider_x, divider_y_end), IM_COL32(100, 100, 100, 255));
ImGui::SetCursorPos(ImVec2(divider_x + 10.0f * g_density, button_y_start));
ImGui::BeginChild("MainContent", ImVec2(window_size.x - divider_x - 20.0f * g_density, window_size.y - titlebar_height - 20.0f * g_density), true);
switch (current_tab) {
case 0:
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "系统信息");
ImGui::Separator();
ImGui::Text("渲染模式: %s", graphics->RenderName);
ImGui::Text("GUI 版本: %s", IMGUI_VERSION);
ImGui::Text("帧率: %.1f FPS (%.2f ms)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
ImGui::Spacing();
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "数据状态");
ImGui::Separator();
if (GlobalMemory::状态 == 2) ImGui::TextColored(ImVec4(0, 1, 0, 1), "✓ 数据已就绪");
else if (GlobalMemory::状态 == 1) ImGui::TextColored(ImVec4(1, 0.6f, 0, 1), ":) 正在初始化...");
else ImGui::TextColored(ImVec4(1, 0, 0, 1), "✗ 未获取数据");
ImGui::Spacing();
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "游戏信息");
ImGui::Separator();
ImGui::Text("进程 ID: %d", pid.load());
ImGui::SameLine();
ImGui::Text("包名: %s", extractedString);
ImGui::Spacing();
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "内存地址");
ImGui::Separator();
ImGui::Text("模块基址: 0x%lx", GlobalMemory::libbase);
ImGui::SameLine();
ImGui::Text("对象数组: 0x%lx", GlobalMemory::Arrayaddr);
ImGui::Text("矩阵地址: 0x%lx", GlobalMemory::Matrix);
ImGui::Spacing();
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "偏移与统计");
ImGui::Separator();
ImGui::Text("矩阵偏移: 0x%lx", GlobalMemory::MatrixOffset);
ImGui::SameLine();
ImGui::Text("数组偏移: 0x%lx", GlobalMemory::ArrayaddrOffset);
ImGui::Text("模块页数: %ld", GlobalMemory::ModulePagesCount);
ImGui::SameLine();
ImGui::Text("对象数量: %d", GlobalMemory::数量);
ImGui::Spacing();
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "角色与坐标");
ImGui::Separator();
ImGui::Text("监管者: %s", 监管者预知);
ImGui::Text("自身坐标: X:%.1f, Y:%.1f, Z:%.1f", Z.X, Z.Y, Z.Z);
break;
case 1:
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "绘制选项");
ImGui::Separator();
ImGui::Columns(2, nullptr, true);
ImGui::Checkbox("增强框体", &show_draw_EnhancedFrame);
ImGui::Checkbox("人物射线", &show_draw_Line);
ImGui::Checkbox("绘制相机", &show_draw_Camera);
ImGui::Checkbox("绘制名字", &show_draw_Name);
ImGui::Checkbox("绘制密码机", &show_draw_sender);
ImGui::Checkbox("红夫人模式", &redqueenmod);
ImGui::NextColumn();
ImGui::Checkbox("绘制距离", &show_draw_Distance);
ImGui::Checkbox("绘制动物", &show_draw_Animal);
ImGui::Checkbox("绘制地窖", &show_draw_Cellar);
ImGui::Checkbox("绘制道具", &show_draw_Prop);
ImGui::Checkbox("预知监管", &show_draw_prophet);
ImGui::Checkbox("标记灵魂体", &show_draw_MarktheSoul);
ImGui::Columns(1);
ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "场景对象距离");
ImGui::Separator();
ImGui::SliderInt("椅子距离", &椅子距离, 10, 100);
ImGui::SliderInt("板子距离", &板子距离, 10, 100);
ImGui::SliderInt("箱子距离", &箱子距离, 10, 100);
if (ImGui::Button("一键重置", ImVec2(button_width, button_height))) {
椅子距离 = 40;
板子距离 = 40;
箱子距离 = 30;
}
ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "场景对象开关");
ImGui::Separator();
ImGui::Checkbox("椅子", &show_draw_Chair);
ImGui::SameLine();
ImGui::Checkbox("板子", &show_draw_BANZI);
ImGui::SameLine();
ImGui::Checkbox("道具箱", &show_draw_BoxItem);
ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();
ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "功能开关");
ImGui::Separator();
ImGui::Checkbox("编号模式", &编号);
ImGui::SameLine();
ImGui::Checkbox("调试模式", &Debugging);
if (ImGui::Button("清理缓存", ImVec2(button_width, button_height))) {
std::lock_guard<std::mutex> lock(data_mutex);
data_buffers[0].clear();
data_buffers[1].clear();
GlobalMemory::数量 = 0;
监管者预知[0] = '\0';
objectIdMapWithSelection.clear();
}
break;
case 2:
ImGui::TextColored(ImVec4(1, 0, 0, 1), "二改屎山项目仅供学习交流24小时后删除");
ImGui::TextColored(ImVec4(1, 0, 0, 1), "倒卖本辅助死全家");
ImGui::Spacing();
ImGui::Text("作者: 神秘人千叶");
ImGui::TextColored(ImVec4(0, 0, 1, 1), "源码来源于t.me/laobafqcd智齿开源精神:)");
break;
}
ImGui::EndChild();
static bool is_dragging = false;
static ImVec2 drag_start_pos;
const ImVec2 mouse_pos_window = ImGui::GetMousePos();
const bool is_hover_title = (mouse_pos_window.x >= window_pos2.x) &&
(mouse_pos_window.x <= window_pos2.x + window_size.x) &&
(mouse_pos_window.y >= window_pos2.y) &&
(mouse_pos_window.y <= window_pos2.y + titlebar_height);
if (ImGui::IsMouseClicked(0) && is_hover_title) {
is_dragging = true;
drag_start_pos = mouse_pos_window - window_pos2;
}
if (is_dragging) {
if (ImGui::IsMouseDown(0)) {
ImVec2 new_pos = mouse_pos_window - drag_start_pos;
new_pos.x = std::clamp(new_pos.x, 0.0f, displayInfo.width  - window_size.x);
new_pos.y = std::clamp(new_pos.y, 0.0f, displayInfo.height - window_size.y);
ImGui::SetWindowPos(new_pos);
} else {
is_dragging = false;
}
}
s.WindowRounding = bakW;
s.FrameRounding  = bakF;
s.ItemSpacing.y  = bakI;
s.WindowPadding  = bakP;
g_window = ImGui::GetCurrentWindow();
ImGui::End();
ImGui::PopFont();
}
}
