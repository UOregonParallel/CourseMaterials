#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <omp.h>

#include <openssl/md5.h>

//const char* chars="abcdefghijklmnopqrstuvwxyz0123456789";
const char* chars="0123456789";

// tests if a hash matches a candidate password
int test(const char* passhash, const char* passcandidate) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    
    MD5((unsigned char*)passcandidate, strlen(passcandidate), digest);
    
    char mdString[34];
    mdString[33]='\0';
    for(int i=0; i<16; i++) {
        sprintf(&mdString[i*2], "%02x", (unsigned char)digest[i]);
    }
    return strncmp(passhash, mdString, strlen(passhash));
}

// maps a password number to string
void genpass(long passnum, char* passbuff) {
    passbuff[8]='\0';
    int charidx;
    int symcount=strlen(chars);
    for(int i=7; i>=0; i--) {
        charidx=passnum%symcount;
        passnum=passnum/symcount;
        passbuff[i]=chars[charidx];
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <password hash>\n",argv[0]);
        return 1;
    }
    char passmatch[9];
    char passcandidate[9];
    long basepass=0;
    long currpass;
    int testresult;
    int notfound=1;
    omp_set_num_threads(6);
    #pragma omp parallel private(currpass, testresult, passcandidate)
    {
    while(notfound) {
        currpass = basepass+omp_get_thread_num();
        genpass(currpass,passcandidate);
        testresult=test(argv[1], passcandidate);
            
        if(testresult==0) {
            notfound=0;
            strncpy(passmatch, passcandidate, 9);
            printf("Found a match... should halt\n");
        }
            
        if(omp_get_thread_num()==0) {
            basepass += omp_get_num_threads();
        }
        #pragma omp barrier
    }
    }
    printf("found: %s\n",passmatch);
    return 0;
}
