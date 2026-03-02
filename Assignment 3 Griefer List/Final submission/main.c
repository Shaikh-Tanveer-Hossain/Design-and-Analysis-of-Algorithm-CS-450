//To compile
//gcc -std=c11 -O2 -lm -o main main.c

//To run:
// main [input file name. the file contains all the nodes] < [File contains the search value] > file you expect to get output
//main griefers.dat < input.txt > output.txt

//Include Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 #include <math.h>
#include <time.h>


#define ALPHA 0.75
#define FAMILY_CHAIN_SIZE 100



//Some global Variables
int Buff_size= 16; // integer in MB
int Tree_elements_total= 0;
int depth=0;
int max_depth=0;
int line_size= 35; // one line size. I kept some safety window (12+1+3+1+10=27)
int tree_print_n=0;
int inward_index = 0;
int read_limit=10000000; //10^7 now
int itr=0;




//Datastructures:------------------------------------
//Tree nodes. Same name can appear many times, hit variable will keep track of that. servers pointer will store the list of servers. 
//I am not expecting exact same entry 2+ times so I am only storing the latest time 
typedef struct  Node {
    char name[13];
    time_t latest_time;
    int last_server;
    int hit;
    struct Node *left, *right;
} Node;



Node *family_chain [FAMILY_CHAIN_SIZE] = { NULL };


//Custom functions:---------------------------------------------------

int count_elements(Node *root) {
    if (!root) return 0;
    return 1 + count_elements(root->left)  + count_elements(root->right);
}



Node* create_node ( const char *name, int server_id, time_t unix_time, int registry) {
    Node *n = malloc(sizeof(Node));
    

    strcpy(n->name, name);
    n->latest_time = unix_time;
    n->last_server = server_id;
    n->hit = 1;
     n->left = NULL;
    n->right = NULL;
    if(registry==1)
        {depth++;
        family_chain[depth-1]=n;
         Tree_elements_total++; //keep count of the total elements I entered
    }
    return n;
}



void inward_traversal(Node *root, char (*names)[13], int *servers, time_t *times) {
    
    if (root == NULL) return;
    inward_traversal(root->left, names, servers, times);
    
    strcpy(names[inward_index], root->name);
    servers[inward_index] = root->last_server;
    times[inward_index] = root->latest_time;
      inward_index++;
    
    inward_traversal(root->right, names, servers, times);
}



// clears unnecessary nodes
void free_subtree(Node *root) {
    if (!root) return;
    free_subtree(root->left);
    free_subtree(root->right);
    free(root);
}

void print_inwardtraversal(char (*names)[13], int *servers, time_t *times,int size)
{
    //printf("\n ITR print\n");
    for (int i = 0; i < size; i++)
    {
        //printf("%3d. Name: %-12s  Server: %4d  Time: %ld\n",i, names[i], servers[i], times[i]);
    }
    //printf(" test itr end\n");
}


int which_child(Node *parent,Node *child)
{
    if (parent == NULL || child == NULL) return 0;
if (parent->left == child) return -1;
else if (parent->right == child) return +1;
else return 0;
}



void Tree_Print (Node **root )
{tree_print_n++;
    // print current node
    printf("%d.  %s  Server: %d  Time: %ld  Hits: %d  Address: %p   L.Address: %p   R.Address: %p   \n", tree_print_n, (*root)->name, (*root)->last_server, (*root)->latest_time, (*root)->hit, (void *)(*root), (void *)(*root)->left,(void *)(*root)->right);

    if((*root)->left != NULL) Tree_Print(&((*root)->left));
    if((*root)->right != NULL) Tree_Print(&((*root)->right));

    return;
}



Node* perfect_tree(char (*names)[13], int *servers, time_t *times,int start, int end, int size)
{   
    int median=0;
    Node* temp=NULL;;
    int left_end=0;
    int right_start=0;
    int median_index=0;
    itr++;
    //printf("itr= %d  (start=%d, end=%d, size=%d)\n", itr, start, end, size);
    if (start<0  || start> end) {
        //printf("Error in perfect tree making (start=%d, end=%d, size=%d)\n",start, end, size);
    return NULL;}

    //if there is one elemen then create a node >> fill it up with the values of start and retunr it
    if (size==1 || start==end)
   { temp= create_node(names[start], servers [start], times[start],0);
    //printf("created new node for index= %d (start=%d, end=%d, size=%d)\n",start, start, end, size);

    return temp;
   }

   if (size>1)
   {

    median =(int)ceil((float)size / 2.0); // median gives the original sequence not array index
    median_index=start+median-1;
    temp= create_node(names[median_index], servers [median_index], times[median_index],0);
   // printf("created new node for index= %d (start=%d, end=%d, size=%d)\n",median_index, start, end, size);


    //fill my left hand
    left_end=median_index-1;
    size=left_end-start+1;
    if (size!=0) temp->left=perfect_tree(names, servers, times,start, left_end, size);

    //fill my right hand
    right_start=median_index+1;
    size=end-right_start+1;
    if (size!=0) temp->right=perfect_tree(names, servers, times,right_start, end, size);
    //return my address

    return temp;

   }
return NULL;
}





