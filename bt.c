#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <limits.h>

/*

 Working: line 148 only getting last line and not any others

Todo:
    Start git for this project
 1. Get unix commands for each option and print within a loop as long as there is a path in the queue
 2. Build a queue for storing file paths
 3.
 4. Redo error messages(using perror)
 5. comment everything

 Notes:
 1. Breadthfirst function-fill queue/call printOutput/dequeue
 2. printOutput-output of current path as per options selected into buffer and prints
 3.






 */


//breadthfirst(root)
//{
//    enqueue(root);
//    while (!empty(queue))
//    {
//        next = dequeue(queue);
//        for each node directly below next
//        {
//            visit(node);
//            if (isa_directory(node))enqueue(node);
//        }
//    }
//}

//*****************************

struct Queue
{
    int front, rear, size;
    unsigned capacity;
//    int* array;
    char** array;
};

struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
//    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    queue->array = (char**) malloc(queue->capacity * sizeof(char**));
    return queue;
}
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
//void enqueue(struct Queue* queue, int item)
void enqueue(struct Queue* queue, char* item)
{
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)%queue->capacity;
//    queue->array[queue->rear] = item;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
//    printf("%s enqueued to queue\n", item);
}
char* dequeue(struct Queue* queue)
{
    if (isEmpty(queue))
//        return INT_MIN;
        return 0;
//    int item = queue->array[queue->front];
    static char item[10];
    queue->front = (queue->front + 1)%queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
char* front(struct Queue* queue)
{
    if (isEmpty(queue))
//        return INT_MIN;
        return 0;
    return queue->array[queue->front];
}

// Function to get rear of queue
char* rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return 0;
    return queue->array[queue->rear];
}

//*****************************

char* addToBuffer(const char* cmd)
{
    FILE *fp = popen(cmd, "r");
    static char output[40];
    if (fp != NULL)
    {

        if (fgets(output, sizeof(output), fp))
        {
            output[strcspn(output, "\n\r")] = 0;
            //strcat(buffer,output);
        }
        pclose(fp);
    }
    return output;
}

void queueAllFD(struct Queue* queue)
{
    FILE *fp = popen("ls -1", "r");
    char fileName[200];
//    char *cp = malloc(1000);

    if (fp != NULL)
    {
//        static char fileName[60];
        while (fgets(fileName, sizeof(fileName), fp))
        {

//            enqueue(queue, "blah");
//            printf("Front item is %s", front(queue));

            printf("fileName %s", fileName);
            fileName[strcspn(fileName, "\n\r")] = 0;
//            strcat(fileName,"\0");


//            char temp[60];
//            strcpy(temp, fileName);
//            enqueue(queue, temp);

            enqueue(queue, fileName);
            printf("Front item is %s\n\n", front(queue));

//            strcpy(enqueue(queue,cp ),fileName);
        }
        pclose(fp);
    }
}

int main (int argc, char *argv[])
{
    int hflag=0;
    int Lflag=0;
    int tflag=0;
    int pflag=0;
    int iflag=0;
    int Iflag=0;
    int uflag=0;
    int gflag=0;
    int sflag=0;
    int dflag=0;
    int index;
    int opt;
    int iValue = 0;
    static char buffer[1000];
    char command[100];
    char path[300];
    strcpy(path, "./bt.c");

//    char *fileName[60];

    while ((opt = getopt(argc, argv, "hLtpiI:ugsdl")) != -1)		//loop for option selecting
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
            case 'I':
                Iflag = 1;
                iValue = atoi(optarg);
//                iValue = strtol(optarg);
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
            case '?':
                if (optopt == 'I')
                {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                }
                else if (isprint (optopt))
                {
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                }
                else
                {
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                }

            default:
                fprintf(stderr, "Usage: %s [-h] [-L -d -g -i -p -s -t -u -In | -l] [dirname]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    struct Queue* queue = createQueue(1000);
    queueAllFD(queue);

//    for (index = optind; index < argc; index++)
//    {
//        printf ("Non-option argument %s\n", argv[index]);
//    }

    if(hflag == 1)
    {
        fprintf(stderr, "hflag!\n");
        exit(0);
    }


    if (Lflag == 1)                 //Follow links
    {
//        fprintf(stderr, "Lflag!\n");
//        system("readlink -f bt.c");
    }
    if (Iflag == 1)                 //Number of spaces for indenting
    {
        fprintf(stderr, "Iflag!\n");
        fprintf(stdout, "Ivalue: %d", iValue);
    }

    if (tflag == 1)                 //File type
    {
        strcpy(command, "stat --format=%A");
        strcat(command," ");
        strcat(command,path);
        strcat(command," | awk '{print substr($0,1,1)}'");
        strcat(buffer,addToBuffer(command));

        if(pflag == 0)
        {
            strcat(buffer," ");
        }
    }
    if (pflag == 1)                 //Permissions
    {
        strcpy(command, "stat --format=%A");
        strcat(command," ");
        strcat(command,path);
        strcat(command," | awk '{print substr($0,2,10)}'");
        strcat(buffer,addToBuffer(command));
        strcat(buffer," ");
    }
    if (iflag == 1)                 //Inode links to a file
    {
        strcpy(command, "stat --format=%h");
        strcat(command," ");
        strcat(command,path);
        strcat(buffer,addToBuffer(command));
        strcat(buffer," ");
    }
    if (uflag == 1)                 //User ID name
    {
        strcpy(command, "stat --format=%U");
        strcat(command," ");
        strcat(command,path);
        strcat(buffer,addToBuffer(command));
        strcat(buffer," ");
    }
    if (gflag == 1)                 //Group ID name
    {
        strcpy(command, "stat --format=%G");
        strcat(command," ");
        strcat(command,path);
        strcat(buffer,addToBuffer(command));
        strcat(buffer," ");
    }
    if (sflag == 1)                 //File size
    {
        strcpy(command, "ls -lah");
        strcat(command," ");
        strcat(command,path);
        strcat(command," | awk '{ print $5}'");
        strcat(buffer,addToBuffer(command));
        strcat(buffer," ");
    }
    if (dflag == 1)                 //Last modification time
    {
        strcpy(command, "date +\"%b %d, %Y\" -r");
        strcat(command," ");
        strcat(command,path);
        strcat(buffer,addToBuffer(command));
        strcat(buffer," ");
    }

    strcat(buffer,"\n");
    printf("%s", buffer);





//    enqueue(queue, "one");
//    enqueue(queue, "two");
//    enqueue(queue, "three");
//    enqueue(queue, "four");
//    dequeue(queue);
//    printf("Front item is %s\n", front(queue));
//    printf("Rear item is %s\n", rear(queue));
//    printf("Queue size: %d\n",queue->size);

    while(queue->size > 0)
    {
        printf("Front item is %s\n", front(queue));
        dequeue(queue);
        printf("Queue size: %d\n",queue->size);
    }

    printf("Rear item is %s\n", rear(queue));

    return 0;
}

