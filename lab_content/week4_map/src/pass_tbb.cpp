#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <openssl/md5.h>

#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/partitioner.h"

#define PASSLENGTH 8
#define TOTALCOMBO 100000000
//const char* chars="abcdefghijklmnopqrstuvwxyz0123456789";
const char* chars="0123456789";

using namespace std;
using namespace tbb;


class tbb_hashCrack
{	
	char *hashedPW;
    char *passmatch;
    
	
	public:
		tbb_hashCrack(char *hashedPW, char *passmatch) : hashedPW(hashedPW), passmatch(passmatch) { }
	
		
	void operator() (const blocked_range<size_t> &r) const 
	{	
		char passcandidate[(PASSLENGTH+1)];
		int testresult;
		int notfound=1; 
		
		for (size_t i = r.begin(); notfound==1 && i < r.end(); i++)
		{
			
			if(notfound) 
			{				
				//----genpass
				long passnum = i;
				passcandidate[PASSLENGTH]='\0';
				int charidx;
				int symcount=strlen(chars);
				for(int j=(PASSLENGTH-1); j>=0; j--) {
					charidx=passnum%symcount;
					passnum=passnum/symcount;
					passcandidate[j]=chars[charidx];
				}
				//---- END genpass
			

				//----test method
				unsigned char digest[MD5_DIGEST_LENGTH];
				MD5((unsigned char*)passcandidate, strlen(passcandidate), digest);
		
				char mdString[34];
				mdString[33]='\0';
				for(int i=0; i<16; i++) {
					sprintf(&mdString[i*2], "%02x", (unsigned char)digest[i]);
				}
				testresult = strncmp(hashedPW, mdString, strlen(hashedPW));
				//---- END test method
		
				if(testresult==0) {
				    notfound=0;
				    strncpy(passmatch, passcandidate, (PASSLENGTH+1));
				    printf("Found a match... should halt\n");
				}
			}
		}
	}
};


int main(int argc, char** argv) 
{
    if(argc != 2) {
        printf("Usage: %s <password hash>\n",argv[0]);
        return 1;
    }
    
    task_scheduler_init init(32);	
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
    
    
    int n = 0;
    char passmatch[PASSLENGTH]={'\0'};
    
    tbb_hashCrack hc(argv[1], passmatch);
	blocked_range<size_t> range(0, TOTALCOMBO);
	parallel_for(range, hc);
    
    printf("found: %s\n",passmatch);
    return 0;
}

