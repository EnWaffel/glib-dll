#include "glib/utils/Utils.h"
#include "glib/math/MathFunctions.h"

#include <locale>
#include <codecvt>
#include <string>
#include <GLFW/glfw3.h>

using namespace glib;

std::wstring Utils::ToWString(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

Vec2 glib::Utils::GetMousePosition(Window* wnd)
{
    return GetMousePosition(wnd, wnd->GetDefaultCamera());
}

Vec2 glib::Utils::GetMousePosition(Window* wnd, Camera* camera)
{
    double xpos, ypos;
    float zoom = camera->zoom;
    glfwGetCursorPos((GLFWwindow*)wnd->GetHandle(), &xpos, &ypos);

    int width, height;
    glfwGetWindowSize((GLFWwindow*)wnd->GetHandle(), &width, &height);

    Vec2 viewportPos = wnd->GetViewportPos();
    Vec2 viewportSize = wnd->GetViewportSize();

    float mousePosInViewX = (float)((xpos - viewportPos.x) / (float)viewportSize.x);
    float mousePosInViewY = (float)((ypos - viewportPos.y) / (float)viewportSize.y);

    float mousePosInWindowX = mousePosInViewX * wnd->GetInitialSize().x;
    float mousePosInWindowY = mousePosInViewY * wnd->GetInitialSize().y;

    return Vec2(mousePosInWindowX, mousePosInWindowY);
}

bool glib::Utils::IsMouseTouchingSprite(Window* wnd, Sprite* sprite)
{
    return IsMouseTouchingSprite(wnd, wnd->GetDefaultCamera(), sprite);
}

bool glib::Utils::IsMouseTouchingSprite(Window* wnd, Camera* camera, Sprite* sprite)
{
    Vec2 mousePos = GetMousePosition(wnd, camera);
    Vec2 diff = Vec2((sprite->size.x * sprite->scale.x) - sprite->size.x, (sprite->size.y * sprite->scale.y) - sprite->size.y);
    return RectRectIntersection(sprite->pos - (diff / 2.0f), sprite->size + diff, mousePos, glib::Vec2(1.0f, 1.0f));
}
