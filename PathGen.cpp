#include<string>
#include<iostream>
#include<fstream>
#include<cstdio>
#include<cstring>
#define KEY_LEN 128
#define PLAIN_LEN 128
#define START_BIT 0
#define ROUND 8

using namespace std;

struct key_para {
	int round,bit;
};

bool Key_known[24][KEY_LEN];
bool Visited[24][KEY_LEN];
bool KeyTable[KEY_LEN][KEY_LEN];
bool AKI_needed[1000];
bool global_flag=false;
key_para my_stack[100];
int stack_top=0,x=1;
FILE *outp;
bool FlowMap[KEY_LEN][KEY_LEN];
bool state[KEY_LEN];
bool lstate[KEY_LEN];
int result[KEY_LEN];
int lresult[KEY_LEN];
int rev_pos[KEY_LEN];

int DependKey(int n,int r) {
    /*
     * Change the following code to fit the key extraction function
     * of the encryption algorithm you wan to analyze
     */
	const int f[8]={1,3,4,6,13,14,15,16};
    return n;
	/*   128-bit Rectangle 
	//Rec128中，子密钥是每行的右侧16个组成的4*16=64bit
	if(n<16) return n;		//第一行
	if(n<32) return n+16;	//第二行，32+(n-16)
	if(n<48) return n+32;	//第三行，64+(n-32)
	return n+48;			//第四行，96+(n-48)
    */
    /*
    //Simon 64-bit
    if(n<16) return -1;
    return n+32;
    //Skinny 64bit z=1
    if(n>=32) return -1;
    return n; 
	*/
}

int main(int argc,char *argv[]) {
    if(argc>1) {
        sscanf(argv[1],"%d",&x);
    }
	bool RoundTable[PLAIN_LEN][PLAIN_LEN];
    /* Change the filename to the dependency matrix of the 
     * round function you want to analyze
     */
	FILE *roundmatrix=fopen("MidoriRound128.txt","r");
	memset(RoundTable,false,sizeof(RoundTable));
	for(int i=0;i<PLAIN_LEN;i++) {
		char line[160];
		fscanf(roundmatrix,"%s",line);
		for(int j=strlen(line)-1;j>=0;j--) {
			if(line[j]=='1') RoundTable[i][j]=true;
		}
	}
	fclose(roundmatrix);

	key_para AKI_para[1000],AKI_para_origin[1000];
	char filename[100];
	int res[PLAIN_LEN];

	int round=1,AKI=0;
	bool KSP[20][PLAIN_LEN];
    for(round=1;round<=ROUND;round++) {
        for(int bit=START_BIT;bit<PLAIN_LEN;bit++) {
            sprintf(filename,"data/Round_%d/bit_%03d.txt",round,bit);
            outp=fopen(filename,"w");
            int AKI_bit=0;
            memset(KSP,false,sizeof(KSP));
            KSP[round][bit]=true;
            for(int r=round-1;r>=0;r--) {
                for(int k=0;k<PLAIN_LEN;k++) {
                    if(KSP[r+1][k]) 
                        for(int kk=0;kk<PLAIN_LEN;kk++) {
                            KSP[r][kk]|=RoundTable[k][kk];
                        }
                }
            }
            memset(AKI_para,0,sizeof(AKI_para));
            memset(AKI_needed,true,sizeof(AKI_needed));
            memset(Visited,false,sizeof(Visited));
            memset(Key_known,false,sizeof(Key_known));
            int count=0;
            for(int r=0;r<round;r++) {
                for(int k=0;k<PLAIN_LEN;k++) {
                    if(KSP[r][k]) {
                        AKI_para[AKI_bit].round=r;
                        AKI_para[AKI_bit].bit=DependKey(k,r);
                        AKI_para_origin[AKI_bit]=AKI_para[AKI_bit];
                        int kk=DependKey(k,r);
                        if(kk>=0) {
                            fprintf(outp,"WK%02d-%03d ",r,kk);
                            AKI_bit++;
                            if(AKI_bit%10==0) fprintf(outp,"\n");
                            /* For Midori-64 */
                            /*
                            if(r==0) {
                                AKI_para[AKI_bit].round=r;
                                AKI_para[AKI_bit].bit=kk+64;
                                AKI_para_origin[AKI_bit]=AKI_para[AKI_bit];
                                fprintf(outp,"WK%02d-%03d ",r,kk+64);
                                AKI_bit++;
                                if(AKI_bit%10==0) fprintf(outp,"\n");
                            }
                            */
                        }
                    }
                }
            }
            //fprintf(outp,"WK%02d-%03d\n",round,bit);
            fclose(outp);
        }
    }
			
	return 0;
}
