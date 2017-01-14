
/*
AI Search
Created by Aishwarya Verghese on 9/10/16
*/

#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <map>
#include <algorithm> 

using namespace std;

string method,source,goal;
map<string,int> heuristics;
int node_counter = 0;

/*
The map of cities with their edge and heuristsic costs will be represented as a graph
There will be an array of vertices and each array cell represnts a vertex
Each vertex will contain the city name and a pointer to it's edge list
The edge list is a Linked List representing it's neibouring states and the cost to them
*/
class graph
{
public:
    
    int num_of_vertices;

    //Edge List node
    struct edges
    {
        string dest;
        int edge_cost;
        edges* next_edge;
    };
    
    //Vertex node
    struct vertices
    {
        string state;
        edges* edge_list; //head of the edges Linked List
    }*v;
    
    void setNumberOfVertices(int num)
    {
        //Make an array of vertices
        num_of_vertices = num;
        struct vertices *arr_of_vertices = new vertices[num_of_vertices];
        v = arr_of_vertices;
        cout << "Number of vertices : " <<num_of_vertices <<endl;
    }
    
    void addvertice(string state,int pos)
    {
        //add a new vertice in the given position
        cout << "Adding vertice : " << state << " at position : " << pos <<endl;
        v[pos].state = state;
        v[pos].edge_list = NULL;
    }
    
    void addedge(string source, string destination, int cost)
    {
        cout << "Adding an edge from " << source << " to " << destination << " with cost : " << cost << endl;
        //add a new edge from source to destination with given cost
        edges *new_edge = new edges;
        new_edge -> dest = destination;
        new_edge -> edge_cost = cost;
        new_edge -> next_edge = NULL;
        for(int i = 0 ; i < num_of_vertices; i++)
        {
            //Find the source
            if(v[i].state == source)
            {
                //If this is the first edge for the source
                if(v[i].edge_list == NULL)
                    v[i].edge_list = new_edge;
                else
                {
                    //Add new edge to the end of the Linked List
                    edges* conductor = v[i].edge_list;
                    while(conductor -> next_edge != NULL)
                        conductor = conductor -> next_edge;
                    conductor -> next_edge = new_edge;
                }
                break;
            }
        }
        
    }
    
    edges* getAdjacentEdges(string state)
    {
        for(int i = 0; i < num_of_vertices; i++)
        {
            if(v[i].state == state)
                return v[i].edge_list;
        }
        cout << "Error in getting adjacent edges \n";
        return NULL;
    }
    
    void checkgraph()
    {
        //print graph representation
        for(int i = 0; i < num_of_vertices; i++)
        {
            cout << v[i].state;
            edges* next = v[i].edge_list;
            while(next != NULL)
            {
                cout<< "->" << next ->dest << "(" << next ->edge_cost <<")";
                next = next -> next_edge;
            }
            cout << endl;
        }
    }
}mapping;

//Search tree created during the search process
class tree
{
    
public:
    struct node
    {
        string state;
        int path_cost;
        node* parent;
        int node_id;
    }*root;
    
    node* addNode(string state,int path_cost,node* parent)
    {
        if( parent == NULL )
            cout << "Adding root node to the tree \n";
        else
            cout << "Adding node " << state << " with path cost " << path_cost << " and parent " << parent -> state << " and Node id " << node_counter <<" to the tree \n";
        
        node* new_node = new node;
        new_node -> state = state;
        new_node -> path_cost = path_cost;
        new_node -> parent = parent;
        new_node -> node_id = node_counter;
        node_counter++;
        return new_node;
    }
    
    //When the goal has been found return the solution
    string getPath(node* goal)
    {
        string path_entity = "";
        stack<string> path;
        string final_path = "";

        //backtrack from goal to initial state and push into stack
        while( goal != NULL )
        {   
            int trueCost = goal -> path_cost;
            if(method[0] == 'A')    //subtract heuristic value for A*
                trueCost -= h(goal -> state);
                
            path_entity = goal->state + " " + to_string(trueCost);
            path.push(path_entity);
            cout << goal -> state << " " << trueCost << endl;
            goal = goal -> parent;
        }

        //Pop fro stack to reverse the order (initial -> goal)
        while(!path.empty())
        {
            final_path += path.top() + "\n";
            path.pop();
        }
        return final_path;
    }
}search_tree;

//If using A* method return heuristic else return 0
int h(string state)
{
    if(method[0] == 'A')
        return heuristics[state];
    return 0;
}

//Returns true if it's the goal state
bool goalTest(string state)
{
    if(state == goal)
    {
        cout << "Goal found \n";
        return true;
    }
    return false;
}

