#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
//should be done
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    ifstream instream;
    bool numdoor = false;
    bool numexit = false;
    char next;
    instream.open(fileName);
    //open file properly
    if(!instream.is_open()){
        return nullptr; 
    }

    instream >> maxRow >> maxCol >> player.row >> player.col;
    //check if values are valid
    if(instream.fail()){
        return nullptr;
    }
    if(maxRow <= 0){
        return nullptr;
    }
    if(maxCol <= 0){
        return nullptr;
    }

    //check if more than int max characters
    if(INT32_MAX/maxRow < maxCol){
        return nullptr;
    }
    if(INT32_MAX/maxCol < maxRow){
        return nullptr;
    }

    //player coord in map
    if(player.row >= maxRow || player.row < 0){
        return nullptr;
    }
    if(player.col >= maxCol || player.col < 0){
        return nullptr;
    }

    char** map = createMap(maxRow,maxCol);
    //if any issues with map
    if(map == nullptr){
        deleteMap(map,maxRow);
        return nullptr;
    }
    for(int i = 0; i < maxRow; i++){
        for(int j = 0; j < maxCol; j++){
            instream >> next;
            //any issues with getting character
            if(instream.fail()){
                deleteMap(map,maxRow);
                return nullptr;
            }
            //check if not enough
            if(instream.eof()){
                deleteMap(map,maxRow);
                return nullptr;
            }
            map[i][j] = next;
            //check if player exists on map when it shouldn't
            if(map[i][j] == TILE_PLAYER){
                deleteMap(map,maxRow);
                return nullptr;
            }
            //check if there is a door
            if(map[i][j] == TILE_DOOR){
                numdoor = true;
            }
            //check if there is an exit
            if(map[i][j] == TILE_EXIT){
                numexit = true;
            }
        }
    }
    //make sure you have at least 1 door or 1 exit
    if(numexit == false && numdoor == false){
        deleteMap(map,maxRow);
        return nullptr;
    }
    //check for extra characters
    if(instream >> next){
        deleteMap(map,maxRow);
        return nullptr;
    }
    //add player into map if coord is empty
    if(map[player.row][player.col] == TILE_OPEN){
        map[player.row][player.col] = TILE_PLAYER;
    }
    //if player coord is on something
    else{
        deleteMap(map,maxRow);
        return nullptr;
    }
    instream.close();
    return map;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
