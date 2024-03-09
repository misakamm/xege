#ifndef EGECONTROLBASE_H
#define EGECONTROLBASE_H

#ifndef EGE_H
#error include "egectlbase.h" must after include "ege.h" or "graphics.h"
#endif

namespace ege
{

class PushTarget
{
public:
    PushTarget() { m_target = gettarget(); }

    PushTarget(PIMAGE target)
    {
        m_target = gettarget();
        settarget(target);
    }

    ~PushTarget() { settarget(m_target); }

private:
    PIMAGE m_target;
};

#define CTL_PREINIT(classname, parent)                 \
    struct preinit_obj                                 \
    {                                                  \
        preinit_obj(classname* This, int inheritlevel) \
        {                                              \
            This->pre_init(inheritlevel);              \
        }                                              \
    } egecontrolbase_preinit_obj;                      \
    enum inherit_e                                     \
    {                                                  \
        inherit_level_e = parent::inherit_level_e + 1, \
    };                                                 \
    static void firstinit(ege::egeControlBase* This)   \
    {                                                  \
        ((classname*)This)->m_inheritlevel = 1;        \
    }                                                  \
    void pre_init(int inheritlevel)                    \
    {                                                  \
        (void)inheritlevel;

#define CTL_PREINITEND }
#define CTL_DEFPARAM   int inherit = inherit_level_e, ege::egeControlBase *pParent = NULL
#define CTL_INITBASE(parent) \
    egecontrolbase_preinit_obj(this, inherit_level_e), parent(inherit, (ege::egeControlBase*)pParent)
#define CTL_INIT                                 \
    InitObject      iobj(this, inherit_level_e); \
    ege::PushTarget _pushtarget(buf());

#define EGECTRL_INITEND() }

class egeControlBase
{
public:
    enum ROP
    {
        COPY = SRCCOPY,
        XOR  = SRCINVERT,
        AND  = SRCAND,
        OR   = SRCPAINT,
    };

    enum blendmode_e
    {
        SOLIDCOPY  = 0,
        ALPHABLEND = 1,
    };

    enum inherit_e
    {
        inherit_level_e = 0,
    };

    // ���캯�������Զ��壬��Ҫ����Ҫѡ��ʹ��ʹ�ú꣬���ǰ���������ĵ�ʾ������
    egeControlBase();
    egeControlBase(int inherit, egeControlBase* pParent);
    ~egeControlBase();

    // �����麯������Ҫֱ���໥����
    virtual LRESULT onMessage(UINT message, WPARAM wParam, LPARAM lParam)
    {
        (void)message;
        (void)wParam;
        (void)lParam;
        return 0;
    }

    // ���º���������ط�0�����ӿؼ����ݼ��������Ϣ
    virtual int onMouse(int x, int y, int flag)
    {
        (void)x;
        (void)y;
        (void)flag;
        return 0;
    }

    virtual int onKeyDown(int key, int flag)
    {
        (void)key;
        (void)flag;
        return 0;
    }

    virtual int onKeyUp(int key, int flag)
    {
        (void)key;
        (void)flag;
        return 0;
    }

    virtual int onKeyChar(int key, int flag)
    {
        (void)key;
        (void)flag;
        return 0;
    }

    // ��Ļ���º�ᱻ���ã����ڸ����߼�
    virtual int onUpdate() { return 0; }

    // ����GetFocus��Ҫ��ý���ʱ���ã�����ֵһ�㷵��0��ʾ��ȡ�������뽹�㣬���ط�0����������뽹��
    virtual int onGetFocus() { return 0; }

    // ʧȥ���뽹��ʱ����
    virtual void onLostFocus() {}

    // ���óߴ�ǰ���ã��Զ�����������
    virtual void onSizing(int* w, int* h)
    {
        (void)w;
        (void)h;
    }

    // ��Ӧ�ߴ�仯����
    virtual void onSize(int w, int h)
    {
        (void)w;
        (void)h;
    }

    // �ػ溯���������뻭��pimg�ϣ��Ա��ܿ��ƻ滭Ŀ��
    virtual void onDraw(PIMAGE pimg) const { (void)pimg; }

    // �ߴ�仯ʱ���ã������ػ����˻���������
    virtual void onResetFilter()
    {
        setbkcolor(BLACK, m_mainFilter);
        cleardevice(m_mainFilter);
    }

    virtual void onAddChild(egeControlBase* pChild) { (void)pChild; }

    virtual void onDelChild(egeControlBase* pChild) { (void)pChild; }

    virtual void onIdle() {} // �����ӿڣ�δ��

    // �������ϵĺ����������ж��壨ע������Ҫһ�£���Ҫ©��OnDraw���const��
    // �������µ�public�������Ե��ã������Զ��壬�κ�Ԥ�����������Ҫֱ�ӷ��ʣ���ʹ��Ԥ���庯��������
public:
    PIMAGE buf() { return m_mainbuf; }

    PIMAGE filter() { return m_mainFilter; }

    egeControlBase* parent() { return m_parent; }

    PIMAGE buf() const { return m_mainbuf; }

    PIMAGE filter() const { return m_mainFilter; }

    const egeControlBase* parent() const { return m_parent; }

    void blendmode(int mode) { m_AlphablendMode = mode; }

    void setrop(int rop) { m_rop = rop; } // ����ö������ROP���������

    void directdraw(bool bdraw) { m_bDirectDraw = (bdraw ? 1 : 0); }
    bool isdirectdraw() const { return (m_bDirectDraw != 0); }

    void autoredraw(bool bautoredraw) { m_bAutoDraw = (bautoredraw ? 1 : 0); }
    bool isautoredraw() const { return (m_bAutoDraw != 0); }

    void visible(bool bvisible) { m_bVisible = (bvisible ? 1 : 0); }
    bool isvisible() const { return (m_bVisible != 0); }

    void enable(bool benable) { m_bEnable = (benable ? 1 : 0); }
    bool isenable() const { return (m_bEnable != 0); }

    void capture(bool bcapture) { m_bCapture = (bcapture ? 1 : 0); }
    bool iscapture() const { return (m_bCapture != 0); }

    void capmouse(bool bcapmouse) { m_bCapMouse = (bcapmouse ? 1 : 0); }
    bool iscapmouse() const { return (m_bCapMouse != 0); }

    bool isfocus() const { return (m_bInputFocus != 0); }

    void move(int x, int y)
    {
        m_x = x;
        m_y = y;
    }

    void size(int w, int h)
    {
        onSizing(&w, &h);
        m_w = w;
        m_h = h;
        resize(m_mainbuf, w, h);
        resize(m_mainFilter, w, h);
        onSize(w, h);
        onResetFilter();
    }

    void zorderup();
    void zorderdown();
    void zorderset(int z);

    int getx()   const { return m_x; }
    int gety()   const { return m_y; }
    int getw()   const { return m_w; }
    int geth()   const { return m_h; }
    int width()  const { return m_w; }
    int height() const { return m_h; }

    int  addchild(egeControlBase* pChild);
    int  delchild(egeControlBase* pChild);
    void draw(PIMAGE pimg);
    void update();
    void mouse(int x, int y, int flag);
    void keymsgdown(unsigned key, int flag);
    void keymsgup(unsigned key, int flag);
    void keymsgchar(unsigned key, int flag);

    bool operator<(const egeControlBase& pbase) const
    {
        if (m_zOrderLayer != pbase.m_zOrderLayer) {
            return m_zOrderLayer < pbase.m_zOrderLayer;
        }
        if (m_zOrder == pbase.m_zOrder) {
            return this < &pbase;
        } else {
            return m_zOrder < pbase.m_zOrder;
        }
    }

protected:
    int allocId();
    int allocZorder();

    class InitObject
    {
    public:
        InitObject(egeControlBase* pThis, int inherit_level);
        ~InitObject();

    private:
        egeControlBase* m_this;
        int             m_inherit_level;
    };

    void (*m_preinit_func)(egeControlBase*);

private:
    void init(egeControlBase* parent);
    void fixzorder();
    void sortzorder();
#if _MSC_VER <= 1200
public:
#endif
    void initok();

private:
    PIMAGE m_mainbuf;    // ������
    PIMAGE m_mainFilter; // ������

protected:
    int m_bVisible;    // �Ƿ�ɼ�
    int m_bEnable;     // �Ƿ�ɻ�����루���̺���꣩
    int m_bAutoDraw;   // �Ƿ��Զ��滭��������
    int m_bCapture;    // �Ƿ�ɻ�ü������뽹��
    int m_bInputFocus; // �Ƿ��Ѿ�������뽹��
    int m_bCapMouse;   // �Ƿ�׽��꣨��ʹ�������������ڣ�
    int m_zOrderLayer; // Z����㣨ֵ�ϴ�����ǰ��ֵ��С�߻ᱻ�����ؼ��ڵ���
    int m_zOrder;      // Z����ֵ�ϴ�����ǰ��ֵ��С�߻ᱻ�����ؼ��ڵ���
    int m_allocId;     // ����id
    int m_allocZorder; // ����Z����

private:
    egeControlBase* m_parent;
    static int      s_maxchildid; // ��һ���ӿؼ�����IDֵ

#ifdef EGE_GRAPH_LIB_BUILD
public:
#else
private:
#endif
    void* m_childmap;    // �ӿؼ�
    void* m_childzorder; // �ӿؼ�����

protected:
    int m_x, m_y; // ���Ͻ�����
    int m_w, m_h; // ���

protected:
    DWORD m_rop;            // ��Ϸ�ʽ
    int   m_AlphablendMode; // �滭��Ϲ��˷�ʽ
    int   m_bDirectDraw;    // ����ֱ�ӻ滭
#if _MSC_VER <= 1200
public:
#endif
    int m_inheritlevel; // �̳в��
};

} /* namespace ege */

#endif /* EGECONTROLBASE_H */