bool loopDetected(tree::node *child,tree::node **encountered_set,int num_of_encountered_states)
{
    for(int i = 0 ; i < num_of_encountered_states; i++)
        if(encountered_set[i] -> state == child->state)
        {
            cout << child <<" has already been explored \n";
            if(encountered_set[i] -> path_cost > child -> path_cost)    //check which one has a better cost
            {
                //Replace with path cost and parent
                encountered_set[i] -> path_cost = child -> path_cost;
                encountered_set[i] -> parent = child -> parent;
            }
            return true;
        }
    return false;
}

string bfs()
{
    cout << "Starting Breadth First Search \n";
    queue<tree::node*> q;
    tree::node* root = search_tree.addNode(source, 0, NULL); //add source as root of the search tree
    
    //check if source is goal
    if(goalTest(source))
    {
        string path = search_tree.getPath(root);
        return path;
    }
    
    q.push(root);   //add root to queue
    cout << "Pushing " << root -> state << " into queue \n";
    
    //Set all states as unencountered
    map<string,bool> encountered;
    for(int i = 0; i < mapping.num_of_vertices; i++)
    {
        encountered[mapping.v[i].state] = false;
    }
    encountered[source] = true;
    
    while(!q.empty())
    {
        //get node we need to explore
        tree::node* exploring_node = q.front();
        q.pop();
        string exploring_state = exploring_node -> state;
        cout << "Currently exploring "<< exploring_state << endl;
        //Get nodes neighbouring edges
        graph::edges *children = mapping.getAdjacentEdges(exploring_state);
        
        //For each child
        while(children != NULL)
        {
            string child = children -> dest;
            if(encountered[child] == false)
            {
                int depth = exploring_node -> path_cost + 1;
                cout << "Checking child "<< child << " at depth " << depth << endl;
                tree::node* child_node = search_tree.addNode(child,depth, exploring_node); //cost is 0 as we only care about depth
                
                if(goalTest(child))
                {
                    string path = search_tree.getPath(child_node);
                    return path;
                }
                else
                {
                    q.push(child_node);
                    cout << "Pushing " << child_node -> state << " into queue \n";
                    encountered[child] = true;
                }
            }
            children = children -> next_edge; //get next child
            
        }
    }
    cout << "BFS error";
    return NULL;
}

string dfs()
{
    cout << "Starting Depth First Search \n";
    stack<tree::node*> s;
    
    //Set all states as unencountered
    map<string,bool> encountered;
    for(int i = 0; i < mapping.num_of_vertices; i++)
    {
        encountered[mapping.v[i].state] = false;
    }
    
    //Add source
    encountered[source] = true;
    tree::node* root = search_tree.addNode(source, 0, NULL);
    s.push(root);
    cout << "Pushing " << root -> state << " into stack \n";
    
    while(!s.empty())
    {
        //get node we need to explore
        tree::node* exploring_node = s.top();
        s.pop();
        string exploring_state = exploring_node -> state;
        cout << "Currently exploring "<< exploring_state << endl;
        
        //check if it is goal
        if(goalTest(exploring_state))
        {
            string path = search_tree.getPath(exploring_node);
            return path;
        }
        
        //Get nodes neighbouring edges
        graph::edges *children = mapping.getAdjacentEdges(exploring_state);
        stack<string> child_stack;
        
        //For each child
        while(children != NULL)
        {
            string child = children -> dest;
            cout << "checking "<< child << endl;
            if(!encountered[child]) //Loop detection
            {
                child_stack.push(child);
                encountered[child] = true;
            }
            else
                cout << child << " ignored \n";
            
            children = children -> next_edge; //get next child
        }
        while(!child_stack.empty()) //Reverse order of children
        {
            string child = child_stack.top();
            child_stack.pop();
            int depth = exploring_node -> path_cost + 1;
            tree::node* child_node = search_tree.addNode(child,depth, exploring_node); //cost is 0 as we only care about depth
            
            s.push(child_node);
            cout << "Pushing " << child << " at depth " << depth<< " into stack \n";
        }
    }
    cout << "DFS error";
    return NULL;
}

bool pairCompare(const tuple<tree::node*, int, int>& a, const tuple<tree::node*, int, int>& b)
{
    if(get<1>(a) < get<1>(b))   //sort by path cost
        return true;
    
    if(get<1>(a) == get<1>(b))      //if path costs are same
        if(get<2>(a) < get<2>(b))   //sort by node id
            return true;
    return false;
}

