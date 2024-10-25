#include "SearchAndRescue.hpp"
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


SearchAndRescue::SearchAndRescue(string fileName)
{
    terrain = new int*[4];
    //terrainSave = new int*[4];
    string line;
    ifstream infile(fileName);
    if (infile.is_open())
    {
        for(int i = 0; getline(infile, line); i++)
        {   
            terrain[i] = new int[4];
            //terrainSave[i] = new int[4];
            stringstream ss(line);
            string s;
            for (int j = 0; getline(ss, s, ' '); j++)
            {
                terrain[i][j] = stoi(s);
                //terrainSave[i][j] = stoi(s);
            }
        }
    }
    path = vector<State*>();
}


SearchAndRescue::~SearchAndRescue()
{
    State* to_del = path.back();
    delete to_del;
    path.pop_back();

    for (int i = 0; i < 4; i++)
    {
        delete terrain[i];
    }
    delete terrain;
}


void SearchAndRescue::possibleActions(State* current)
{
    current->possible_actions=vector<string>();
    /*if (current->x>0 && terrain[current->x-1][current->y]>=0) {
        current->possible_actions.push_back("up");
    }
    if (current->x<arrSize-1 && terrain[current->x+1][current->y]>=0) {
        current->possible_actions.push_back("down");
    }
    if (current->y>0 && terrain[current->x][current->y-1]>=0) {
        current->possible_actions.push_back("left");
    }
    if (current->y<arrSize-1 && terrain[current->x][current->y+1]>=0) {
        current->possible_actions.push_back("right");
    }*/
    if (current->y<arrSize-1) {
        current->possible_actions.push_back("up");
    }
    if (current->y>0) {
        current->possible_actions.push_back("down");
    }
    if (current->x>0) {
        current->possible_actions.push_back("left");
    }
    if (current->x<arrSize-1) {
        current->possible_actions.push_back("right");
    }
    return;
}


State* SearchAndRescue::result(State* current, string action)
{
    State* newState = new State{current->x, current->y, current->saved_people, action, vector<string>()};
    if (action=="up") {
        newState->y++;
    } else if (action=="down") {
        newState->y--;
    } else if (action=="left") {
        newState->x--;
    } else if (action=="right") {
        newState->x++;
    }
    /*if (terrain[newState->x][newState->y]==2) {
        newState->saved_people=current->saved_people+1;
        terrain[newState->x][newState->y]=1;
    }*/
    //possibleActions(newState);
    return newState;
}


vector<State*> SearchAndRescue::expand(State* current)
{
    vector<State*> expansion = vector<State*>();
    possibleActions(current);
    for (size_t i=0; i<current->possible_actions.size(); i++) {
        expansion.push_back(result(current, current->possible_actions[i]));
    }
    return expansion;
}


bool SearchAndRescue::iterativeDeepeningWrapper(State* start)
{
    /*if (terrain[start->x][start->y]==2) {
        start->saved_people+=1;
    }*/
    /*path.push_back(start);
    int depth_limit=STARTING_DEPTH;
    
    while (depth_limit<=16) {
        //cout << "depth limit: " << depth_limit << " ";
        if (iterativeDeepeningSearch(start, depth_limit)) {
            //cout << "RETURNED TRUE FOR DEPTH LIMIT " << depth_limit << "!" << endl;
            return true;
        } else {
            /*if (terrain[start->x][start->y]<0) {
                terrain[start->x][start->y]*=-1;
            }*/
            //if (terrain[start->x][start->y]!=terrainSave[start->x][start->y]) {
                /*terrain[start->x][start->y]=terrainSave[start->x][start->y];
            }
            path = vector<State*>();
            path.push_back(start);
            //cout << "RETURNED FALSE FOR DEPTH LIMIT " << depth_limit << "!" << endl;
            depth_limit++;
        }
    }
    path.push_back(start);
    return false;*/
    int depth_limit = 0;
    while (depth_limit<=16) {
        //path.clear();
        //path.push_back(start);
        bool found = iterativeDeepeningSearch(start, depth_limit);
        if (found) {
            path.push_back(start);
            vector<State*> pathTemp = path;
            for (size_t i=0; i<pathTemp.size(); i++) {
                path[path.size()-i-1]=pathTemp[i];
            }
            return true;
        }
        depth_limit++;
        path.clear();
    }
    return false;
}

