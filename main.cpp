#include <iostream>
#include <list>
#include <stack>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

class Graph{
    int V; 
    int cntr;
    vector<list<int>> scc;
    list<int> *adj;
    int *whichscc;

public:
    Graph(int V); 
    void print();
    void addEdge(int v, int w);
    void SCCRecursive(int u, int discTime[], int lowest[], stack<int> *stack_scc, bool stackMember[]);
    void SCC(char* argv[]);
};
Graph::Graph(int V){
    cntr = 0;
    this->V = V;
    adj = new list<int>[V];
    whichscc = new int[V];
}

void Graph::addEdge(int a, int b){
    adj[a].push_back(b); 
}

void Graph::SCCRecursive(int u, int discTime[], int lowest[], stack<int> *stack_scc, bool stackMember[]){
    static int time = 0;		//static variable used because we want it to be the same in every recursive call

    discTime[u] = lowest[u] = ++time;
    stack_scc->push(u);
    stackMember[u] = true;

    list<int>::iterator i;
    for (i = adj[u].begin(); i != adj[u].end(); ++i){
        int v = *i;
        if (discTime[v] == -1){
            SCCRecursive(v, discTime, lowest, stack_scc, stackMember);
            lowest[u]  = min(lowest[u], lowest[v]);
        }
        else if (stackMember[v] == true)
            lowest[u]  = min(lowest[u], discTime[v]);
    }

    int w = 0;
    if (lowest[u] == discTime[u]){
        scc.emplace_back(*new list<int>);
        while (stack_scc->top() != u){
            w = (int) stack_scc->top();
            stackMember[w] = false;
            stack_scc->pop();
            scc[cntr].push_back(w);            
            whichscc[w] = cntr;
        }
        w = (int) stack_scc->top();
        stackMember[w] = false;
        stack_scc->pop();
        scc[cntr].push_back(w);
        whichscc[w] = cntr;
        cntr++;
    }
}

void Graph::SCC(char* argv[])
{
	stack<int> *stack_scc = new stack<int>();
    int *discTime = new int[V];
    int *lowest = new int[V];
    bool *stackMember = new bool[V];

    for (int i = 0; i < V; i++){
        discTime[i] = -1;
        lowest[i] = -1;
        stackMember[i] = false;
    }

    for (int i = 0; i < V; i++) {
        if (discTime[i] == -1)
            SCCRecursive(i, discTime, lowest, stack_scc, stackMember);
    }

    int answer = cntr;
    int indOfSCC[cntr];
    for (int i=0; i<cntr; i++){
        indOfSCC[i] = 0;
    }

    for (int i=0; i<V; i++){
        list<int>::iterator x;
        for (x = adj[i].begin(); x != adj[i].end(); ++x){
            if (whichscc[i]!=whichscc[*x]){
                indOfSCC[whichscc[*x]]++;
                if(indOfSCC[whichscc[*x]]==1)
                	answer--;
            }
        }
    }

    FILE *fp;
    fp = fopen(argv[2],"w");
    int useless = fprintf(fp, "%d ", answer);
    for (int x = 0; x < cntr ; ++x) {
        if(indOfSCC[x]==0){
        	int useless1 = fprintf(fp, "%d ", scc[x].front()+1);
        }
    }
    fclose(fp);
}

int main(int argc, char* argv[])
{
	
    if (argc != 3) {
        cout << "Run the code with the following command: ./project3 [input_file] [output_file]" << endl;
        return 1;
    }

    int number_of_piggybanks = 0;
    int number_of_keys = 0;
    int vertex = 0;
    FILE *fp;
    fp = fopen(argv[1],"r");
    int z = fscanf(fp,"%d",&number_of_piggybanks);
    Graph g(number_of_piggybanks);
    for(int i = 0; i < number_of_piggybanks; i++){
        int tr = fscanf(fp,"%d",&number_of_keys);
        for(int j = 0; j < number_of_keys; j++){
            int gz = fscanf(fp,"%d",&vertex);
            g.addEdge(i,vertex-1);
        }
    }
    fclose(fp);

    g.SCC(argv);

    return 0;
}
