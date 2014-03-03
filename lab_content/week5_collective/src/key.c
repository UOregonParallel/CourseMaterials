#ifndef WINTER2014PARALLELWK5LABKEY
#define WINTER2014PARALLELWK5LABKEY
//INSTRUCTOR NOTE: very simple  for a key. Plenty of bogus metadata and bad design for the key to motivate data reorg
#define MAXKEYLENGTH 8192
#define cursysCharLen (2<<(sizeof(char)*8))
#ifndef true
#include <stdlib.h>
#include <stdio.h>
#define true 1
#define false 0
#endif
//a key for xor'ing
typedef struct 
{
   //length of the key
   int myKeyLength;
   //description, could be any necessary metadata. Not necessary in this assignment
   char myKeyDescriptor[128]; //INSTRUCTOR NOTE: BOGUS, intended to beat up cache in accessing myKeyLength;
   //The actual key
   char* myKey;
} xorKey;
//return bit from key at position "position"
char getBit(xorKey skey, int position)
{
   return skey.myKey[position%skey.myKeyLength];
} 
//utility function to generate a key of length "length"
void genKey( xorKey *pkey,int length)
{
  pkey->myKeyLength=length;
  pkey->myKey=(char*)malloc(sizeof(char)*length);
  int i=0;
  for(i=0;i<length;i++)
  {
     pkey->myKey[i]=(char)(rand()%cursysCharLen);
    // printf("%d\n",tkey->myKey[i]);
  } 
     
}
#endif
