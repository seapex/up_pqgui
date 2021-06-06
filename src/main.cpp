#include <stdint.h>
#include <stdio.h>
#include <FL/Fl.H>
#include <FL/Fl_Ask.H>
#include <FL/x.H>   // needed for fl_display
#pragma comment( linker, "/subsystem:/"windows/" /entry:/"mainCRTStartup/"" ) // 设置入口地址
#include "windows.h"
#include "icon.h"

#include "config.h"
#include "up_pqied.h"
#include "up_gui.h"
#include "main_window.h"

SharePara g_share_para;   //share param

/*!
system() to hide command window

    Input:  cmd -- command line
            cnt -- size of output buffer in btes
    Output: out -- command output buffer
    Return: true=success, false=failure
*/
bool system_hide(char *out, int cnt, const char *cmd)
{
    SECURITY_ATTRIBUTES sa;
    sa.nLength   =   sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor   =   NULL;
    sa.bInheritHandle   =   TRUE;

    HANDLE hRead, hWrite;
    if (!CreatePipe(&hRead, &hWrite, &sa, 0)) {
        return false;
    }

    STARTUPINFO si;
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfo(&si);
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    PROCESS_INFORMATION pi;
    char cmd_buf[256];
    strncpy(cmd_buf, cmd, 256);
    printf("_debug_:%d@%s.\n", __LINE__, __FUNCTION__);
    if (!CreateProcess(NULL, cmd_buf, NULL, NULL, true, 0, NULL, NULL, &si, &pi)) {
        return false;
    }
    CloseHandle(hWrite);

    if (out) {
        DWORD bytesRead;
        memset(out, 0, cnt);
        ReadFile(hRead, out, cnt-1, &bytesRead, NULL);
    }
    return true;
}

int main(int argc, char**argv)
{
    char title[48];
    /*uint32_t ver = system("up_pqied.exe -v > nul");
    sprintf(title, "up_pqied:%d.%d.%d GUI:%d.%d", ver>>22, (ver>>10)&0x3ff, ver&0x3ff, _VERSION_MAJOR, _VERSION_MINOR);
    */
    char obuf[64];
    int ver[3], luav[2];
    if (system_hide(obuf, 64, "up_pqied.exe -v")) {
        printf("%s\n", obuf);
        sscanf(obuf, "up_pqied %d.%d", &ver[0], &ver[1]);
    }
    if (system_hide(obuf, 64, "up_pqied.exe -luav")) {
        printf("%s\n", obuf);
        sscanf(obuf, "preprocess.lua %d.%d", &luav[0], &luav[1]);
    }
    sprintf(title, "%d.%d | up_pqied:%d.%d prxx.lc:%d.%d", _VERSION_MAJOR, _VERSION_MINOR, ver[0], ver[1], luav[0], luav[1]);
    MainWindow *window = new MainWindow(360, 226, title);
    window->end();
    g_share_para.update = false;
    window->callback(CBMainWindow, &g_share_para);
    g_share_para.pmwin = window;
    window->position( (Fl::w()-280)/2, (Fl::h()-168)/2 );
    window->icon((char*)LoadIcon(fl_display, MAKEINTRESOURCE(IDI_ICON2))); // <-- ADD THIS LINE
    window->show(argc, argv);
    Fl::add_idle(CBIdle, window);
    //KeyFileCreate();

    //return Fl::run();
    while(Fl::wait()) {
        Sleep(50);  //sleep for 50ms
    }
}

void CBIdle(void * data)
{
    MainWindow *pwin = (MainWindow *)data;
    if (g_share_para.type.item_idx[g_share_para.type.val]>10) {
        pwin->chk_btn_force_->deactivate();
        pwin->choice_prog_->deactivate();
        pwin->choice_61850_->deactivate();
        pwin->label_reboot_->deactivate();
        pwin->label_debug_->deactivate();
    } else {
        pwin->chk_btn_force_->activate();
        if (pwin->chk_btn_force_->value()) pwin->choice_prog_->activate();
        else pwin->choice_prog_->deactivate();
        pwin->choice_61850_->activate();
        pwin->label_reboot_->activate();
        pwin->label_debug_->activate();
    }
}

void CBMainWindow(Fl_Widget * wdg, void * data)
{
    SharePara * para = (SharePara *)data;
    if (fl_choice("Are you sure you want to quit?", "Yes", "No", NULL)!=0) {
        return;
    }
    MainWindow *pwin = (MainWindow *)wdg;
    if (para->reboot == 0) {
        para->reboot = 1;
        para->update = true;
    }
    if (para->kernel == 1) {
        para->kernel = 0;
        para->update = true;
    }
    if (para->pause == 1) {
        para->pause = 0;
        para->update = true;
    }
    ParaFileSave(para);

    //KeyFileClean();
    exit(0);
}

