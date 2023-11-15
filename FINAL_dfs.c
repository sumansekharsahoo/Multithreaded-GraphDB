#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VERTICES 1000

typedef struct Stack_t
{

    char *vertices;
    char *ans;
    bool *visited;
    char **adjacencyMatrix;
    int ans_ptr;
    char *cur_vertex;
    char start_vertex;
    int num_vertices;
    int top;

} Stack;

    
void s_initializer(Stack *s, char start_vertex, int no_of_vertices, char **adj_matrix)
{

    s->num_vertices = no_of_vertices;
     
    s->start_vertex = start_vertex;

    // dynamic allocation - can be done for ans, visited
    //s->adjacencyMatrix = malloc(no_of_vertices * sizeof(char *));
    //for (int i = 0; i < no_of_vertices; i++)
     //   s->adjacencyMatrix[i] = malloc(no_of_vertices * sizeof(char));
	
    //for (int i = 0; i < no_of_vertices; i++)
    //{
     //   for (int j = 0; j < no_of_vertices; j++)
     //   {
       //     s->adjacencyMatrix[i][j] = adj_matrix[i][j];
     //   }
    //}
   
    s->adjacencyMatrix = adj_matrix;
    
    s->visited = malloc(no_of_vertices * sizeof(bool));
    
    memset(s->visited, false, no_of_vertices * sizeof(bool));
    
    s->ans = malloc(no_of_vertices * sizeof(char));
    s->top = -1;
    s->ans_ptr = 0;
	

    s->cur_vertex = malloc(sizeof(char));
    *(s->cur_vertex) = start_vertex;
    
    
}

void push(char vertex, Stack *s)
{
    // pthread_mutex_lock(&stackMutex);
    
    s->vertices[++(s->top)] = vertex;
    
    // pthread_mutex_unlock(&stackMutex);
}

char pop(Stack *s)
{
    // pthread_mutex_lock(&stackMutex);
    char vertex = '$';
    
    if (s->top >= 0)
    {
        vertex = s->vertices[(s->top)--];
    }
    
    // pthread_mutex_unlock(&stackMutex);
    return vertex;
}

void *add(void *arg)
{
    //void *ans;
    
    Stack *s = (Stack *)arg;
    
    //fflush(stdout);
    int current_vertex = (int)(*(s->cur_vertex) - '0');
    //if (s->visited[current_vertex] == false)
    //{
    
        //fflush(stdout);
    
    	
        
        
        for (int i = 0; i < s->num_vertices; i++)
        {
        	
            if (s->adjacencyMatrix[current_vertex-1][i]=='1' && !s->visited[i])
            {
            	s->visited[i] = true;
                push((char)(i + '0' + 1), s);
            }
        }
        
        
    //}
}

void *dfs(void *arg)
{
    //void *ans;
    
    Stack *s = (Stack *)arg;
    
    push(s->start_vertex, s);
    	s->visited[(int)(*(s->cur_vertex) - '0') -1] = true;
	//printf("I am here");
    while (s->top >= 0)
    {
        pthread_t dfs_add;
        char currentVertex = pop(s);
        
        *(s->cur_vertex) = currentVertex;
        s->ans[s->ans_ptr] = *(s->cur_vertex);
    	
    	(s->ans_ptr)++;
        pthread_create(&dfs_add, NULL, add, (void *)s);
        
        pthread_join(dfs_add, NULL);
    }
    
}

int main()
{

    FILE *fpointer;
    char buf[100];
    char start_vertex = '4';
    fpointer = fopen("G2.txt", "r");
    if (fpointer == NULL)
    {
        printf("Failed to open file.\n");
        return 1;
    }

    fgets(buf, sizeof(buf), fpointer);
    int no_of_vertices = (buf[0] - '0') + 0;
    //char adj_matrix[no_of_vertices][no_of_vertices];
    
    char ** adj_matrix;
    adj_matrix = malloc(no_of_vertices * sizeof(char *));
    for (int i = 0; i < no_of_vertices; i++)
        adj_matrix[i] = malloc(no_of_vertices * sizeof(char));
    for (int i = 0; i < no_of_vertices; i++)
    {
        fgets(buf, sizeof(buf), fpointer);
        for (int j = 0; j < no_of_vertices; j++)
        {
            adj_matrix[i][j] = (buf[ j]);
        }
    }
    fclose(fpointer);

    
    //Stack st1;
    Stack *stack;
    stack = (Stack *)malloc(sizeof(Stack));
    s_initializer(stack, start_vertex, no_of_vertices, adj_matrix);
	printf("current is :  \n");
    pthread_t dfs_main;
    pthread_create(&dfs_main, NULL, dfs, (void *)stack);
    pthread_join(dfs_main, NULL);
    //char *seq = (char *)stack->ans;
    stack->ans[stack->ans_ptr] = '\0';
	printf("ans  is : %s",stack->ans);
	fflush(stdout);
    return 0;
}