void Balance_Tree()
{ 
    int child_size=0;
    int parent_size=0;
    int start=0;
    int end=0;
    int side=0;
    int size;
    //int l=depth;
    //printf("family_chain[%d] = %s\n", l - 1, family_chain[l - 1]->name);


    //run a for loop with depth and decrease each time.
    for (int i=depth; i>1; i--)
    {   
        Node *child  = family_chain[i-1];
        Node *parent = family_chain[i-2];
        //take the value form family_chain[i-1] and its parent
        child_size=count_elements(child);
        parent_size=count_elements(parent);
       // printf("Child =  %d  C.name= %s  cs=%d  parent = %d   P.name= %s  Ps= %d\n", i-1,child->name,child_size, i-2,parent->name, parent_size);

        //if (child_size > floor(ALPHA * parent_size)){printf("Test point B\n");}
       
        if (child_size > floor(ALPHA * parent_size)) //allow one more value then its parents count
        {   
            //Tree is imbalanced and needs to be balanced
            //scapegoat found =child =family_chain[i-1]
          //  printf("Scape goat found. number= %d , name= %s , paernt= %s, parent number=%d\n",i-1,child->name,parent->name, i-2);
            
            //declear an 3 arrys of size the child_size
            // Allocate temporary arrays to rebuild this subtree
            char (*temp_name_array)[13] = malloc(child_size * sizeof(*temp_name_array));

            if (!temp_name_array ) {
            printf(" allocation failed when ring balancing. temp_name_array\n");
            return;
            }
            
            int *temp_server_id_array = malloc(child_size * sizeof(int));
            if ( !temp_server_id_array ) {
            printf(" allocation failed when ring balancing. temp_server_id_array\n");
            return;
            }
            time_t *temp_unix_time_array = malloc(child_size * sizeof(time_t));
            if ( !temp_unix_time_array) {
            printf(" allocation failed when ring balancing. temp_unix_time_array\n");
            return;
            }

            //now call the inward_traversal and pass the arrys and the child node address
            inward_index = 0;
            inward_traversal(child, temp_name_array, temp_server_id_array, temp_unix_time_array);
            start=0;
            end=child_size-1;
            size= end-start+1;
            print_inwardtraversal(temp_name_array, temp_server_id_array, temp_unix_time_array, size);

            //now we find out if the child is left child or right child of the parent node. -1 means left +1 means right. otherwise error
            side= which_child(parent,child);

            itr=0;
            if (side==-1) 
            {  // printf("Left start_index= %d end index= %d size=%d\n",start,end,size);
                parent->left=perfect_tree(temp_name_array, temp_server_id_array, temp_unix_time_array,start,end,size);
                
            }
            else if (side==1) 
            {//printf("right start_index= %d end index= %d size=%d\n", start,end,size);
                parent->right=perfect_tree(temp_name_array, temp_server_id_array, temp_unix_time_array,start,end,size);
                
            }
            else printf("error in finding parent child relationship");
            
           
            
            //comment later
            //printf("imbalance\n" );
            //printf("Inward_Traverse arry print\n");
            //for (int j=0; j<child_size; j++)
            //{  
                
             //  printf("%s \n",temp_name_array[j] );
           // }
            //printf("balanced subtree\n" );
            tree_print_n=0;
           // Tree_Print(&parent);
            free_subtree(child); // free old subtree
            free(temp_name_array);
            free(temp_server_id_array);
            free(temp_unix_time_array);

            break;



            //lets clear the subtree now as I already have it in the array
           // free_subtree(family_chain[i-1]);

            //create a perfect tree of element size = child_size. call the function create_perfect_tree. all values are null here . 
            
            //the root of this tree will be the child of family_chain[depth-1]
            //once done call a function called value_put_in_perfect_tree. it takes the array, the perferct subtree. call root element assign it the median value. call itself with left subpart of the arry and left node as a place to input and do the same in the right. check if the node exists before sending
        }


    }
}

    
     



