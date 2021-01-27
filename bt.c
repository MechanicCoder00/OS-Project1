//#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>

/*
 * Project : Operating systems assignment 1
 * Author : Scott Tabaka
 * Date Due : 2/10/2020
 * Course : CMPSCI 4760
 * Purpose : Print out various information for all files and directories within a directory using unix.
 */

int hflag=0;
int Lflag=0;
int tflag=0;
int pflag=0;
int iflag=0;
int uflag=0;
int gflag=0;
int sflag=0;
int dflag=0;
int opt;
char command[100];
static char error[100];
static char usage[100];

struct Queue                        //queue struct
{
    int front, rear, size;
    unsigned capacity;
    char** array;
};

struct Queue* createQueue(unsigned capacity)        //function to create queue
{
    struct Queue* queue = malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = malloc(queue->capacity * sizeof(char*));
    return queue;
}

int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}

int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

void enqueue(struct Queue* queue, char* item)   //adds an item to the back of the queue
{
    if (isFull(queue))
    {
        return;
    }
    queue->rear = (queue->rear + 1)%queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}
char* dequeue(struct Queue* queue)          //removes an item from the front of the queue
{
    if (isEmpty(queue))
    {
        return "";
    }
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return "";
}

char* front(struct Queue* queue)            //returns the char array stored on the front of the array
{
    if (isEmpty(queue))
    {
        return "";
    }
    return queue->array[queue->front];
}

char* getCmdOutput(const char *cmd)          //Will return the output of a unix system command as a char[]
{
    FILE *fp = popen(cmd, "r");
    static char output[40];
    if (fp != NULL)
    {
        if (fgets(output, sizeof(output), fp))
        {
            output[strcspn(output, "\n\r")] = 0;        //strips the newline off of a string
        }
        pclose(fp);
    }
    return output;
}

void breadthFirst(struct Queue *queue, char *path)     //Function to capture all files and dir paths and enter them into the queue
{
    static char cmd[40];
    strcpy(cmd,"ls -1");
    strcat(cmd," ");
    strcat(cmd,path);

    FILE *fp = popen(cmd, "r");             //Takes a unix command and returns the output into a file pointer
    static char temp[100];

    if (fp != NULL)
    {
        while (fgets(temp, sizeof(temp), fp))       //traverses the output line by line and stores it in the variable "temp"
        {
            char *fileName = malloc(100);
            temp[strcspn(temp, "\n\r")] = 0;

            if(strcmp(path, "") != 0)          //If path is not empty copy path into "fileName"
            {
                strcpy(fileName,path);
                strcat(fileName, "/");
            }
            strcat(fileName, temp);             //Append path to a file/directory onto the current path directory
            enqueue(queue, fileName);           //Adds "fileName into queue
        }
        pclose(fp);
    }
}

int isDirectory(char *path)        //Checks if argument is a directory(1) or not(0)
{
    static char temp5[100];
    strcpy(temp5, "stat --format=%A");
    strcat(temp5, " ");
    strcat(temp5, path);
    strcat(temp5, " | awk '{print substr($0,1,1)}'");

    if(strcmp("d", getCmdOutput(temp5))==0)         //Checks if dir
    {
        return 1;
    } else
    {
        return 0;
    }
}

