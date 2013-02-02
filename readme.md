EGE (Easy Graphics Engine)
===========================================================
HomePage1     http://misakamm.github.com/xege<br />
HomePage2     http://misakamm.bitbucket.org/index.htm<br />
HomePage3     http://tcgraphics.sourceforge.net<br />
HomePage4     http://tcgraphics.svn.sourceforge.net/viewvc/tcgraphics/index.htm<br />
mail list     https://lists.sourceforge.net/lists/listinfo/tcgraphics-faqs<br />
teiba1        http://tieba.baidu.com/f?kw=ege%C4%EF<br />
teiba2        http://tieba.baidu.com/f?kw=ege<br />
resource      http://code.google.com/p/misaka/downloads/list<br />
Blog:         http://blog.misakamm.org<br />
E-Mail:       mailto:misakamm[at gmail com]<br />
QQ:           moegirl[at misakamm org]<br />
详细帮助文档，在压缩包里的man目录下，用浏览器打开index.htm就可以看到了


EGE图形库
-----------------------------------------------------------

　　EGE（Easy Graphics Engine），是windows下的简易绘图库，是一个类似BGI(graphics.h)的面向C/C++语言新手的图形库，它的目标也是为了替代TC的BGI库而存在。它的使用方法与TC中的graphics.h相当接近，对新手来说，简单，友好，容易上手，免费开源，而且因为接口意义直观，即使是之前完全没有接触过图形编程的，也能迅速学会基本的绘图。 目前，EGE图形库已经完美支持VC6, VC2008, VC2010, C-Free, DevCpp, Code::Blocks, wxDev, Eclipse for C/C++等IDE，即支持使用MinGW为编译环境的IDE。如果你需要在VC下使用graphics.h，那么ege将会是很好的替代品。

为什么要写这个库？
-----------------------------------------------------------
　　许多学编程的都是从 C 语言开始入门的，而目前的现状是：<br />
　　1. 有些学校以 Turbo C 为环境讲 C 语言，只是 Turbo C 的环境实在太老了，复制粘贴都很不方便。并且dos环境在现在的操作系统支持很有限，并且dos下可用颜色数太少。<br />
　　2. 有些学校直接拿 VC 来讲 C 语言，因为 VC 的编辑和调试环境都很优秀，并且 VC 有适合教学的免费版本。可惜初学者在 VC 下一般只会做一些文字性的练习题，想画条直线画个圆都很难，还要注册窗口类、建消息循环等等，初学者会受严重打击的，甚至有初学者以为C只能在“黑框”下使用。<br />
　　3. 还有计算机图形学，这门课程的重点是绘图算法，而不是 Windows 编程。所以，许多老师不得不用 TC 教学，因为 Windows 绘图太复杂了，会偏离教学的重点。新的图形学的书有不少是用的 OpenGL，可是门槛依然很高。<br />
　　如果您刚开始学 C 语言，或者您是一位教C语言的老师，再或者您在教计算机图形学，那么这个库一定会让您兴奋的。采用ege图形库，您将可以在VC的环境中方便的处理和生成图像，甚至制作动画和游戏。<br />

ege图形库的优点<br />
　　★效率较好　特别在窗口锁定绘图模式下，640 x 480的半透明混合，可以直接使用 getpixel / putpixel 完成，并且优化后可以在大约1.5G CPU台式机器上达到60fps （60帧/秒）<br />
　　★灵活性强　绘图可以直接针对一个PIMAGE，或者画在控件上，不必只能画在屏幕上<br />
　　★功能更多　支持拉伸贴图，支持图片旋转，支持透明半透明贴图，支持图像模糊滤镜操作，可以用对话框函数进行图形化的输入，可以方便地对帧率进行准确的控制，可以读取常见的流行的图片格式（bmp/jpg/png），可以保存图片为bmp或png格式。<br />
　　★免费开源　本图形库为免费开源的图形库，你不但可以获取本图形库的全部源代码，你也可以参与到本图形库的开发，详情请联系作者misakamm

ege简要使用说明
-----------------------------------------------------------
　　目前模拟了绝大多数 BGI 的绘图函数。使用上，基本的绘图函数和 TC / BC 没太大区别。看一个画圆的例子吧：<br />
> \#include "graphics.h" // 就是需要引用这个图形库
> int main()
> {
>     initgraph(640, 480); // 初始化，显示一个窗口，这里和 TC 略有区别
>     circle(200, 200, 100); // 画圆，圆心(200, 200)，半径 100
>     getch(); // 暂停一下等待用户按键
>     closegraph(); // 关闭图形界面
>     return 0;
> }
　　呵呵，很简单吧。更详细的请参阅ege文档。

ege的将来
-----------------------------------------------------------
　　目前ege项目之下，有另一个项目Xege，x代表x-window，意为跨越支持x-window的平台（主要为linux系统）。Xege将发展为一个强大的，开源的，跨平台的，同时也同样简单易上手的图形库。目前Xege在开发中。