Node* Tree_Search(const char *name, Node **current, int search_and_insert, int server_id, time_t unix_time)
{
   // my tree at least have one element

    // store the current node in family_chain and increase depth
    family_chain[depth] = *current;
    depth++;

    int compare_value = strcmp(name, (*current)->name);

    if (compare_value == 0) {
        // this value already exists
        if (search_and_insert == 1) {
            // update existing info if newer timestamp
            if (unix_time > (*current)->latest_time) {
                (*current)->latest_time = unix_time;
                (*current)->last_server = server_id;
            }
            (*current)->hit++;
        }
        return *current;
    }
    else if (compare_value < 0) {
        // go left
        if ((*current)->left == NULL) {
            if (search_and_insert == 1) {
                // create new node
                (*current)->left = create_node(name, server_id, unix_time,1);
                return (*current)->left;}
             else {
                return NULL;
              }
        }
         else {return Tree_Search(name, &((*current)->left), search_and_insert, server_id, unix_time);}
    }
    else { // if compare_value > 0
        // go right
        if ((*current)->right == NULL) {
            if (search_and_insert == 1) {
                (*current)->right = create_node(name, server_id, unix_time,1);
                return (*current)->right;   }
             else {
                return NULL;}
            
        }
         else {return Tree_Search(name, &((*current)->right), search_and_insert, server_id, unix_time);  }
        
    }
}


void print_family_chain()
{
  for (int i = 0; i < depth; i++) {
      printf("[%d] ptr=%p  ", i, (void*)family_chain[i]);
     if (family_chain[i])
        printf("name=%s\n", family_chain[i]->name);
    
         else
        printf("(NULL)\n");
}

}


Node* Tree_insert(const char *name, int server_id, time_t unix_time, Node **root)
{    int pre_element=Tree_elements_total;
    Node *destination = NULL;
    int search_and_insert = 1;


    //for the very 1st element
    if (Tree_elements_total==0) {
        *root = create_node(name, server_id, unix_time,1);
        return *root;
    }

    for (int i = 0; i < FAMILY_CHAIN_SIZE; i++) family_chain[i] = NULL;
    depth=0;

    // 1st search for the correct location . if found just send back the address of that node. if not create a new node. load the values and send me back the address of new node
    destination = Tree_Search(name, root, search_and_insert, server_id, unix_time);
    
    //printf("test point");
    //check if I inserted anything in Tree_Search. if not that means it already there.
    if (pre_element==Tree_elements_total){return destination;}
    else{
            //max_depth = (int)(log(Tree_elements_total) / log(1.0 / ALPHA)) + 1;
            max_depth = (int)(floor(log((double)Tree_elements_total) / log(1.0 / ALPHA)));

           // printf("element= %d name=%s  max depth=%d c current depth=%d \n",Tree_elements_total, destination->name,max_depth,depth);
           // print_family_chain();
        if (depth>max_depth) 
        {
             //printf("Imbalance found\n");
             //printf("last node = %s\n", family_chain[depth-1]->name);
             Balance_Tree();
           
        }
    }
    

}






int file_read(const char *filename, Node **root) {

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("File Not Found");
        return 1;
    }
    
    int k=0;
    char line[line_size];       // one line size. I kept some safety window (12+1+3+1+10=27)
    char name[13];       // max 12 chars + '\0'
    int server_id;
    time_t unix_time;
    Node* temp_node= NULL;

    setvbuf(fp, NULL, _IOFBF, Buff_size * 1024 * 1024);


    while (fgets(line, sizeof(line), fp) != NULL   && k<read_limit) {

       k++;
        if (sscanf(line, "%12s %d %ld", name, &server_id, &unix_time) == 3) {
            
            //printf("%s %d %ld \n",name, server_id, unix_time);
            
            //Insert it if not already there
            temp_node= Tree_insert(name, server_id, unix_time, root);

            //printf("%s %d %ld \n",temp_node->name, temp_node->last_server, temp_node->latest_time);
        }
    }

    fclose(fp);
    //tree_print_n=0;
    //Tree_Print(root);
    return 0;
}




int main(int argc, char **argv) 
{
    int read_status=0;

    Node *root = NULL;
    Node *found = NULL;
    int search_option=1;
    char input_name[13];
    if (argc < 2) {
        printf("wrong Input Pattern");
        return 1;
    }

    //Set buffer size
    setvbuf(stdin,  NULL, _IOFBF, Buff_size * 1024 * 1024);   // input buffer
    setvbuf(stdout, NULL, _IOFBF, Buff_size * 1024 * 1024);   // output buffer


    //Read the Griefers list file. This read function also loads it into the tree
    read_status=file_read(argv[1], &root);
    if (read_status==1){return 1;}

    if(search_option==1)
   { //search for std in parameter
    while (scanf("%12s", input_name)  == 1) {
        // reset family chain and depth bef ore each search
        for (int i = 0; i < FAMILY_CHAIN_SIZE;  i++) family_chain[i] = NULL;
        depth = 0;

        found =   Tree_Search(input_name, &root, 0, 0, 0);

        if ( found == NULL)   {
            printf("%s is not currently banned from any servers.\n", input_name);
        } else {
            printf("%s was banned from %d servers. most recently on: %ld\n", found->name, found->hit, found->latest_time);
        }
    }
   }
    

    return 0;
}