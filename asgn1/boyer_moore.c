#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int match (const char* s, size_t q, size_t i){
    while (i < strlen(s) && s[q] == s[i]) {
    ++q;
    ++i;
    }
    return q;
}

size_t* comp_z(const char* p){
    int j = strlen(p);
    size_t* i = (size_t*)malloc(sizeof(size_t));
    *i = j;
    return i;
}
int main(){
	size_t* a = comp_z("abcd");
	printf("%d\n",(int)*a);
}   

