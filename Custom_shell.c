
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>



void RunCommand(char *commands[], int in_fd_, int out_fd_, int err_fd_) {
    pid_t pid = fork();
    if (pid == 0)
    {

        if (in_fd_ != 0)
        {
            dup2(in_fd_, 0);
            close(in_fd_);
        }
        if (out_fd_ != 1)
        {
            dup2(out_fd_, 1);
            close(out_fd_);
        }
   if (err_fd_ != 2)
        {
            dup2(err_fd_, 2);
            close(err_fd_);
        }
        execvp(commands[0], commands);

    }
    else if (pid > 0)
    {

        wait(NULL);
    }
  else
    {
      printf("fork error");
    }
}

int main( int argc , char * argv[])
{

    char* commands[50][50];


    char comm[50];
    bool flag=true;
    while (flag)
    {

    printf("Enter a command : \n ");
        int count = read(0, &comm, sizeof(comm));
        comm[count - 1] = '\0';

        char* exit_ = "exit";
        char* EXIT = "EXIT";

         if (strcmp(comm, exit_) == 0 || strcmp(comm, EXIT) == 0)
        {
            break;
        }

        char *comm_= strtok(comm, " \n");

        int i = 0;
        int j = 0;
        int in_fd_ = 0;
        int out_fd_ = 1;
        int err_fd_=2;

        while (comm_ != NULL) {
            if (strcmp(comm_, "|") == 0)
            {

                commands[i][j] = NULL;
                int pipe_fd_[2];
                pipe(pipe_fd_);
                RunCommand(commands[i], input_fd_, pipe_fd_[1],err_fd_);
                close(pipe_fd_[1]);
                j = 0;
                i++;
                in_fd_ = pipe_fd_[0];
                out_fd_ = 1;
            }
            else if (strcmp(comm_, "<") == 0)
            {

                comm_ = strtok(NULL, " \n");
                in_fd_ = open(comm_, O_RDONLY);

                if (in_fd_ < 0)
                {
                    printf("CANNOT OPEN THE FILE FOR READ \n");

                }
            }
            else if (strcmp(comm_, ">") == 0)
            {

                comm_= strtok(NULL, " \n");
                out_fd_ = open(comm_, O_WRONLY);

                if (out_fd_ < 0)
                {
                    printf("CANNOT OPEN THE FILE FOR READ \n");
                }
            }
            else if (strcmp(comm_, "2>") == 0)
{
    comm_ = strtok(NULL, " \n");
    err_fd_ = open(comm_, O_WRONLY);
    if (err_fd_ < 0)
    {
        printf("CANNOT OPEN THE FILE FOR WRITE \n");
    }
}
else if (strcmp(comm_, "mkfifo") == 0)
            {

               comm_ = strtok(NULL, " \n");
                int ret_status = mkfifo(comm_, 0666);
                if (ret_status!= 0)
                {
                    printf("error in creating fifo \n");
                }
                else
                {
                printf(" fifo created : %s \n ", comm_);
                }

            }
            else
            {

                commands[i][j] = comm_;
                j++;
            }

            comm_= strtok(NULL, " \n");
        }

        //last command left
        *(*commands+i)+j) = NULL;
        RunCommand(commands[i], inp_fd_, out_fd_, err_fd_);
    }


    return 0;
}
