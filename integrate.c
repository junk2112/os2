#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
double result = 0;//final result
const int n = 4;//number of threads
struct distance 
{
  double a,b,delta;
};
double f (double x)//function for integrate
{
  return x*2; 
}
double integrate (double , double , double );
void * integrate_part (void * d)
{
  struct distance *dist = (struct distance *)d;
  double a = dist->a, b = dist->b, delta = dist->delta;
  double x1 = a, x2 = a + delta;
  double deltaResult = 0;
  //printf("x1 is %f, x2 is %f\n",x1,x2);
  if (x2 > b)
  {
    printf("delta is so big\n");
    exit(1);
  }
  while (x2 <= b)
  {
    deltaResult += delta * f( (x2 + x1)/2 );
    x1 = x2;
    x2 += delta;
  }
  result += deltaResult;
}

double integrate (double a, double b, double delta)
{
  if (a >= b || delta <= 0)
  {
    printf("\"b\" must be more than \"a\" and delta must be more than 0\n");
    exit(1);
  }
  pthread_t ids[n];
  struct distance dist[n];
  int i;
  for (i=0; i<n; ++i)
  {
    dist[i].a = a + (b-a)*i/n;
    dist[i].b = a + (b-a)*(i+1)/n;
    dist[i].delta = delta;
    //printf("a[%d] is %f, b[%d] is %f\n", i, dist[i].a, i, dist[i].b);
  }
  for (i=0; i<n; ++i)
  {
    if(pthread_create(&ids[i], NULL, integrate_part, &dist[i]) != 0)
    {
      printf("pthread_create error\n");
      exit(1);
    }
  }
  for (i=0; i<n; ++i)
  {
    if( pthread_join(ids[i], NULL) != 0 )
    {
      printf("pthread_join error\n");
      exit(1);
    }
  }
  
}


int main (int arc, char **argv, char **envp)
{
  if (arc != 4)
  {
    printf("you should enter \"a\", \"b\" and \"delta\"\n");
    exit(1);
  }
  double a,b,delta;
  a = atof(argv[1]);
  b = atof(argv[2]);
  delta = atof(argv[3]);
  printf("\"a\" is %f\n\"b\" is %f\n\"delta\" is %f\nnumber of threads is %d\n",a,b,delta,n);
  integrate(a, b, delta);
  printf("result is %f\n", result);
  return 0;
}