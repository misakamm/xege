#pragma once

namespace ege
{

mouse_msg mouseMessageConvert(UINT message, WPARAM wParam, LPARAM lParam, int* keyCode = NULL);

}
