#ifndef _UP_GUI_H_
#define _UP_GUI_H_
//---------------------------------------------------------------------------
const static char *private_key = {
"PuTTY-User-Key-File-2: ssh-rsa\n\
Encryption: none\n\
Comment: rsa-key-pqnet\n\
Public-Lines: 4\n\
AAAAB3NzaC1yc2EAAAABJQAAAIBoqTe0S3FYH9ahtdS4yfDNBo7apGyYhDiJWHr2\n\
aj1Ob11EmiSDrSJBWHRfxP1sFerjLgD+Kc1c/xtHBnXM76MyDBKs18c7juLtSeXW\n\
UCqQrL+rqmLnmcvDqV2WmvCE7c1NIFHzEXy7t+bPMAZOwE7vCoGB55YW7eCHV730\n\
X+G3CQ==\n\
Private-Lines: 8\n\
AAAAgBahIM0JZJ1uqvKILf5w2h599V+02TW7uTJz/ukrujp48ZJK13Zqn59tEj44\n\
biU1K94XyN0CHpCl3GJiQvvgyVamQ9u9a/JnQicFJLY3VOYSWt/OYcOG7rwPTt+7\n\
ELN0VPkaKkjcxhTqmpRRd/p4iSx6h6I4KR3xzDmRG3M7lOlNAAAAQQC+TyC2lbTR\n\
Ys2dFWlZDYNOGgYQ4LFO3OBzZdxqnZGcH2BqL+aNU3YhvL9houlbxd6XKw+BlKYU\n\
gaU0IVp4f7HTAAAAQQCMybPVHkWf7is1BqI3NGVJwVp+/g363r1vKu9vJbVXpOyd\n\
HnuHFSTZHyHVmH73DqK8oyQiQ0ZbDlfkfaTz0U4zAAAAQF/sNFXFX+Umn2ShmcyJ\n\
FM94Q+1P3f/VO47/uZ1jg4lClYktq4e93arozLixuA4nhRAOK0n2Fz/SkWT/Mgbp\n\
Oaw=\n\
Private-MAC: 13ec09c2ca3e3f23141575481d43dea7698ba46a\n"
};

const int kMaxTypeNum = 32;
struct ChoiceItem {
    int val;   //Correspond to choice_type_->value()
               //bit access for prog. 0=no, 1=yes
    int item_idx[kMaxTypeNum];  //Save type index
};

struct SharePara {
    ChoiceItem type;    //Correspond to choice_type_
    ChoiceItem prog;    //Correspond to choice_prog_
    ChoiceItem t61850;  //Correspond to choice_61850_
    ChoiceItem vendor;  //Correspond to choice_vendor_
    int force;
    int force_prog; //bit access, 0=no, 1=yes
    char ip[32];
    char port[8];
    bool update;    //Whether these parameters have been updated. 1=yes, 0=no
    void *pmwin;    //point to MainWindow
    int reboot;     //Whether reboot after upgrade. 1=yes, 0=no
    int debug;
    int kernel;     //Whether update kernel. 1=yes, 0=no
    int pause;      //Whether auto close cmd line.  1=yes, 0=no
};

extern SharePara g_share_para;   //share param

void ParaFileRead(SharePara *para);
void ParaFileSave(SharePara *para);
void CBMainWindow(class Fl_Widget * wdg, void * data);
void CBIdle(void * data);
void KeyFileCreate();
void KeyFileClean();

#endif //_UP_GUI_H_
