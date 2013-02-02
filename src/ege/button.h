/** 
 * @file button.h
 * @brief the EGE control:button
 * @author StudyC.cn@gmail.com
 * @date 2011-08-09
 */
#ifndef _EGE_BUTTON_H_
#define _EGE_BUTTON_H_

#ifndef _EGE_H_
#error include "button.h" must after include "ege.h" or "graphics.h"
#endif

#include <algorithm>

#ifdef DEBUG
#include "ege/label.h"
#else
#define logout(msg)
#endif

/**
 * @brief 按钮控件
 */
class button : public egeControlBase
{
    public:
        //overrides
        // 屏幕更新后会被调用，用于更新逻辑
        // 以下虚函数都不要直接相互调用
        // 以下函数如果返回非0则不向子控件传递键盘鼠标消息
        /**
         * @brief 响应空格与回车的按下
         *
         * @param key
         * @param flag
         *
         * @return 0
         */
        virtual int  onKeyDown(int key, int flag) {
            if((key==13)||(key==32)){
                _pushed=true;
                redraw();
            }
            return 0; 
        }
        /**
         * @brief 响应空格与回车的弹起
         *
         * @param key
         * @param flag
         *
         * @return 0
         */
        virtual int  onKeyUp(int key, int flag) {
            if(((key==13)||(key==32))&&_pushed){
                if(_on_click){
                    if(!_on_click(callback_param)){
                        onClick();
                    }
                }else{
                    onClick();
                }
            }
            _pushed=false;
            redraw();
            return 0;
        }

