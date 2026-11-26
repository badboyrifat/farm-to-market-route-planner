#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

int n = 0;
string name[20];
int road[20][20];
int INF = 1000000;
int seen[20];
int group[20];


int loadData(string fileName)
{
    ifstream fin(fileName.c_str());

    if(fin.fail())
    {
        return 0;
    }

    fin >> n;
    if(n < 2 || n > 20)
    {
        return 0;
    }

    for(int i = 0; i < n; i++)
    {
        fin >> name[i];
    }

    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            road[i][j] = 0;
        }
    }

    int m;
    fin >> m;

    for(int k = 0; k < m; k++)
    {
        int a, b, d;
        fin >> a >> b >> d;

        if(fin.fail())
        {
            return 0;
        }
        if(a < 0 || a >= n || b < 0 || b >= n)
        {
            return 0;
        }
        if(a == b || d <= 0)
        {
            return 0;
        }

        road[a][b] = d;
        road[b][a] = d;
    }

    return 1;
}

int readNumber()
{
    int x;
    cin >> x;

    if(cin.fail())
    {
        if(cin.eof())
        {
            return 0;
        }
        cin.clear();
        cin.ignore(1000, '\n');
        return -1;
    }

    return x;
}

int readPlace(string text)
{
    cout << text;
    int x = readNumber();

    if(x < 0 || x >= n)
    {
        cout << "Invalid place number" << endl;
        return -1;
    }

    return x;
}

void showNetwork()
{
    cout << endl << "Places:" << endl;
    for(int i = 0; i < n; i++)
    {
        cout << i << " = " << name[i];
        if(i == 0)
        {
            cout << " (main market)";
        }
        cout << endl;
    }

    cout << "Roads:" << endl;
    for(int i = 0; i < n; i++)
    {
        for(int j = i + 1; j < n; j++)
        {
            if(road[i][j] > 0)
            {
                cout << name[i] << " <-> " << name[j] << " : " << road[i][j] << " km" << endl;
            }
        }
    }
}

void printPath(int prev[], int v)
{
    if(prev[v] != -1)
    {
        printPath(prev, prev[v]);
        cout << " -> ";
    }
    cout << name[v];
}


void dijkstra(int src, int dst)
{
    int dist[20], prev[20], done[20];

    for(int i = 0; i < n; i++)
    {
        dist[i] = INF;
        prev[i] = -1;
        done[i] = 0;
    }
    dist[src] = 0;

    for(int k = 0; k < n; k++)
    {
        int u = -1;
        for(int i = 0; i < n; i++)
        {
            if(done[i] == 0 && dist[i] < INF)
            {
                if(u == -1 || dist[i] < dist[u])
                {
                    u = i;
                }
            }
        }

        if(u == -1)
        {
            break;
        }
        done[u] = 1;

        for(int v = 0; v < n; v++)
        {
            if(road[u][v] > 0 && dist[u] + road[u][v] < dist[v])
            {
                dist[v] = dist[u] + road[u][v];
                prev[v] = u;
            }
        }
    }

    if(dist[dst] == INF)
    {
        cout << "No route found" << endl;
    }
    else
    {
        cout << "Shortest route: ";
        printPath(prev, dst);
        cout << endl << "Total distance: " << dist[dst] << " km" << endl;

        cout << "Distance from " << name[src] << " to every place:" << endl;
        for(int i = 0; i < n; i++)
        {
            cout << name[i] << " : ";
            if(dist[i] == INF)
            {
                cout << "not reachable" << endl;
            }
            else
            {
                cout << dist[i] << " km" << endl;
            }
        }
    }
}


void bfs(int src, int dst)
{
    int hops[20], prev[20], q[20];
    int head = 0, tail = 0;

    for(int i = 0; i < n; i++)
    {
        hops[i] = -1;
        prev[i] = -1;
    }

    hops[src] = 0;
    q[tail] = src;
    tail++;

    while(head < tail)
    {
        int u = q[head];
        head++;

        for(int v = 0; v < n; v++)
        {
            if(road[u][v] > 0 && hops[v] == -1)
            {
                hops[v] = hops[u] + 1;
                prev[v] = u;
                q[tail] = v;
                tail++;
            }
        }
    }

    if(hops[dst] == -1)
    {
        cout << "No route found" << endl;
    }
    else
    {
        cout << "Fewest-segment route: ";
        printPath(prev, dst);
        cout << endl << "Road segments: " << hops[dst] << endl;

        cout << "Segments from " << name[src] << " to every place:" << endl;
        for(int i = 0; i < n; i++)
        {
            cout << name[i] << " : ";
            if(hops[i] == -1)
            {
                cout << "not reachable" << endl;
            }
            else
            {
                cout << hops[i] << endl;
            }
        }
    }
}


void dfs(int u)
{
    seen[u] = 1;

    for(int v = 0; v < n; v++)
    {
        if(road[u][v] > 0 && seen[v] == 0)
        {
            dfs(v);
        }
    }
}


