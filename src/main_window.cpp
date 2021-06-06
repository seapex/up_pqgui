#include <cstdio>
#include <FL/Fl.H>
#include <FL/names.h>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>

#include "main_window.h"
#include "up_gui.h"

using namespace std;
static const char * kTypeMapFile = ".sys/script/type_map.lst"; //Type map config file
static const char * kForceUpFile = ".sys/force_up.lst"; //force update config file
static const char * kT61850File = ".sys/cst61850.lst"; //61850 type config file
static const char * kVendorsFile = ".sys/vendors.lst"; //vendor type config file

MainWindow::MainWindow(int W, int H, const char *Label) : Fl_Window(W, H, Label)
{
    ParaFileRead(&g_share_para);

    int y = 12, x = 78;
    choice_type_ = new CustomChoice(x, y, 250, 24, "Type: ");
    ReadCfgFile(kTypeMap, &g_share_para);
    if (g_share_para.type.val > choice_type_->size()-2) {
        choice_type_->value(0);
    } else {
        choice_type_->value(g_share_para.type.val);
    }
    choice_type_->callback(MainWindow::CBChoiceType, &g_share_para);
    //choice_type_->take_focus();
    //choice_type_->do_callback();

    y += 34;
    choice_61850_ = new Fl_Choice(x, y, 186, 24, "61850: ");
    ReadCfgFile(kType61850, &g_share_para);
    if (g_share_para.t61850.val > choice_61850_->size()-2) {
        choice_61850_->value(0);
    } else {
        choice_61850_->value(g_share_para.t61850.val);
    }
    choice_61850_->callback(MainWindow::CBChoice61850, &g_share_para);
    //label_reboot_ = new Fl_Box(FL_NO_BOX, x+196, y, 24, 14, "@1+");
    label_reboot_ = new Fl_Box(FL_THIN_DOWN_BOX, x+194, y, 36, 24, "@1+");
    label_reboot_->labelcolor(FL_BLUE);
    label_reboot_->align(FL_ALIGN_CENTER);
    //label_reboot_->labelcolor(FL_INACTIVE_COLOR);
    //label_reboot_->labelcolor(FL_DARK3);
    label_reboot_->clear_visible();
    g_share_para.reboot = 1;
    
    y += 34;
    choice_prog_ = new Fl_Choice(x, y, 100, 24, "Prog: ");
    ReadCfgFile(kForceUp, &g_share_para);
    choice_prog_->value(0);
    choice_prog_->callback(MainWindow::CBChoiceProg, &g_share_para);
    chk_btn_force_ = new Fl_Check_Button(x+118, y, 112, 24, "Force Update");
    chk_btn_force_->callback(MainWindow::CBButtonForce, &g_share_para);
    chk_btn_force_->value(g_share_para.force);
    if (g_share_para.force) chk_btn_force_->labelcolor(FL_RED);
    chk_btn_force_->redraw_label();

    y += 34;
    edit_ip_ = new Fl_Input(x, y, 120, 24, "IP: ");
    edit_ip_->value(g_share_para.ip);
    edit_ip_->callback(MainWindow::CBEditIp, &g_share_para);
    edit_port_ = new Fl_Input(x+174, y, 56, 20, "Port: ");
    edit_port_->value(g_share_para.port);
    edit_port_->callback(MainWindow::CBEditPort, &g_share_para);
    edit_port_->deactivate();
    label_debug_ = new Fl_Box(FL_THIN_DOWN_BOX, x-58, y, 30, 24, "@>[]");
    label_debug_->labelcolor(FL_BLUE);
    label_debug_->align(FL_ALIGN_CENTER);
    label_debug_->clear_visible();
    g_share_para.debug = 0;
    //debug_box_ = new Fl_Box(FL_BORDER_BOX,192, 127, 8, 8, NULL);
    //fl_rect(176, 144, 8, 8, FL_RED);

    y += 34;
    choice_vendor_ = new Fl_Choice(x, y, 100, 24, "Vendor: ");
    ReadCfgFile(kVendor, &g_share_para);
    if (g_share_para.vendor.val > choice_vendor_->size()-2) {
        choice_vendor_->value(0);
    } else {
        choice_vendor_->value(g_share_para.vendor.val);
    }
    choice_vendor_->callback(MainWindow::CBChoiceVendor, &g_share_para);
    choice_vendor_->deactivate();
    chk_btn_kernel_ = new Fl_Check_Button(x+118, y, 112, 24, "Kernel Update");
    chk_btn_kernel_->callback(MainWindow::CBButtonKernel, &g_share_para);
    chk_btn_kernel_->value(g_share_para.kernel);
    chk_btn_kernel_->deactivate();

    y += 30;
    chk_btn_pause_ = new Fl_Check_Button(x-56, y, 170, 24, "Auto close the cmd line");
    chk_btn_pause_->callback(MainWindow::CBButtonPause, &g_share_para);
    chk_btn_pause_->value(!g_share_para.pause);
    chk_btn_pause_->redraw_label();
    chk_btn_pause_->clear_visible();

    y += 6;
    button_start_ = new Fl_Button(x+194, y, 64, 32, "Start");
    button_start_->callback(MainWindow::CBButtonStart, &g_share_para);
    button_start_->take_focus();

    button_test_ = new TestButton(x+96, y, 64, 32, "Test");
    button_test_->callback(MainWindow::CBButtonTest, &g_share_para);
    button_test_->clear_visible();

    y += 20;
    //status_info_ = new Fl_Box(FL_DOWN_BOX, 4, y, 200, 18, "Hello");
    status_info_ = new Fl_Box(4, y, 200, 18);
    //printf("status_info_->align()=%x\n", status_info_->align());
    status_info_->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    status_info_->labelcolor(FL_BLUE);
}

