#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h> 
#include <string.h>

int n = 9;
int main (int arc, char ** argv, char **envp) 
{
  char temp[16];
  int fd;
  if ( (fd = open("./test.txt", O_RDWR|O_CREAT)) == -1)
  {
    printf("1open error\n");
    return 1;  
  }
  //read (fd, temp, sizeof(temp));
  //int x = atoi(temp);
  //lseek(fd, 0, SEEK_SET);
  write(fd, "0\0", 2);
  close(fd);
  
  sem_t * sr;
  if ( (sr = sem_open ("/semName", O_RDWR|O_CREAT)) == SEM_FAILED)
  {
   printf("sem_open error\n");
   return 1;
  }

  int pid;
  if ( (pid = fork()) == -1)
  {
    printf("fork error\n");
    return 1;
  }
  else 
    if (pid == 0)//child
    {
      char temp[16];
      int fd,i;
      for (i=0; i<n; ++i)
      {
	int value;
	sem_getvalue(sr, &value);
	printf("child before: sem value is %d\n",value);
	
	sem_wait(sr);
	
	sem_getvalue(sr, &value);
	printf("child after: sem value is %d\n",value);
	
	if ( (fd = open("./test.txt", O_RDWR|O_CREAT)) == -1)
	{
	  printf("open error\n");
	  return 1;
	}
	read(fd, &temp, 16);
	int number = atoi(temp);
	number++;
	lseek(fd, 0, SEEK_SET);
	sprintf(temp, "%d", number);
	printf("child`s temp is %s\n", temp);
	write(fd, &temp, strlen(temp));
	write(fd, "\0", 1);
	close(fd);
	sem_post(sr);
      }
    } 
    else
    {
      char temp[16];
      int fd,i;
      for (i=0; i<n; ++i)
      {
	int value;
	sem_getvalue(sr, &value);
	printf("parent before: sem value is %d\n",value);
	
	sem_wait(sr);
	
	sem_getvalue(sr, &value);
	printf("parent after: sem value is %d\n",value);
	
	if ( (fd = open("./test.txt", O_RDWR|O_CREAT)) == -1)
	{
	  printf("open error\n");
	  return 1;
	}
	read(fd, &temp, 16);
	int number = atoi(temp);
	number++;
	lseek(fd, 0, SEEK_SET);
	sprintf(temp, "%d", number);
	printf("parent`s temp is %s\n", temp);
	write(fd, &temp, strlen(temp));
	write(fd, "\0", 1);
	close(fd);
	sem_post(sr);
      } 
    }
  
  return 0;
}