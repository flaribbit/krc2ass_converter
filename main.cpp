#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include <sys/stat.h>
#include "zlib/zconf.h"
#include "zlib/zlib.h"

#define MAXLEN 1024
#define MAXBUF 2<<20
HINSTANCE hInst;
char *kaomoji[3]={"(￣▽￣)","(￣ω￣)","(￣ε￣)"};
//unsigned char key[]={'0','G','a','w','^','2','t','G','Q','6','1','-',0xCE,0xD2,'n','i'};
unsigned char key[]={0x40,0x47,0x61,0x77,0x5E,0x32,0x74,0x47,0x51,0x36,0x31,0x2D,0xCE,0xD2,0x6E,0x69};
unsigned char dst[MAXBUF];

int ProcessFile(HWND hwndDlg,char *file){
	int i;
	FILE *krcfile;
	struct stat stat;
	unsigned char *src;
	unsigned long dstsize=MAXBUF;
	krcfile=fopen(file,"rb");
	fstat(fileno(krcfile),&stat);
	src=(unsigned char*)malloc(stat.st_size);
	fread(src,1,stat.st_size,krcfile);
	if(memcmp(src,"krc1",4)){
		MessageBox(hwndDlg,"文件类型错误！","错误",MB_ICONSTOP);
		return -1;
	}
	src+=4;
	for(i=0;i<stat.st_size;i++){
		src[i]=(unsigned char)(src[i]^key[i%16]);
	}
	if(uncompress(dst,&dstsize,src,stat.st_size)==Z_OK){
		int start,end,tmp,j;
		char buf[10],out[MAXLEN],*p;
		FILE *fp;
		//生成输出文件名
		strcpy(out,file);
		for(p=out;p[3];p++);
		strcpy(p,"txt");
		//打开文件
		fp=fopen(out,"w");
		//[53883,3092]<0,632,0>One <632,784,0>Night <1416,372,0>in <1788,548,0>北<2336,755,0>京
		//处理数据
		for(i=0;i<dstsize;i++){
			switch(dst[i]){
			//case '<':
			//	break;
			case '[':
				i++;
				//如果是时间
				if(dst[i]>='0'&&dst[i]<='9'){
					//提取开始时间
					start=atoi((char*)dst+i);
					//提取结束时间
					while(dst[i-1]!=',')i++;
					end=atoi((char*)dst+i)+start;
					//写入文件
					fprintf(fp,"Dialogue: 0,%02d:%02d.%02d,%02d:%02d.%02d,Default,,0,0,0,,",
								start/60000,start/1000%60,start/10%100,
								end/60000,end/1000%60,end/10%100);
					//处理歌词
					while(dst[i]!='\n'){
						//找到第二个数
						while(dst[i-1]!='<')i++;
						while(dst[i-1]!=',')i++;
						tmp=atoi((char*)dst+i);
						fprintf(fp,"{\\k%d}",(tmp+5)/10);
						//找到文本
						while(dst[i-1]!='>')i++;
						while(dst[i]!='<'){
							fputc(dst[i],fp);
							if(dst[i]=='\n')break;
							i++;
						}
					}
					//while(dst[i]!='\n')i++;
				}
				//否则是信息
				else{
					//忽略信息
					while(dst[i]!='\n')i++;
				}
				break;
			}
		}//处理数据结束
		fclose(fp);
	}
	//puts((char*)dst);
	return 0;
}

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
    	time_t now;
    	SetDlgItemText(hwndDlg,IDC_STATIC1,kaomoji[time(&now)%3]);
    }
    return TRUE;

    case WM_CLOSE:
    {
        EndDialog(hwndDlg, 0);
    }
    return TRUE;

    //丢文件
    case WM_DROPFILES:
	{
		char file[MAXLEN];
		DragQueryFile((HDROP)wParam,0,file,MAXLEN);
		puts(file);
		ProcessFile(hwndDlg,file);
	}
	return TRUE;
	
    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        }
    }
    return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
