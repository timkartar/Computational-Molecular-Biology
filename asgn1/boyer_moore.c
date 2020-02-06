#include<stdio.h>
#include<stdlib.h>
#include<string.h>

////////////////// C translation of provided Z-algorithm code /////////////////
//input : pattern p
//output: pointer to size_t array of size length(p)

int match (const char* s, size_t q, size_t i){
    while (i < strlen(s) && s[q] == s[i]) {
    ++q;
    ++i;
    }
    return q;
}

size_t* comp_z(const char* p){
    size_t* Z = (size_t*)malloc(sizeof(size_t)*strlen(p));
    Z[0]=0;
    char* the_case = "0";
    size_t l=0, r=0;
    for(size_t k=1; k< strlen(p); ++k){
        if(k >= r){  //case 1: full comparison
            the_case = "1";
            Z[k] = match(p, 0, k);
            r = k + Z[k];
            l = k;
        }
        else { //case 2: inside a Z-box
            const size_t k_prime = k - 1;
            const size_t beta_len = r - k;
            if (Z[k_prime] < beta_len){ // Case 2a: stay inside z-box
                the_case = "2a";
                Z[k] = Z[k_prime];
            }
            else{ //Case 2b: need to match outside the Z-box
                the_case = "2b";
                Z[k] = match(p,beta_len,r);
                r = k + Z[k];
                l = k;
            }
        }
        printf("%zu\t%zu\t%zu\t%zu\t%s\n",k+1,l+1,r,Z[k],the_case);
    }
    return Z;
}
/*****************************************************************************/


///////////////////// Extended Bad Character Rule  ////////////////////////////

/* Utility Functions*/

int chartoidx(char c){
    if(c == 'A'){return 0;}
    if(c == 'C'){return 1;}
    if(c == 'G'){return 2;}
    if(c == 'T'){return 3;}
    return -1;
}

typedef struct listnode{
    struct listnode* head; 
    struct listnode* next; 
    size_t idx; 
} node; 

/* Print all the elements in a linked list */

void printlist(node *head) {
    node *current_node = head;
    	while ( current_node != NULL) {
        printf("%zu ", current_node->idx);
        current_node = current_node->next;
    }
}

/* Preprocessing for Extended Bad Character Rule*/

node** preproc_ebcr(const char* p, int n){
    const char * alpha = "ACGT";
    node*  tails[4];
    node**  heads = (node**)malloc(sizeof(node*)*strlen(alpha));
    for(int i = 0;i< (int)strlen(alpha);i++){
        tails[i] =  NULL;
    }
    for(int i = n-1; i >= 0 ;i--){
        int idx_in_alpha = chartoidx(p[i]);
        node* item = (node*)malloc(sizeof(node));
        item -> idx = i;
        item -> next = NULL;
        if(*(tails + idx_in_alpha)== NULL){
            *(tails + idx_in_alpha) = item;
            heads[idx_in_alpha] = tails[idx_in_alpha];
        }
        else{
        (*(tails + idx_in_alpha ))->next =  item;
        (*(tails + idx_in_alpha)) = (*(tails + idx_in_alpha)) -> next;
        }        
        printf("%zu \n",heads[idx_in_alpha]->idx);
    }
    return heads;
}

/* Extended Bad Character Rule */
size_t ebcr(char x, size_t pi,  node** heads){
    int idx_x = chartoidx(x);
    node * x_list = heads[idx_x];
    while(x_list != NULL){
        if(x_list -> idx < pi){
            return x_list -> idx;
        }
        x_list = x_list -> next;
    }
    return 0;
}
/******************************************************************************/

/////////////////////// Strong Good Suffix Rule //////////////////////////////
void strrev(char *str){
    char c, *front, *back;

    if(!str || !*str)
        return;
    for(front=str,back=str+strlen(str)-1;front < back;front++,back--){
        c=*front;*front=*back;*back=c;
    }
}
/* Preprocessing for the strong good suffix rule */
size_t* comp_N(char* p){
    strrev(p);
    printf("%s",p);
    int n = strlen(p);
    size_t* N = (size_t*)malloc(sizeof(size_t)*n);
    size_t* Z = comp_z(p);
    for(int i=0;i < n;i++){
        N[i] = Z[n-i+1];
    }
    strrev(p);
    printf("%s",p);
    return N;    
}
size_t* comp_L_dash(char * p){
    int n = strlen(p);
    size_t * N  = comp_N(p);
    size_t* L_dash = (size_t*)malloc(sizeof(size_t)*n);
    for (int j =0;j<n;j++){
        size_t i = n - N[j] + 1;
        L_dash[i] = j;
    }
    return L_dash;
}
size_t* comp_l_dash(const char * p){
    size_t * out = malloc(sizeof(size_t));
    return out;
}

/* The strong good suffix rule */
size_t sgsr(const char * p, int i){
    return 0;
}

/*****************************************************************************/

///////////////////// Boyer-Moore ////////////////////////////////////////////

int Boyer_Moore (const char* t, const char* p, node** heads, size_t* l, size_t m, int  n){
    size_t occur_count = 0;
    size_t k = n;
    while(k < m){
        size_t i = n;
        size_t h = k;
        while(i>=0 && p[i] == t[h]){
            i = i-1; 
            h = h-1;
        }
        if (i == 0){
            occur_count += 1;
            k = k + n - l[1];
        }
        else{
            size_t ebcr_shift = ebcr(t[h],i,heads);
            size_t sgsr_shift = sgsr(p,i);
            size_t shift_idx =  (ebcr_shift < sgsr_shift) ? ebcr_shift : sgsr_shift;
            k = k + n - shift_idx;
        }
    }
    return occur_count;
}

/***************************************************************************/


////////////////////////////////////// Main ///////////////////////////////////

int main(){
    char * p = "AAAAACAGTTACCCAATGACA";
    int n = strlen(p);
    char * s = "AAAAACAGTTACCCAAAAACAGTAAAAACAGTTACCCAATGAAAAACAAAAACAGTTACCAAAAACAGTTACCCAATGACA";
    //int m = strlen(s);
    //comp_z(p);
    node** heads  = preproc_ebcr(p,n);
    for(int i=0;i<4;i++){
        printf("\n");
        printlist(heads[i]);        
    }
    printf("\n %zu \n\n",ebcr('C',5,heads));
    return EXIT_SUCCESS;
}   

