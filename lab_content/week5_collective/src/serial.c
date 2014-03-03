//The code in this file isn't remotely cryptographically secure and shouldn't be used for cryptographic purposes

//INSTRUCTOR NOTE: It is nice but not required, that you give your students the following warnings
//INSTRUCTOR NOTE: 1) the results of xor encryption can be zero. Measure the string length once, at the beginning of the program, or you may get the unlucky case in which one character is encrypted as \0, leading your second measurement of the string length (strlen) to be wrong and produce a bad result
//INSTRUCTOR NOTE: 2) "^" is bitwise XOR.
#include "key.c"
#include <stdio.h>
#define RAND_SEED 2701
void encode(char* plainText, xorKey** keyList, int ptextlen, int numKeys)
{
  int keyLoop=0;
  int charLoop=0;
  for(charLoop=0;charLoop<ptextlen;charLoop++)
  {
    char cipherChar=plainText[charLoop]; 
    for(keyLoop=0;keyLoop<numKeys;keyLoop++)
    {
       cipherChar=cipherChar ^ getBit(*(keyList[keyLoop]),charLoop);
    }
    plainText[charLoop]=cipherChar;
  }
}

void decode(char* plainText, xorKey** keyList, int ptextlen, int numKeys)
{
  encode(plainText,keyList,ptextlen,numKeys); //isn't symmetric key cryptography awesome? 
}
int main(int argc, char* argv[])
{
  char ptextString[]="ALIEN INVASION DESCHUTES HALL: ABDUCT COMPUTER SCIENTISTS AND QUADRUPEDS"; //string to encode
  int ptextlen=strlen(ptextString); //length of that string
  printf("Plaintext: %s\n",ptextString);  //print the string
  srand(RAND_SEED); //seed the random number generator
  if(argc!=2)
      printf("Usage: %s <number of keys>\n",argv[0]);
  int numKeys=atoi(argv[1]); //get the number of keys to encode with
  xorKey** keyList=(xorKey**)malloc(sizeof(xorKey*)*numKeys); //form a list of keys of that size
  int keyLoop=0; //loop counter
  int charLoop=0; //loop counter
  for(keyLoop=0;keyLoop<numKeys;keyLoop++) //for each key in the list
  {
     int thisKeyLength=16+rand()%(MAXKEYLENGTH-16); //give us a random length between 16 and the longest possible key
     keyList[keyLoop]=(xorKey*)malloc(sizeof(xorKey)); //allocate space for the key
     genKey(keyList[keyLoop],thisKeyLength); //fill it with data
  }
  encode(ptextString,keyList,ptextlen,numKeys); //store encoded result back in ptextString
  printf("Encoded %s\n",ptextString);  
  decode(ptextString,keyList,ptextlen,numKeys);
  
  printf("Decoded %s\n",ptextString);  
  return 0;

}
