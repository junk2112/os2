#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>

const int shm_size = 64;
const int n = 10;

int main(int argc, char** argv)
{
	int i;
	sem_t *sem1, *sem2;
	int pid;
	if ( (pid = fork()) == -1 )
	{
		printf("fork error\n");
		return 1;
	}
	if ( pid != 0) //parent
	{
		if((sem1 = sem_open("/_SEM1_", O_CREAT, 0666, 0)) == SEM_FAILED)
		{
			printf("semaphore error\n");
			sem_unlink("/_SEM1_");
			return 1;
		}
		if((sem2 = sem_open("/_SEM2_", O_CREAT, 0666, 0)) == SEM_FAILED)
		{
			printf("semaphore error\n");
			sem_unlink("/_SEM2_");
			sem_unlink("/_SEM2_");
			return 1;
		}
		sem_wait(sem2);
		int fd = shm_open("/_MEMORY_", O_CREAT | O_RDWR, 0666);
		ftruncate(fd, shm_size);
		char *p = (char*)mmap(NULL, shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
		for(i=0; i<n; ++i)
		{
			sem_post(sem1);
			char buffer[30];
			sprintf(buffer,"message %d",i);
			strcpy(p, buffer);
			printf("Parent: write \"%s\"\n", p);
			sem_wait(sem2);
		}
		sem_close(sem1);
		sem_close(sem2);
		munmap(p, shm_size);
		close(fd);
		sem_unlink("/_SEM1_");
		sem_unlink("/_SEM2_");
		shm_unlink("/_MEMORY_");
	}
	else if (pid == 0)//child
	{
		if((sem1 = sem_open("/_SEM1_", O_CREAT, 0666, 0)) == SEM_FAILED)
		{
			printf("semaphore error\n");
			sem_unlink("/_SEM1_");
			return 1;
		}
		if((sem2 = sem_open("/_SEM2_", O_CREAT, 0666, 0)) == SEM_FAILED)
		{
			printf("semaphore error\n");
			sem_unlink("/_SEM2_");
			sem_unlink("/_SEM2_");
			return 1;
		}
		sem_post(sem2);
		sem_wait(sem1);
		int fd = shm_open("/_MEMORY_", O_RDWR, 0666);
		char *p = (char *)mmap(NULL, shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
		for(i=0; i<n; ++i)
		{
			sem_post(sem2);
			char *s = (char*)malloc(sizeof(char)*shm_size);
			strcpy(s, p);
			printf("Child:   read \"%s\"\n", s);
			free(s);
			sem_wait(sem1);
		}
		sem_post(sem2);
		munmap(p, shm_size);
		close(fd);
	}
	return 0;
}
