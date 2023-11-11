#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool visitedall(bool arr[], int len)
{
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == false)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    FILE *fpointer;
    char buf[100];

    int start_vertex = 4;

    fpointer = fopen("G2.txt", "r");

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

    // for (int i = 0; i < no_of_vertices; i++)
    // {
    //     for (int j = 0; j < no_of_vertices; j++)
    //     {
    //         printf("%d", adj_matrix[i][j]);
    //         printf(" ");
    //     }
    //     printf("\n");
    // }

    bool visited[no_of_vertices];
    for (int i = 0; i < no_of_vertices; i++)
    {
        visited[i] = false;
    }

    int queue[no_of_vertices];
    queue[0] = start_vertex - 1;
    int front = 0;
    int back = 0;

    int ans[no_of_vertices];
    visited[start_vertex - 1] = true;

    while (front != no_of_vertices)
    {
        int node = queue[front];
        ans[front] = node + 1;
        for (int i = 0; i < no_of_vertices; i++)
        {
            if (adj_matrix[node][i] == 1 && !visited[i])
            {
                back++;
                queue[back] = i;
                visited[i] = true;
            }
        }
        front++;
    }

    for (int i = 0; i < no_of_vertices; i++)
    {
        printf("%d ", ans[i]);
    }

    return 0;
}