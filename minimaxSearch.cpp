#include <iostream>
#include <utility>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
using namespace std;

//function
int countFlipPieces(string gameboard, int playerColor, string grid, int direction){
    // 0上 1右上 2右 3右下 4下 5左下 6左 7左上 direction方向
    int row= grid[0]- 'A', col= grid[1]- 'a';
    char opponent, player;
    if(playerColor== 1){
        opponent= 'O';
        player= 'X';
    }
    else{
        opponent= 'X';
        player= 'O';
    }
    
    int index= row* 6+ col;
    int Xstep= 0, Ystep= 0, counttt= 0;
    
    if(direction== 0){ // 上
        Xstep= -1;
        Ystep= 0;
    }
    else if(direction== 1){ // 右上
        Xstep= -1;
        Ystep= 1;
    }
    else if(direction== 2){ // 右
        Xstep= 0;
        Ystep= 1;
    }
    else if(direction== 3){ // 右下
        Xstep= 1;
        Ystep= 1;
    }
    else if(direction== 4){ // 下
        Xstep= 1;
        Ystep= 0;
    }
    else if(direction== 5){ // 左下
        Xstep= 1;
        Ystep= -1;
    }
    else if(direction== 6){ // 左
        Xstep= 0;
        Ystep= -1;
    }
    else if(direction== 7){ // 左上
        Xstep= -1;
        Ystep= -1;
    }
    
    row+= Xstep; col+= Ystep;
    index= row* 6+ col;
    
    while(row>= 0&& row< 6&& col>= 0&& col< 6){
        if(gameboard[index]== player) return counttt;
        else if(gameboard[index]== opponent) counttt++;
        else return 0;
        row+= Xstep; col+= Ystep;
        index= row* 6+ col;
    }
    return 0;
}
int countColorPieces(string gameboard, int playerColor){
    int i, count=0;
    
    if(playerColor==1){
        for(i=0;i<36;i++){
            if(gameboard[i]=='X') count++;
        }
    }else if(playerColor==2){
        for(i=0;i<36;i++){
            if(gameboard[i]=='O') count++;
        }
    }
    return count;
}
bool isValidMove(string gameboard, int playerColor, string grid){
    for(int i=0;i<8;i++){
        if(countFlipPieces(gameboard, playerColor, grid, i)>0) return true;
    }
    return false;
}
bool passTurn(string gameboard, int playerColor){
    for(int i=0;i<36;i++){
        if(gameboard[i]=='+'){
            string temp="";
            temp=temp+char(i/6+'A')+char(i%6+'a');
            if(isValidMove(gameboard, playerColor, temp)) {
                return false;
            }
        }
    }
    return true;
}
bool gameOver(string gameboard){
    return passTurn(gameboard, 1) && passTurn(gameboard, 2);
}
int heuristicScore(string gameboard, int playerColor){
    return countColorPieces(gameboard, playerColor) - countColorPieces(gameboard, playerColor==1 ? 2 : 1);
}