int MainWindow::handle(int event)
{
    if (Fl::event_clicks()) {
        Fl::event_clicks(0);
        int x = Fl::event_x();
        int y = Fl::event_y();
        if (x>label_reboot_->x()&&x<label_reboot_->x()+label_reboot_->w()&&
            y>label_reboot_->y()&&y<label_reboot_->y()+label_reboot_->h()) {
            if ( label_reboot_->visible() ) {
                label_reboot_->clear_visible();
                g_share_para.reboot = 1;
                redraw();
            } else {
                label_reboot_->set_visible();
                label_reboot_->redraw();
                g_share_para.reboot = 0;
            }
            g_share_para.update = true;
        } else if (x>label_debug_->x()&&x<label_debug_->x()+label_debug_->w()
            &&y>label_debug_->y()&&y<label_debug_->y()+label_debug_->h()) {
            if ( label_debug_->visible() ) {
                label_debug_->clear_visible();
                g_share_para.debug = 0;
                redraw();
            } else {
                label_debug_->set_visible();
                label_debug_->redraw();
                g_share_para.debug = 1;
            }
            g_share_para.update = true;
        } else if (x>edit_port_->x()-32 && x<edit_port_->x()-8
            && y>edit_port_->y() && y<edit_port_->y()+edit_port_->h()) {
            if ( edit_port_->active() ) {
                edit_port_->deactivate();
            } else {
                edit_port_->activate();
            }
        } else if (x>choice_vendor_->x()-50&&x<choice_vendor_->x()-10
            &&y>choice_vendor_->y()&&y<choice_vendor_->y()+choice_vendor_->h()) {
            if ( choice_vendor_->active() ) {
                choice_vendor_->deactivate();
            } else {
                choice_vendor_->activate();
            }
        } else if (x>chk_btn_kernel_->x()+chk_btn_kernel_->w()-18&&x<chk_btn_kernel_->x()+chk_btn_kernel_->w()
            &&y>chk_btn_kernel_->y()&&y<chk_btn_kernel_->y()+chk_btn_kernel_->h()) {
            if ( chk_btn_kernel_->active() ) {
                chk_btn_kernel_->deactivate();
                g_share_para.kernel = 0;
                chk_btn_kernel_->value(g_share_para.kernel);
                chk_btn_kernel_->labelcolor(FL_FOREGROUND_COLOR);
                
            } else {
                chk_btn_kernel_->activate();
            }
        } else if (x>chk_btn_pause_->x()-20&&x<chk_btn_pause_->x()
            &&y>chk_btn_pause_->y()&&y<chk_btn_pause_->y()+chk_btn_pause_->h()) {
            if ( chk_btn_pause_->visible() ) {
                chk_btn_pause_->clear_visible();
                redraw();
            } else {
                chk_btn_pause_->set_visible();
                chk_btn_pause_->redraw();
            }
        }
        
    }
    //printf("event=%s\n",  fl_eventnames[event]);
    //printf("double-click is %d\n", Fl::event_clicks());
    switch(event) {
        case FL_CLOSE:
            printf("Never recieved this event\n");
            return 1;
        default:
            return Fl_Window::handle(event);
    }
}

