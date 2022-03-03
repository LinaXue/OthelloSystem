#include<iostream>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<algorithm>
#include<map>
#include<utility>
#include<vector>

#define n 6
#define N 36

using namespace std;

int countFilpsPieces(string gameboard, int playerColor, string grid, int direction);
bool isValidMove(string gameboard, int playerColor, string grid);
bool passTurn(string gameboard, int playerColor);
bool gameOver(string gameboard);
string flipPieces(string gameboard, int playerColor, string grid);
int countColorPieces(string gameboard, int playerColor);
int heuristicScore(string gameboard, int playerColor, int heuristicChoose);
pair<string, int> minimaxSearch(string gameboard, int playerColor, int depth, int alpha, int beta, int heuristic);
string inputMove(string gameboard, int playerColor);

class RandomPlayer{
    public:
    int playerTile;

    RandomPlayer(int playerColor){
        playerTile= playerColor;
    }
    string action(string gameboard){
        if(passTurn(gameboard, playerTile)){
            cout<< (playerTile== 1? "(X)黑":"(O)白")<< "子沒有位置可以下了"<< endl;
            return "pass";
        }else{
            vector<string> validPos;
            for(int x= 0; x< n; x++){
                for(int y= 0; y< n; y++){
                    if(gameboard[x*n+ y]== '+'){
                        char xChar= ('A'+ x), yChar= ('a'+ y);
                        string grid= "";
                        grid+= xChar; grid+= yChar;
                        if(isValidMove(gameboard, playerTile, grid)){
                            validPos.push_back(grid);
                        }
                    }
                }
            }
            int num= validPos.size();
            int randnum= rand()% num;
            string grid= validPos[randnum];
            // cout<< "選擇 \""<< grid<< "\""<< endl;
            return grid;
        }
    }
};

class MinimaxPlayer{
    public:
    int playerTile;
    int heuristic; // 0= origin, 1= good, 2= best 

    MinimaxPlayer(int playerColor, int heuristicChoose){
        playerTile= playerColor;
        heuristic= heuristicChoose;
    }
    string action(string gameboard){
        if(passTurn(gameboard, playerTile)){
            // cout<< (playerTile== 1? "(X)黑":"(O)白")<< "子沒有位置可以下了"<< endl;
            return "pass";
        }else{
            pair<string, int>bestNode;
            // gameboard, playerColor, depth, alpha, beta, heuristic
            bestNode= minimaxSearch(gameboard, playerTile, 10, -9999999, 9999999, heuristic);
            cout<< "選擇 \""<< bestNode.first<< "\""<< endl;
            return bestNode.first;
        }
    }
};

class InputPlayer{
    public:
    int playerTile;

    InputPlayer(int playerColor){
        playerTile= playerColor;
    }
    string action(string gameboard){
        if(passTurn(gameboard, playerTile)){
            cout<< (playerTile== 1? "(X)黑":"(O)白")<< "子沒有位置可以下了"<< endl;
            return "pass";
        }else{
            string grid= inputMove(gameboard, playerTile);
            return grid;
        }
    }
};

string inputMove(string gameboard, int playerColor){
    string grid;
    while(1){
        cout<< "請輸入棋格位置(ex. \"Ae\"), \"over\"下一局: ";
        cin>> grid;
        if(grid== "over"){
            break;
        }else if(!(grid.length()== 2&& 
             int(grid[0])>= int('A')&& int(grid[0])<= int('A')+ n-1&& 
             int(grid[1])>= int('a')&& int(grid[1])<= int('a')+ n-1))
             cout<< "輸入格式錯誤"<< endl;
        else if(!isValidMove(gameboard, playerColor, grid))
            cout<< "此位置不能走"<< endl;
        else break;
    }
    return grid;
}

class Game{
    public:
    string gameboard;
    int turn; // player 1= X(black), player 2= O(white) 玩家顏色
    
    Game(){
        for(int i= 0; i< N; i++){
            gameboard+= "+";
            turn= 1; // 黑子先行
        }
        gameboard[(n/2- 1)*6+ (n/2- 1)]= 'X'; //14
        gameboard[(n/2- 1)*6+ (n/2)]= 'O'; //15
        gameboard[(n/2)*6+ (n/2- 1)]= 'O'; //20
        gameboard[(n/2)*6+ (n/2)]= 'X'; //21
    }

