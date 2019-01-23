#include <stdbool.h>
#include<stdio.h>
#include<cs50.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
bool check(const char *word);
typedef struct node
{
    bool is_word;
    struct node *children[27];
}
node;
int main(void)
{
node *root=malloc(sizeof(node));
string s="Shashwat'ssla'";
node *temp=root;
for(int i=0,n=strlen(s);i<n;i++)
{
    if(s[i]=='\'')
    {
        if(temp->children[0]==NULL)
        {
            node *newnode=malloc(sizeof(node));
            temp->children[0]=newnode;
            temp=temp->children[0];
            if(i==n-1)
            {
                temp->is_word=true;
            }
        }
        else
        {
            temp=temp->children[0];
            if(i==n-1)
            {
                temp->is_word=true;
            }
        }
        continue;
    }
    int x;
    int ch;
    ch=toupper(s[i]);
    x=ch-64;
    printf("%i\n",x);
    if(temp->children[x]==NULL)
    {
        node *newnode=malloc(sizeof(node));
        temp->children[x]=newnode;
        temp=temp->children[x];
        if(i==n-1)
        {
            temp->is_word=true;
        }
    }
    else
    {
        temp=temp->children[x];
        if(i==n-1)
        {
            temp->is_word=true;
        }
    }

}



temp=root;
s="ShashWAT'SSla";
for(int i=0,n=strlen(s);i<n;i++)
{
    if(s[i]=='\'')
    {
        if(temp->children[0]==NULL)
        {

            printf("%s doesn't exist in dictionary",s);
            break;
        }
        else
        {
            temp=temp->children[0];
            if(i==n-1 && temp->is_word==true)
            {
                printf("%s exists in dictionary",s);
            }
            if(i==n-1 && temp->is_word==false)
            {
                printf("%s doesn't exist in dictionary",s);
            }
        }
        continue;
    }
    int x;
    int ch;
    ch=toupper(s[i]);
    x=ch-64;
    printf("%i\n",x);
    if(temp->children[x]==NULL)
    {
     printf("%s doesn't exist in dictionary",s);
     break;
    }
    else
    {
        temp=temp->children[x];
        if(i==n-1 && temp->is_word==true)
        {
            printf("%s exists in dictionary",s);
        }
        if(i==n-1 && temp->is_word==false)
        {
            printf("%s doesn't exist in dictionary",s);
        }
    }

}



return 0;

}
bool check(const char *word)
{
    // TODO
    return false;
}