int CustomChoice::handle(int event)
{
    if (Fl::event_clicks()) {
        printf("double-click happened in CustomChoice\n");
        Fl::event_clicks(0);
    }
    switch(event) {
        case FL_CLOSE:
            printf("Never recieved this event\n");
            return 1;
        default:
            return Fl_Choice::handle(event);
    }
}

void MainWindow::CBEditIp(Fl_Widget * wdg, void * data)
{
    SharePara * para = (SharePara *)data;
    Fl_Input * in = (Fl_Input*) wdg;
    strcpy(para->ip, in->value());
    para->update = true;
}

void MainWindow::CBEditPort(Fl_Widget * wdg, void * data)
{
    SharePara * para = (SharePara *)data;
    Fl_Input * in = (Fl_Input*) wdg;
    strcpy(para->port, in->value());
    g_share_para.update = true;
}

void MainWindow::CBButtonStart(Fl_Widget * buttn, void * data)
{
    static char str_inf[64];
    buttn->deactivate();  //Deactivate the button prevent button from being pressed again
    SharePara * para = (SharePara *)data;
    ParaFileSave(para);
    MainWindow * pwin = (MainWindow *)para->pmwin;
    pwin->status_info_->label("");

    const char *cmd_t;
    if (para->type.item_idx[para->type.val]<=10) cmd_t = "Upgrade";
    else cmd_t = "Operation";
    
    char stri[64];
    sprintf(stri, "up_pqied.exe %s", para->ip);
    if (para->debug) strcpy(&stri[strlen(stri)], " dbg");
    puts(stri);
    int ret = system(stri);
    if (ret==0) {
        pwin->status_info_->labelcolor(FL_BLUE);
        sprintf(str_inf, "%s succeed!", cmd_t);
        pwin->status_info_->label(str_inf);
    } else if (ret==1) {
        pwin->status_info_->labelcolor(FL_FOREGROUND_COLOR);
        pwin->status_info_->label("Don't need to upgrade!");
        //fl_alert("Don't need upgrade!! %d", ret);
    } else if (ret==2) {
        pwin->status_info_->labelcolor(FL_RED);
        pwin->status_info_->label("系统版本过旧，需先用串口升级!");
        //fl_alert("Don't need upgrade!! %d", ret);
    } else {
        pwin->status_info_->labelcolor(FL_RED);
        sprintf(str_inf, "%s failed!", cmd_t);
        pwin->status_info_->label(str_inf);
    }

    buttn->activate();  // reactivate button
}

void MainWindow::CBButtonTest(Fl_Widget * buttn, void * data)
{
    buttn->deactivate();  //Deactivate the button prevent button from being pressed again
    SharePara * para = (SharePara *)data;
    printf("ip:%s, port:%s, type:%d\n", para->ip, para->port, para->type);
    ParaFileSave(para);
    buttn->activate();  // reactivate button
}

void MainWindow::CBButtonForce(Fl_Widget * buttn, void * data)
{
    SharePara * para = (SharePara *)data;
    Fl_Button *pchkbtn =  (Fl_Button *)buttn;
    //printf("buttn->value=%d, label=%s\n", pchkbtn->value(), pchkbtn->label());
    para->force = pchkbtn->value();    
    if (para->force) pchkbtn->labelcolor(FL_RED);
    else pchkbtn->labelcolor(FL_FOREGROUND_COLOR);
    pchkbtn->redraw_label();
    para->update = true;
}

