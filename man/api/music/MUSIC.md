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


## MUSIC 类
 >头文件包含: **#include <graphics.h>**


### 函数
#### 构造函数

```c++
MUSIC();
```

#### 成员函数

<input id="OpenFile" type="checkbox"> 
<label class="drop" for="OpenFile">OpenFile()&emsp;&emsp;&emsp;&emsp; 打开音乐文件</label>
<div>

**声明**
```cpp
DWORD OpenFile(LPCSTR  filepath);

DWORD OpenFile(LPCWSTR filepath);
```
**功能**
打开音乐文件。
成功打开后播放状态为 MUSIC_MODE_STOP

**参数**
filepath
带路径文件名。

**返回值**
操作成功返回0, 操作失败返回非0。
</div></br>

<input id="IsOpen" type="checkbox"> 
<label class="drop" for="IsOpen">IsOpen()&emsp;&emsp;&emsp;&emsp;&emsp; 判断是否打开了音乐文件</label>
<div>

**声明**
```cpp
int   IsOpen();
```
**功能**
判断是否打开了音乐文件。

**参数**
（无）

**返回值**
1，表示已打开。
0，表示未打开。

</div></br>

<input id="GetPlayStatus" type="checkbox"> 
<label class="drop" for="GetPlayStatus">GetPlayStatus() &emsp;&emsp;获取播放状态</label>
<div>

**声明**
```c++
DWORD GetPlayStatus();
```
**功能**
获取播放状态。

**参数**
（无）

**返回值**
播放状态，为以下值之一。
```cpp
MUSIC_MODE_NOT_OPEN     //没有正确打开
MUSIC_MODE_NOT_READY    //设备没准备好 （较少使用）
MUSIC_MODE_PAUSE        //暂停中
MUSIC_MODE_PLAY         //正在播放
MUSIC_MODE_STOP         //成功打开后，或者播放完是这个状态
MUSIC_MODE_OPEN         //打开中 （较少使用）
MUSIC_MODE_SEEK         //定位中 （较少使用）
```
</div></br>

<input id="Play" type="checkbox"> 
<label class="drop" for="Play">Play()&emsp;&emsp;&emsp;&emsp;&emsp; &emsp; 播放音乐</label>
<div>

**声明**
```c++
DWORD Play(DWORD dwFrom = MUSIC_ERROR, DWORD dwTo = MUSIC_ERROR);
```
**功能**
播放音乐。Play()为继续播放, Play(0)为从头开始播放。
播放状态变为MUSIC_MODE_PLAY

**参数**
* dwFrom
音乐播放开始位置(毫秒)。

* dwTo
音乐播放结束位置(毫秒)。

**返回值**
操作成功返回0, 操作失败返回非0。
</div></br>

<input id="Pause" type="checkbox"> 
<label class="drop" for="Pause">Pause()&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;暂停播放</label>
<div></br>

**声明**
```c++
DWORD Pause();
```
**功能**
暂停播放。播放状态变为 MUSIC_MODE_PAUSE。

**参数**
（无）

**返回值**
操作成功返回0, 操作失败返回非0。
</div></br>

<input id="Stop" type="checkbox"> 
<label class="drop" for="Stop">Stop() &emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 停止播放</label>
<div>

**声明**
```c++
DWORD Stop();
```
**功能**
停止播放。播放状态变为 MUSIC_MODE_STOP。
播放将暂停，但播放进度不变。

**参数**
(无)

**返回值**
操作成功返回0, 操作失败返回非0。
</div></br>

<input id="SetVolume" type="checkbox"> 
<label class="drop" for="SetVolume">SetVolume()&emsp;&emsp;&emsp;&emsp;设置播放音量</label>
<div>

**声明**
```c++
DWORD SetVolume(float value);
```
**功能**
设置播放音量。

**参数**
value
音量。范围从0~1。

**返回值**
操作成功返回0, 操作失败返回非0。
</div></br>

<input id="Seek" type="checkbox"> 
<label class="drop" for="Seek">Seek()&emsp;&emsp;&emsp;&emsp;&emsp;&emsp; 调整播放进度</label>
<div>

**声明**
```c++
DWORD Seek(DWORD dwTo);
```
**功能**
播放位置定位。目前无效，用Play(dwTo)代替。

**参数**
dwTo
播放进度，单位毫秒。

**返回值**
操作成功返回0, 操作失败返回非0。
</div></br>

<input id="GetPosition" type="checkbox"> 
<label class="drop" for="GetPosition">GetPosition() &emsp;&emsp;&emsp; 获取播放进度</label>
<div>

**声明**
```c++
DWORD GetPosition();
```
**功能**
获取播放进度。

**参数**
（无)

**返回值**
播放进度，单位毫秒。
</div></br>

<input id="GetLength" type="checkbox"> 
<label class="drop" for="GetLength">GetLength()&emsp;&emsp;&emsp;&emsp;获取音乐总时长</label>
<div>

**声明**
```c++
DWORD GetLength();
```
**功能**
获取音乐总时长，单位毫秒。

**参数**
（无）

**返回值**
音乐总时长，单位毫秒。
</div></br>