void blockedRoadCheck(int a, int b)
{
    if(a == b || road[a][b] == 0)
    {
        cout << "There is no direct road between these places" << endl;
    }
    else
    {
        int saved = road[a][b];
        road[a][b] = 0;
        road[b][a] = 0;

        for(int i = 0; i < n; i++)
        {
            seen[i] = 0;
        }
        dfs(0);

        cout << "Road " << name[a] << " <-> " << name[b] << " is blocked" << endl;

        int cut = 0;
        for(int i = 0; i < n; i++)
        {
            if(seen[i] == 0)
            {
                cout << "Cut off from market: " << name[i] << endl;
                cut++;
            }
        }

        if(cut == 0)
        {
            cout << "All places can still reach the market" << endl;
        }

        road[a][b] = saved;
        road[b][a] = saved;
    }
}


void prim()
{
    int key[20], parent[20], inTree[20];
    int total = 0, count = 0;

    for(int i = 0; i < n; i++)
    {
        key[i] = INF;
        parent[i] = -1;
        inTree[i] = 0;
    }
    key[0] = 0;

    cout << "Prim roads:" << endl;

    for(int k = 0; k < n; k++)
    {
        int u = -1;
        for(int i = 0; i < n; i++)
        {
            if(inTree[i] == 0 && key[i] < INF)
            {
                if(u == -1 || key[i] < key[u])
                {
                    u = i;
                }
            }
        }

        if(u == -1)
        {
            break;
        }

        inTree[u] = 1;
        count++;
        total += key[u];

        if(parent[u] != -1)
        {
            cout << name[parent[u]] << " - " << name[u] << " : " << key[u] << " km" << endl;
        }

        for(int v = 0; v < n; v++)
        {
            if(road[u][v] > 0 && inTree[v] == 0 && road[u][v] < key[v])
            {
                key[v] = road[u][v];
                parent[v] = u;
            }
        }
    }

    if(count < n)
    {
        cout << "Network is not fully connected" << endl;
    }
    cout << "Total length: " << total << " km" << endl;
}

int findGroup(int x)
{
    while(group[x] != x)
    {
        x = group[x];
    }
    return x;
}


void kruskal()
{
    int ea[200], eb[200], ew[200];
    int m = 0;

    for(int i = 0; i < n; i++)
    {
        for(int j = i + 1; j < n; j++)
        {
            if(road[i][j] > 0)
            {
                ea[m] = i;
                eb[m] = j;
                ew[m] = road[i][j];
                m++;
            }
        }
    }

    for(int i = 0; i < m - 1; i++)
    {
        for(int j = 0; j < m - i - 1; j++)
        {
            if(ew[j] > ew[j + 1])
            {
                swap(ew[j], ew[j + 1]);
                swap(ea[j], ea[j + 1]);
                swap(eb[j], eb[j + 1]);
            }
        }
    }

    for(int i = 0; i < n; i++)
    {
        group[i] = i;
    }

    int total = 0, used = 0;
    cout << "Kruskal roads:" << endl;

    for(int i = 0; i < m; i++)
    {
        int ga = findGroup(ea[i]);
        int gb = findGroup(eb[i]);

        if(ga != gb)
        {
            group[ga] = gb;
            total += ew[i];
            used++;
            cout << name[ea[i]] << " - " << name[eb[i]] << " : " << ew[i] << " km" << endl;
        }
    }

    if(used < n - 1)
    {
        cout << "Network is not fully connected" << endl;
    }
    cout << "Total length: " << total << " km" << endl;
}

int main()
{
    string fileName;
    cout << "Enter data file name : ";
    cin >> fileName;

    if(loadData(fileName) == 0)
    {
        cout << "Could not read data file" << endl;
        return 1;
    }

    int choice;
    do
    {
        cout << endl << "Farm-to-Market Planner" << endl;
        cout << "1. Show network" << endl;
        cout << "2. Shortest route (Dijkstra)" << endl;
        cout << "3. Fewest road segments (BFS)" << endl;
        cout << "4. Blocked road check (DFS)" << endl;
        cout << "5. Minimum road network (Prim)" << endl;
        cout << "6. Minimum road network (Kruskal)" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice : ";
        choice = readNumber();

        if(choice == 1)
        {
            showNetwork();
        }
        else if(choice == 2 || choice == 3)
        {
            showNetwork();
            int a = readPlace("From place number : ");
            int b = -1;
            if(a != -1)
            {
                b = readPlace("To place number : ");
            }

            if(a != -1 && b != -1)
            {
                if(choice == 2)
                {
                    dijkstra(a, b);
                }
                else
                {
                    bfs(a, b);
                }
            }
        }
        else if(choice == 4)
        {
            showNetwork();
            int a = readPlace("Blocked road, first place number : ");
            int b = -1;
            if(a != -1)
            {
                b = readPlace("Blocked road, second place number : ");
            }

            if(a != -1 && b != -1)
            {
                blockedRoadCheck(a, b);
            }
        }
        else if(choice == 5)
        {
            prim();
        }
        else if(choice == 6)
        {
            kruskal();
        }
        else if(choice != 0)
        {
            cout << "Wrong choice, enter 0 to 6" << endl;
        }
    }
    while(choice != 0);

    cout << "Goodbye" << endl;
    return 0;
}
