//S4 CSE-B
//Group members
//1.R.Vyshnavi  AM.EN.U4CSE16152
//2.N.Saranya   AM.EN.U4CSE16146
//3.M.Sowjanya	AM.EN.U4CSE16142

//Only one of us is submitting the code to avoid plagarism.So kindly ignore the no submissions for the other 2 persons in our group.


#include <stdio.h> 
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <semaphore.h>

void Wait(int sem_set_id,int semnum)
{
    struct sembuf sem_op;
    sem_op.sem_num = semnum;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
}

void signal(int sem_set_id,int semnum)
{
    struct sembuf sem_op;
    sem_op.sem_num = semnum;
    sem_op.sem_op = 1;   
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
}


void child(int semid, int shmid, int shmid1, int shmid2)
{
	Wait(semid,0);
	
	int* a=shmat(shmid,0,0);
			int i=0;
			for(i=0;a[i]!=-1;i++)
			{}
			if(a[i]==-1)
			{
				a[i]=getpid();
			}
			int f;
			printf("Enter Max Vector ");
			for(f=i+1;f<i+6;f++)
			{
				scanf("%d",&a[f]);
			}
			
			a[i+6]=-1;
			shmdt(a);
			
			int* d=shmat(shmid1,0,0);
			for(i=0;d[i]!=-1;i++)
			{}
			if(d[i]==-1)
			{
				d[i]=getpid();
			}
			printf("Enter Allocation Vector ");
			for(f=i+1;f<i+6;f++)
			{
				scanf("%d",&d[f]);
			}
			d[i+6]=-1;
			shmdt(d);
			
			
	signal(semid,7);//signal for parent
	signal(semid,0);
	Wait(semid,8);
	int *p=shmat(shmid2,0,0);
	p[0]=getpid();
	int k;
	printf("\nEnter child rqst %d ",p[0]);
	for(k=1;k<=5;k++)
	{
		int x;
		scanf("%d",&x);
		p[k]=x;
	}
	//rqst
	shmdt(p);
	sleep(1);
	signal(semid,1);//giving rqst to parent
	Wait(semid,9);
	signal(semid,8);
}

int main()
{
	int shmid,semid,shmid1,shmid2;
	shmid=shmget(IPC_PRIVATE,1000*sizeof(int),0777|IPC_CREAT);
	shmid1=shmget(IPC_PRIVATE,1000*sizeof(int),0777|IPC_CREAT);
	shmid2=shmget(IPC_PRIVATE,1000*sizeof(int),0777|IPC_CREAT);
	//Max shared memory
	int * b=shmat(shmid,0,0);
	b[0]=-1;
	shmdt(b);
	//Allocation shared memory
	int * c=shmat(shmid1,0,0);
	c[0]=-1;
	shmdt(c);
	
	semid=semget(IPC_PRIVATE,10,0777|IPC_CREAT);
	
	semctl(semid,0,SETVAL,1);//mutex
	semctl(semid,1,SETVAL,0);//2mutex
	semctl(semid,2,SETVAL,5);//R1
	semctl(semid,3,SETVAL,8);//R2
	semctl(semid,4,SETVAL,4);//R3
	semctl(semid,5,SETVAL,9);//R4
	semctl(semid,6,SETVAL,2);//R5
	semctl(semid,7,SETVAL,0);//Max child count
	semctl(semid,8,SETVAL,0);//rqst mutex
	semctl(semid,9,SETVAL,0);
	
	int pid=fork();
	if(pid==0)
	{
		int pid1=fork();
		if(pid1==0)
		{
			
			child(semid,shmid,shmid1,shmid2);
		}
		else
		{
			int pid2=fork();
			if(pid2==0)
			{
				child(semid,shmid,shmid1,shmid2);
				
			}
			else
			{
				int pid3=fork();
				if(pid3==0)
				{
					child(semid,shmid,shmid1,shmid2);
					
				}
				else
				{
					child(semid,shmid,shmid1,shmid2);
					
				}
			}
		}
	}
	else
	{
		int pid4=fork();
		if(pid4==0)
		{
			int pid5=fork();
			if(pid5==0)
			{
				child(semid,shmid,shmid1,shmid2);
				
			}
			else
			{
				int pid6=fork();
				if(pid6==0)
				{
					child(semid,shmid,shmid1,shmid2);
					
				}
				else
				{
					child(semid,shmid,shmid1,shmid2);
					
				}
			}
		}
		else
		{
			int pid7=fork();
			if(pid7==0)
			{
				int pid8=fork();
				if(pid8==0)
				{
					child(semid,shmid,shmid1,shmid2);
					
				}
				else
				{
					int pid9=fork();
					if(pid9==0)
					{
						child(semid,shmid,shmid1,shmid2);
						
					}
					else
					{
						child(semid,shmid,shmid1,shmid2);
					}
				}
			}
			else
			{
				while(semctl(semid,7,GETVAL,0)!=10);
				signal(semid,8);
				int count=10;
				while(count)
				{
					Wait(semid,7);//just for loop
					Wait(semid,0);
					Wait(semid,1);//waiting for rqst
					//bankers algorithm
					
					int *x=shmat(shmid,0,0);
					int *y=shmat(shmid1,0,0);
					int *z=shmat(shmid2,0,0);
					int t,temp,l,flag,n;
					flag=0;
					for(t=0;x[t]!=z[0];t++)
					{}
					l=t;
					temp=1;
					printf("Child %d request is ",z[0]);
					for(n=t+1;n<=t+5;n++)
					{
						printf("%d ",z[temp]);
					}
					printf("\n");
					for(l=t+1;l<=t+5;l++)
					{
						if(z[temp]>(x[l]-y[l]))
						{
							flag=1;
						}
						else if(z[temp]>semctl(semid,temp+1,GETVAL,0))
						{
							flag=1;
						}
						temp++;
					}
					if(flag==1)
						printf("%d childs Request not accepted....\n\n",z[0]);
					else
					{
						temp=1;
						printf("Available : ");
						for(n=2;n<7;n++)
						{
							int q=semctl(semid,n,GETVAL,0);
							printf("%d ",q);
						}
						printf("\n");
						printf("Allocation : ");
						for(n=t+1;n<=t+5;n++)
						{
							printf("%d ",y[n]);
						}
						printf("\n");
						printf("Need : ");
						for(n=t+1;n<=t+5;n++)
						{
							printf("%d ",x[n]-y[n]);
						}
						printf("\n");
						for(l=t+1;l<=t+5;l++)
						{
							y[l]=y[l]+z[temp];
							int q=semctl(semid,temp+1,GETVAL,0)-z[temp];
							semctl(semid,temp+1,SETVAL,q);
							temp++;
						}
						printf("%d childs request accepted.....\n",z[0]);
					}
					shmdt(x);
					shmdt(y);
					shmdt(z);
					signal(semid,9);
					count--;
					signal(semid,0);
					
				}
				int status;
				while( wait(&status)>0);
			}
		}
	}
	return 0;
}
