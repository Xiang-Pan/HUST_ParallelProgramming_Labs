#include <bits/stdc++.h>
// #include <vector>
# include "akari.h"
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

namespace aka
{

typedef struct point
{
	int x;
	int y;
	int val;
} point;

point black[100];

int cover [5][20][20];
int type  [5][20][20];
int map  [20][20];



int dx[4] = {1,-1,0,0};
int dy[4] = {0,0,1,-1};

bool ok;
int idx;

int ans;

int n=0;
int m=0;
int b=0;

//parallel para struct
struct theread_para
{
    int id;
    int count;
    int depth;
    int dt;
};

int thread_num=4;
// std::thread threads[4];
theread_para paras[4];
int tn;
int parallel_depth=0;



bool cmp(point A, point B)
{
	return A.val < B.val;
}

bool in_range(int x, int y)
{
	return (x >= 1 && y >= 1 && x <= n && y <= m);
}


int on(int x, int y,int dt)// put in (x,y)
{
	int ret=1; //light num
	int i = 0;
	int j = 0;
	for(int k=0;k<4;k++)
	{
		i=x+dx[k];
		j=y+dy[k];
		while (in_range(i,j)&& type[dt][i][j] != 2)
		{
			cover[dt][i][j]++;
			if(cover[dt][i][j] == 1)
			{
				ret ++;
			}
			i+=dx[k];
			j+=dy[k];
		}
	}
	return ret;
}

int off(int x,int y,int dt)
{
	int ret=1; //light num
	int i = 0;
	int j = 0;
	for(int k=0;k<4;k++)
	{
		i=x+dx[k];
		j=y+dy[k];
		while (in_range(i,j) &&type[dt][i][j] != 2)
		{
			cover[dt][i][j]--;
			i+=dx[k];
			j+=dy[k];
		}
	}
	return ret;
}


int check_overlap(int dt)
{
	int i,j;
    int tmp;
    int ret = 2;
    for(i = 0;i < b;i++) // check black
    {
        if(black[i].val == -1)
            continue;
        tmp = 0;
        for(j = 0;j < 4;j ++)//check black neibor
        {
            int tx = black[i].x + dx[j];
            int ty = black[i].y + dy[j];
			if(!in_range(tx,ty))
			{
				continue;
			}
            if(type[dt][tx][ty] == 5)
                tmp ++;
        }
        if(tmp > black[i].val)
            return 0;
        if(tmp != black[i].val)
            ret = 1;
    }
    return ret;
}

void dfs_for_white(int x,int y,int count,int depth,int dt)
{
	if(ok)
	{
		return;
	}
	if(count == m*n-b) 
	{
		ok=true;
		for(int i=1;i<=n;i++)
		{
			for(int j=1;j<=m;j++)
			{
				map[i][j]=type[dt][i][j];
			}
		}
	}
	for(int i = x;i <= n;i ++)
    {
        for(int j = 1;j <= m;j ++)
        {
            if(type[dt][i][j] == 0 && cover[dt][i][j] == 0)
            {
                type[dt][i][j] = 5;
                if(check_overlap(dt) != 2) //check condition
                {
                    type[dt][i][j] = 0;
                    continue;
                }
                int light_num = on(i,j,dt);
                dfs_for_white(i,j,count + light_num,depth + 1,dt);
                off(i,j,dt);
                type[dt][i][j] = 0;
            }
        }
    }
	return;
}


void dfs_for_black_pthread()
{

}

void dfs_for_black(int id,int count,int depth,int dt)
{
    if(dt!=0)
    {
        // (threads[dt]).detach();
    }
    // if(dt!=-1)
    // {
    //     int i=dt;
    //     int tx = black[id].x + dx[i];
    //     int ty = black[id].y + dy[i];
    //     if(type[tx][ty] == 0 && cover[tx][ty] == 0)
    //     {
    //         type[tx][ty] = 5;
    //         if(check_overlap(dt) == 0)
    //         {
    //             type[tx][ty] = 0;
    //             return;
    //         }
    //         int tp = on(tx,ty);
    //         dfs_for_black(id,count + tp,depth + 1,-1);
    //         off(tx,ty);
    //         type[tx][ty] = 0;
    //     }
    // }
	if(ok)
	{
		return;
	}
	if(id == b)
    {
        if(check_overlap(dt) == 2)
        {
            dfs_for_white(1,1,count,depth,dt);
        }
        return;
    }
	int i=0;
	int j=0;
	for(i=0;i<4;i++) // chekc neibor
	{
		if(type[dt][black[id].x + dx[i]][black[id].y + dy[i]] == 5)
            j++;
	}
	if(j > black[id].val)
    {
        return;
	}
	else
    {
        if(j == black[id].val)
            dfs_for_black(id + 1,count,depth,dt);
        else
        {
            if(parallel_depth<=0) // contrain the depth for parallel
            {
                parallel_depth+=1;
                // threads[0] = std::thread();
                // thread t[5];
                pthread_t threads[thread_num];
                for(tn=1;tn<=thread_num;tn++)
                {
                    
                    memcpy(type[tn],type[0],sizeof(type[0]));
                    // pthread_create(&threads[tn],NULL,par_fun,(void *)&tn);
                    // t[tn] =std::thread(&dfs_for_black,id,count,depth,tn);
                    // t[tn].detach();
                }
                for(tn=1;tn<=thread_num;tn++)
                {
                    
                }
            }
            else
            {
                for(i = 0;i < 4;i ++)
                {
                    int tx = black[id].x + dx[i];
                    int ty = black[id].y + dy[i];
                    if(!in_range(tx,ty))
                        continue;
                    if(type[dt][tx][ty] == 0 && cover[dt][tx][ty] == 0)
                    {
                        type[dt][tx][ty] = 5;
                        if(check_overlap(dt) == 0)
                        {
                            type[dt][tx][ty] = 0;
                            continue;
                        }
                        int tp = on(tx,ty,dt);
                        dfs_for_black(id,count + tp,depth + 1,dt);
                        off(tx,ty,dt);
                        type[dt][tx][ty] = 0;
                    }
                }
            }
        }
    }
}





vector<vector<int> > solveAkari(vector<vector<int> > &g) 
{
	n=g.size();
	m=g[0].size();
	b=0;
	for(int i=0;i<g.size();i++) // add black to struct
	{
		for(int j=0;j<g[i].size();j++)
		{
			if(g[i][j]>=-1)
			{
				black[b].x=i+1;
				black[b].y=j+1;
				black[b++].val=g[i][j];
				type[0][i+1][j+1]=2;
			}
		}
	}
	sort(black,black+b,cmp);
	ok=false;
	int i;
	for(i=0;i<b;i++)
	{
		if(black[i].val>0)
		{
			break;
		}
	}
	while(1)
	{
		if(ok||ans>m*n-b)
		{
			break;
		}
        // int **a=(int **)&type;
		dfs_for_black(i,0,0,0);
		if(ok)
		{
			break;
		}
		ans++;
	}
    for(int i=1;i<=n;i++)
    {
    	for(int j=1;j<=m;j++)
        {
    		if(map[i][j]==5) 
            {
                g[i - 1][j - 1] = 5;
            }
		}
    }
    for(tn=0;tn<=thread_num;tn++)
    {
    //    threads[tn].join();
    }
    return g;
}

} // namespace aka


