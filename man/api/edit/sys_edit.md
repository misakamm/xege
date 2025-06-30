<style>
    h2, h3 {
        display:inline-block;
    }
    drop {
        cursor: pointer;
        display: block;
        background: #090;
    }
    input[type="checkbox"]  {
        display: none; /* hide the checkboxes */
    }
    input +.drop +  div    {
        display:none;
    }
    .drop:after {
        content:'▼';
    }
    :checked  + .drop:after {
        content:'▲';
    }
    input:checked + .drop + div{
        display:block;
    }
</style>


## sys_edit 类
 >头文件包含: **#include <ege/sys_edit.h>**

>继承自: **egeControlBase**

### 函数
#### 构造函数
使用默认参数即可
```c++
sys_edit(int inherit = inherit_level_e, ege::egeControlBase* pParent = NULL);
```

#### 成员函数

<input id="create" type="checkbox"> 
<label class="drop" for="create">create()&emsp;&emsp;&emsp; 创建输入框</label>
<div>

**声明**
```cpp
int create(bool multiline = false, int scrollbar = 2);</code>
```
**功能**
创建输入框。

**参数**
* multiline
输入框是否是多行,true表示多行输入框，false表示单行输入框。
* scrollbar
未实现，暂保留。

**返回值**
0
</div></br>

<input id="move" type="checkbox"> 
<label class="drop" for="move">move() &emsp;&emsp;&emsp; 移动输入框</label>
<div>

**声明**
```cpp
void move(int x, int y);
```
**功能**
移动输入框到目标位置。

**参数**
x, y
位置坐标。

**返回值**
（无）
</div></br>

<input id="size" type="checkbox"> 
<label class="drop" for="size">size()&emsp;&emsp;&emsp;&emsp; 设置输入框大小</label>
<div>

**声明**
```c++
void size(int w, int h)
```
**功能**
设置输入框大小。

**参数**
w, h
输入框宽高。

**返回值**
（无）
</div></br>

<input id="visable" type="checkbox"> 
<label class="drop" for="visable">visable()&emsp;&emsp;&emsp;设置可见性</label>
<div></br>

**声明**
```c++
void visible(bool bvisable);
```
**功能**
设置输入框可见性。

**参数**
bvisable
可见性，ture 表示可见，false 表示不可见

**返回值**
（无）
</div></br>

<input id="setfocus" type="checkbox"> 
<label class="drop" for="setfocus">setfocus() &emsp;&emsp; 获取焦点</label>
<div>

**声明**
```c++
void setfocus();
```
**功能**
设置输入框获取焦点。

**参数**
(无)

**返回值**
（无）
</div></br>

<input id="gettext" type="checkbox"> 
<label class="drop" for="gettext">gettext()&emsp;&emsp;&emsp; 获取输入框内容</label>
<div>

**声明**
```c++
void gettext(int maxlen, LPSTR text);   //字符型

void gettext(int maxlen, LPWSTR text);  //宽字符型
```
**功能**
获取输入框内容，写入字符缓存区 text 中，内容超出maxlen时截断。

**参数**
maxlen
缓存区最大可容纳字符数，包括末尾结束符。

text
字符串缓存区首地址。

**返回值**
（无）
</div></br>

<input id="settext" type="checkbox"> 
<label class="drop" for="settext">settext()&emsp;&emsp;&emsp; 设置输入框内容</label>
<div>

**声明**
```c++
void settext(LPCSTR text);  //字符型

void settext(LPCWSTR text); //宽字符型
```
**功能**
设置输入框内容。

**参数**
text
要设置的内容

**返回值**
（无）
</div></br>

<input id="getx" type="checkbox"> 
<label class="drop" for="getx">getx()&emsp;&emsp;&emsp;&emsp; 获取输入框左上角x坐标</label>
<div>

**声明**
```c++
int getx();
```
**功能**
获取输入框左上角的x坐标。

**参数**
（无)

**返回值**
输入框左上角的x坐标。
</div></br>

<input id="gety" type="checkbox"> 
<label class="drop" for="gety">gety()&emsp;&emsp;&emsp;&emsp; 获取输入框左上角y坐标</label>
<div>

**声明**
```c++
int gety();
```
**功能**
获取输入框左上角y坐标。

**参数**
（无）

**返回值**
输入框左上角的y坐标。
</div></br>

<input id="getw, width" type="checkbox"> 
<label class="drop" for="getw, width">getw() , width() 获取输入框的宽度</label>
<div>

**声明**
```c++
int getw();
int width();
```
**功能**
获取输入框的宽度。

**参数**
（无）

**返回值**
输入框的宽度。
</div></br>

<input id="geth, height" type="checkbox"> 
<label class="drop" for="geth, height">geth() , height() 获取输入框的高度</label>
<div>

##### geth(), height()
**声明**
```c++
int geth();
int height();
```
**功能**
获取输入框的高度。

**参数**
（无）

**返回值**
输入框的高度。
</div></br>

<input id="setmaxlen" type="checkbox"> 
<label class="drop" for="setmaxlen">setmaxlen()&emsp;&emsp;设置输入框允许输入的最大字符数</label>
<div>

**声明**
```c++
void setmaxlen(int maxlen);
```
**功能**
设置输入框允许输入的最大字符数。

**参数**
maxlen
最大字符数。（英文字符，标点符号，汉字等都算一个字符)

**返回值**
（无）
</div></br>

<input id="setbgcolor" type="checkbox"> 
<label class="drop" for="setbgcolor">setbgcolor()&emsp;&emsp;设置输入框背景颜色</label>
<div>

**声明**
```c++
void setbgcolor(color_t bgcolor);
```
**功能**
设置输入框背景颜色。

**参数**
bgcolor
背景颜色

**返回值**
（无）
</div></br>

<input id="setcolor" type="checkbox"> 
<label class="drop" for="setcolor">setcolor()&emsp;&emsp;&emsp;设置输入框文字颜色</label>
<div>

**声明**
```c++
void setcolor(color_t color);
```
**功能**
设置输入框的文字颜色。

**参数**
color
文字颜色

**返回值**
（无）
</div></br>

<input id="setfont" type="checkbox"> 
<label class="drop" for="setfont">setfont() &emsp;&emsp;&emsp; 设置输入框文字字体</label>
<div>

**声明**
```c++
void setfont(int h, int w, LPCSTR fontface);

void setfont(int h, int w, LPCWSTR fontface);
```
**功能**
设置输入框文字字体。

**参数**
h
字体的高度。

w
字符的宽度，为0则宽度自适应。

fontface
字体的样式名（如"宋体"）。

**返回值**
（无）
</div></br>

<input id="setreadonly" type="checkbox"> 
<label class="drop" for="setreadonly">setreadonly()&emsp; 设置输入框只读</label>
<div>

**声明**
```c++
void setreadonly(bool readonly);
```
**功能**
设置输入框是否为只读。只读则无法输入。

**参数**
readonly
是否只读，true 为只读，false 为允许输入。
输入框创建后默认为允许输入。
</div></br>

<input id="destory" type="checkbox"> 
<label class="drop" for="destory">destory() &emsp;&emsp;&emsp;销毁输入框</label>
<div>

**声明**
```c++
int destory();
```
**功能**
销毁输入框。销毁后输入框需重新创建。

**参数**
（无）

**返回值**
1，表示进行了销毁操作。
0, 表示之前已经被销毁。

</div></br>
