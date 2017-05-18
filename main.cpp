#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<queue>
#include<algorithm>
#include<cstring>
#define INF (1<<30)
#define LEN 128
#define KEY_LEN 128
#define N 12800
#define M 163840
#define ROUND 10

using namespace std;

class AKIFlow {
private:
    bool keymatrix[KEY_LEN][KEY_LEN];
    int n,m,s,t;
    queue<int> q;
    int head[N],cnt,cur[N];
    int dis[N],ef[N],gap[N];
    int cut[N],cuttop;
    bool vis[N];
    struct edge {
        int v,w,next;
    } e[M];
    struct para {
        int round,bit;
    } v[N];

    void addedge(int u,int v,int w) {
        e[cnt]=(edge){v,w,head[u]}; head[u]=cnt++;
        e[cnt]=(edge){u,0,head[v]}; head[v]=cnt++;
    }

    void push(int u,int i) {
        int aug=min(e[i].w,ef[u]),v=e[i].v;
        if(v!=s && v!=t && ef[v]==0) q.push(v);
        e[i].w-=aug;
        e[i^1].w+=aug;
        if(u!=s) ef[u]-=aug;
        if(v!=s) ef[v]+=aug;
    }

    void relabel(int u) {
        int mindis=INF,nl=dis[u];
        --gap[dis[u]];
        for(int i=head[u];i!=-1;i=e[i].next) {
            if(e[i].w!=0 && mindis>dis[e[i].v]) {
                cur[u]=i;
                mindis=dis[e[i].v];
            }
        }
        ++gap[dis[u]=mindis+1];
                        //!!!
        if(gap[nl]!=0 || nl>=n) return;
        for(int i=0;i<n;i++) {
                            //!!!
            if(dis[i]>nl && dis[i]<n && i!=s) {
                --gap[dis[i]];
                      //!!!
                ++gap[n];
                      //!!!
                dis[i]=n;
            }
        }
    }

    void discharge(int u) {
        while(ef[u]>0) {
            if(cur[u]==-1) relabel(u);
            int v=e[cur[u]].v,w=e[cur[u]].w;
            if(w>0 && dis[u]==dis[v]+1) push(u,cur[u]);
            cur[u]=e[cur[u]].next;
        }
    }

    void BFS() {
        q.push(t);dis[t]=0;
        while(!q.empty()) {
            int u=q.front();
            q.pop();
            for(int i=head[u];i!=-1;i=e[i].next) {
                if(e[i].w==0 && dis[e[i].v]==-1 && e[i].v!=s) {
                    dis[e[i].v]=dis[u]+1;
                    q.push(e[i].v);
                }
            }
        }
    }

    void GetCut(int now) {
        vis[now]=true;
        for(int i=head[now];i!=-1;i=e[i].next) {
            if(!vis[e[i].v] && e[i].v==now^1 && e[i].v==now+1 && e[i].w==0) {
                cut[cuttop]=now;
                cuttop++;
            }
            if(e[i].w>0 && !vis[e[i].v]) {
                GetCut(e[i].v);
            }
        }
    }

    int maxflow() {
        int i;
        for(i=0;i<n;i++) {
            cur[i]=head[i];
            dis[i]=-1;
            ef[i]=gap[i]=0;
        }
        BFS(); 
        //!!!
        dis[s]=n-1;
        ef[s]=INF;
                                        //!!!
        for(i=0;i<n;i++) if(dis[i]==-1) dis[i]=n-1;
        for(i=0;i<n;i++) gap[dis[i]]++;
        for(i=head[s];i!=-1;i=e[i].next) {
            push(s,i);
        }
        while(!q.empty()) {
            int u=q.front();
            q.pop();
            discharge(u);
        }
        return ef[t];
    }

    bool compare(para a,para b) {
        if(a.round==b.round) return a.bit<b.bit;
        return a.round<b.round;
    }

public:

    AKIFlow() {
        memset(head,-1,sizeof(head));
        /* Change the file name to the key dependency matrix 
         * you want to analyze
         */
        FILE *KeyFile=fopen("MidoriKey.txt","r");
        char ch;
        for(int i=0;i<KEY_LEN;i++) {
            for(int j=0;j<KEY_LEN;j++) {
                while(ch!='0' && ch!='1') fscanf(KeyFile,"%c",&ch);
                /* WARNING */
                keymatrix[i][j]=ch-'0';
                ch='\0';
            }
            fscanf(KeyFile,"%c",&ch);
        }
        fclose(KeyFile);
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
            addedge(top*2+1,1,INF);
            addedge(top*2,top*2+1,1);
            top++;
        }
        while(btm<top) {
            int toptmp=top;
            for(int i=btm;i<top;i++) {
                if(v[i].round==0) {
                    addedge(0,i*2,INF);
                    continue;
                }
                for(int j=0;j<KEY_LEN;j++) {
                    if(keymatrix[v[i].bit][j]) {
                        if(used[v[i].round-1][j]==-1) {
                            addedge(toptmp*2,toptmp*2+1,1);
                            v[toptmp].round=v[i].round-1;
                            v[toptmp].bit=j;
                            used[v[i].round-1][j]=toptmp;
                            //if(v[toptmp].round==0) addedge(0,toptmp*2,INF);
                            toptmp++;
                        }
                        addedge(used[v[i].round-1][j]*2+1,i*2,INF);
                    }
                }
            }
            btm=top;
            top=toptmp;
        }
        /*
        for(int i=0;i<top;i++) printf("WK%d-%d\n",v[i].round,v[i].bit);
        for(int i=0;i<top*2;i++) {
            printf("vertex: No.%d\n",i);
            for(int j=head[i];j!=-1;j=e[j].next) {
                printf("\tedge: ->%d with c=%d\n",e[j].v,e[j].w);
            }
        }
        */
        fclose(DPath);
        n=top*2;
        s=0;
        t=1;
        //sort(v+1,v+top,compare);
    }

    int GetAKI() {
        return maxflow();
    }

    void PrintCut() {
        cuttop=0;
        memset(vis,false,sizeof(vis));
        GetCut(0);
        for(int i=0;i<cuttop;i++) {
            if(!vis[cut[i]^1]) {
                printf("WK%02d-%03d ",v[cut[i]/2].round,v[cut[i]/2].bit);
                if((i+1)%10==0) printf("\n");
            }
        }
        printf("\n------\n");
        for(int i=head[t];i!=-1;i=e[i].next) {
            if(e[i].w>0) {
            printf("WK%02d-%03d ",v[e[i].v/2].round,v[e[i].v/2].bit);
            if((i+1)%10==0) printf("\n");
            }
        }
    }

    void Clean() {
        cuttop=0;
        cnt=0;
        memset(head,-1,sizeof(head));
    }
};

int main(int argc, char *argv[]) {
    AKIFlow AKI;
    if(argc>1) {
        AKI.init(argv[1]);
        printf("%d\n",AKI.GetAKI());
        AKI.PrintCut();
    } else {
        char DPathFile[50];
        for(int i=1;i<=ROUND;i++) {
            int min=INF,minbit;
            for(int j=16;j<LEN;j++) {
                sprintf(DPathFile,"data/Round_%d/bit_%03d.txt",i,j);
                AKI.init(DPathFile);
                int ans=AKI.GetAKI();
                if(ans<min) {
                    min=ans;
                    minbit=j;
                }
                AKI.Clean();
            }
            printf("Round %d: AKI=%d on %d-th bit\n",i,min,minbit);
        }
    }
    return 0;
}
