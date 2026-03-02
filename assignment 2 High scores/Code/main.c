//I assume there is no negetive value
#include <stdio.h>
#include <stdlib.h>

//comment after completion. Next 1 lines
#include <time.h>


//Which method we are using?
int custom = 0;
int Buff_size= 16; // integer in MB



int read_file_std(int array[][6])
{
    int i = 0;
    int a, b, c, d, e;
    while (scanf("%d %d %d %d %d", &a, &b, &c, &d, &e) == 5)
    {
        array[i][0] = a;
        array[i][1] = b;
        array[i][2] = c;
        array[i][3] = d;
        array[i][4] = e;
        array[i][5] = a + b + c + d + e;
        i++;
    }
    return i;
}



int read_file_custom(int count_sort_board[][6], int ultra_high_values[][6])
{
    int a, b, c, d, e;
    int i = 0;  // count variable for ultra high values
    int values[6]={0};

    while (scanf("%d %d %d %d %d", &a, &b, &c, &d, &e) == 5)
    {
        
        values[0] = a;
        values[1] = b;
        values[2] = c;
        values[3] = d;
        values[4] = e;
        values[5] = a + b + c + d + e;  // total_XP

        // For each skill and total XP
        for (int j = 0; j < 6; j++)
        {
            if (values[j] < 10000)
            {
                count_sort_board[values[j]][j]++;  // counting sort board frequency
            }
            else
            {
                ultra_high_values[i][j] = values[j];  // store ultra-high value
            }
        }

        i++;  // next ultra-high row
    }

    return i;  // nu
}



int main(int argc , char *argv[])
{   
    //comment next 2 line
    clock_t start_time, end_time;
    double time_taken;

    int player_num=0;
    int ultra_count =0;





    //Comment Next 1 lines at the end
     // Clock start
    start_time = clock();



    //Input correctness check:
    if ( argc !=2 ) {printf("Incorrect input format");};

    if (argv[1][0]=='C' || argv[1][0]=='c') {custom=1;};

    //Set buffer size
    setvbuf(stdin,  NULL, _IOFBF, Buff_size * 1024 * 1024);   // input buffer
    setvbuf(stdout, NULL, _IOFBF, Buff_size * 1024 * 1024);   // output buffer


    //Standard sort system:
    if (custom ==0)
    {
        
       static int array[1000000][6];
       player_num = read_file_std(array);
       printf("player found %d \n", player_num);


       //sorting using qsort:
            // Compare function for the qsort
            int compare_function(const void *a, const void *b)
            {
            int x = *(const int *)a;
            int y = *(const int *)b;
            return (y - x);  // descending
            }

            //creating a single long array for qsort
            int *temp = malloc(sizeof(int) * player_num);
            if (temp == NULL)
            {
            printf("Failed to allocate memory");
            exit(1);
            };

            //Calling the library function QSort
            //working on each column at a time
            for(int i=0; i<6; i++)
            {

            // Getting values for temp 1d array
            for (int j = 0; j < player_num; j++)
            temp[j] = array[j][i];

            //Use qsort
            qsort(temp, player_num, sizeof(int), compare_function);
            //Writing in file
            if (i==0) printf("SKILL_BREAKDANCING\n");
            else if (i==1) printf("\nSKILL_APICULTURE\n");
            else if (i==2) printf("\nSKILL_BASKET\n");
            else if (i==3) printf("\nSKILL_XBASKET\n");
            else if (i==4) printf("\nSKILL_SWORD\n");
            else if (i==5) printf("\nTOTAL_XP\n");

            //print the temp
            for (int k=0; k<player_num; k++)
            printf("%d\n", temp[k]);

            };

        };


    //Custom sort system:
    if (custom ==1)
    {
        
        //Create an array to do the count sort recording
        static int count_sort_board[10001][6] = {0};
        // Read from the file. It will do sort as well
        ultra_count = read_file_custom(count_sort_board);
        //write the result in file
        write_matrix_custom(count_sort_board, ultra_high_values, ultra_count);

    };




    //Comment Next 3 lines at the end
    //Clock Stop
    end_time = clock();
    time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Program execution took %f seconds.\n", time_taken);

}