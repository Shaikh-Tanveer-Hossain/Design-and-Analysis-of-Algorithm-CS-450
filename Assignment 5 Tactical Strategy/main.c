//I have used A* algorithm to find the adjecent tiles of the destination. I have used the heurestic as Chevy-chev-1.
//Please consider me for the extra 20% credit mentioned in the assignment ruberic. 
//compile:gcc -std=c11 -O2 -lm main1.c -o main1
// or: gcc -std=c11 -O2 -lm main.c -o main

//run: main1.exe < sample_input.txt > my_output.txt

//libraries: 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //

const int intial_row=5;

//Global variable
int row_max=0, col_max=0;
int start[1][2]= {0,0};
int end[1][2]={0,0};
int  heap_array_size=0;
int heap_used=0;

typedef struct {
    int row;
    int col;
    int parent_row;
    int parent_col;
    float cost;
    float heuristic;
} tile;

void matrix_print (char **lines ){
    for(int i=0;i<row_max;i++){
         for(int j=0;j<col_max;j++){
        printf("%c", lines[i][j]);
            if(j<col_max-1)printf(" ");
        }
        printf("\n");
    }
}


//Function to calculate raw cost based on tile  type
float raw_cost(int row, int col, char **lines_cpy){
    char terrain = lines_cpy[row][col];
       if (terrain == '2') {
    return 0.0;   // destination tile
}
if (terrain == '.') {
    return 1.0;   // road
}
if (terrain == '=') {
    return 50.0;  // wall
}
if (terrain == ',') {
    return 2.0;   // road
}
if (terrain == 'o') {
    return 3.0;   // rock
}
printf("unknown situation");
}

//calculate the relational cost
float relationship(int row, int col, int i, int j){
    //corner move
    if(row!=i && col!=j){
        return 1.5;
    }
    //left-right or up-down move
    return 1.0;
}

float calculate_heuristic (int i, int j){
    //Chevyshev distance -1
    //I am using cheveshev distance as it has cost 1 for any 8 side movement. for straght movement it is similar to the given probem. But for corner it will underestimate which is good. also the -1 is to consider that any tiles adjecent to the destination has zero cost to the final tile
    int dx = abs(i - end[0][0]);
    int dy = abs(j - end[0][1]);        
    return (fmax(dx, dy) - 1);
}

//Heap functions
void Heap_up(tile *Heap_array, int start_index){
    
    if (start_index ==0) return; //base case
    int parent_index = (start_index - 1) / 2;

    if (Heap_array[start_index].heuristic < Heap_array[parent_index].heuristic){
        //swap
        tile temp = Heap_array[start_index];
        Heap_array[start_index] = Heap_array[parent_index];
        Heap_array[parent_index] =  temp;

        //recursively call Heap_up on parent index
        Heap_up(Heap_array, parent_index);
    }
}


void Heap_insert(tile *Heap_array, tile new_tile){
    Heap_array[heap_used] = new_tile;
    if (heap_used > 0) {
        Heap_up(Heap_array, heap_used);
    }
    heap_used++;
}


void heap_down(tile *Heap_array, int index){

    int smallest_heurestic_index = 0;
    int left_child_index = 2 * index + 1;
    if (left_child_index >= heap_used) return; //no children and end of the tree. so go back and down procedure is complete
    int right_child_index = left_child_index + 1; 
    if(right_child_index >= heap_used){
        //No right child
        smallest_heurestic_index = left_child_index;
    }
    else{
        //both child is there
        smallest_heurestic_index = (Heap_array[left_child_index].heuristic < Heap_array[right_child_index].heuristic) ? left_child_index : right_child_index;
    }

    if(Heap_array[index].heuristic > Heap_array[smallest_heurestic_index].heuristic){
        //swap with children
        tile temp = Heap_array[index];
        Heap_array[index] = Heap_array[smallest_heurestic_index];
        Heap_array[smallest_heurestic_index] = temp;

        //recursively call heap_down on smallest_heurestic_index
        heap_down(Heap_array , smallest_heurestic_index);
    }

}


