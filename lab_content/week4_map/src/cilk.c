#include <stdio.h>
#include <string.h>
#include <cilk/cilk.h>
#include <sys/time.h>
#include <math.h>
#include <openssl/md5.h>
#ifndef MD5_DIGEST_LENGTH
#define MD5_DIGEST_LENGTH 16
#endif
#define PASSLENGTH 8
#define TOTALCOMBO 100000000 //TODO: PIN THIS TO 10^PASSLENGTH
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
    passbuff[PASSLENGTH]='\0';
    int charidx;
    int symcount=strlen(chars);
    for(int i=(PASSLENGTH-1); i>=0; i--) {
        charidx=passnum%symcount;
        passnum=passnum/symcount;
        passbuff[i]=chars[charidx];
    }
}

void crackHash(char* hash,char* passmatch) {
    char passcandidate[(PASSLENGTH+1)];
    long basepass=0;
    long currpass;
    int testresult;
    int notfound=1;
    #define NPROCS 2000
    #pragma cilk grainsize=TOTALCOMBO/NPROCS
    cilk_for(int i = 0; i < NPROCS; i++) {
        for(int k=(TOTALCOMBO/NPROCS)*i;k<(TOTALCOMBO/NPROCS)*(i+1);k++) {
            if(notfound) {
                genpass(k,passcandidate);
                testresult=test(hash, passcandidate);
                if(testresult==0) { //Note that this is *absurdly* dangerous, and locks should be used unless you know that only one candidate will match
                    notfound=0;
                    strncpy(passmatch, passcandidate, (PASSLENGTH+1));
                    printf("Found a match... should halt\n");
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s <password hash>\n",argv[0]);
        return 1;
    }
    char passmatch[PASSLENGTH];
    crackHash(argv[1],passmatch);
    printf("Found match %s\n",passmatch);
    return 0;
}
