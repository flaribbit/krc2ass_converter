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
char *kaomoji[3]={"(������)","(���أ�)","(���ţ�)"};
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
		MessageBox(hwndDlg,"�ļ����ʹ���","����",MB_ICONSTOP);
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
		//��������ļ���
		strcpy(out,file);
		for(p=out;p[3];p++);
		strcpy(p,"txt");
		//���ļ�
		fp=fopen(out,"w");
		//[53883,3092]<0,632,0>One <632,784,0>Night <1416,372,0>in <1788,548,0>��<2336,755,0>��
		//��������
		for(i=0;i<dstsize;i++){
			switch(dst[i]){
			//case '<':
			//	break;
			case '[':
				i++;
				//�����ʱ��
				if(dst[i]>='0'&&dst[i]<='9'){
					//��ȡ��ʼʱ��
					start=atoi((char*)dst+i);
					//��ȡ����ʱ��
					while(dst[i-1]!=',')i++;
					end=atoi((char*)dst+i)+start;
					//д���ļ�
					fprintf(fp,"Dialogue: 0,%02d:%02d.%02d,%02d:%02d.%02d,Default,,0,0,0,,",
								start/60000,start/1000%60,start/10%100,
								end/60000,end/1000%60,end/10%100);
					//������
					while(dst[i]!='\n'){
						//�ҵ��ڶ�����
						while(dst[i-1]!='<')i++;
						while(dst[i-1]!=',')i++;
						tmp=atoi((char*)dst+i);
						fprintf(fp,"{\\k%d}",(tmp+5)/10);
						//�ҵ��ı�
						while(dst[i-1]!='>')i++;
						while(dst[i]!='<'){
							fputc(dst[i],fp);
							if(dst[i]=='\n')break;
							i++;
						}
					}
					//while(dst[i]!='\n')i++;
				}
				//��������Ϣ
				else{
					//������Ϣ
					while(dst[i]!='\n')i++;
				}
				break;
			}
		}//�������ݽ���
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

    //���ļ�
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
