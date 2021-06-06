#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_
//---------------------------------------------------------------------------
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>

enum CfgFileType {kTypeMap, kForceUp, kType61850, kVendor};

class TestButton:public Fl_Button {
public:
    TestButton(int x, int y, int w, int h, const char *label=0):Fl_Button(x,y,w,h,label) {};
    //virtual int handle(int event);
    int cnt;
};

class CustomChoice:public Fl_Choice {
public:
    CustomChoice(int x, int y, int w, int h, const char *label=0):Fl_Choice(x,y,w,h,label) {};
private:
    virtual int handle(int event);
};

class MainWindow:public Fl_Window {
public:
    MainWindow(int W,int H,const char *Label=0);
    
    Fl_Button *button_start_;
    CustomChoice *choice_type_;
    Fl_Choice *choice_prog_;
    Fl_Choice *choice_61850_;
    Fl_Choice *choice_vendor_;
    Fl_Input *edit_ip_;
    Fl_Input *edit_port_;
    Fl_Output *text_inf_;
    TestButton *button_test_;
    Fl_Check_Button *chk_btn_force_;
    Fl_Check_Button *chk_btn_kernel_;
    Fl_Box *label_reboot_;
    Fl_Box *label_debug_;
    Fl_Box *status_info_;
    Fl_Box *debug_box_;
    Fl_Check_Button *chk_btn_pause_;

private:
    virtual int handle(int event);
    static void CBButtonStart(Fl_Widget *wdg, void * type);
    static void CBChoiceType(Fl_Widget *wdg, void * type);
    static void CBChoiceProg(Fl_Widget *wdg, void * type);
    static void CBButtonTest(Fl_Widget *wdg, void * type);
    static void CBEditIp(Fl_Widget *wdg, void * type);
    static void CBEditPort(Fl_Widget *wdg, void * type);
    static void CBButtonForce(Fl_Widget *wdg, void * type);
    static void CBButtonKernel(Fl_Widget *wdg, void * type);
    static void CBChoice61850(Fl_Widget *wdg, void * type);
    static void CBChoiceVendor(Fl_Widget * choic, void * data);
    static void CBButtonPause(Fl_Widget *wdg, void * type);

    
    int ReadCfgFile(CfgFileType type, void *para);
};


#endif //_MAIN_WINDOW_H_