int num_validMove(string gameboard, int playerColor){
    // 整個棋盤中有幾個位置是可以走的
    int counttt= 0;
    for(int i= 0; i< 6; i++){
        for(int j= 0; j< 6; j++){
            char xChar= ('A'+ i), yChar= ('a'+ j);
            string grid= "";
            grid+= xChar; grid+= yChar;
            if(isValidMove(gameboard, playerColor, grid))
                counttt++;
        }
    }
    return counttt;
}
int goodHeuristic(string gameboard, int playerColor){
    int opppnentColor;
    char myChar;
    char oppChar;
    if(playerColor== 1){
        opppnentColor= 2;
        myChar= 'O';
        oppChar= 'X';
    }else{
        opppnentColor= 1;
        myChar= 'X';
        oppChar= 'O';
    }
    
    int my_tiles = 0, opp_tiles = 0, my_front_tiles = 0, opp_front_tiles = 0;
    double pieceDiff= 0, corner= 0, closeCorner= 0, mobility= 0, frontier= 0, diskWeight= 0;
    
    int weight[6][6]= { {18, -3, 10, 10, -3, 18},
        {-3, -7, 0, 0, -7, -3},
        {10, 0, 1, 1, 0, 10},
        {10, 0, 1, 1, 0, 10},
        {-3, -7, 0, 0, -7, -3},
        {18, -3, 10, 10, -3, 18} };
    int x_d[] = {-1, -1, 0, 1, 1, 1, 0, -1};
    int y_d[] = {0, 1, 1, 1, 0, -1, -1, -1};
    
    // Piece difference, frontier disks and disk squares
    for(int i= 0; i< 6; i++){
        for(int j= 0; j< 6; j++){
            if(gameboard[i*6+ j]== myChar){
                diskWeight+= weight[i][j];
                my_tiles++;
            }else if(gameboard[i*6+ j]== oppChar){
                diskWeight-= weight[i][j];
                opp_tiles++;
            }
            if(gameboard[i*6+ j]== '+'){
                for(int k= 0; k< 8; k++){ // 8個方向
                    int x= i+ x_d[k];
                    int y= j+ y_d[k];
                    if(x>= 0&& x < 6&& y>= 0&& y< 6&& gameboard[x*6+ y]== '+'){
                        if(gameboard[i*6+ j]== myChar)
                            my_front_tiles++;
                        else opp_front_tiles++;
                        break;
                    }
                }
            }
        }
    }
    
    // std::cout<< endl<< "my_tile: "<< my_tiles<< endl;
    // std::cout<< "opp_tile: "<< opp_tiles<< endl;
    // std::cout<< "my_front_tile: "<< my_front_tiles<< endl;
    // std::cout<< "opp_front_tile: "<< opp_front_tiles<< endl;
    
    if(my_tiles> opp_tiles)
        pieceDiff= (float)(100.0* my_tiles)/(float)(my_tiles+ opp_tiles);
    else if(my_tiles< opp_tiles)
        pieceDiff= -(float)(100.0* opp_tiles)/(float)(my_tiles+ opp_tiles);
    else pieceDiff= 0;
    
    if(my_front_tiles> opp_front_tiles)
        frontier= -(float)(100.0* my_front_tiles)/(float)(my_front_tiles+ opp_front_tiles);
    else if(my_front_tiles < opp_front_tiles)
        frontier= (float)(100.0* opp_front_tiles)/(float)(my_front_tiles+ opp_front_tiles);
    else frontier= 0;
    
    // Corner occupancy
    my_tiles= 0;
    opp_tiles= 0;
    if(gameboard[0]== myChar) my_tiles++; // 左上
    else if(gameboard[0]== oppChar) opp_tiles++;
    if(gameboard[5]== myChar) my_tiles++; // 右上
    else if(gameboard[5]== oppChar) opp_tiles++;
    if(gameboard[30]== myChar) my_tiles++; // 左下
    else if(gameboard[30]== oppChar) opp_tiles++;
    if(gameboard[35]== myChar) my_tiles++; // 右下
    else if(gameboard[35]== oppChar) opp_tiles++;
    corner= 25.0* (float)(my_tiles- opp_tiles);
    
    // Corner closeness
    my_tiles= 0;
    opp_tiles= 0;
    if(gameboard[0]== '+'){
        if(gameboard[1]== myChar) my_tiles++;
        else if(gameboard[1]== oppChar) opp_tiles++;
        if(gameboard[6]== myChar) my_tiles++;
        else if(gameboard[6]== oppChar) opp_tiles++;
        if(gameboard[7]== myChar) my_tiles++;
        else if(gameboard[7]== oppChar) opp_tiles++;
    }
    if(gameboard[5]== '+'){
        if(gameboard[4]== myChar) my_tiles++;
        else if(gameboard[4]== oppChar) opp_tiles++;
        if(gameboard[11]== myChar) my_tiles++;
        else if(gameboard[11]== oppChar) opp_tiles++;
        if(gameboard[10]== myChar) my_tiles++;
        else if(gameboard[10]== oppChar) opp_tiles++;
    }
    if(gameboard[30]== '+'){
        if(gameboard[24]== myChar) my_tiles++;
        else if(gameboard[24]== oppChar) opp_tiles++;
        if(gameboard[31]== myChar) my_tiles++;
        else if(gameboard[31]== oppChar) opp_tiles++;
        if(gameboard[25]== myChar) my_tiles++;
        else if(gameboard[25]== oppChar) opp_tiles++;
    }
    if(gameboard[35]== '+'){
        if(gameboard[34]== myChar) my_tiles++;
        else if(gameboard[34]== oppChar) opp_tiles++;
        if(gameboard[29]== myChar) my_tiles++;
        else if(gameboard[29]== oppChar) opp_tiles++;
        if(gameboard[28]== myChar) my_tiles++;
        else if(gameboard[28]== oppChar) opp_tiles++;
    }
    closeCorner= -12.5* (float)(my_tiles- opp_tiles);
    
    // Mobility
    my_tiles= num_validMove(gameboard, playerColor);
    opp_tiles= num_validMove(gameboard, opppnentColor);
    if(my_tiles> opp_tiles)
        mobility= (float)(100.0* my_tiles)/(float)(my_tiles+ opp_tiles);
    else if(my_tiles < opp_tiles)
        mobility= -(float)(100.0* opp_tiles)/(float)(my_tiles+ opp_tiles);
    else mobility= 0;
    
    // final weighted score
    double score= (10.0* pieceDiff)
    + (801.724* corner)+ (382.026* closeCorner)
    + (78.922* mobility)
    // + (74.0* frontier)
    + (10.0* diskWeight);
    
    // cout<< endl<< "pieceDiff: "<< pieceDiff<< endl;
    // cout<< "corner: "<< corner<< endl;
    // cout<< "closeCorner: "<< closeCorner<< endl;
    // cout<< "mobility: "<< mobility<< endl;
    // cout<< "frontier: "<< frontier<< endl;
    // cout<< "diskWeight: "<< diskWeight<< endl;
    // cout<< "h Score: "<< score<< endl;
    return (int)score;
}
int heuristicScore(string gameboard, int playerColor, int heuristicChoose){
    // 可換其他種設計方法
    if(heuristicChoose== 0){
        int opppnentColor= 2? (playerColor== 1): 1;
        return countColorPieces(gameboard, playerColor)- countColorPieces(gameboard, opppnentColor);
    }else if(heuristicChoose== 1){
        return goodHeuristic(gameboard, playerColor);
    }
    cout<< "Choose Error"<< endl;
    return -1;
}
string flipPieces(string gameboard, int playerColor, string grid){
    // return 新狀態
    string newboard= gameboard;
    char opponent, player;
    if(playerColor== 1) player= 'X';
    else player= 'O';
    
    if(isValidMove(gameboard, playerColor, grid)){
        int row= grid[0]- 'A', col= grid[1]- 'a';
        int index= row* 6+ col;
        int step= 0;
        
        for(int d= 0; d< 8; d++){
            int counttt= countFlipPieces(newboard, playerColor, grid, d);
            if(d== 0) step= -6;
            else if(d== 1) step= -6+1;
            else if(d== 2) step= 1;
            else if(d== 3) step= 6+1;
            else if(d== 4) step= 6;
            else if(d== 5) step= 6-1;
            else if(d== 6) step= -1;
            else if(d== 7) step= -6-1;
            for(int i= 1; i<= counttt; i++){
                newboard[index+ step* i]= player;
            }
        }
        newboard[index]= player;
    }
    return newboard;
}