    void printBoard(){
        string line= "  +-";
        cout<< "    ";
        for(int i= 0; i< n; i++){
            cout<< char('a'+ i)<< " ";
            line+= "--";
        }
        line+= "+";

        cout<< endl<< line<< endl;
        for(int i= 0; i< N; i++){
            if(i% n== 0) cout<< char('A'+ (i/n))<< " | ";
            cout<< gameboard[i] <<" ";
            if(i% n== 5) cout<< "|"<< endl;
        }
        cout<< line<< endl;
    }

    int showPoints(){
        // return winner
        int blackScore= countColorPieces(gameboard, 1);
        int whiteScore= countColorPieces(gameboard, 2);
        cout<< "黑子 "<< blackScore<< " 分"<< endl;
        cout<< "白子 "<< whiteScore<< " 分"<< endl;
        
        if(blackScore== whiteScore) return 0;
        return (blackScore> whiteScore? 1: 2);
    }

    void startGame(int playerTile){
        int onePlayer= playerTile;
        int twoPlayer= (onePlayer== 1? 2: 1);

        MinimaxPlayer player1= MinimaxPlayer(twoPlayer, 1);
        InputPlayer player2= InputPlayer(onePlayer);

        if(player1.playerTile== 1) cout<< "Player is "<< "(X)黑"<< "子"<< endl;
        else cout<< "Player is "<< "(O)白"<< "子"<< endl;

        while(!gameOver(gameboard)){
            cout<< "______________________________________________"<< endl;
            printBoard();
            cout<< "輪到"<< (turn== 1? "(X)黑":"(O)白")<< "子"<< endl;
            
            string grid;
            if(turn== player1.playerTile){
                grid= player1.action(gameboard);
            }
            else{
                grid= player2.action(gameboard);
                if(grid== "over"){
                    cout<< "玩家不想玩了！"<< endl<< endl;
                    return;
                }
            }
            if(grid!= "pass")
                gameboard= flipPieces(gameboard, turn, grid);

            int winner= showPoints();
            turn= (turn+ 2)% 2 + 1;
        }

        cout<< "遊戲結束！"<< endl<< endl;
        printBoard();
        int winner= showPoints();
        if(winner== 0) cout<< "平手"<< endl;
        else if(winner== 2) cout<< "白子獲勝！"<< endl;
        else cout<< "黑子獲勝！"<< endl;
        cout<< "遊戲結束"<< endl<< endl;
    }
};

int countFilpsPieces(string gameboard, int playerColor, string grid, int direction){
    // return 有多少對家的棋子可以被翻面
    // state "++++XXOO++++XO+O" 棋盤狀態
    // player 1= X(black), player 2= O(white) 玩家顏色
    // grid at "Xy", X列y行 位置
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

    int index= row* n+ col;
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
    index= row* n+ col;

    while(row>= 0&& row< n&& col>= 0&& col< n){
        if(gameboard[index]== player) return counttt;
        else if(gameboard[index]== opponent) counttt++;
        else return 0;
        row+= Xstep; col+= Ystep;
        index= row* n+ col;
    }
    return 0;
}

bool isValidMove(string gameboard, int playerColor, string grid){
    for(int d= 0; d< 8; d++){
        if(countFilpsPieces(gameboard, playerColor, grid, d)> 0)
            return true;
    }
    return false;
}

bool passTurn(string gameboard, int playerColor){
    // 此玩家沒有任呵地方可以放棋子，只能pass
    for(int x= 0; x< n; x++){
        for(int y= 0; y< n; y++){
            if(gameboard[x*n+ y]== '+'){
                char xChar= 'A'+ x;
                char yChar= 'a'+ y;
                string grid= "";
                grid+= xChar; grid+= yChar;
                if(isValidMove(gameboard, playerColor, grid))
                    return false;
            }
        }
    }
    return true;
}

bool gameOver(string gameboard){
    return (passTurn(gameboard, 1)&& passTurn(gameboard, 2));
}

string flipPieces(string gameboard, int playerColor, string grid){
    // return 新狀態
    string newboard= gameboard;
    char opponent, player;
    if(playerColor== 1) player= 'X';
    else player= 'O';

    if(isValidMove(gameboard, playerColor, grid)){
        int row= grid[0]- 'A', col= grid[1]- 'a';
        int index= row* n+ col;
        int step= 0;

        for(int d= 0; d< 8; d++){
            int counttt= countFilpsPieces(newboard, playerColor, grid, d);
            if(d== 0) step= -n;
            else if(d== 1) step= -n+1;
            else if(d== 2) step= 1;
            else if(d== 3) step= n+1;
            else if(d== 4) step= n;
            else if(d== 5) step= n-1;
            else if(d== 6) step= -1;
            else if(d== 7) step= -n-1;
            for(int i= 1; i<= counttt; i++){
                newboard[index+ step* i]= player;
            }
        }
        newboard[index]= player;
    }
    return newboard;
}

