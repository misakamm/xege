# Version 20.08

## 新特性

- 库文件名统一为 `graphics[64].lib` 或 `libgraphics[64].a`
- 初始化图形对象或调用 `resize` 时，支持将图形对象的长或宽设置为 0
- 增加 `INIT_UNICODE` 初始化选项，此选项会创建 Unicode 窗口
- 增加 `ege::seticon` 函数，可通过资源 ID 设置窗口图标
- 增加 `ege::ege_drawtext`，支持绘制文字时使用 Alpha 通道混合，呈现半透明效果
- 增加 `putpixel_withalpha` 和 `putpixel_withalpha_f` 函数，支持带透明通道绘制像素点
- 允许在 `initgraph` 前调用 `newimage` 创建图形对象
- 支持加载资源中 PNG 格式图片
- 使 `getkey` 可返回 `key_msg_char` 类型的消息，现在 EGE 支持读取输入法输入了
- 允许在调用 `initgraph` 前设置窗口标题和图标

## 修复 BUG

- 修正 `putimage` 系列函数裁剪区计算错误的 BUG
- 修复了 `initgraph` 的内存泄漏情况
- 修复了 `setactivepage()` 和 `setvisualpage()` 无法使用的问题
- 修正 `putpixel` 等函数颜色格式错误
- 修正关于线型设置的 BUG
- 修正某些函数传入 `NULL` 时段错误的 BUG

## 其它

- 使用 CMake 作为编译系统
- 从 `ege.h` 中移出 `EgeControlBase` 的定义到 `ege/egecontrolbase.h` 中
- 默认字体设置为宋体
- 使用 `PCIMAGE` 作为 `const IMAGE*` 的别名，并作为某些函数的形参类型
- 生成的静态库文件中不再包含 gdiplus 静态库
- 改用误差更小的 Alpha 通道混合算法
- 增加之前缺少的键码
- `resize` 不再用默认背景色清空图像
- 修改了 `initgraph` 的接口定义