bool SearchAndRescue::iterativeDeepeningSearch(State* current, int depth_limit)
{
    //path.push_back(current);
    //printPath();
    //cout << endl;
    //cout << "current depth: " << depth_limit << "\tx: " << current->x << "\ty: " << current->y << "\tprev_action: " << current->prev_action << "\tgoal status: " << isGoal(current) << "\n";
    /*if (isGoal(current)) {
        return true;
    }*/
    bool is2=false;
    if (isGoal(current)) { 
        //path.insert(path.begin(), current);
        path.push_back(current);
        return true;
    }
    //terrain[current->x][current->y]*=-1;
    /*terrain[current->x][current->y]=1;
    if (depth_limit==0) {
        return false;
    }*/
    if (depth_limit<=0) {
        return false;
    }
    /*if (terrain[current->x][current->y]==2) {
        current->saved_people+=1;
        terrain[current->x][current->y]=1;
    }*/
    if (terrain[current->x][current->y] == 2) {
        current->saved_people++;
        is2=true;
        terrain[current->x][current->y]=1;
    }
    vector<State*> expansion = expand(current);

    /*for (size_t i=0; i<expansion.size(); i++) {
        path.push_back(expansion[i]);
        bool isTheGoal = iterativeDeepeningSearch(expansion[i], depth_limit-1);
        if (isTheGoal) {
            return true;
        } else {
            terrain[path.back()->x][path.back()->y]=terrainSave[path.back()->x][path.back()->y];
            path.pop_back();
        }
        
        bool isTheGoal = iterativeDeepeningSearch(expansion[i], depth_limit-1);
        if (!isTheGoal) {
            //terrain[path.back()->x][path.back()->y]*=-1;
            terrain[path.back()->x][path.back()->y]=terrainSave[path.back()->x][path.back()->y];
            path.pop_back();
        } else {
            return true;
        }
    }*/

    if (is2) {
        terrain[current->x][current->y]=2;
        current->saved_people--;
    }

    for (size_t i=0; i<expansion.size(); i++) {
        bool isTheGoal = iterativeDeepeningSearch(expansion[i], depth_limit-1);
        if (isTheGoal) {
            //path.insert(path.begin(), expansion[i]);
            path.push_back(expansion[i]);
            return true;
        }
    }
    return false;
    /*bool is2=false;
    if (isGoal(current)) { 
        path.insert(path.begin(), current);
        //path.push_back(current);
        return true;
    }
    if (depth_limit<=0) {
        return false;
    }
    if (terrain[current->x][current->y] == 2) {
        current->saved_people++;
        is2=true;
        terrain[current->x][current->y]=1;
    }
    vector<State*> expansion = expand(current);
    for (size_t i=0; i<expansion.size(); i++) {
        bool isTheGoal = iterativeDeepeningSearch(expansion[i], depth_limit-1);
        if (isTheGoal) {
            path.insert(path.begin(), expansion[i]);
            //path.push_back(expansion[i]);
            return true;
        }
    }
    if (is2) {
        terrain[current->x][current->y]=2;
        current->saved_people--;
    }*/
}


void SearchAndRescue::printPath()
{
    for (size_t i=0; i<path.size(); i++) {
        cout << "x: " << path[i]->x << "\ty: " << path[i]->y << "\tprev_action: " << path[i]->prev_action << "\n";
    }
}


bool SearchAndRescue::isGoal(State* current)
{
    if ((terrain[current->x][current->y]==3) && (current->saved_people==PEOPLE_TO_SAVE)) {
        return true;
    }
    return false;
}


void SearchAndRescue::printTerrain()
{
    for (int x=0; x<arrSize; x++) {
        for (int y=0; y<arrSize; y++) {
            cout << terrain[y][3-x] << " ";
        }
        cout << endl;
    }
}

/*
IDW: 
depth limit = starting depth
while (true) {
    bool found = IDS(start, depth limit)
    if found return true
    depth liomit++;
)
return false;

IDS: 
bool is2=false;
if isGoal(current): 
    path.insert(path.begin(), current);
    return true
if (depth limit <=0) {
    return false
}
if (terrain[current->x][current->y] == 2) {
    current->savedpeople++;
    is2=true;
    terrain[current->x][current->y]=1;
}
vector<State*> expansion = expand(current);
for (State* state : expansion) {
    bool found = IDS(state, depthlimit-1);
    if (found) {
        path.insert(path.begin(current));
        return true;
    }
}
if (is2) {
    terrain[current->x][current->y]=2;
    current->saved_people--;
}
*/