#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define MAX 32

struct setindex{
	char ch;
	int start;
	int length;
};
typedef struct setindex index;
struct sethash{
	char hashword[MAX];
	struct sethash*link;
};
typedef struct sethash hash;
//自定义函数
void GetWord(FILE*fp,char str[][MAX],int*line);
int LinearSearch(int*time,char str[][MAX],char word[],int n);
int BinarySearch(int*time,char str[][MAX],char word[],int n);
void CreateIndexList(index d[],char str[][MAX],int n);
int IndexSearch(int*time,char str[][MAX],char word[],int n,index d[]);
unsigned int HashCode(char *str);
void CreateHashList(hash s[],char str[][MAX],int n);
int HashSearch(int*time,char word[],int n,hash s[]);
/*main*/
int main()
{
	char word[MAX];
	char wordlist[3500][MAX];
	int type,time=0,line=0;
	int result;
	FILE*fp_dictionary=fopen("dictionary3000.txt","r");
	scanf("%s",word);
	scanf("%d",&type);
	GetWord(fp_dictionary,wordlist,&line);
	index d[26];
	CreateIndexList(d,wordlist,line);
	hash s[3500];
	CreateHashList(s,wordlist,line);
//	for(i=0;i<line;i++) printf("%s\n",s[i].hashword);
//  for(i=0;i<26;i++) printf("%c %d %d\n",d[i].ch,d[i].start,d[i].length);
//	for(i=0;i<line;i++) printf("%s\n",wordlist[i]);
    switch(type)
    {
    	case 1:
    		result=LinearSearch(&time,wordlist,word,line);
    		printf("%d %d",result,time);
    		break;
    	case 2:
    		result=BinarySearch(&time,wordlist,word,line);
    		printf("%d %d",result,time);
    		break;
    	case 3:
    		result=IndexSearch(&time,wordlist,word,line,d);
    		printf("%d %d",result,time);
            break;
        case 4:
        	result=HashSearch(&time,word,line,s);
        	printf("%d %d",result,time);
        	break;
    	default:break;
	}
	return 0;
} 

//自定义函数
void GetWord(FILE*fp,char str[][MAX],int*line)
{
	char src[MAX];
	int i=0,j=0;
	while(fgets(src,MAX,fp)!=NULL)
	{ 
	   (*line)++;
	   for(j=0;j<strlen(src);j++)
	   {
	   	    if(!isalpha(src[j]))
	   	    {
	   	    	src[j]='\0';
	   	    	break;
			}
	   }
	   strcpy(str[i],src);
	   i++;
	}	
} 
int LinearSearch(int*time,char str[][MAX],char word[],int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		(*time)++;
		if(strcmp(str[i],word)<0) continue;
		if(strcmp(str[i],word)==0) return 1;
		if(strcmp(str[i],word)>0) break;		
	}
	return 0;
}
int BinarySearch(int*time,char str[][MAX],char word[],int n)
{
	int left=0,right=n-1,mid;
	while(left<=right)
	{
		(*time)++;
		mid=(left+right)/2;
		if(strcmp(str[mid],word)>0) right=mid-1;
		else if(strcmp(str[mid],word)<0) left=mid+1;
		else if(strcmp(str[mid],word)==0) return 1;
	}
	return 0;
}
void CreateIndexList(index d[],char str[][MAX],int n)
{
	int i,j=0,sum;
	for(i=0;i<26;i++)
	{
		sum=0;
		d[i].ch='a'+i;
		d[i].start=j;
		while(j<n)
		{
			if(str[j++][0]==d[i].ch) sum++;
			else break;
		}		
		d[i].length=sum;
		j--;
	}	
}
int IndexSearch(int*time,char str[][MAX],char word[],int n,index d[])
{
	int i,left=0,right=0,mid;
	for(i=0;i<26;i++)
	{
		if(word[0]==d[i].ch)
		{
			left=d[i].start;
			right=d[i].start+d[i].length-1;
		}
	}
	while(left<=right)
	{
		(*time)++;
		mid=(left+right)/2;
		if(strcmp(str[mid],word)<0) left=mid+1;
		else if(strcmp(str[mid],word)>0) right=mid-1;
		else if(strcmp(str[mid],word)==0) return 1;
	}
	return 0;
}
unsigned int HashCode(char*str)
{
	unsigned int h=0;
	char *p;
	for(p=str;*p!='\0';p++) h=37*h+*p;
	return h%3001;
}
void CreateHashList(hash s[],char str[][MAX],int n)
{
	int i,h;
	hash*p,*q,*r;
	for(i=0;i<n;i++) 
	{
		strcpy(s[i].hashword,"\0");
		s[i].link=NULL;
	}
	for(i=0;i<n;i++)
	{
		h=HashCode(str[i]);
		if(strcmp(s[h].hashword,"\0")==0)	 strcpy(s[h].hashword,str[i]);
		else
		{
			p=(hash*)malloc(sizeof(hash));
			strcpy(p->hashword,str[i]);
			p->link=NULL;
			if(s[h].link==NULL) s[h].link=p;
			else
			{
				r=&s[h];
				for(q=s[h].link;q->link!=NULL;q=q->link)
				{
					if(strcmp(q->hashword,p->hashword)>0)
					{
						r->link=p;
						p->link=q;
						break;
					}
					else r=q;
				}
			}
		//创建有序链表	
		}
	}
}
int HashSearch(int*time,char word[],int n,hash s[])
{
	int h;
	h=HashCode(word);
	hash*p;
	(*time)++;
	if(strcmp(s[h].hashword,word)==0) return 1;
	else
	{
		for(p=s[h].link;p!=NULL;p=p->link)
	    {   
	        (*time)++;
		    if(strcmp(p->hashword,word)==0) return 1;
	    }
	}
    return 0;
}
