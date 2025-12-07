#include <thread>
#include <memory>
#include "draw.h"
#include "AndroidImgui.h"
#include "GraphicsManager.h"
#include "Android_draw/ThreadAffinity.h"
extern void 音量();
char extractedString[64]{};
std::atomic<int> pid;
Timer DrawFPS;
float fps = 60;
long int value1 = 970061201, value2 = 16384, value3 = 257;
int main(int argc, char *argv[]) {
::graphics = GraphicsManager::getGraphicsInterface(GraphicsManager::VULKAN);
::screen_config();
::native_window_screen_x = (::displayInfo.height > ::displayInfo.width ? ::displayInfo.height : ::displayInfo.width);
::native_window_screen_y = ::native_window_screen_x;
::abs_ScreenX = ::native_window_screen_x;
::abs_ScreenY = (::displayInfo.height < ::displayInfo.width ? ::displayInfo.height : ::displayInfo.width);
::window = android::ANativeWindowCreator::Create("FuckNetEase", native_window_screen_x, native_window_screen_y,true);
graphics->Init_Render(::window, native_window_screen_x, native_window_screen_y);
Touch::Init({static_cast<float>(::abs_ScreenX), static_cast<float>(::abs_ScreenY)}, true);
Touch::setOrientation(displayInfo.orientation);
Timer draw_timer("DrawThread");
draw_timer.BindCurrentThreadToCores(true, "DrawThread");
std::thread(read_thread, value1, value2, value3).detach();
std::thread(音量).detach();
DrawFPS.SetFps(fps);
DrawFPS.InitFpsControl();
DrawFPS.GetCpuCoreCount();
::init_My_drawdata();
static bool flag = true;
while (flag) {
drawBegin();
graphics->NewFrame();
Layout_tick_UI(&flag);
graphics->EndFrame();
DrawFPS.SetFps(fps);
DrawFPS.ControlFps();
}
graphics->Shutdown();
android::ANativeWindowCreator::Destroy(::window);
return 0;
}