int countColorPieces(string gameboard, int playerColor){
    // 數棋盤上共有幾個黑棋/白棋
    char opponent, player;
    if(playerColor== 1) player= 'X';
    else player= 'O';

    int counttt= 0;
    for(int i= 0; i< gameboard.length(); i++){
        if(gameboard[i]== player) counttt++;
    }
    return counttt;
}

int num_validMove(string gameboard, int playerColor){
    // 整個棋盤中有幾個位置是可以走的
    int counttt= 0;
    for(int i= 0; i< n; i++){
        for(int j= 0; j< n; j++){
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

    int weight[n][n]= { {18, -3, 10, 10, -3, 18}, 
                        {-3, -7,  0,  0, -7, -3},
                        {10,  0,  1,  1,  0, 10},
    	                {10,  0,  1,  1,  0, 10},
    	                {-3, -7,  0,  0, -7, -3},
    	                {18, -3, 10, 10, -3, 18} };
    int x_d[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int y_d[] = {0, 1, 1, 1, 0, -1, -1, -1};

    // Piece difference, frontier disks and disk squares
    for(int i= 0; i< n; i++){
        for(int j= 0; j< n; j++){
            if(gameboard[i*n+ j]== myChar){
                diskWeight+= weight[i][j];
                my_tiles++;
            }else if(gameboard[i*n+ j]== oppChar){
                diskWeight-= weight[i][j];
                opp_tiles++;
            }
            if(gameboard[i*n+ j]!= '+'){
                for(int k= 0; k< 8; k++){ // 8個方向
                    int x= i+ x_d[k];
                    int y= j+ y_d[k];
                    if(x>= 0&& x < n&& y>= 0&& y< n&& gameboard[x*n+ y]== '+'){
                        if(gameboard[i*n+ j]== myChar)
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
    // return (int)score;
    return diskWeight;
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

pair<string, int> minimaxSearch(string gameboard, int playerColor, int depth, int alpha, int beta, int heuristic){
    // return <最佳位置, 最大/小值> ex.<"Ae",-5>
    if(depth== 0 || gameOver(gameboard))
        return pair<string, int>("", heuristicScore(gameboard, playerColor, heuristic));
    
    map<string, int> hScore;
    map<string, int>::iterator iter;
    int opppnentColor;
    if(playerColor== 1) opppnentColor= 2;
    else opppnentColor= 1;

    for(int x= 0; x< n; x++){
        for(int y= 0; y< n; y++){
            if(gameboard[x*n+ y]== '+'){
                char xChar= ('A'+ x), yChar= ('a'+ y);
                string grid= "";
                grid+= xChar; grid+= yChar;
                if(isValidMove(gameboard, playerColor, grid)){
                    string filped= flipPieces(gameboard, playerColor, grid);
                    hScore[grid]= heuristicScore(filped, playerColor, heuristic);
                }
            }
        }
    }

    for(iter= hScore.begin(); iter!= hScore.end(); iter++){
        string filped= flipPieces(gameboard, playerColor, iter->first);
        iter->second= (minimaxSearch(filped, opppnentColor, depth-1, alpha, beta, heuristic)).second;
    }

    string maxGrid= "", mingrid= "";
    int max= -9999999, min= 9999999;

    if(depth% 2== 0){ // max node
        for(iter= hScore.begin(); iter!= hScore.end(); iter++){
            if(iter->second> max){
                max= iter->second;
                maxGrid= iter->first;
            }
            if(max> alpha) alpha= max;
            if(beta<= alpha) break;
        }
        pair<string, int>maxNode(maxGrid, max);
        return maxNode;
    }
    else{ // min node
        for(iter= hScore.begin(); iter!= hScore.end(); iter++){
            if(iter->second< min){
                min= iter->second;
                mingrid= iter->first;
            }
            if(min< beta) beta= min;
            if(beta<= alpha) break;
        }
        pair<string, int>minNode(mingrid, min);
        return minNode;
    }
}

int main(){
    srand(time(NULL));
    int playGame= 1;
    cout<< "Welcome to Othello, (1玩家先手, 2玩家後手, 3結束遊戲): ";
    while(cin>> playGame&& playGame!= 3){
        if(playGame!= 1&&playGame!= 2) break;
        Game game1;
        game1.startGame(playGame);
        cout<< "Welcome to Othello, (1玩家先手, 2玩家後手, 3結束遊戲): ";
    }
    return 0;
}