//player class
class minimaxSearchWithAlphaBetaPlayer{
public:
    string gameboard;
    int playerColor;
    
    string action(){
        if(!passTurn(gameboard, playerColor)){
            int index=minimaxSearchWithAlphaBeta(gameboard, playerColor, 10, playerColor, -INT_MAX, INT_MAX).second;
            string action="";
            action=action+char(index/6+'A')+char(index%6+'a');
            return action;
        }else{
            return "pass";
        }
    }
    pair<int, int> minimaxSearchWithAlphaBeta(string gameboard, int playerColor, int depth, int myTurn, int a, int b){
        int i, temp2=-2, index=-1;
        if(depth==0 || gameOver(gameboard)){
            return pair<int, int> (heuristicScore(gameboard, playerColor), index);
        }
        if(playerColor==myTurn){    //MAX_Player
            for(i=0;i<36;i++){
                string temp="";
                temp=temp+char(i/6+'A')+char(i%6+'a');
                if(gameboard[i]=='+' && isValidMove(gameboard, playerColor, temp)) {
                    string new_board=flipPieces(gameboard, playerColor, temp);
                    if((temp2=(minimaxSearchWithAlphaBeta(new_board, playerColor == 1 ? 2 : 1, depth-1, myTurn, a, b).first))>a) {
                        a=temp2;
                        index=i;
                    }
                    if(b<=a) break;
                }
            }
            if(i==36){  //pass
                if((temp2=(minimaxSearchWithAlphaBeta(gameboard, playerColor == 1 ? 2 : 1, depth-1, myTurn, a, b).first))>a) {
                    a=temp2;    //?
                }
            }
            return pair<int, int>(a, index);
        }else{      //MIN_Player
            for(i=0;i<36;i++){
                string temp="";
                temp=temp+char(i/6+'A')+char(i%6+'a');
                if(gameboard[i]=='+' && isValidMove(gameboard, playerColor, temp)) {
                    string new_board=flipPieces(gameboard, playerColor, temp);
                    if((temp2=minimaxSearchWithAlphaBeta(new_board, playerColor == 1 ? 2 : 1, depth-1, myTurn, a, b).first)<b) {
                        b=temp2;    //?
                    }
                    if(b<=a) break;
                }
            }
            if(i==36){  //pass
                if((temp2=(minimaxSearchWithAlphaBeta(gameboard, playerColor == 1 ? 2 : 1, depth-1, myTurn, a, b).first))<b) {
                    b=temp2;
                    index=i;
                }
            }
            return pair<int, int>(b, index);
        }
    }
}; //BestPlayer

