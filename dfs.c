#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// bool visitedall(bool arr[], int len)
// {
//     for (int i = 0; i < len; i++)
//     {
//         if (arr[i] == false)
//         {
//             return false;
//         }
//     }
//     return true;
// }

int main()
{
    // just file reading. Same as BFS

    FILE *fpointer;
    char buf[1000];

    int start_vertex = 2;

    fpointer = fopen("G4.txt", "r");

    if (fpointer == NULL)
    {
        printf("Failed to open file.\n");
        return 1;
    }

    fgets(buf, sizeof(buf), fpointer);
    // printf("%c", buf[0]);
    int no_of_vertices = (buf[0] - '0') + 0;

    int adj_matrix[no_of_vertices][no_of_vertices];

    for (int i = 0; i < no_of_vertices; i++)
    {
        fgets(buf, sizeof(buf), fpointer);
        for (int j = 0; j < no_of_vertices; j++)
        {
            adj_matrix[i][j] = ((buf[2 * j] - '0') + 0);
        }
    }
    fclose(fpointer);

    // Actual dfs starts here

    bool visited[no_of_vertices];
    for (int i = 0; i < no_of_vertices; i++)
    {
        visited[i] = false;
    }

    int stack[10000];
    stack[0] = start_vertex - 1;
    int ans[no_of_vertices];

    int top = 1;
    int counter = 0;

    while (top != 0)
    {
        if (visited[stack[top - 1]] == true)
        {
            top--;
        }
        else
        {
            int node = stack[top - 1];
            // printf("%d\n", node);
            top--;
            ans[counter] = node + 1;
            counter++;
            visited[node] = true;
            for (int i = 0; i < no_of_vertices; i++)
            {
                if (adj_matrix[node][i] == 1 && !visited[i])
                {
                    stack[top] = i;
                    top++;
                }
            }
        }
    }
    for (int i = 0; i < no_of_vertices; i++)
    {
        printf("%d ", ans[i]);
    }

    return 0;
}