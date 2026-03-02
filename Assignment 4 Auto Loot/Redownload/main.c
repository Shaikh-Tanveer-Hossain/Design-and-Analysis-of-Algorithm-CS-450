//To compile: gcc -std=c11 -O2 main.c -o main.exe -lm
//To run: .\main.exe < sample_input.txt > my_output_sample_output.txt
//

#include <stdio.h>
#include <sys/time.h>
#include  <string.h>

//#include <windows.h>


//constants
#define MAX_ITEMS 128
#define NAME_LEN  127

//Global variables
int bag_capacity=0;  //The 1st integer of the file will be kept here
int item_features[MAX_ITEMS+1][2]; // array for keeping the weight and values
char item_names[MAX_ITEMS+1][NAME_LEN]; //keep the item names here
int item_count=0; // total number of items found in the array
int final_list [MAX_ITEMS+1]; //keep the item numbers which I want to take
int item_taken=0; //items I have collected in the bag


void item_find(int r, int c, int rows, int cols, const int array[rows][cols])
{
    //printf("\nrow %d , column %d, rows %d, columns %d array_value %d",r,c,rows,cols, array[r][c]);
if (r==0 || c==0) {return;} //reached to the left or top edge
if(r>=rows || c>= cols) {printf("\nError\n"); return;}

if (array[r][c]==array[r-1][c])
{item_find(r-1,c,rows,cols,array );}
else 
{item_taken++;
final_list[item_taken]=r;
int C_weight=item_features[r][0];
//printf("\nitem number %d , totoal taken= %d, item name: %s weight %d , Value %d \n",r,item_taken,item_names[final_list[item_taken]],item_features[r][0],item_features[r][1]);
//printf("next row = %d, C_weight= %d , next column= %d, next value= %d\n",r-1,C_weight,c-C_weight,array[r-1][c-C_weight]);
item_find(r-1,c-C_weight,rows,cols,array);}

}



void print_array(int row_start, int row_end, int column_start, int column_end , int rows, int cols, const int array[rows][cols])
{

printf("\n");
for (int k=0;k<bag_capacity+1;k++) printf(" %d ",k);
printf("-----\n");
for (int i=row_start; i<row_end+1;i++)
{
    for(int j=column_start; j<column_end+1; j++)
    {
        printf(" %d   ",array[i][j]);
    }
    printf("\n");
}
    
}

int main(void){

struct timeval start, end;
//start my clock
gettimeofday(&start, NULL);


//Now I will read the input file. it is given in argv[1].
//The 1st integer is my bag_capacity
scanf("%d", &bag_capacity);
//printf("%d\n",bag_capacity);
//after that  a while loop will read thourh each line of the file and store the rows in arrays
int total_item=0;
 //printf("Input:\n");
 while ( total_item < MAX_ITEMS+1) {
    int weight = 0, value = 0;
 if (scanf(" %126[^;];%d;%d", item_names[total_item+1], &weight, &value) == 3) {
     
     total_item++;
            item_features[total_item][0] = weight;
            item_features[total_item][1] = value;

            //comment it after finishing
           // printf("Line %d  Name: %s, Weight=%d, value=%d \n",  total_item ,item_names[item_count],weight,value );
           //printf("%s",item_names[total_item-1]);
          
           //printf("%s;%d;%d\n",item_names[total_item],weight,value );
        } else {
            // We have reached the end of the file so break
            break;
        }


 }

 //printf("total item = %d\n",total_item);
 int array [total_item+1][bag_capacity+1];
 for (int i=0;i<bag_capacity+1;i++){array[0][i]=0;}
 for (int i=0;i<total_item+1;i++){array[i][0]=0;}

 

 for (int i=1; i<total_item+1;i++)
 { 
 for(int j=1;j<bag_capacity+1; j++)
{   
    
    int C_weight=item_features[i][0];
    int C_value= item_features[i][1];
    //printf(" %d,%d;%d,%d ",i,j,C_weight,C_value);

    if (C_weight>j) {array[i][j]=array[i-1][j];  continue;}

    

    int top= array[i-1][j];
    int alter= C_value+array[i-1][(j-C_weight)];
    int value= top>alter ? top:alter;
    array[i][j]=value;
}
//printf("\n");
}
 

item_find(total_item,bag_capacity,total_item+1, bag_capacity+1, array);



 //Array print
//print_array (0,total_item,0,bag_capacity, total_item+1, bag_capacity+1, array);


int total_weight_taken=0;
int total_value=0;
//Print items in my bag
for (int i=item_taken;i>0;i--)
{   int item_weight=item_features[final_list[i]][0];
    int item_value=item_features[final_list[i]][1];
    total_weight_taken+=item_weight;
    total_value+=item_value;
    printf("%s, %d, %d\n",item_names[final_list[i]],item_weight,item_value);
}
//Print final weight and value 
printf("final weight: %d\n",total_weight_taken);
printf("final value: %d\n",total_value);


 //Sleep(3000);

//calcualte my duration
    gettimeofday(&end, NULL);    // stop clock
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    if (microseconds < 0) { seconds -= 1; microseconds += 1000000; }
    long total_microseconds = seconds * 1000000 + microseconds;
    printf("Total Time %ld microseconds", total_microseconds);

    return 0;
}