void checkAkari(vector<vector<int> > & g, vector<vector<int> > & ans);
void displayAkari(vector<vector<int> > & ans);

int main ()
{
    int n, m, tmp;
    freopen("in.txt","r",stdin);
    cin >> n >> m;
    vector<vector<int> > g;
    for (int i = 1; i <= n; ++i) 
    {
        g.push_back(vector<int>());
        for (int j = 1; j <= m; ++j) {
            cin >> tmp;
            g[i-1].push_back(tmp);
        }
    }
    vector<vector<int> > ans = aka::solveAkari(g);
    checkAkari(g, ans);
    return 0;
}

void checkAkari(vector<vector<int> > & g, vector<vector<int> > & ans)
{
    int n = (int)g.size(), m = (int)g[0].size();
    if (n != (int)ans.size() || m != (int)ans[0].size()) {
        cout << "The original size does not match!" << endl;
        displayAkari(ans);
        return ;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (g[i][j] >= -1 && g[i][j] <= 4) {
                if (ans[i][j] != g[i][j]) {
                    cout << "The original picture does not match the black grid!" << endl;
                    displayAkari(ans);
                    return ;
                }
            }
            else {
                if (ans[i][j] >= -1 && ans[i][j] <= 4) {
                    cout << "The original picture does not match the black grid!" << endl;
                    displayAkari(ans);
                    return ;
                }
            }
        }
    }
    int ps[4][2] = {-1, 0, 1, 0, 0, -1, 0, 1};
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (ans[i][j] >= 0 && ans[i][j] <= 4) {
                int count = 0;
                for (int k = 0; k < 4; ++k) {
                    int dx = i + ps[k][0], dy = j + ps[k][1];
                    if (dx >= 0 && dx < n && dy >= 0 && dy < m && ans[dx][dy] == 5) ++count;
                }
                if (count != ans[i][j]) {
                    cout << "The number of lights around the black grid does not match the number!" << endl;
                    displayAkari(ans);
                    return ;
                }
            }
            else if (ans[i][j] == 5) {
                for (int k = i + 1; k < n; ++k) {
                    if (ans[k][j] >= -1 && ans[k][j] <= 4) break;
                    if (ans[k][j] == 5) {
                        cout << "Two lights shine on each other!" << endl;
                        displayAkari(ans);
                        return ;
                    }
                }
                for (int k = j + 1; k < m; ++k) {
                    if (ans[i][k] >= -1 && ans[i][k] <= 4) break;
                    if (ans[i][k] == 5) {
                        cout << "Two lights shine on each other!" << endl;
                        displayAkari(ans);
                        return ;
                    }
                }
            }
            else if (ans[i][j] == -2) {
                bool type = true;
                for (int k = i + 1; k < n; ++k) {
                    if (ans[k][j] >= -1 && ans[k][j] <= 4) break;
                    if (ans[k][j] == 5) type = false;
                }
                for (int k = i - 1; k >= 0; --k) {
                    if (ans[k][j] >= -1 && ans[k][j] <= 4) break;
                    if (ans[k][j] == 5) type = false;
                }
                for (int k = j + 1; k < m; ++k) {
                    if (ans[i][k] >= -1 && ans[i][k] <= 4) break;
                    if (ans[i][k] == 5) type = false;
                }
                for (int k = j - 1; k >= 0; --k) {
                    if (ans[i][k] >= -1 && ans[i][k] <= 4) break;
                    if (ans[i][k] == 5) type = false;
                }
                if (type == true) {
                    cout << "There's a place where all the lights don't shine!" << endl;
                    displayAkari(ans);
                    return ;
                }	
            }
        }
    }
    cout << "The answer is right!";
}

void displayAkari(vector<vector<int> > & ans)
{
    printf("your magic matrix is following: \n");
    int n = ans.size(), m = ans[0].size();
    for (int i = 0; i < n; ++i) 
	{
        for (int j = 0; j < m; ++j) 
		{
            printf("%4d", ans[i][j]);
        }
        printf("\n");
    }
}