//should be done
void getDirection(char input, int& nextRow, int& nextCol) {
    switch(input){
        //key s
        case MOVE_DOWN :
            nextRow += 1; 
            break;
        //key w
        case MOVE_UP :
            nextRow -= 1;
            break;
        //key a
        case MOVE_LEFT :
            nextCol -= 1;
            break;
        //key d
        case MOVE_RIGHT :
            nextCol += 1;
            break;
        //any other character
        default :
            break;
    }
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
//should be done
char** createMap(int maxRow, int maxCol) {
    //if map coord are not valid
    if(maxRow <= 0 || maxCol <= 0){
        return nullptr;
    }
    char** map = new char*[maxRow];
    for(int i = 0; i < maxRow; i++){
        map[i] = new char[maxCol];
    }
    //set every character to open tile
    for(int i = 0; i < maxRow; i++){
        for(int j = 0; j < maxCol; j++){
            map[i][j] = TILE_OPEN;
        }
    }
    return map;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
//should be done
void deleteMap(char**& map, int& maxRow) {
    //if map is already null don't do nothing
    if(map == nullptr){
        maxRow = 0;
        map = nullptr;
    }
    else{
        //delete every value in array
        for(int i = 0; i < maxRow; i++){
        delete[] map[i];
        }
        delete[] map;
        maxRow = 0;
        map = nullptr;
    }
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
//should be done
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    int oldmaxRow = 0;
    //make sure resize is possible
    if(maxRow <= 0 || maxCol <= 0 || map == nullptr){
        return nullptr;
    }
    maxRow *= 2;
    maxCol *= 2;
    char** newmap = createMap(maxRow,maxCol);
    //subarea a
    for(int i = 0; i < maxRow/2; i++){
        for(int j = 0; j < maxCol/2; j++){
            newmap[i][j] = map[i][j];
        }
    }
    //subarea b
    for(int i = maxRow/2; i < maxRow; i++){
        for(int j = maxCol/2; j < maxCol; j++){
            if(map[i-maxRow/2][j-maxCol/2] == TILE_PLAYER){
                newmap[i][j] = TILE_OPEN;
            }
            else{
                newmap[i][j] = map[i-maxRow/2][j-maxCol/2];
            }
        }
    }
    //subarea c
    for(int i = maxRow/2; i < maxRow; i++){
        for(int j = 0; j < maxCol/2; j++){
            if(map[i-maxRow/2][j] == TILE_PLAYER){
                newmap[i][j] = TILE_OPEN;
            }
            else{
                newmap[i][j] = map[i-maxRow/2][j];
            }
        }
    }
    //subarea d
    for(int i = 0; i < maxRow/2; i++){
        for(int j = maxCol/2; j < maxCol; j++){
            if(map[i][j-maxCol/2] == TILE_PLAYER){
                newmap[i][j] = TILE_OPEN;
            }
            else{
                newmap[i][j] = map[i][j-maxCol/2];
            }
        }
    }
    //delete old map value
    oldmaxRow = maxRow / 2;
    deleteMap(map,oldmaxRow);
    return newmap;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
//should be done
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    //check for any stay possibilities
    if((nextRow == player.row && nextCol == player.col) || nextRow >= maxRow || nextRow < 0 || nextCol >= maxCol || nextCol < 0 || map[nextRow][nextCol] == TILE_PILLAR || map[nextRow][nextCol] == TILE_MONSTER){
        return STATUS_STAY;
    }
    //getting treasure
    else if(map[nextRow][nextCol] == TILE_TREASURE){
        player.treasure += 1;
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        return STATUS_TREASURE;
    }
    //getting amulet
    else if(map[nextRow][nextCol] == TILE_AMULET){
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        return STATUS_AMULET;
    }
    //going into door
    else if(map[nextRow][nextCol] == TILE_DOOR){
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        return STATUS_LEAVE;
    }
    //going into exit
    else if(map[nextRow][nextCol] == TILE_EXIT){
        if(player.treasure < 1){
            return STATUS_STAY;
        }
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        return STATUS_ESCAPE;
    }
    //any other player movement
    else{
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[nextRow][nextCol] = TILE_PLAYER;
        return STATUS_MOVE;
    }
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    //start at player and go right
    for(int i = player.col+1; i < maxCol; i++){
        //if a pillar exists before getting to the monster
        if(map[player.row][i] == TILE_PILLAR){
            break;
        }
        //if monster is found
        else if(map[player.row][i] == TILE_MONSTER){
            //turn old tile to blank
            map[player.row][i] = TILE_OPEN;
            //double check if monster is on the right of player
            if(player.col < i){
                map[player.row][i-1] = TILE_MONSTER;
            }
        }
    }
    //start at player and go left
    for(int i = player.col-1; i >= 0; i--){
        //if a pillar exists before getting to the monster
        if(map[player.row][i] == TILE_PILLAR){
            break;
        }
        //if monster is found
        else if(map[player.row][i] == TILE_MONSTER){
            //turn old tile to blank
            map[player.row][i] = TILE_OPEN;
            //double check if monster is on the left of player
            if(player.col > i){
                map[player.row][i+1] = TILE_MONSTER;
            }
        }
    }

    //start at player and go down
    for(int i = player.row+1; i < maxRow; i++){
        //if a pillar exists before getting to the monster
        if(map[i][player.col] == TILE_PILLAR){
            break;
        }
        //if monster is found
        else if(map[i][player.col] == TILE_MONSTER){
            //turn old tile to blank
            map[i][player.col] = TILE_OPEN;
            //double check monster is below player
            if(player.row < i){
                map[i-1][player.col] = TILE_MONSTER;
            }
        }
    }

    //start at player and move up
    for(int i = player.row-1; i >= 0; i--){
        //if a pillar exists before getting to the monster
        if(map[i][player.col] == TILE_PILLAR){
            break;
        }
        //if monster is found
        else if(map[i][player.col] == TILE_MONSTER){
            //turn old tile to blank
            map[i][player.col] = TILE_OPEN;
            //double check monster is above player
            if(player.row > i){
                map[i+1][player.col] = TILE_MONSTER;
            }
        }
    }
    //if monster is on player
    if(map[player.row][player.col] == TILE_MONSTER){
        return true;
    }
    return false;
}