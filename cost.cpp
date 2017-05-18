#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<queue>
#include<algorithm>
#include<cstring>
#define INF (1<<30)
#define LEN 64
#define KEY_LEN 128
#define N 12800
#define M 163840
#define ROUND 6

using namespace std;

class AKIFlow {
private:
    bool keymatrix[KEY_LEN][KEY_LEN];
    bool vis[N];
    int n,m,s,t,sumFlow;
    queue<int> q;
    int head[N],cnt,pp[N],dis[N];
    struct edge {
        int u,v,w,c,next;
    } e[M];
    struct para {
        int round,bit;
    } v[N];

    void addedge(int u,int v,int c,int w) {
        e[cnt]=(edge){u,v,w,c,head[u]}; head[u]=cnt++;
        e[cnt]=(edge){v,u,0,-c,head[v]}; head[v]=cnt++;
    }

    bool SPFA(int s,int t,int n) {
        int i,u,v;
        queue<int>qu;
        memset(vis,false,sizeof(vis));
        memset(pp,-1,sizeof(pp));
        for(i=0;i<=n;i++) {
            dis[i]=INF;
        }
        vis[s]=true;
        dis[s]=0;
        qu.push(s);
        while(!qu.empty()) {
            u=qu.front();
            qu.pop();
            vis[u]=false;
            for(i=head[u];i!=-1;i=e[i].next) {
                v=e[i].v;
                if(e[i].w && dis[v]>dis[u]+e[i].c) {
                    dis[v]=dis[u]+e[i].c;
                    pp[v]=i;
                    if(!vis[v]) {
                        qu.push(v);
                        vis[v]=true;
                    }
                }
            }
        }
        if(dis[t]==INF) return false;
        else return true;
    }

    int MCMF(int s,int t,int n) {
        int flow=0;
        int i,minflow,mincost;
        mincost=0;
        while(SPFA(s,t,n)) {
            minflow=INF+1;
            for(i=pp[t];i!=-1;i=pp[e[i].u]) {
                if(e[i].w<minflow) minflow=e[i].w;
            }
            flow+=minflow;
            for(i=pp[t];i!=-1;i=pp[e[i].u]) {
                e[i].w-=minflow;
                e[i^1].w+=minflow;
            }
            mincost+=dis[t]*minflow;
        }
        sumFlow=flow;
        return sumFlow;
    }

    bool compare(para a,para b) {
        if(a.round==b.round) return a.bit<b.bit;
        return a.round<b.round;
    }

public:

    AKIFlow() {
        memset(head,-1,sizeof(head));
        for(int i=0;i<KEY_LEN;i++) {
            for(int j=0;j<KEY_LEN;j++) {
                keymatrix[i][j]=true;
            }
        }
    }

    void init(char DPathFile[]) {
        FILE *DPath=fopen(DPathFile,"r");
        int btm=1,top=1;
        char param[20];
        int used[30][KEY_LEN];
        memset(used,-1,sizeof(used));
        while(fscanf(DPath,"%s",param)>0) {
            sscanf(param,"WK%d-%d",&v[top].round,&v[top].bit);
            used[v[top].round][v[top].bit]=top;
            addedge(top*2+1,1,0,INF);
            addedge(top*2,top*2+1,0,1);
            top++;
        }
        while(btm<top) {
            int toptmp=top;
            for(int i=btm;i<top;i++) {
                if(v[i].round==0) {
                    addedge(0,i*2,0,INF);
                    continue;
                }
                for(int j=0;j<KEY_LEN;j++) {
                    if(keymatrix[v[i].bit][j]) {
                        if(used[v[i].round-1][j]==-1) {
                            addedge(toptmp*2,toptmp*2+1,0,1);
                            v[toptmp].round=v[i].round-1;
                            v[toptmp].bit=j;
                            used[v[i].round-1][j]=toptmp;
                            toptmp++;
                        }
                        addedge(used[v[i].round-1][j]*2+1,i*2,1,INF);
                    }
                }
            }
            btm=top;
            top=toptmp;
        }
        fclose(DPath);
        n=top*2;
        s=0;
        t=1;
    }

    int GetAKI() {
        int ans=MCMF(s,t,n);
        bool ansM[KEY_LEN][KEY_LEN];
        memset(ansM,false,sizeof(ansM));
        for(int i=0;i<cnt;i++) {
            if(e[i].w>0 && e[i].u!=0 && e[i].v!=1 && e[i].u!=(e[i].v^1)) {
                if(v[e[i].v/2].round<v[e[i].u/2].round)  
                    ansM[v[e[i].u/2].bit][v[e[i].v/2].bit]=true; 
            }
        }
        for(int i=0;i<KEY_LEN;i++) {
            bool flag=true;
            for(int j=0;j<KEY_LEN;j++) {
                if(ansM[i][j]) flag=false;
            }
        }
        FILE *outp=fopen("AnsMatrix.txt","w");
        for(int i=0;i<KEY_LEN;i++) {
            for(int j=0;j<KEY_LEN;j++) {
                fprintf(outp,"%d",ansM[i][j]);
            }
            fprintf(outp,"\n");
        }
        fclose(outp);
        return ans;
    }

    void Clean() {
        cnt=0;
        memset(head,-1,sizeof(head));
    }
};

int main(int argc, char *argv[]) {
    AKIFlow AKI;
    if(argc>1) {
        AKI.init(argv[1]);
        printf("M over with flow %d.\n",AKI.GetAKI());
    }
    return 0;
}
