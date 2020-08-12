# EGE 编译指南

EGE 源码使用 CMake 构建编译系统，以支持各种编译器和 IDE。

请在 [cmake.org](https://cmake.org) 下载最新版 CMake，并在安装时选择将 CMake 
目录添加到 `PATH` 环境变量中。本指南默认在 CMD 或者 PowerShell 命令行下进行编译，
但仍使用 `$` 作为提示符。如果您对 CMake 有足够的把握，亦可使用 CMake GUI 
进行配置和生成。

## 基本编译步骤

1. 创建 build 文件夹并设为当前目录
  ```sh
  $ mkdir build
  $ cd build
  ```

2. 执行 `cmake` 命令生成编译配置文件
  ```sh
  $ cmake .. [编译配置]
  ```

  `[编译配置]` 指定特定的编译平台，将在后文详述。

3. 进行编译
  ```sh
  $ cmake --build .
  ```

编译过程将在 `build` 目录下生成相应的静态库文件。

如果想在完成编译后使用其它编译器再次编译，请先清空 `build` 目录，CMD 命令为
```sh
$ for /D %d in (*) do @rmdir %d /S /Q
$ del * /S /Q
```
清空目录后再从步骤 2 继续执行。

## 编译配置

不同编译平台的差别主要是配置编译步骤的第二步。

### MinGW

```sh
$ cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
```

CMake 会自动检测安装的 MinGW 编译器并生成编译配置。

编译配置中的 `-DCMAKE_BUILD_TYPE=Release` 是构建类型（Build type），表示生成优化级别较高的
发布版。

如果想指定编译套件（如 Dev-C++ 自带的 TDM-GCC 4.9.2），可在执行 `cmake` 
前设置 `PATH` 环境变量指向特定的 MinGW 所在位置，CMake 在配置时会采用最先
在 PATH 中检测到的编译器进行编译。例如，Dev-C++ 安装目录为 `C:\Dev-Cpp`，则
在 CMD 中执行以下命令：
```cmd
$ set PATH="C:\Dev-Cpp\MinGW64\bin";%PATH%
```
在 PowerShell 中则是：
```ps
$ $env:PATH="C:\Dev-Cpp\MinGW64\bin;$env:PATH"
```

注意，CodeBloks 附带的 MinGW 只能在 
[MSYS Makefiles 配置](<#-msys-makefiles-配置>) 
下编译。在此建议您下载不附带 MinGW 的 CodeBlocks 并单独安装最新版 TDM-GCC64，
CodeBlocks 会自动识别已安装的 TDM-GCC。

#### 使用 64 位 MinGW 编译 32 位 EGE 库

以上步骤对 32 位与 64 位 MinGW 均适用，分别会产生对应 64 位和 32 位版本的 EGE 静态库。

64 位 MinGW 支持编译 32 位目标，要想达到这一效果，在以上步骤中设置 `PATH` 环境变量
之后，执行 `cmake` 之前需要设置 `CC` 和 `CXX` 环境变量。在 CMD 中命令为：
```cmd
$ set CC="gcc -m32"
$ set CXX="g++ -m32"
```
在 PowerShell 中为：
```ps
$ $env:CC="gcc -m32"
$ $env:CXX="g++ -m32"
```
之后再执行 `cmake .. -G "MinGW Makefiles"` 命令。

#### MSYS Makefiles 配置

如果您在使用 MSYS2 或 git-bash，您可以用 `MSYS Makefiles` 生成适合此类环境的编译系统。

所需要的命令和上面描述的没有区别，但需要把 CMD 或 PowerShell 命令换成 Bash 命令，
比如设置环境变量：
```sh
$ export PATH=/C/Dev-Cpp/MinGW64/bin:$PATH
```
相应的 CMake 生成指令是
```sh
$ cmake .. -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release
```

注意，您需要从 `pacman` 包管理器安装或者
[相关网站](https://sourceforge.net/projects/ezwinports/files/)
上下载 MSYS make 程序并将其复制到可执行路径如 `/usr/bin` 中。

扩展阅读：[Windows 上的编译系统](https://www.chirsz.cc/blog/2020-03/compile-sys-on-win.html)


### Visual C++ 6.0

CMake 自 3.6 后停止了对生成 VC6 项目的支持，但仍可生成 NMake 编译系统以支持 VC6 编译。

首先，检查你的 VC6 安装目录（以下用 `VC6PATH` 指称，如果是免安装版则对应解压出的 
`vc6` 文件夹的路径，这个路径应包含 `VC98` 和 `Common` 文件夹），在 
`VC6PATH\VC98\Bin` 文件夹中的 `VCVARS32.BAT` 文件中开始部分的内容应和 `VC6PATH` 
相一致，例如安装到 `D:\VC6` 的 VC6，在 `D:\VC6\VC98\Bin\VCVARS32.BAT` 中的开头部分
应当是：
```bat
rem Root of Visual Developer Studio installed files.
rem
set MSDevDir=D:\VC6\Common\msdev98

rem
rem Root of Visual C++ installed files.
rem
set MSVCDir=D:\VC6\VC98
```
不一致的情况常出现于免安装版 VC6，此时需修改 `VCVARS32.BAT` 内容使其与 VC6 实际所在
目录一致。

确认 `VCVARS32.BAT` 内容正确后，在 EGE 源码的 `src\build` 目录下执行此批处理文件，
在上面的例子中就是执行：
```sh
$ "D:\VC6\VC98\Bin\VCVARS32.BAT"
``` 
执行成功后即建立 VC6 命令行环境，就可以继续执行编译步骤二了：
```sh
$ cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release
```

编译配置中的 `-DCMAKE_BUILD_TYPE=Release` 是构建类型（Build type），表示生成优化级别较高的
发布版。

### Visual Studio

使用 `cmake -G` 命令查看支持的 Visual Studio 版本，选择自己安装的 VS 版本，
如“Visual Studio 14 2015”作为 `-G` 的参数传递给 CMake。可用 `-A` 参数指定
目标平台，默认为 32 位 x86 平台 `Win32`，可选的参数有 `x64`，`ARM`，`ARM64`。

因此编译 32 位 EGE 库需要执行：
```sh
$ cmake .. -G "Visual Studio 14 2015" -A Win32
```

编译 64 位 EGE 库：
```sh
$ cmake .. -G "Visual Studio 14 2015" -A x64
```

另外要注意的一点是，Visual Studio 的步骤三应该是：
```sh
$ cmake --build . --config Release
```

因为 Visual Studio 是多配置的编译系统，需要在执行编译时选择配置类型（Configuration type）。这
也是必须的选项，因为使用 `Release` 配置编译出的库文件才能同时被 `Debug` 和 `Release` 配置的
项目使用。

## 编译临时测试文件

有时候为了测试新添加的功能，需要写一些测试用例，但编译安装修改后 EGE 再在项目外编译测试程序
比较麻烦，在项目里修改 CMake 配置并添加源文件和编译指令又会被 git 识别为未暂存的修改，会对 git 
使用造成干扰。

项目 CMake 配置中已经写好了，开发者可以新建 `temp` 目录并添加源文件和 `CMakeLists.txt` 配置
文件，编译系统会自动配置，在编译 EGE 库后编译 `temp` 目录，而 temp 目录已被 `.gitignore` 
排除在外，不会对 git 使用造成干扰。

例如，我们想要测试读取字符输入，于是新建 `temp` 目录，在 `temp` 目录下
新建 `CMakeLists.txt` 内容如下：

```cmake
add_executable(temp_test temp_test.cpp)

target_link_libraries(temp_test xege)

```

新建 `temp/temp_test.cpp` 内容如下：

```cpp
#include <ege.h>

using namespace ege;

int main(int argc, char const *argv[])
{
  initgraph(640, 480);

  circle(120, 120, 100);

  int i = 0;
  while (is_run()) {
    key_msg msg = getkey();
    if (msg.msg == key_msg_char) {
      xyprintf(0, i * 20, "%d", msg.key);
      ++i;
    }
  }

  return 0;
}

```

执行前文所述编译步骤后在 `build/temp` 目录下就会生成可执行文件 `temp_test.exe`。