        /**
         * @brief 响应鼠标左键点击
         *
         * @param x
         * @param y
         * @param flag
         *
         * @return 0
         */
        virtual int onMouse(int x, int y, int flag) {
            if((flag&mouse_flag_left)&&(flag&mouse_msg_down)){
                capmouse(true);
                _pushed=true;
                redraw();
            }else if((flag&mouse_flag_left)&&(flag&mouse_msg_up)){
                if(_pushed){
                    if(_on_click){
                        if(!_on_click(callback_param)){
                            onClick();
                        }
                    }else{
                        onClick();
                    }
                    capmouse(false);
                    _pushed=false;
                }
                redraw();
            }
            return 0; 
        }
        /**
         * @brief 屏幕更新后会被调用，用于更新逻辑
         *
         * @return 0
         */
        virtual int  onUpdate() { 
            return 0;
        }
        /**
         * @brief 在要获得焦点时调用，返回值一般返回0表示获取键盘输入焦点，返回非0放弃获得输入焦点
         *
         * @return 0
         */
        virtual int  onGetFocus() {
            return 0;
        }
        /**
         * @brief 失去输入焦点时调用
         */
        virtual void onLostFocus() {
            _pushed=false;
            redraw();
        }
        // 设置尺寸前调用，自定义修正函数
        virtual void onSizing(int *w, int *h) {
            //egeControlBase::onSizing(w,h);
        }
        // 响应尺寸变化函数
        virtual void onSize(int w, int h) {
            //egeControlBase::onSize(w,h);
            updatesidewidth();
            redraw();
        }
        // 重绘函数，尽量请画到pimg上，以便能控制绘画目标
        virtual void onDraw(PIMAGE pimg) const {
        }
        // 尺寸变化时调用，用于重画过滤缓冲区内容
        virtual void onResetFilter() {
        }
        //virtual void onAddChild(egeControlBase* pChild) {}
        //virtual void onDelChild(egeControlBase* pChild) {}
        //virtual void onIdle() {} // 保留接口，未用
        // 这里以上的函数可以自行定义（注意声明要一致，不要漏掉OnDraw里的const）
        //init
        CTL_PREINIT(button, egeControlBase) {
            // do sth. before sub objects' construct function call
        } CTL_PREINITEND;
        button(CTL_DEFPARAM) : CTL_INITBASE(egeControlBase) {
            CTL_INIT; // must be the first line
            size(64, 32);
            _font_height = 12;
            strcpy(_face, "宋体");
            _line_color=BLACK;
            _bg_color=RGB(100,100,100);
            _text_color=BLACK;
            _shadow_color=RGB(50,50,50);
            updatesidewidth();

            _on_click=NULL;
            callback_param=NULL;
            _pushed=false;
            _alpha=0xff;
#ifdef DEBUG
            _logger=NULL;
#endif
            //redraw();
            //blendmode(true);
        }
        //member functions
        /**
         * @brief 在回调函数指针为NULL或回调函数返回0时被调用
         */
        virtual void onClick(){
        }
        /**
         * @brief 重绘控件
         */
        virtual void redraw() const {
            PushTarget targer(buf());
            setbkcolor_f(RED);
            setcolor(RED);
            cleardevice();
            setbkmode(TRANSPARENT);
            setfillstyle(_bg_color,SOLID_FILL);
            bar(0, 0, getw()-1, geth()-1);
            setfont(_font_height, 0, _face);
            setcolor(_text_color);

            //settextjustify(LEFT_TEXT,CENTER_TEXT);
            //outtextrect(_side_width, _side_width, getw()-_side_width, geth()-_side_width, _caption);
            //outtextrect(0, 0, getw(), geth(), _caption);
            int x=(getw()-textwidth(_caption))/2;
            int y=(geth()-textheight(_caption))/2;
            outtextxy(x,y,_caption);
            setbkcolor(_line_color);
            rectangle(0,0,getw(),geth());
            rectangle(_side_width,_side_width,getw()-_side_width,geth()-_side_width);
            setfillstyle(_shadow_color,SOLID_FILL);
            if(_pushed){
                int points[12]={
                    0,0,
                    getw()-1,0,
                    getw()-_side_width, _side_width-1,
                    _side_width-1,_side_width-1,
                    _side_width-1, geth()-_side_width-1,
                    0,geth()-1,
                };
                fillpoly(6,points);
            }else{
                int points[12]={
                    0,geth()-1,
                    _side_width-1,geth()-_side_width,
                    getw()-_side_width, geth()-_side_width,
                    getw()-_side_width, _side_width-1,
                    getw()-1,0,
                    getw()-1,geth()-1,
                };
                fillpoly(6,points);
            }
            line(getw()-_side_width, _side_width, getw(), 0);
            line(_side_width-1, geth()-_side_width, 0, geth());
            //line(getw()-_side_width, geth()-_side_width, getw(), geth());
            //line(0,0, _side_width, _side_width);
            setbkcolor_f(RGB(_alpha, _alpha, _alpha), filter());
            cleardevice(filter());
        }
        //attributes
        /**
         * @brief 设置alpha值
         *
         * @param alpha 0x00 - 0xff
         */
        void alpha(int alpha){
            if(alpha<0){
                _alpha=0;
            }else if(alpha>0xff){
                _alpha=0xff;
            }else{
                _alpha=alpha;
            }
            if(_alpha<0xff){
                blendmode(true);
            }else{
                blendmode(false);
            }

        }
        /**
         * @brief 返回alpha值
         *
         * @return alpha
         */
        int alpha() const{
            return _alpha;
        }
        /**
         * @brief 设置背景色
         *
         * @param color 背景色
         */
        void bgcolor(COLORREF color){
            _bg_color=color;
            redraw();
        }
        /**
         * @brief 返回背景色
         *
         * @return 背景色
         */
        COLORREF bgcolor() const{
            return _bg_color;
        }
        /**
         * @brief 设置按钮点击回调函数
         *
         * @param fun 回调函数指针，当且仅当返回值为0时会自动调用onClick
         * @param param 附加参数，将会原样传递给回调函数
         */
        void callback(int (*fun)(void*),void* param){
            callback_param=param;
            _on_click=fun;
            redraw();
        }
        /**
         * @brief 返回回调函数指针
         *
         * @return 回调函数指针；若未设置，返回NULL
         */
        template <typename T>
            T callback() const{
                return _on_click;
            }
        /**
         * @brief 设置文本
         *
         * @param text 文本
         */
        void caption(const char* text) {
            strcpy(_caption, text);
            redraw();
        }
        /**
         * @brief 返回文本
         *
         * @return 文本
         */
        const char* caption() const{
            return _caption;
            //redraw();
        }
        /**
         * @brief 设置字体
         *
         * @param fontface 字体名
         */
        void font(const char* fontface) {
            strcpy(_face, fontface);
            redraw();
        }
        /**
         * @brief 返回字体
         *
         * @return 字体名
         */
        const char* font() const{
            return _face;
            //redraw();
        }
        /**
         * @brief 设置字体尺寸，待续
         *
         * @param height
         */
        void fontsize(int height) {
            _font_height = height;
            redraw();
        }
        /**
         * @brief 返回字体尺寸
         *
         * @return 字体尺寸
         */
        int fontsize() const{
            return _font_height;
        }
        /**
         * @brief 设置按钮的线条颜色
         *
         * @param color 颜色
         */
        void linecolor(COLORREF color){
            _line_color=color;
            redraw();
        }
        /**
         * @brief 返回按钮线条颜色
         *
         * @return 线条颜色
         */
        COLORREF linecolor() const{
            return _line_color;
        }
#ifdef DEBUG
        /**
         * @brief 设置调试信息出口
         *
         * @param logger
         */
        void logger(label* logger){
            _logger=logger;
        }
        /**
         * @brief 返回调试信息出口
         *
         * @return 
         */
        label* logger() const{
            return _logger;
        }
#endif
        /**
         * @brief 设置阴影颜色
         *
         * @param color 阴影颜色
         */
        void shadowcolor(COLORREF color){
            _shadow_color=color;
            redraw();
        }
        /**
         * @brief 返回阴影颜色
         *
         * @return 阴影颜色
         */
        COLORREF shadowcolor() const{
            return _shadow_color;
        }
        /**
         * @brief 设置文本颜色
         *
         * @param color 文本颜色
         */
        void textcolor(COLORREF color){
            _text_color=color;
            redraw();
        }
        /**
         * @brief 返回文本颜色
         *
         * @return 文本颜色
         */
        COLORREF textcolor() const{
            return _text_color;
        }
    protected:
        /**
         * @brief 修正边的宽度
         */
        void updatesidewidth(){
            _side_width=std::min(geth(),getw())*0.2;
        }
#ifdef DEBUG
        /**
         * @brief 输出调试信息
         *
         * @param msg 调试信息文本
         */
        void logout(const char* msg){
            if(_logger){
                _logger->setcaption(msg);
            }
        }
#endif
        bool _pushed;
        int (*_on_click)(void*);
        char _caption[1024];
        char _face[32];
        COLORREF _line_color;
        COLORREF _bg_color;
        COLORREF _text_color;
        COLORREF _shadow_color;
        int _side_width;
        int _font_height;
        int _alpha;
        void* callback_param;
#ifdef DEBUG
        label* _logger;
#endif
};

#endif /* _EGE_BUTTON_H_ */