void ParaFileRead(SharePara *para)
{
    memset(para, 0, sizeof(para));
    para->prog.val = 1;
    strcpy(para->ip, "192.168.1.100");
    strcpy(para->port, "12307");
    para->reboot = 1;
    para->kernel = 0;
    para->pause = 0;

    FILE *fp;
    const char *file_name = kCfgFile;
    if ((fp = fopen(file_name, "r"))==NULL) {
        return;
    }

    char stri[64];
    char name[32], val[32];
    unsigned long scan = 0;
    while(fgets(stri, 64, fp)!=NULL) {
        do {
            sscanf(stri, "%[a-zA-Z0-9_]%*[ =]%s", name, val);
            //printf("name=%s, val=%s\n", name, val);
            if (!(scan&1)) {
                if ( !strcmp(name, "ip") ) {
                    strcpy(para->ip, val);
                    scan |= 1;
                    break;
                }
            }
            if (!(scan&(1<<1))) {
                if ( !strcmp(name, "port") ) {
                    strcpy(para->port, val);
                    scan |= (1<<1);
                    break;
                }
            }
            if (!(scan&(1<<2))) {
                if ( !strcmp(name, "typeidx") ) {
                    para->type.val = atoi(val);
                    scan |= (1<<2);
                    break;
                }
            }
            if (!(scan&(1<<3))) {
                if ( !strcmp(name, "prog") ) {
                    sscanf(val, "%x", &para->prog.val);
                    scan |= (1<<3);
                    break;
                }
            }
            if (!(scan&(1<<4))) {
                if ( !strcmp(name, "force") ) {
                    para->force = atoi(val);
                    scan |= (1<<4);
                    break;
                }
            }
            if (!(scan&(1<<5))) {
                if ( !strcmp(name, "cst61850") ) {
                    para->t61850.val = atoi(val)-1;
                    scan |= (1<<5);
                    break;
                }
            }
            if (!(scan&(1<<6))) {
                if ( !strcmp(name, "reboot") ) {
                    para->reboot = atoi(val);
                    scan |= (1<<6);
                    break;
                }
            }
            if (!(scan&(1<<7))) {
                if ( !strcmp(name, "vendor") ) {
                    para->vendor.val = atoi(val);
                    if (para->vendor.val) para->vendor.val -= 1;
                    scan |= (1<<7);
                    break;
                }
            }
        } while(0);
    }
    fclose(fp);
}

/*!
    Input:  idx -- typeidx
    Return: type
*/
int TypeIdx2Type(int idx)
{
    const char * file_name = ".sys/script/type_map.lst";
    FILE *fp;
    if ((fp = fopen(file_name, "r"))==NULL) {
        printf("Can't open %s\n", file_name);
        return -2;
    }

    char stri[64];
    int ret, num=100;
    for(int i=0; i<idx+1;) {
        ret = fscanf(fp, "%d:%[^\n]", &num, stri);
        if (ret==EOF) break;
        if (ret<2) {
            fgets(stri, 64, fp);
            continue;
        }
        i++;
    }
    fclose(fp);
    if (num<=2) num = 1;
    return num;
}

void ParaFileSave(SharePara *para)
{
    if (!para->update) return;
    para->update = false;
    FILE *fp;
    const char *file_name = kCfgFile;
    if ((fp = fopen(file_name, "w"))==NULL) {
        return;
    }

    fprintf(fp, "ip = %s\n", para->ip);
    fprintf(fp, "port = %s\n", para->port);
    fprintf(fp, "typeidx = %d\n", para->type.val);
    //fprintf(fp, "type = %d\n", TypeIdx2Type(para->type.val));
    fprintf(fp, "type = %d\n", para->type.item_idx[para->type.val]);
    
    fprintf(fp, "prog = 0x%X\n", para->prog.val);
    fprintf(fp, "force = %d\n", para->force);
    fprintf(fp, "cst61850 = %d\n", para->t61850.val+1);
    fprintf(fp, "reboot = %d\n", para->reboot);
    fprintf(fp, "vendor = %d\n", para->vendor.val+1);
    fprintf(fp, "kernel = %d\n", para->kernel);
    fprintf(fp, "pause = %d\n", para->pause);
    fclose(fp);
}

void KeyFileCreate()
{
    FILE *f_strm;
    const char *file_name = kKeyFile;

    f_strm = fopen(file_name, "w");
    fwrite(private_key, strlen(private_key), 1, f_strm);
    fclose(f_strm);

    file_name = kRespYesFile;
    f_strm = fopen(file_name, "w");
    fwrite("y", 1, 1, f_strm);
    fclose(f_strm);

    /*file_name = kRespNoFile;
    f_strm = fopen(file_name, "w");
    fwrite("n", 1, 1, f_strm);
    fclose(f_strm);*/
}

void KeyFileClean()
{
    remove(kKeyFile);
    remove(kRespYesFile);
    //remove(kRespNoFile);
}