void MainWindow::CBButtonPause(Fl_Widget * buttn, void * data)
{
    SharePara * para = (SharePara *)data;
    Fl_Button *pchkbtn =  (Fl_Button *)buttn;
    //printf("buttn->value=%d, label=%s\n", pchkbtn->value(), pchkbtn->label());
    para->pause = !pchkbtn->value();    
    pchkbtn->redraw_label();
    para->update = true;
}

void MainWindow::CBButtonKernel(Fl_Widget * buttn, void * data)
{
    SharePara * para = (SharePara *)data;
    Fl_Button *pchkbtn =  (Fl_Button *)buttn;
    //printf("buttn->value=%d, label=%s\n", pchkbtn->value(), pchkbtn->label());
    para->kernel = pchkbtn->value();    
    if (para->kernel) pchkbtn->labelcolor(FL_RED);
    else pchkbtn->labelcolor(FL_FOREGROUND_COLOR);
    pchkbtn->redraw_label();
    para->update = true;
}

void MainWindow::CBChoiceType(Fl_Widget * choic, void * data)
{
    Fl_Choice * ch = (Fl_Choice*) choic;
    SharePara * para = (SharePara *)data;
    para->type.val = ch->value();
    para->update = true;
}

void MainWindow::CBChoice61850(Fl_Widget * choic, void * data)
{
    Fl_Choice * ch = (Fl_Choice*) choic;
    SharePara * para = (SharePara *)data;
    para->t61850.val = ch->value();
    para->update = true;
}

void MainWindow::CBChoiceProg(Fl_Widget * choic, void * data)
{
    Fl_Choice * ch = (Fl_Choice*) choic;
    SharePara * para = (SharePara *)data;

    const Fl_Menu_Item *item = ch->mvalue();
    int i = ch->value();
    if (item->value()) para->prog.val |= (1<<i);
    else para->prog.val &= ~(1<<i);

    para->update = true;
}

void MainWindow::CBChoiceVendor(Fl_Widget * choic, void * data)
{
    Fl_Choice * ch = (Fl_Choice*) choic;
    SharePara * para = (SharePara *)data;
    para->vendor.val = ch->value();
    para->update = true;
}

/*!
    Input:  type -- config file type.
    Return: 0=success, -1=type unknown, -2=file error 
*/
int MainWindow::ReadCfgFile(CfgFileType type, void *para)
{
    SharePara *p = (SharePara *)para;
    Fl_Choice *pchoice;
    int * pidx;
    const char * file_name;
    switch(type) {
        case kTypeMap:
            file_name = kTypeMapFile;
            pidx = p->type.item_idx;
            pchoice = choice_type_;
            break;
        case kForceUp:
            file_name = kForceUpFile;
            pidx = p->prog.item_idx;
            pchoice = choice_prog_;
            break;
        case kType61850:
            file_name = kT61850File;
            pidx = p->t61850.item_idx;
            pchoice = choice_61850_;
            break;
        case kVendor:
            file_name = kVendorsFile;
            pidx = p->vendor.item_idx;
            pchoice = choice_vendor_;
            break;
        default: return -1;
    }
    FILE *fp;
    if ((fp = fopen(file_name, "r"))==NULL) {
        printf("Can't open %s\n", file_name); return -2;
    }

    char stri[64];
    int ret;
    for(int i=0; i<kMaxTypeNum;) {
        ret = fscanf(fp, "%d:%[^\n]", pidx, stri);
        if (ret==EOF) break;
        if (ret<2) {
            fgets(stri, 64, fp);
            continue;
        }
        pchoice->add(stri);
        if (*pidx>100) pchoice->mode(i, FL_MENU_INVISIBLE);
        i++; pidx++;
    }
    fclose(fp);
    
    if (type==kForceUp) {
        Fl_Menu_Item *pmitem = (Fl_Menu_Item *)pchoice->menu();
        for (int i=0; i<pchoice->size(); i++) {
            pchoice->mode(i, FL_MENU_TOGGLE);
            if (p->prog.val&(1<<i)) pmitem->set();
            pmitem++; 
        }
    }
    return 0;
}
