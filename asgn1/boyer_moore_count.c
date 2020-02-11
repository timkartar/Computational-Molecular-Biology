#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

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
    size_t l = 0, r = 0;
    for(size_t k=1; k< strlen(p); ++k){
        if(k >= r){                         //case 1: full comparison
            the_case = "1";
            Z[k] = match(p, 0, k);
            r = k + Z[k];
            l = k;
        }
        else {                              //case 2: inside a Z-box
            const size_t k_prime = k - l;
            const size_t beta_len = r - k;
            if (Z[k_prime] < beta_len){     // Case 2a: stay inside z-box
                the_case = "2a";
                Z[k] = Z[k_prime];
            }
            else{                            //Case 2b: need to match outside the Z-box
                the_case = "2b";
                Z[k] = match(p,beta_len,r);
                r = k + Z[k];
                l = k;
            }
        }
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
    struct listnode* next; 
    size_t idx; 
} node; 

/* Print all the elements in a linked list */

void printlist(node *head) {                                            //to debug occurence lists
    node *current_node = head;
    	while ( current_node != NULL) {
        printf("%zu ", current_node->idx);
        current_node = current_node->next;
    }
}

/* Preprocessing for Extended Bad Character Rule*/

node** preproc_ebcr(const char* p, int n){                  //pointer array to char occurence lists
    const char * alpha = "ACGT";
    node*  tails[4];
    node**  heads = (node**)malloc(sizeof(node*)*strlen(alpha));
    for(int i = 0;i< (int)strlen(alpha);i++){
        tails[i] =  NULL;
    }
    for(int i = n-1; i >= 0 ;i--){
        int idx_in_alpha = chartoidx(p[i]);
        node* item = (node*)malloc(sizeof(node));
        item->idx = i;
        item->next = NULL;
        if(*(tails + idx_in_alpha)== NULL){
            *(tails + idx_in_alpha) = item;
            heads[idx_in_alpha] = tails[idx_in_alpha];
        }
        else{
        (*(tails + idx_in_alpha ))->next =  item;
        (*(tails + idx_in_alpha)) = (*(tails + idx_in_alpha))->next;
        }        
    }
    return heads;
}

/* Extended Bad Character Rule */
size_t ebcr(char x, size_t pi,  node** heads){              //Returns poisition j < i which should
    if(x != 'A' && x!= 'G' && x!='C' && x!='T'){            //be shifted to 
        return 0;                                           //position of mismatch(i).
    }
    int idx_x = chartoidx(x);
    node * x_list = heads[idx_x];
    while(x_list != NULL){
        if(x_list->idx < pi){
            return x_list->idx;
        }
        x_list = x_list->next;
    }
    return 0;
}
/******************************************************************************/

/////////////////////// Strong Good Suffix Rule //////////////////////////////

/* Preprocessing for the strong good suffix rule */
size_t* comp_N(char* p){                                    //reserse p, calculate Z and then N[i] = Z[n-i-1]
    int n = strlen(p);
    char* rev_p = (char*)malloc(sizeof(char)*n);
    for(int i = 0; i < n; i++){
        rev_p[i] = p[n-i-1];
    }
    size_t* N = (size_t*)malloc(sizeof(size_t)*n);
    size_t* Z = comp_z(rev_p);
    for(int i = 0; i < n; i++){
        N[i] = Z[n-i-1];
    }
    free(rev_p);
    return N;    
}
size_t* comp_L_prime(char * p){                              //calculate L' from N using Gusfield's pseudocode
    int n = strlen(p);
    size_t * N  = comp_N(p);
    size_t* L_prime = (size_t*)malloc(sizeof(size_t)*n);
    for(int j = 0; j < n; j++){
        size_t i = n - N[j];
        L_prime[i] = j;
    }
    return L_prime;
}
size_t * comp_l_prime(char * p){                             //Gusfield left this part as exercise to do in O(n)
    size_t * N  = comp_N(p);                                //go top down to check for N[j] == j and
    int n = strlen(p);                                      //assign bottom up checking condition.
    size_t * l_d =  (size_t*)malloc(sizeof(size_t) * n);
    for(int j=0; j < n; j++){
        l_d[j]= 0;
    }
    int i = 0;
    for(int j = n-1; j >= 0; j--){
        if(N[j] == (size_t)j){
            while(i < n && j < n-i){
                l_d[i] = j;
                i++;
            }
        }
    }
    return l_d;
}