void Heap_Explore(tile *Heap_array, tile TILE_ARRAY[row_max][col_max], char **lines_cpy)
   { //take the minimum element from the heap
    tile min_tile = Heap_array[0];
    //check if min_tile is the end tile
    if (min_tile.row == end[0][0] && min_tile.col == end[0][1]){
        //printf("Destination reached");
        return;
    }
    //remove the element from the heap
    Heap_array[0] = Heap_array[heap_used-1 ]; 
    heap_used--;
    if (heap_used > 1) heap_down(Heap_array ,0);

    
    
    //insert the neighbours of min_tile into the heap if not already explored
   
    int row = min_tile.row;
    int col = min_tile.col;
    int parent_row = min_tile.parent_row;
    int parent_col = min_tile.parent_col;     

    for(int i=row-1; i<=row+1; i++){
        if(i<0 || i>=row_max) continue; //out of matrix
        for(int j=col-1; j<=col+1; j++){
            if(j<0 || j>=col_max) continue; //out of matrix
            if(i==row && j==col) continue; //same tile . so no need to insert
            if(i==parent_row && j==parent_col) continue; //parent tile . so no need to insert
            if(lines_cpy[i][j]=='1' || lines_cpy[i][j]=='3' || lines_cpy[i][j]=='0')continue; //unexplorable tile or start tile. so ignore

            //For the valid neighbour tiles, lets calculate cost and heuristic
            float cost= raw_cost(i,j, lines_cpy) * relationship(row,col,i,j) + min_tile.cost;
            float heuristic = calculate_heuristic(i,j)+ cost;
            //update the tile in TILE_ARRAY if cost is less than previously found out cost
            if(cost < TILE_ARRAY[i][j].cost){
                TILE_ARRAY[i][j].cost = cost;
                TILE_ARRAY[i][j].heuristic = heuristic;
                TILE_ARRAY[i][j].parent_row = row;
                TILE_ARRAY[i][j].parent_col = col;

                //insert the tile into the heap
                Heap_insert(Heap_array, TILE_ARRAY[i][j]);}
        }
    }
    Heap_Explore(Heap_array, TILE_ARRAY, lines_cpy);
}



void A_star_search(char **lines_cpy, tile TILE_ARRAY[row_max][col_max], tile *Heap_array){
    //insert the start tile into the heap
    Heap_insert(Heap_array, TILE_ARRAY[start[0][0]][start[0][1]]);

    //print the heap used for testing
      //printf("Heap used: %d\n", heap_used);
    //printf("Heap_array[%d]: row=%d, col=%d, cost=%.2f, heuristic=%.2f\n", i, Heap_array[i].row, Heap_array[i].col, Heap_array[i].cost, Heap_array[i].heuristic);
    
    Heap_Explore(Heap_array, TILE_ARRAY, lines_cpy);

}

void Print_TILE_ARRAY(tile TILE_ARRAY[][col_max], int what){
    for(int i=0;i<row_max;i++){
        for(int j=0;j<col_max;j++){
            if (what==1)
            {printf("%.2f    ",  TILE_ARRAY[i][j].cost);}
            else {printf("%.2f    ",  TILE_ARRAY[i][j].heuristic);}
        }
        printf("\n");
    }
}



void path_make(char **lines_cpy, tile TILE_ARRAY[row_max][col_max], tile *Heap_array, int row, int col){
    //start from the end tile recurressively go to the parent title unless we reach the source
    int parent_row = TILE_ARRAY[row][col].parent_row;
    int parent_col = TILE_ARRAY[row][col].parent_col;

    if (parent_row == start[0][0] && parent_col == start[0][1]) {
        //reached the start tile . done changing
        return;
    }

    //change the parent tile to '*'
    lines_cpy[parent_row][parent_col] = '*';
    //recursively call path_make on parent tile 
    path_make(lines_cpy, TILE_ARRAY, Heap_array, parent_row, parent_col); 
}






