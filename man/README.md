# EGE（Easy Graphics Engine)

>**原作者**：misakamm

| 相关网站 | 网址 |
| --- | --- |
| 官网 | <https://xege.org> (含群讨论联系方式) |
| 源码 (Github) | <https://github.com/wysaid/xege> |
| 源码 (Gitee) | <https://gitee.com/xege/xege> |
| 社区 | <https://club.xege.org> |
| 教程以及介绍 | [EGE教程&介绍](https://blog.csdn.net/qq_39151563/article/details/100154767) (by [`依稀`](https://blog.csdn.net/qq_39151563?type=blog) ) |
| 在线文档页 | <http://xege.org/manual> |
| 原始作者misakamm主页 | ~~<http://misakamm.github.io/xege> (此页面不更新, 版本较老)~~ |
| ~~原始代码仓库(已暂停维护)~~ | ~~<https://github.com/misakamm/xege>~~ |

## 预编译库下载

| 站点名称 |下载链接 |
| --- | --- |
| EGE官网下载 | <https://xege.org/install_and_config> |
| Github | <https://github.com/wysaid/xege.org> |
| Gitee | <https://gitee.com/xege/xege.org> |
| 百度网盘 | <https://pan.baidu.com/s/1qWxAgeK> |

## 开发工具

在 Windows 上, EGE 支持`Visual Studio 6.0`(aka `vc6.0`), `Visual Studio 2010`, `Visual Studio 2015`, `Visual Studio 2017`, `Visual Studio 2019`, `Visual Studio 2022`(及以上版本), `MinGW`, `小熊猫C++`, `CLion`, `C-Free`, `Dev-C++`, `Code::Blocks` 等 IDE。

在 Linux 和 macOS 上, EGE 支持基于 mingw-w64 的交叉编译环境。支持基于 mingw-w64 编译的 Windows 程序在 Linux 和 macOS 上运行(需要 `Wine` 支持)。

在新版本中，特别增强了对于 `Visual Studio Code` 的支持， `Visual Studio Code` 使用者可以在插件市场直接搜索 `ege` 插件安装, 安装 ege 插件之后, 使用 `Visual Studio Code` 打开任意文件夹, 使用 `win + shift + p`(Windows/Linux) 或者 `cmd + shift + p`(macOS) 快捷键, 输入 `EGE: setup with this project` 等指令即可完成快速项目配置和运行。相关操作支持 Windows、Linux 和 macOS。

## 自己编译ege源代码

### 获取源码

请去上方源码下载页面直接下载源码， 或者使用 git 克隆源码仓库。

1. Github:  `git clone https://github.com/wysaid/xege --recursive`
2. Gitee(国内访问更快):  `git clone https://gitee.com/xege/xege --recursive`

### 编译源码

EGE 项目是一个 CMake 项目， 使用标准的 CMake 构建方式即可编译，熟手可以自行操作。

对于新手来说， 可以参考如下步骤：

1. 下载安装[CMake](https://cmake.org)
2. 任意编译器, 推荐安装 Visual Studio 2022 以上的版本以获得最好的体验。以 `Visual Studio 2022` 为例， 在 Windows 商店中， 直接搜索 `Visual Studio Installer` 安装， 之后启动 `Visual Studio Installer` 选择 C++ 开发环境并安装即可。
3. 启动 `cmake-gui.exe`, 选择源码目录和编译目录，点击 `Configure` 按钮， 选择你的编译器， 然后点击 `Generate` 按钮生成项目文件。之后点击 `Open Project` 打开项目文件， 然后在 Visual Studio 中编译即可。

## 库教程和文档目录（注意：本文档对应的ege版本为 24.04

* [基本说明](http://xege.org/manual/intro/index.htm)
* [图形库安装说明](http://xege.org/manual/setup/index.htm)
* [使用预览](http://xege.org/manual/preview/index.htm)
* [入门基础示例和教程](http://xege.org/manual/tutorial/index.htm)
* [库函数目录](http://xege.org/manual/api/index.htm)
* [示例程序](http://xege.org/manual/demo/index.htm)
* [网友小程序（新！有兴趣可投稿哦！）](http://xege.org/manual/netfriendsdemo/index.htm)
* [关于本图形库](http://xege.org/manual/about/index.htm)

> 如果以上目录文字显示较小，请用Ctrl+鼠标滚轮调整文字大小，或者用你的浏览器设置修改即可
