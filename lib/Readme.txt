# 2015/03/29 version 1.1 by cyd AT bupt.edu.cn
# 2015/02/03 initial version by cyd AT bupt.edu.cn
本目录下是经过编译后生成的库文件。

目录结构如下：
其中的“xx”代表vc的版本号，比如vc2013，“xx”代表“13”
其中的“xxx”代表mingw的版本号，比如mingw4.7.2，“xxx”代表“4.7.2”

\-+---vc20xx                   //这是用于VC++的库文件
  |      |
  |      +----graphics.lib   //这是用于win32编译环境的release版本的库文件（不带调试信息）
  |      |
  |      \----amd64
  |            +----graphics64.lib   //这是用于x64编译环境的release版本的库文件（不带调试信息）
  |
  +---TDM-GCC-64                //使用TDM MinGW64 (gcc 4.9.2)编译出来的库文件
  |       +----libgraphics64.a  //这是用于64位编译环境的库文件(g++ -m64)
  |       +----libgraphics.a    //这是用于32位编译环境的库文件（g++ -m32, 64位编译器编译32位程序）
  +---mingw32-x.x.x
          +----libgraphics.a    //这是使用MinGW32 (gcc x.x.x)编译出来的32位库文件
		  
说明1: 一般情况下，请使用用于win32编译环境的release版本的库文件即可。
说明2: 目前（2015/02)所使用的mingw来源如下
      mingw-4.7.2：   来自于Dev-Cpp_5.5.3_MinGW_4.7.2_Setup.exe内嵌mingw
	  mingw-4.8.1：   来自于Dev-Cpp 5.6.0 MinGW 4.8.1 Setup.exe内嵌mingw
	  (已弃用)mingw64-4.9.2： 来自于mingw-w64-bin-x86_64-20150102.7z (http://www.drangon.org/mingw/)
	                  按照这个drangon的说法，该包中使用的是MinGW64工程的3.3.0版本。
					  这里为了习惯，我将它按照gcc版本命名
	                  但是这个版本有问题，只能编译64位库文件。
					  当使用-m32编译32位库文件时，报告错误：找不到cstdio中包含的<bits/c++conf
      tdm64-gcc-4.9.2-3.exe：dev-c++的最新版本都用tdm发行的mingw。这个很好用。
	                  但是该环境中包含64位和32位两个编译环境。
					  如果dev-c++与该版本mingw集成时，要注意编译器版本选择，否则一定会报告链接错误