/* The strong good suffix rule */
size_t sgsr(size_t* L, size_t* l, int i,int n){                         //Returns position i which should be at current position 
    if(i == n){                                                         //of p[n-1] after shifting. (0-based index)
        return (size_t)n-1;
    }
    if(L[i]==0){
        return l[i];    
    }
    return L[i];
}

/*****************************************************************************/

///////////////////// Boyer-Moore ////////////////////////////////////////////
static long int comparisons = 0;
int Boyer_Moore (const char* t, const char* p, node** heads, 
        size_t* l, size_t* L, size_t m, int  n){
    size_t occur_count = 0;
    size_t k = n-1;
    while(k < m){
        int i = n-1;
        size_t h = k;
        while(i>=0 && p[i] == t[h]){
            comparisons ++;                                             // comparisons resulting in match
            i = i-1; 
            h = h-1;
        }
        if (i == -1){
            //printf("match found at %zu\n",k - n + 2);
            occur_count += 1;
            if(l[1] == 0){
                k = k + 1;
            }
            else{
                k = k + n - 1 - l[1];
            }
        }
        else{
            comparisons++;                                              // comparisons resulting in mismatch
            size_t ebcr_pos = ebcr(t[h],i,heads);
            size_t ebcr_shift = ebcr_pos == 0 ? 1 : i - ebcr(t[h],i,heads);

            size_t sgsr_shift = n - 1 - sgsr(L,l,i+1,n);
            size_t shift =  (ebcr_shift > sgsr_shift) ? ebcr_shift : sgsr_shift;
            if(i == n-1){
                k = k + 1;
            }
            else{
                k = k + shift;
            }
        }
    }
    return occur_count;
}

/***************************************************************************/
/////////////////////////// Read FASTA File ///////////////////////////////////
long int read_fasta_file(const char *filename, char **the_sequence) {
    FILE *in;
    char *buffer, *current_position;
    long int file_size, remaining_space, line_length;

    in = fopen(filename, "rb");                                         // open to read in binary (one big chunk)
    if (in == NULL)
        return 0;

                                                                        // get the file size
    fseek(in, 0, SEEK_END);                                             // seek from zero to the end
    file_size = ftell(in);                                              // the size is how far we moved...
    rewind(in);                                                         // and then move back to the start

                                                                        // allocate memory to contain the whole file:
    buffer = (char *)malloc(sizeof(char)*file_size);
    if (buffer == NULL)                                                 // check that the allocation succeeded
        return 0;

    current_position = buffer;
    remaining_space = file_size;
    while ((line_length = getline(&current_position,
                                &remaining_space, in)) != -1) {
        if (*current_position != '>') {
                                                                        // subtract 1 below because getline includes the '\n'
            current_position += (line_length - 1);
            remaining_space -= (line_length - 1);
        }
    }

    *the_sequence = buffer;
    return file_size - remaining_space;
}


/*****************************************************************************/

////////////////////////////////////// Main ///////////////////////////////////

int main(const int argc, char *const argv[]) {
    if (argc != 3) {
       printf("input must be: naive <pattern> <FASTA>\n");
       return -1;
    }

    char * p = argv[1];
    int n = strlen(p);
    char * t;
                                                                        // read the text and assign its length
    size_t m = read_fasta_file(argv[2], &t);
    if (m == 0) {
        fprintf(stderr, "problem reading file: %s\n", argv[2]);
        return -1;
    }    
    node** heads  = preproc_ebcr(p,n);
    size_t* l_prime = comp_l_prime(p);
    //printf("\n");
    size_t* L_prime = comp_L_prime(p);
    
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    int count = Boyer_Moore(t, p, heads, l_prime, L_prime, m, n);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Total Matches found:\t%d\nChar comparisons:\t%ld\nTime taken in seconds:\t%lf\n\n", count, comparisons, cpu_time_used);
    free(heads);
    free(l_prime);
    free(L_prime);
    return EXIT_SUCCESS;
}   