void printInfo(char *path)      //This function prints info for each path argument given depending on options
{
    char type[20];                  //char arrays for storing each option info output
    char perm[20];
    char link[20];
    char uid[20];
    char gid[20];
    char size[20];
    char time[20];

    if (hflag == 1)         //This prints info for the help option
    {
        printf("%s\n", usage);
        printf("Prints all file and directories in breadth order with selected options.\n\n");
        printf("-h Print a help message and exit.\n");
        printf("-L Follow symbolic links\n");
        printf("-t Print information on ﬁle type\n");
        printf("-p Print permission bits(r-read w-write x-execute)\n");
        printf("-i Print the number of links to ﬁle in inode table\n");
        printf("-u Print the UID associated with the ﬁle\n");
        printf("-g Print the GID associated with the ﬁle\n");
        printf("-s Print the size of ﬁle in bytes\n");
        printf("-d Show the time of last modiﬁcation\n");
        printf("-l Print information on the ﬁle as if all of the options are speciﬁed\n");

        exit(0);
    }

    if (Lflag == 1)                 //Follow links(Not functional)
    {
//        fprintf(stderr, "Lflag!\n");
//        system("readlink -f bt.c");
    }

    if (tflag == 1)                 //File type
    {
        strcpy(command, "stat --format=%A");
        strcat(command, " ");
        strcat(command, path);
        strcat(command, " | awk '{print substr($0,1,1)}'");
        strcpy(type, getCmdOutput(command));

        printf("%-1s", type);

        if (pflag == 0)             //Adds a space behind the file type output if permissions does not exist
        {
            printf(" ");
        }
    }
    if (pflag == 1)                 //Permissions
    {
        strcpy(command, "stat --format=%A");
        strcat(command, " ");
        strcat(command, path);
        strcat(command, " | awk '{print substr($0,2,10)}'");
        strcpy(perm, getCmdOutput(command));

        printf("%-9s ", perm);
    }
    if (iflag == 1)                 //Inode links to a file
    {
        strcpy(command, "stat --format=%h");
        strcat(command, " ");
        strcat(command, path);
        strcpy(link, getCmdOutput(command));

        printf("%-3s ", link);
    }
    if (uflag == 1)                 //User ID name
    {
        strcpy(command, "stat --format=%U");
        strcat(command, " ");
        strcat(command, path);
        strcpy(uid, getCmdOutput(command));

        printf("%-9s ", uid);
    }
    if (gflag == 1)                 //Group ID name
    {
        strcpy(command, "stat --format=%G");
        strcat(command, " ");
        strcat(command, path);
        strcpy(gid, getCmdOutput(command));

        printf("%-9s ", gid);
    }
    if (sflag == 1)                 //File size
    {
        strcpy(command, "ls -ldh");
        strcat(command, " ");
        strcat(command, path);
        strcat(command, " | awk '{ print $5}'");
        strcpy(size, getCmdOutput(command));

        printf("%-5s ", size);
    }
    if (dflag == 1)                 //Last modification time
    {
        strcpy(command, "date +\"%b %d, %Y\" -r");
        strcat(command, " ");
        strcat(command, path);
        strcpy(time, getCmdOutput(command));

        printf("%-12s ", time);
    }

    printf("%-s\n", path);         //Prints the path and name for the current file/directory
}


int main (int argc, char *argv[])
{
    int index;
    strcpy(error,argv[0]);
    strcat(error,": Error");

    strcpy(usage,"Usage: ");
    strcat(usage,argv[0]);
    strcat(usage," [-h] [-L -t -p -i -u -g -s -d | -l] [dirname]");

    while ((opt = getopt(argc, argv, "hLtpiugsdl")) != -1)		//loop for option selecting
    {
        switch (opt)
        {
            case 'h':
                hflag = 1;
                break;
            case 'L':
                Lflag = 1;
                break;
            case 't':
                tflag = 1;
                break;
            case 'p':
                pflag = 1;
                break;
            case 'i':
                iflag = 1;
                break;
            case 'u':
                uflag = 1;
                break;
            case 'g':
                gflag = 1;
                break;
            case 's':
                sflag = 1;
                break;
            case 'd':
                dflag = 1;
                break;
            case 'l':
                Lflag = dflag = gflag = iflag = pflag = sflag = tflag = uflag = 1;
                break;
            default:
                fprintf(stderr, "%s\n", usage);
                exit(EXIT_FAILURE);
        }
    }

    if(argc-optind > 1)             //Checks to make sure there is only 1 argument, if any, after options
    {
        printf("%s: Too many arguments\n",error);
        fprintf(stderr, "%s\n", usage);
        exit(EXIT_FAILURE);
    }

    struct Queue* queue = createQueue(1000);        //Creates queue
    char path[200];
    strcpy(path, "");
    char *temp2 = malloc(strlen(path));

    for (index = optind; index < argc; index++)
    {
        int result;
        result = access(argv[index], F_OK);         //Checks if a valid file or directory

        if ( result == 0 )
        {
            strcpy(path,argv[index]);
        }
        else
        {
            perror(error);                          //perror message if file/directory not valid
            exit(EXIT_FAILURE);
        }
    }

    strcpy(temp2,path);
    enqueue(queue, temp2);

    while(queue->size > 0)
    {
        strcpy(path,front(queue));                 //reading from queue and stores into "path"

        if(strcmp(path, "") != 0)                       //If path is not empty
        {
            if(isDirectory(path) == 1)                  //And if path is a directory
            {
                breadthFirst(queue, path);              //Call breadthfirst function
            }
        }else
        {
            breadthFirst(queue, path);                  //Call breadthfirst function if path is empty
        }

        if(strcmp(path, "") != 0)                       //If path is not empty print info
        {
            printInfo(path);
        }

        dequeue(queue);                                 //Call dequeue
    }

    free(queue->array);                         //free allocated memory
    free(queue);
    free(temp2);

    return 0;
}