int main() {

    char **lines = NULL;
    char **lines_cpy = NULL;
    int row=0, row_allocated=0, col_alocated=0,column=0;
    int coma_count=0, equal_count=0, dot_count=0, rock_count=0, zero_count=0, one_count=0, two_count=0, three_count=0;
    char *line=NULL;
  

    //allocate memory for 10 rows
    lines = malloc(sizeof(char*)*intial_row);
    row_allocated=intial_row;
    //printf("Allocated %d rows initially\n", row_allocated); //test
    //allocate 10 chars for 1st row
    lines[0] = malloc(sizeof(char)*intial_row);
    col_alocated=intial_row;
    //printf("Allocated %d columns initially\n", col_alocated); //test

    //reading the input
    while(1){
        //read one character at a time
        char ch=getchar();
        //counting characters
        if(ch==',') {coma_count++; }
        else if(ch=='=') {equal_count++;}
        else if(ch=='.') {dot_count++;}
        else if(ch=='o') {rock_count++;}
        else if(ch=='0') {zero_count++; 
        //start found
        start[0][0]=row;
        start[0][1]=column;
        }
        else if(ch=='1') {one_count++;}
        else if(ch=='2') {two_count++;
        //end found
        end[0][0]=row;
        end[0][1]=column;
        }
        else if(ch=='3') {three_count++;}

        if(ch==' ') continue;
        if(ch=='\n'){
            if(row==0) 
            {col_max=column;
                //reallocate the 1st row to exact size
                lines[0] = realloc(lines[0], sizeof(char)*col_max);
                col_alocated=col_max;
            }
            row++;column=0;//printf("\n");
            
            continue;}
        if(ch==EOF) {break;}
        //printf("%c ", ch);
        

        //saving the charecter

        //increase columns if needed . only for row 1
        if (row==0 && column>=col_alocated) {
            lines[row] = realloc(lines[row], sizeof(char)*col_alocated*2);
            col_alocated = col_alocated*2;
            //printf("Reallocated column size to %d\n", col_alocated); //test
        }
        if(row>0 && column==0 && row<row_allocated){
            //allocate memory for new row
            lines[row] = malloc(sizeof(char)*col_alocated);
            //printf("Allocated %d columns for row %d\n", col_alocated, row); //test
        }

        //increase rows if needed
        if (row>=row_allocated) {
            lines = realloc(lines, sizeof(char*)*row_allocated*2);
            row_allocated = row_allocated*2;
            //printf("Reallocated %d rows\n", row_allocated); //test
            lines[row] = malloc(sizeof(char)*col_alocated);
        }

        lines[row][column]=ch;
        //printf("%c " , *&lines[row][column]);
        column++;

    }

     row_max=row+1;

    /*
     printf("\nTotal Rows: %d, Total Cols: %d\n", row_max, col_max); //test
     printf("coma_count=%d, equal_count=%d, dot_count=%d, rock_count=%d, zero_count=%d, one_count=%d, two_count=%d, three_count=%d\n", coma_count, equal_count, dot_count, rock_count, zero_count, one_count, two_count, three_count); //test
     printf("Total %d", coma_count + equal_count + dot_count + rock_count + zero_count + one_count + two_count + three_count); //test
    */
    //reallocate the array to exact size
    lines = realloc(lines, sizeof(char*)*row_max);
    //print the row and col number of new array
    
    //make a copy of the matrix
    lines_cpy = malloc(sizeof(char*)*row_max);
    for(int i=0;i<row_max;i++){
        lines_cpy[i] = malloc(sizeof(char)*col_max);
         for(int j=0;j<col_max;j++){
            lines_cpy[i][j] = lines[i][j];
        }
    }

    /*
    printf("\n");
    //print the 1st element
        printf("First element: %c\n", lines[0][0]);
    //print the last element of 1st row
        printf("Last element of 1st row: %c\n", lines[0][col_max-1]);
    //print the last element of last row
        printf("Last element of last row: %c\n", lines[row_max-1][col_max-1]);
    */
    //matrix_print(lines_cpy);
    //sprintf("\n\n\n\n");
    //start point
    //printf("Start point: (%d, %d)\n", start[0][0], start[0][1]);
    //end point    
    //printf("End point: (%d, %d)\n", end[0][0], end[0][1]);  

    //declearing an 2D array of size row_max*col_max of tile structs TILE_ARRAY
    tile TILE_ARRAY[row_max][col_max]; 
    for(int i=0;i<row_max;i++){
        for(int j=0;j<col_max;j++){
            TILE_ARRAY[i][j].row = i;
            TILE_ARRAY[i][j].col = j;
            TILE_ARRAY[i][j].parent_row = -1; //initially no parent
            TILE_ARRAY[i][j].parent_col = -1; //initially no parent
            TILE_ARRAY[i][j].cost = INFINITY; //initially infinite cost
            TILE_ARRAY[i][j].heuristic = INFINITY;
        }
    }

    //print some element for testing from TILE_ARRAY
    //printf("TILE_ARRAY[0][0]: row=%d, col=%d, parent_row=%d, parent_col=%d, cost=%.2f, heuristic=%.2f\n", TILE_ARRAY[0][0].row, TILE_ARRAY[0][0].col, TILE_ARRAY[0][0].parent_row, TILE_ARRAY[0][0].parent_col, TILE_ARRAY[0][0].cost, TILE_ARRAY[0][0].heuristic);

    //printf("\n\n");
    //Setting Start tile cost to 0
    TILE_ARRAY[start[0][0]][start[0][1]].cost = 0.0;
    TILE_ARRAY[start[0][0]][start[0][1]].heuristic = 0.0;


    //print the whole TILE_ARRAY cost for testing
    //Print_TILE_ARRAY(TILE_ARRAY,1);
    //printf("\n\n");

    //print the whole TILE_ARRAY heuretic for testing
    //Print_TILE_ARRAY(TILE_ARRAY,2);


    //Creating a 1D matrix for Heap implementation
    heap_array_size = row_max * col_max;
    tile Heap_array[heap_array_size];

    //start exploring from the start tile
    A_star_search(lines_cpy, TILE_ARRAY, Heap_array);
    
    //Now the lines_cpy matrix needs to be modified witht the path being found. Update with *
    path_make(lines_cpy, TILE_ARRAY,Heap_array, end[0][0], end[0][1]);
    //print the final path
    matrix_print(lines_cpy);
    
    //print the total cost to reach the destination (up to 2 decimals)
    printf("Total cost: %.1f\n", (double)TILE_ARRAY[end[0][0]][end[0][1]].cost);



    return 0; }