string ucs()
{
    cout << "Starting with UCS Search \n";
    vector<tuple<tree::node*,int,int>> frontier;
    map<string, int> explored;
    tree::node* root = search_tree.addNode(source, h(source), NULL); //add source as root of the search tree
    frontier.push_back(make_tuple(root,0 + h(source), root -> node_id));
    
    while(!frontier.empty())
    {
        tree::node* exploring_node = get<0>(frontier[0]);
        frontier.erase(frontier.begin() + 0);
        
        string exploring_state = exploring_node -> state;//exploring_node -> state;
        cout << "Currently exploring "<< exploring_state << " with path cost " << exploring_node -> path_cost <<endl;
        
        //check if it is goal
        if(goalTest(exploring_state))
        {
            string path = search_tree.getPath(exploring_node);
            return path;
        }
        
        //Get nodes neighbouring edges
        graph::edges *children = mapping.getAdjacentEdges(exploring_state);
        //For each child
        while(children != NULL)
        {
            int flag = 0; //Flag will be 0 if child should be added to queue
            string child = children -> dest;
            
            //g(child) = (f(parent) - h(parent)) + (edge cost + h(child))
            int child_pathcost = (exploring_node -> path_cost - h(exploring_state))+ children -> edge_cost + h(child);
            //Make a node for child
            tree::node* child_node = search_tree.addNode(child, child_pathcost, exploring_node);
            
            cout << "checking child "<< child << " with pathcost " << child_pathcost << endl;
            
            //check if child exists in frontier
            for(int i = 0; i < frontier.size(); i++)
            {
                //child exists in frontier
                if(get<0>(frontier[i]) -> state == child)
                {
                    //child exists in frontier has lesser cost
                    if(child_pathcost < get<0>(frontier[i]) -> path_cost)
                    {
                        cout << "Child exists in frontier with a lower cost\n";
                        frontier.erase(frontier.begin() + i);
                        flag = 0;
                        break;
                    }
                    else
                        flag = 1; //child exists in frontier and has higher cost
                }
            }
            
            //check if child exists in explored
            if(explored.find(child) != explored.end())
            {
                //child has lesser cost
                if(child_pathcost < explored[child])
                {
                    cout << "Child exists in explored with a lower cost\n";
                    explored.erase(child);
                }
                else
                    flag = 1; //child exists in explored and has higher cost
            }
            
            //child is either new or exists with lesser path cost
            if(flag != 1)
            {
                frontier.push_back(make_tuple(child_node,child_pathcost,child_node -> node_id));
                cout << "Pushing " << child << " with cost " << child_pathcost<< " into priority queue \n";
            }
            else
                cout << "Ignoring child\n";
            
            children = children -> next_edge; //Move on to next child
        }
        
        cout << "Adding " << exploring_state << " to explored set\n";
        explored[exploring_state] = exploring_node -> path_cost;
        sort(frontier.begin(), frontier.end(), pairCompare); //sort after adding all the children
    }
    
    cout << method << "error\n";
    return NULL;
}

string a()
{
    return ucs();
}

//Read from input.txt file
void readInput(string file_name)
{
    ifstream input;
    input.open (file_name);
    
    string line;
    if (input.is_open())
    {
        cout << "opened input file \n";
        
        input >> method;
        input >> source;
        input >> goal;
        
        cout << "Method : " <<method <<endl;
        cout << "Source : " <<source <<endl;
        cout << "Goal : " <<goal <<endl;
        
        //skip live traffic data
        int num;
        input >> num;
        getline(input,line);    //end of line after num
        while(num > 0)
        {
            //ignore these lines
            getline(input,line);
            num--;
        }
        
        //get sunday traffic data - get vertices
        input >> num;
        mapping.setNumberOfVertices(num);   //mapping -> graph object
        int num_of_vertices_added = 0;
        string state; int cost;
        while(num_of_vertices_added < num)
        {
            //add new vertice
            input >> state;
            input >> cost;
            
            mapping.addvertice(state,num_of_vertices_added);
            heuristics[state] = cost;
            num_of_vertices_added++;
        }
        
        input.clear();
        input.seekg(0, ios::beg);  //go back to beginning to get edges
        
        //skip first three lines
        getline(input,line);
        getline(input,line);
        getline(input,line);
        
        //get edges and edge cost (live traffic data)
        input >> num;
        while(num>0)
        {
            string src,dest;
            int cost;
            
            input >> src;
            input >> dest;
            input >> cost;
            
            mapping.addedge(src,dest,cost);
            
            num--;
        }
        
        input.close();
        cout << "Closed input file \n";
    }
    else
    {
        cout << "Error opening input file \n";
        exit(0);
    }

}

void putOutput(string fileName,string path)
{
    ofstream output;
    output.open(fileName);
    
    if(output.is_open())
    {
        output << path;
    }
    else
    {
        cout << "Error opening output file \n";
        exit(0);
    }
    output.close();
}

int main()
{
    readInput("input.txt");
    mapping.checkgraph();
    string path;
    switch(method[0])
    {
        case 'B' : path = bfs();break;
        case 'D' : path = dfs();break;
        case 'U' : path = ucs();break;
        case 'A' : path = a();break;
        default : cout << "Error - wrong method";
    }
    putOutput("output.txt",path);
}