void printGameboard(string gameboard){
    for(int i=0;i<6;i++){
        for(int j=0;j<6;j++){
            cout<<gameboard[i*6+j]<<"  ";
        }
        cout<<endl;
    }
}
void AIAction(minimaxSearchWithAlphaBetaPlayer &AIplayer, string &gameboard, int AITurn){
    if(passTurn(gameboard, AITurn)){
        cout<<"AlphaBetaPruningPlayer pass"<<endl;
    }else{
        cout<<"Wait for AlphaBetaPruningPlayer."<<endl;
        string action2=AIplayer.action();
        cout<<"AlphaBetaPruningPlayer action: "<<action2<<endl;
        gameboard=flipPieces(gameboard, AITurn, action2);
        AIplayer.gameboard=gameboard;
        cout<<"AlphaBetaPruningPlayer result:"<<endl;
        printGameboard(gameboard);
    }
}
void humanAction(string &gameboard, minimaxSearchWithAlphaBetaPlayer &AIplayer, int humanTurn){
    if(passTurn(gameboard, humanTurn)){
        cout<<"HumanPlayer pass"<<endl;
    }else{
        cout<<"Wait for HumanPlayer."<<endl;
        cout<<"Enter action (ex:Ae):";
        string action1;
        cin>>action1;
        while(!isValidMove(gameboard, humanTurn, action1)){
            cout<<"Not valid move, input action again: ";
            cin>>action1;
        }
        gameboard=flipPieces(gameboard, humanTurn, action1);
        AIplayer.gameboard=gameboard;
        cout<<"HumanPlayer result:"<<endl;
        printGameboard(gameboard);
    }
}
void AIvsHuman(){
    string gameboard, action;
    int AIOrder=0;
    minimaxSearchWithAlphaBetaPlayer AIPlayer;
    gameboard="++++++++++++++OX++++XO++++++++++++++";   //intial gameboard
    AIPlayer.gameboard=gameboard;
    
    cout<<"AI order (1 or 2):";
    cin>>AIOrder;
    while(AIOrder!=1&&AIOrder!=2){
        cout<<"Enter 1 or 2:";
        cin>>AIOrder;
    }
    
    cout<<"start"<<endl;
    if(AIOrder==1){
        AIPlayer.playerColor=1;
        cout<<"initial gameboard:"<<endl;
        printGameboard(gameboard);
        while(!gameOver(gameboard)){
            AIAction(AIPlayer, gameboard, 1);
            humanAction(gameboard, AIPlayer, 2);
        }
    }else{
        AIPlayer.playerColor=2;
        cout<<"initial gameboard:"<<endl;
        printGameboard(gameboard);
        while(!gameOver(gameboard)){
            humanAction(gameboard, AIPlayer, 1);
            AIAction(AIPlayer, gameboard, 2);
        }
    }
    
    cout<<endl<<"game end"<<endl;
    cout<<"final: "<<endl;
    printGameboard(gameboard);

    int humanScores=heuristicScore(gameboard, AIOrder==1?2:1);
    cout<<"HumanPlayer scores: "<<humanScores<<endl;
    int AIscores=heuristicScore(gameboard, AIOrder);
    cout<<"AIPlayer scores: "<<AIscores<<endl;
    if(humanScores==0){
        cout<<"tie"<<endl;
    }else if(humanScores>0){
        cout<<"HumanPlayer win"<<endl;
    }else{
        cout<<"AIPlayer win"<<endl;
    }
}

int main(int argc, const char * argv[]) {
    srand (time(NULL));
    
    AIvsHuman();
    //AIvsAI();
    
    return 0;
}
