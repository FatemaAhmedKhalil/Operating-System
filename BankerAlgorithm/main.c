#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//a function fed with file name and array and its job to read the
//numbers from the file and store it in the array
void readmatrix(int rows, int cols, int (*a)[cols], const char* filename)
{
    //pointer to the target file
    FILE *pf;
    pf = fopen (filename, "r");
    for(int i=0; i<rows; ++i)
    {
        for(int j=0; j<cols; ++j)
        {
            //reading single number by single number and store it
            fscanf(pf, "%d", a[i]+j);
        }
    }
    fclose (pf);
    return;
}

int main(void)
{
    char name[15];
    scanf("%s",name); //getting file name

    int matrix[11][3];
    readmatrix(11, 3, matrix, name);

    int exection_stages[5]; //to store the arrangement of execution of processes
    int index = 0; //to show the index of the next execution process
    int allocation[5][3]; //allocation matrix
    int max[5][3]; //max requests
    int available[3]; //available resources

    //split the read matrix to three matrices
    for(int i=0; i<11; ++i)
        for(int j=0; j<3; ++j)
            if (i<5)
                allocation[i][j] = matrix[i][j];
            else if (i>=5 && i<10)
                max[i%5][j] = matrix[i][j];
            else
                available[j] = matrix[i][j];

	int remain[5][3];
	for (int i=0; i<5; i++)
		for (int j=0; j<3; j++)
			remain[i][j] = max[i][j] - allocation[i][j]; //remaining resources for each process = max request - allocation

    //number of remaining active processes if it down to zero, it mean we arrived to safe state
    int p = 5  ;
    bool found;

    while(p != 0)
    {
        found = false ;
        //iterate through all process to find a process we can allocate it
        // some resources if and only if it will end the process
        for(int i=0 ; i<5; i++)
        {
            if(remain[i][0]<=available[0] && remain[i][1]<=available[1] && remain[i][2]<=available[2] && remain[i][0]!=-1)
            {
                found = true;
                // after ending the process we return the allocated resources to available resources
                available[0]+= allocation[i][0];
                available[1]+= allocation[i][1];
                available[2]+= allocation[i][2];

                // we set the remaining resources for ended process -ve value
                //to easily discover that the process is served before
                remain[i][0] = -1;
                remain[i][1] = -1;
                remain[i][2] = -1;
                p--;
                exection_stages[index] = i;
                index++;
            }
        }

            if(!found)
            {
                printf("Unsafe State\n");
                return 0;
            }
    }

    if(found)
        printf("Safe State\n");


    for(int i=0; i<4; i++)
        printf("%d ==> ", exection_stages[i]);

    printf("%d ", exection_stages[4]);
       return 0;
}
