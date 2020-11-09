//
//  suffix_arr.cpp
//  microDNA_finder
//
//  Created by Yiming Wang on 2020/4/7.
//

#include "SuffixArray.h"

// set only the highest bit as 1, i.e. 1000...
//const unsigned int EMPTY_k=((unsigned int)1)<<(sizeof(unsigned int)*8-1);
const uint_t EMPTY_k=((uint_t)1)<<(sizeof(uint_t)*8-1);

// get s[i] at a certain level
#define chr(i) (cs==sizeof(int_t)?((int_t*)s)[i]:((unsigned char *)s)[i])



#define true 1
#define false 0

#define DEPTH 0
#define TIME  0
#define RMQ   2  //variants = (1, trivial) (2, using Gog's stack)
#define BINARY 0 //binary search on stack operations

#define STACK_SIZE 895 //to use 10Kb of working space

/**/

void t_start(time_t *t_time, clock_t *c_clock){

    *t_time = time(NULL);
    *c_clock =  clock();
}

double t_stop(time_t t_time, clock_t c_clock){

    double aux1 = (clock() - c_clock) / (double)(CLOCKS_PER_SEC);
    double aux2 = difftime (time(NULL),t_time);
    printf("CLOCK = %lf TIME = %lf\n", aux1, aux2);
    return aux1;
}

/**/

typedef struct _pair{
  uint_t idx;
  int_t lcp;
} t_pair;

int compare (const void * a, const void * b){
  if(*(const uint_t *)a < *(const uint_t *)b) return -1;
  if(*(const uint_t *)a > *(const uint_t *)b) return 1;
return 0;
}

int type_cmp(void *a, void *b){ return (*(uint_t*)a)-(*(uint_t*)b); }

void compute_lcp_phi_sparse(int_t *s, uint_t *SA1,
  uint_t *RA, int_t *LCP, int_t *PLCP,
  uint_t n1, int cs) {

  uint_t i;

  PLCP[SA1[0]]=0;//PLCP* (lms) is stored in PLCP array
  for(i=1; i<n1; i++)
    PLCP[SA1[i]] = LCP[i];

  LCP[SA1[0]]=0;//PHI is stored in LCP array
  for(i=1; i<n1; i++)
    LCP[SA1[i]] = SA1[i-1]; //RA[SA1[i-1]];

  int_t l=0; //q=0;
  for(i=0; i<n1-1;i++){

    l = max(PLCP[i], l);//consider the LCP-value of the lms-substrings

    while(chr(RA[i]+l)==chr(RA[LCP[i]]+l)) ++l;
    PLCP[i]=l;

    if(LCP[i]==n1-1) l -= RA[i+1]-RA[i];
    else l -= max(RA[i+1]-RA[i], RA[LCP[i]+1]-RA[LCP[i]]);//LCP[i] stores the distance of i-th suffix to its successor
  }

  LCP[0]=0;
  for(i=1; i<n1;i++) LCP[i]=PLCP[SA1[i]];

}

/*****************************************************************************/

void getBuckets_k(int_t *s,
  uint_t *bkt, uint_t n,
  unsigned int K, int end, int cs) {
  uint_t i, sum=0;
  
  // clear all buckets .
  for(i=0; i<K; i++) bkt[i]=0;
  
  // compute the size of each bucket .
  for(i=0; i<n; i++) bkt[chr(i)]++;
  
  for(i=0; i<K; i++) {
    sum+=bkt[i];
    bkt[i]=end ? sum-1 : sum-bkt[i];
  }
}

void putSuffix0(uint_t *SA,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t n1, int cs) {
  uint_t i, j;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  // put the suffixes into their buckets.
  for(i=n1-1; i>0; i--) {
    j=SA[i]; SA[i]=0;
    SA[bkt[chr(j)]--]=j;
  }
  SA[0]=n-1; // set the single sentinel suffix.
}

void putSuffix0_LCP(uint_t *SA, int_t *LCP,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t n1, int cs) {
  uint_t i, j;
  int_t l;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  // put the suffixes into their buckets.
  for(i=n1-1; i>0; i--) {
    j=SA[i]; SA[i]=U_MAX;
    l=LCP[i]; LCP[i]=0;

    SA[bkt[chr(j)]]=j;
    LCP[bkt[chr(j)]--]=l;
  }
  SA[0]=n-1; // set the single sentinel suffix.

//  for(i=0;i<K;i++) if(bkt[i]+1<n) if(!LCP[bkt[i]+1]) LCP[bkt[i]+1]=-2;
}

/*****************************************************************************/

void induceSAl0(uint_t *SA,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t suffix, int cs) {
  uint_t i, j;

  // find the head of each bucket.
  getBuckets_k(s, bkt, n, K, false, cs);

  bkt[0]++; // skip the virtual sentinel.
  for(i=0; i<n; i++)
    if(SA[i]>0) {
      j=SA[i]-1;
      if(chr(j)>=chr(j+1)) {
        SA[bkt[chr(j)]]=j;
        bkt[chr(j)]++;
        if(!suffix && i>0) SA[i]=0;
      }
    }
}

void stack_push(t_pair* STACK, int_t *top, uint_t idx, int_t lcp){

  STACK[*top].idx=idx;
  STACK[*top].lcp=lcp;

  (*top)++;
}


void induceSAl0_LCP(uint_t *SA, int_t *LCP,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int cs) {
  uint_t i, j;
    

  for(i=0;i<K;i++)
    if(bkt[i]+1<n) if(SA[bkt[i]+1]!=U_MAX) LCP[bkt[i]+1]=I_MIN;

  // find the head of each bucket.
  getBuckets_k(s, bkt, n, K, false, cs);
    
  for(i=0;i<K;i++)
    if(bkt[i]<n) LCP[bkt[i]]=-2;

  #if RMQ == 1
  int_t *M=(int_t *)malloc(sizeof(int_t)*K);
  for(i=0;i<K;i++){
    M[i]=I_MAX;
  }
  #elif RMQ == 2
  uint_t* last_occ = (uint_t*) malloc(K*sizeof(uint_t));
  uint_t* tmp = (uint_t*) malloc(K*sizeof(uint_t));

  t_pair* STACK = (t_pair*) malloc((STACK_SIZE+1)*sizeof(t_pair));
  int_t top = 0;
  //init
  stack_push(STACK, &top, 0, -1);
  for(i=0;i<K;i++) last_occ[i]=0;
  #endif

    
  #if DEBUG
  printf("inducing..\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif
    

  bkt[0]++; // skip the virtual sentinel.
  for(i=0; i<n; i++){

    if(SA[i]!=U_MAX){

      if(LCP[i]==I_MIN){ //is a L/S-seam position
        int_t l=0;
      if(SA[bkt[chr(SA[i])]-1]<n-1)
           while(chr(SA[i]+l)==chr(SA[bkt[chr(SA[i])]-1]+l))++l;
        LCP[i]=l;
      }
      #if RMQ == 1
        uint_t k;
        for(k=0; k<K; k++) if(M[k]>LCP[i]) M[k] = max(0,LCP[i]);
      #elif RMQ == 2
        int_t min_lcp=0;
        uint_t last;

        if(!SA[i]) last = 0;
        else{
          last = last_occ[chr(SA[i]-1)];
          last_occ[chr(SA[i]-1)] = i+1;
        }
 
        int_t lcp=max(0,LCP[i]);
        #if BINARY == 1
          int_t a=0, b=top-1;
          int_t m = (b-a)/2;

          while(a<=b){
            if(STACK[m].lcp==lcp){ break; }
            if(STACK[m].lcp>lcp) b=m-1;
            else a=m+1;
            m=a+(b-a)/2;
          }
          top = m;
        #else
          while(STACK[(top)-1].lcp>=lcp) (top)--;
        #endif

        stack_push(STACK, &top, i+1, lcp);
        j = top-1;

        #if BINARY == 1
          a=0, b=top-1;
          m = (b-a)/2;
          while(a<=b){
            if(STACK[m].idx==last){ m++; break;}
            if(STACK[m].idx>last) b=m-1;
            else a=m+1;
            m=a+(b-a)/2;
          }
          j = m-1;
        #else
          while(STACK[j].idx>last) j--;
        #endif

        min_lcp=STACK[(j+1)].lcp;
      #endif

      if(SA[i]>0) {

        j=SA[i]-1;
        if(chr(j)>=chr(j+1)) { //L-type

          SA[bkt[chr(j)]]=j;

          #if RMQ == 1
      LCP[bkt[chr(j)]]+=M[chr(j)]+1;
        M[chr(j)] = I_MAX;
          #elif RMQ == 2
      LCP[bkt[chr(j)]]+=min_lcp+1;
          #endif

          bkt[chr(j)]++;
        }
      
    if(bkt[chr(SA[i])]-1<i){ //if is LMS-type
      SA[i]=U_MAX;
        }
      }

      #if RMQ == 2
      if(top>=STACK_SIZE){//if stack is full

        int_t j;
        memcpy(tmp, last_occ, K*sizeof(uint_t));
    qsort(tmp, K, sizeof(uint_t), compare);
       
        int_t curr=1, end=1;
        STACK[top].idx=U_MAX;
        for(j=0;j<K; j++){

          if(STACK[end-1].idx < tmp[j]+1){

              //search (can be binary)
            #if BINARY == 1
              int_t a=curr-1, b=top-1;
              int_t m = (b-a)/2, last=tmp[j]+1;
              while(a<=b){
                if(STACK[m].idx==last){ break;}
                if(STACK[m].idx>last) b=m-1;
                else a=m+1;
                m=a+(b-a)/2;
              }
              curr = m;
            #else
              while(STACK[curr].idx<tmp[j]+1) curr++;
        #endif

            if(curr<top) {
              STACK[end].idx=STACK[curr].idx;
              STACK[end].lcp=STACK[curr].lcp;
              end++;
          curr++;
            }
      }
        }
 
        if(end>=STACK_SIZE){
          fprintf(stderr,"ERROR: induceSAl0_LCP\n");
          exit(1);
        }
        top = end;
      }
      #endif
    }
  }
    
  #if RMQ == 1
  free(M);
  #elif RMQ == 2
  free(STACK);
  free(last_occ);
  free(tmp);
  #endif
}

/*****************************************************************************/

void induceSAs0(uint_t *SA,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int_t suffix, int cs) {
  uint_t i, j;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  for(i=n-1; i>0; i--)
    if(SA[i]>0) {
      j=SA[i]-1;
      if(chr(j)<=chr(j+1) && bkt[chr(j)]<i) {
        SA[bkt[chr(j)]]=j;
        bkt[chr(j)]--;
        if(!suffix) SA[i]=0;
      }
    }
}

void induceSAs0_LCP(uint_t *SA, int_t *LCP,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int cs) {
  uint_t i, j;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  #if RMQ == 1
  int_t *M=(int_t *)malloc(sizeof(int_t)*K);
  for(i=0;i<K;i++) M[i]=I_MAX;
  #elif RMQ == 2
  uint_t* last_occ = (uint_t*) malloc(K*sizeof(uint_t));
  uint_t* tmp = (uint_t*) malloc(K*sizeof(uint_t));

  t_pair* STACK = (t_pair*) malloc((STACK_SIZE+1)*sizeof(t_pair));
  int_t top = 0;
  //init
  stack_push(STACK, &top, n, -1);
  for(i=0;i<K;i++) last_occ[i]=n-1;
  #endif

  #if DEBUG
  printf("inducing..\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif

  for(i=n-1; i>0; i--){

    if(SA[i]>0) {
      j=SA[i]-1;
      if(chr(j)<=chr(j+1) && bkt[chr(j)]<i) {// induce S-type

        SA[bkt[chr(j)]]=j;

        #if RMQ == 1
    if(LCP[bkt[chr(j)]+1]>=0)
      LCP[bkt[chr(j)]+1]=M[chr(j)]+1;
        #elif RMQ == 2
        int_t min = I_MAX, end = top-1;

    int_t last=last_occ[chr(j)];
        //search (can be binary)
        #if BINARY == 1
          int_t a=0, b=top-1;
          int_t m = (b-a)/2;
          while(a<=b){
            if(STACK[m].idx==last){ break;}
            if(STACK[m].idx<last) b=m-1;
            else a=m+1;
            m=a+(b-a)/2;
          }
          end = m-1;
        #else
          while(STACK[end].idx<=last) end--;
    #endif

        min=STACK[(end+1)].lcp;
        last_occ[chr(j)] = i;

    if(LCP[bkt[chr(j)]+1]>=0)
          LCP[bkt[chr(j)]+1]=min+1;

        #endif

        #if RMQ == 1
    if(LCP[bkt[chr(j)]]>0)
      LCP[bkt[chr(j)]]=I_MAX;
    M[chr(j)] = I_MAX;
        #endif

        bkt[chr(j)]--;

    if(SA[bkt[chr(j)]]!=U_MAX) {//L/S-seam
          int_t l=0;
          while(chr(SA[bkt[chr(j)]+1]+l)==chr(SA[bkt[chr(j)]]+l))++l;
          LCP[bkt[chr(j)]+1]=l;
    }
      }
    }

    if(LCP[i]<0) LCP[i]=0;

    #if RMQ == 1
    int_t k;
    for(k=0; k<K; k++) if(M[k]>LCP[i]) M[k] = LCP[i];
    #elif RMQ == 2
      int_t lcp=max(0,LCP[i]);

      
      //search (can be binary)
      #if BINARY == 1
        int_t a=0, b=top-1;
        int_t m = (b-a)/2;
        while(a<=b){
          if(STACK[m].lcp==lcp){ break; }
          if(STACK[m].lcp>lcp) b=m-1;
          else a=m+1;
          m=a+(b-a)/2;
        }
        top = m;
      #else
        while(STACK[(top)-1].lcp>=lcp) (top)--;
      #endif
      stack_push(STACK, &top, i, lcp);

      if(top>=STACK_SIZE){

          int_t j;
          memcpy(tmp, last_occ, K*sizeof(uint_t));
      qsort(tmp, K, sizeof(uint_t), compare);

          int_t curr=1, end=1;

          for(j=K-1;j>=0; j--){

            if(STACK[end-1].idx > tmp[j]){

            #if BINARY == 1
              int_t a=curr-1, b=top-1;
              int_t m = (b-a)/2, last=tmp[j];
              while(a<=b){
                if(STACK[m].idx==last){ break;}
                if(STACK[m].idx<last) b=m-1;
                else a=m+1;
                m=a+(b-a)/2;
              }
              curr = m;
            #else
              while(STACK[curr].idx>tmp[j] && curr < top) curr++;
        #endif

              if(curr>=top) break;
              STACK[end].idx=STACK[curr].idx;
              STACK[end].lcp=STACK[curr].lcp;
              end++;
              curr++;
            }
          }

          if(end>=STACK_SIZE){
            fprintf(stderr,"ERROR: induceSAl0_LCP\n");
            exit(1);
          }
          top = end;
      }
      #endif
  }

  LCP[0]=0;
  
  //variant 1
  #if RMQ == 1
  free(M);
  #elif RMQ == 2
  free(STACK);
  free(last_occ);
  free(tmp);
  #endif
}


/*****************************************************************************/

void putSubstr0(uint_t *SA,
  int_t *s, uint_t *bkt,
  uint_t n, unsigned int K, int cs) {
  uint_t i, cur_t, succ_t;

  // find the end of each bucket.
  getBuckets_k(s, bkt, n, K, true, cs);

  // set each item in SA as empty.
  for(i=0; i<n; i++) SA[i]=0;

  succ_t=0; // s[n-2] must be L-type.
  for(i=n-2; i>0; i--) {
    cur_t=(chr(i-1)<chr(i) ||
           (chr(i-1)==chr(i) && succ_t==1)
          )?1:0;
    if(cur_t==0 && succ_t==1) SA[bkt[chr(i)]--]=i;
    succ_t=cur_t;
  }

  // set the single sentinel LMS-substring.
  SA[0]=n-1;
}

/*****************************************************************************/

void putSuffix1(int_t *SA, int_t *s, int_t n1, int cs) {
  int_t i, j, pos=n1, cur, pre=-1;
  
  for(i=n1-1; i>0; i--) {
    j=SA[i]; SA[i]=EMPTY_k;
    cur=chr(j);
    if(cur!=pre) {
      pre=cur; pos=cur;
    }
    SA[pos--]=j;
  }
}

void induceSAl1(int_t *SA, int_t *s,
  int_t n, int_t suffix, int cs) {
  int_t h, i, j, step=1;
  
  for(i=0; i<n; i+=step) {
    step=1; j=SA[i]-1;
    if(SA[i]<=0) continue;
    int_t c=chr(j), c1=chr(j+1);
    int_t isL=c>=c1;
    if(!isL) continue;

    // s[j] is L-type.

    int_t d=SA[c];
    if(d>=0) {
      // SA[c] is borrowed by the left
      //   neighbor bucket.
      // shift-left the items in the
      //   left neighbor bucket.
      int_t foo, bar;
      foo=SA[c];
      for(h=c-1; SA[h]>=0||SA[h]==EMPTY_k; h--)
      { bar=SA[h]; SA[h]=foo; foo=bar; }
      SA[h]=foo;
      if(h<i) step=0;

      d=EMPTY_k;
    }

    if(d==EMPTY_k) { // SA[c] is empty.
      if(c<n-1 && SA[c+1]==EMPTY_k) {
        SA[c]=-1; // init the counter.
        SA[c+1]=j;
      }
      else
        SA[c]=j; // a size-1 bucket.
    }
    else { // SA[c] is reused as a counter.
        int_t pos=c-d+1;
        if(pos>n-1 || SA[pos]!=EMPTY_k) {
          // we are running into the right
          //   neighbor bucket.
          // shift-left one step the items
          //   of bucket(SA, S, j).
          for(h=0; h<-d; h++)
            SA[c+h]=SA[c+h+1];
          pos--;
          if(c<i) step=0;
        }
        else
          SA[c]--;

        SA[pos]=j;
    }

    int_t c2;
    int_t isL1=(j+1<n-1) && (c1>(c2=chr(j+2)) || (c1==c2 && c1<i));  // is s[SA[i]] L-type?
    if((!suffix || !isL1) && i>0) {
      int_t i1=(step==0)?i-1:i;
      SA[i1]=EMPTY_k;
    }
  }

  // scan to shift-left the items in each bucket
  //   with its head being reused as a counter.
  for(i=1; i<n; i++) {
    j=SA[i];
    if(j<0 && j!=EMPTY_k) { // is SA[i] a counter?
      for(h=0; h<-j; h++)
        SA[i+h]=SA[i+h+1];
      SA[i+h]=EMPTY_k;
    }
  }
}

void induceSAs1(int_t *SA, int_t *s,
  int_t n, int_t suffix, int cs) {
  int_t h, i, j, step=1;
  
  for(i=n-1; i>0; i-=step) {
    step=1; j=SA[i]-1;
    if(SA[i]<=0) continue;
    int_t c=chr(j), c1=chr(j+1);
    int_t isS=(c<c1) || (c==c1 && c>i);
    if(!isS) continue;

    // s[j] is S-type

    int_t d=SA[c];
    if(d>=0) {
      // SA[c] is borrowed by the right
      //   neighbor bucket.
      // shift-right the items in the
      //   right neighbor bucket.
      int_t foo, bar;
      foo=SA[c];
      for(h=c+1; SA[h]>=0||SA[h]==EMPTY_k; h++)
      { bar=SA[h]; SA[h]=foo; foo=bar; }
      SA[h]=foo;
      if(h>i) step=0;

      d=EMPTY_k;
    }

    if(d==EMPTY_k) { // SA[c] is empty.
      if(SA[c-1]==EMPTY_k) {
        SA[c]=-1; // init the counter.
        SA[c-1]=j;
      }
      else
        SA[c]=j; // a size-1 bucket.
    }
    else { // SA[c] is reused as a counter.
        int_t pos=c+d-1;
        if(SA[pos]!=EMPTY_k) {
          // we are running into the left
          //   neighbor bucket.
          // shift-right one step the items
          //   of bucket(SA, S, j).
          for(h=0; h<-d; h++)
            SA[c-h]=SA[c-h-1];
          pos++;
          if(c>i) step=0;
        }
        else
          SA[c]--;

        SA[pos]=j;
    }

    if(!suffix) {
      int_t i1=(step==0)?i+1:i;
      SA[i1]=EMPTY_k;
    }
  }

  // scan to shift-right the items in each bucket
  //   with its head being reused as a counter.
  if(!suffix)
    for(i=n-1; i>0; i--) {
      j=SA[i];
      if(j<0 && j!=EMPTY_k) { // is SA[i] a counter?
        for(h=0; h<-j; h++)
          SA[i-h]=SA[i-h-1];
        SA[i-h]=EMPTY_k;
      }
    }
}

void putSubstr1(int_t *SA, int_t *s, int_t n, int cs) {
  int_t h, i, j;

  for(i=0; i<n; i++) SA[i]=EMPTY_k;

  int_t c, c1, t, t1;
  c1=chr(n-2);
  t1=0;
  for(i=n-2; i>0; i--) {
    c=c1; t=t1;
    c1=chr(i-1);
    t1=c1<c || (c1==c && t);
    if(t && !t1) {
      if(SA[c]>=0) {
        // SA[c] is borrowed by the right
        //   neighbor bucket.
        // shift-right the items in the
        //   right neighbor bucket.
        int_t foo, bar;
        foo=SA[c];
        for(h=c+1; SA[h]>=0; h++)
        { bar=SA[h]; SA[h]=foo; foo=bar; }
        SA[h]=foo;

        SA[c]=EMPTY_k;
      }

      int_t d=SA[c];
      if(d==EMPTY_k) { // SA[c] is empty.
        if(SA[c-1]==EMPTY_k) {
          SA[c]=-1; // init the counter.
          SA[c-1]=i;
        }
        else
          SA[c]=i; // a size-1 bucket.
      }
      else { // SA[c] is reused as a counter
          int_t pos=c+d-1;
          if(SA[pos]!=EMPTY_k) {
            // we are running into the left
            //   neighbor bucket.
            // shift-right one step the items
            //   of bucket(SA, S, i).
            for(h=0; h<-d; h++)
              SA[c-h]=SA[c-h-1];
            pos++;
          }
          else
            SA[c]--;

          SA[pos]=i;
      }
    }
  }

  // scan to shift-right the items in each bucket
  //   with its head being reused as a counter.
  for(i=n-1; i>0; i--) {
    j=SA[i];
    if(j<0 && j!=EMPTY_k) { // is SA[i] a counter?
      for(h=0; h<-j; h++)
        SA[i-h]=SA[i-h-1];
      SA[i-h]=EMPTY_k;
    }
  }

  // put the single sentinel LMS-substring.
  SA[0]=n-1;
}

uint_t getLengthOfLMS(int_t *s,
  uint_t n, int level, uint_t x, int cs) {
  if(x==n-1) return 1;
  
  uint_t dist=0, i=1;
  while(1) {
    if(chr(x+i)<chr(x+i-1)) break;
    i++;
  }
  while(1) {
    if(x+i>n-1 || chr(x+i)>chr(x+i-1)) break;
    if(x+i==n-1 || chr(x+i)<chr(x+i-1)) dist=i;
    i++;
  }
  
  return dist+1;
}

/*****************************************************************************/

uint_t nameSubstr_LCP(uint_t *SA, int_t *LCP,
  int_t *s, uint_t *s1, uint_t n,
  uint_t m, uint_t n1, int level, int cs) {
  uint_t i, j, cur_t, succ_t;

  // init the name array buffer
  for(i=n1; i<n; i++) SA[i]=EMPTY_k;

  // scan to compute the interim s1
  uint_t name=0, name_ctr=0;
  uint_t pre_pos=n-1, pre_len=0;
  for(i=0; i<n1; i++) {
    int diff=false;
    uint_t len, pos=SA[i];

    int_t d;
    len=getLengthOfLMS(s, n, level, pos, cs);
    if(len!=pre_len) diff=true;
    else{
      for(d=0; d<len; d++)
        if(pos+d==n-1 || pre_pos+d==n-1 ||
           chr(pos+d)!=chr(pre_pos+d)) {
          diff=true; break;
        }
      LCP[i] = d;
    }

    if(diff) {
      name=i; name_ctr++;
      SA[name]=1; // a new name.
      pre_pos=pos; pre_len=len;
    }
    else
      SA[name]++; // count this name.

    SA[n1+pos/2]=name;
  }

  // compact the interim s1 sparsely stored
  //   in SA[n1, n-1] into SA[m-n1, m-1].
  for(i=n-1, j=m-1; i>=n1; i--)
    if(SA[i]!=EMPTY_k) SA[j--]=SA[i];

  // rename each S-type character of the
  //   interim s1 as the end of its bucket
  //   to produce the final s1.
  succ_t=1;
  for(i=n1-1; i>0; i--) {
    int_t ch=s1[i], ch1=s1[i-1];
    cur_t=(ch1< ch || (ch1==ch && succ_t==1))?1:0;
    if(cur_t==1) {
      s1[i-1]+=SA[s1[i-1]]-1;
    }
    succ_t=cur_t;
  }

  return name_ctr;
}

uint_t nameSubstr(uint_t *SA,
  int_t *s, uint_t *s1, uint_t n,
  uint_t m, uint_t n1, int level, int cs) {
  uint_t i, j, cur_t, succ_t;

  // init the name array buffer
  for(i=n1; i<n; i++) SA[i]=EMPTY_k;

  // scan to compute the interim s1
  uint_t name=0, name_ctr=0;
  uint_t pre_pos=n-1, pre_len=0;
  for(i=0; i<n1; i++) {
    int diff=false;
    uint_t len, pos=SA[i];

    uint_t d;
    len=getLengthOfLMS(s, n, level, pos, cs);
    if(len!=pre_len) diff=true;
    else
      for(d=0; d<len; d++)
        if(pos+d==n-1 || pre_pos+d==n-1 ||
           chr(pos+d)!=chr(pre_pos+d)) {
          diff=true; break;
        }

    if(diff) {
      name=i; name_ctr++;
      SA[name]=1; // a new name.
      pre_pos=pos; pre_len=len;
    }
    else
      SA[name]++; // count this name.

    SA[n1+pos/2]=name;
  }

  // compact the interim s1 sparsely stored
  //   in SA[n1, n-1] into SA[m-n1, m-1].
  for(i=n-1, j=m-1; i>=n1; i--)
    if(SA[i]!=EMPTY_k) SA[j--]=SA[i];

  // rename each S-type character of the
  //   interim s1 as the end of its bucket
  //   to produce the final s1.
  succ_t=1;
  for(i=n1-1; i>0; i--) {
    int_t ch=s1[i], ch1=s1[i-1];
    cur_t=(ch1< ch || (ch1==ch && succ_t==1))?1:0;
    if(cur_t==1) {
      s1[i-1]+=SA[s1[i-1]]-1;
    }
    succ_t=cur_t;
  }

  return name_ctr;
}

/*****************************************************************************/
// find lms position and stores it on s1 (in text order)

void getSAlms(uint_t *SA,
  int_t *s,
  uint_t *s1, uint_t n,
  uint_t n1, int level, int cs) {
  uint_t i, j, cur_t, succ_t;

  j=n1-1; s1[j--]=n-1;
  succ_t=0; // s[n-2] must be L-type
  for(i=n-2; i>0; i--) {
    cur_t=(chr(i-1)<chr(i) ||
          (chr(i-1)==chr(i) && succ_t==1))?1:0;
    if(cur_t==0 && succ_t==1) s1[j--]=i;
    succ_t=cur_t;
  }
}

/*****************************************************************************/

int_t SACA_K(int_t *s, uint_t *SA,
  uint_t n, unsigned int K,
  uint_t m, int cs, int level) {
  uint_t i;
  uint_t *bkt=NULL;

  #if TIME
    time_t t_time = 0;
    clock_t c_time = 0;
  #endif

  #if DEPTH
  time_t t_start = time(NULL);
  clock_t c_start =  clock();
  #endif

  #if DEBUG
  if(level==0) {
    int_t j;
    for(j=0; j<n; j++)
      printf("%" PRIdN ") %" PRIdN "\n", j, chr(j));
    printf("\n");
  }
  #endif

  // stage 1: reduce the problem by at least 1/2.
  if(level==0) {

    bkt=(uint_t *)malloc(sizeof(int_t)*K);
    putSubstr0(SA, s, bkt, n, K, cs);

    #if DEBUG
    printf("bucket LMS-subs\n");
    for(i=0; i<n; i++)
          printf("%" PRIdN "\t", SA[i]);
    printf("\n");
    #endif
    induceSAl0(SA, s, bkt, n, K, false, cs);

    #if DEBUG
    printf("L-type\n");
    for(i=0; i<n; i++)
          printf("%" PRIdN "\t", SA[i]);
    printf("\n");
    #endif

    induceSAs0(SA, s, bkt, n, K, false, cs);

    #if DEBUG
    printf("S-type\n");
    for(i=0; i<n; i++)
          printf("%" PRIdN "\t", SA[i]);
    printf("\n");
    #endif
  }
  else {
    putSubstr1((int_t *)SA, (int_t *)s,(int_t)n, cs);
    induceSAl1((int_t *)SA, (int_t *)s, n ,false, cs);
    induceSAs1((int_t *)SA, (int_t *)s, n, false, cs);
  }

  // now, all the LMS-substrings are sorted and
  //   stored sparsely in SA.

  // compact all the sorted substrings into
  //   the first n1 items of SA.
  // 2*n1 must be not larger than n.
  uint_t n1=0;
  for(i=0; i<n; i++)
    if((!level&&SA[i]>0) || (level&&((int_t *)SA)[i]>0))
      SA[n1++]=SA[i];

  #if DEBUG
  if(level==0) {
    printf("SA1\n");
    for(i=0; i<n1; i++)
          printf("%" PRIdN "\t", SA[i]);
    printf("\n");
  }
  #endif

  uint_t *SA1=SA, *s1=SA+m-n1;
  uint_t name_ctr;
  name_ctr=nameSubstr(SA,s,s1,n,m,n1,level,cs);

  // stage 2: solve the reduced problem.
  int_t depth=1;
  // recurse if names are not yet unique.
  if(name_ctr<n1)
    depth += SACA_K((int_t*)s1, SA1,
          n1, 0, m-n1, sizeof(int_t), level+1);
  else // get the suffix array of s1 directly.
    for(i=0; i<n1; i++) SA1[s1[i]]=i;

  // stage 3: induce SA(S) from SA(S1).

  #if DEBUG
  if(level==0) {
    printf("SA1\n");
    for(i=0; i<n1; i++)
          printf("%" PRIdN "\t", SA1[i]);
    printf("\n");
    printf("\nstage 3:\n\n");
  }
  #endif
  
  getSAlms(SA, s, s1, n, n1, level, cs);

  for(i=0; i<n1; i++) SA[i]=s1[SA[i]];
  for(i=n1; i<n; i++) SA[i]=level?EMPTY_k:0;

  #if DEBUG
  if(level==0) {
    printf("SA\n");
    for(i=0; i<n; i++)
      printf("%" PRIdN "\t", SA[i]);
    printf("\n");
  }
  #endif

  if(level==0) {

    #if TIME
      t_start(&t_time, &c_time);
    #endif

    putSuffix0(SA, s, bkt, n, K, n1, cs);

    #if TIME
      printf("SA (mapped):\n");
      t_stop(t_time, c_time);
    #endif

    #if DEBUG
    printf("SA (mapped)\n");
    for(i=0; i<n; i++)
          printf("%" PRIdN "\t", SA[i]);
    printf("\n");
    #endif

    #if TIME
      t_start(&t_time, &c_time);
    #endif

    induceSAl0(SA, s, bkt, n, K, true, cs);

    #if TIME
      printf("L-type:\n");
      t_stop(t_time, c_time);
    #endif

    #if TIME
      t_start(&t_time, &c_time);
    #endif
    induceSAs0(SA, s, bkt, n, K, true, cs);

    #if TIME
      printf("S-type:\n");
      t_stop(t_time, c_time);
    #endif

    free(bkt);
  }
  else {
    putSuffix1((int_t *)SA, (int_t *)s, n1, cs);
    induceSAl1((int_t *)SA, (int_t *)s, n, true, cs);
    induceSAs1((int_t *)SA, (int_t *)s, n, true, cs);
  }

  #if DEPTH
  printf("depth %" PRIdN ":\nname_ctr = %" PRIdN ", n1 =%" PRIdN ", n = %" PRIdN "\n", depth, name_ctr, n1, n);
  t_stop(t_start, c_start);
  #endif

return depth;
}

/*****************************************************************************/

int_t SACA_K_LCP(int_t *s, uint_t *SA, int_t *LCP,
  uint_t n, unsigned int K,
  uint_t m, int cs, int level) {
  uint_t i;
  uint_t *bkt=NULL;

  //avoid Use of uninitialised value (valgrind)
  for(i=0;i<n;i++) SA[i]=LCP[i]=0;
  
  #if TIME
    time_t t_time = 0;
    clock_t c_time = 0;
  #endif

  #if DEPTH
  time_t t_start = time(NULL);
  clock_t c_start =  clock();
  #endif

  // stage 1: reduce the problem by at least 1/2.
  bkt=(uint_t *)malloc(sizeof(int_t)*K);
  putSubstr0(SA, s, bkt, n, K, cs);

  #if DEBUG
  printf("bucket LMS-subs\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  #endif
  induceSAl0(SA, s, bkt, n, K, false, cs);

  #if DEBUG
  printf("L-type\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  #endif

  induceSAs0(SA, s, bkt, n, K, false, cs);

  #if DEBUG
  printf("S-type\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  #endif
    
    

  // now, all the LMS-substrings are sorted and
  //   stored sparsely in SA.

  // compact all the sorted substrings into
  //   the first n1 items of SA.
  // 2*n1 must be not larger than n.
  uint_t n1=0;
  for(i=0; i<n; i++)
    if(SA[i]>0)
      SA[n1++]=SA[i];

  #if DEBUG
  printf("SA1\n");
  for(i=0; i<n1; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  #endif

  uint_t *SA1=SA, *s1=SA+m-n1;
  uint_t name_ctr;

  #if DEBUG
  printf("\nSA\n");
  for(i=0; i<n; i++)
    printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif

  name_ctr=nameSubstr_LCP(SA,LCP,s,s1,n,m,n1,level,cs);

  #if DEBUG
  printf("nameSubstr:\n");
  printf("SA\n");
  for(i=0; i<n; i++)
    printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif

  // stage 2: solve the reduced problem.
  int_t depth=1;
  // recurse if names are not yet unique.
  if(name_ctr<n1)
    depth += SACA_K((int_t*)s1, SA1,
          n1, 0, m-n1, sizeof(int_t), level+1);
  else // get the suffix array of s1 directly.
    for(i=0; i<n1; i++) SA1[s1[i]]=i;

  // stage 3: induce SA(S) from SA(S1).
  #if DEBUG
  printf("recursive:\n");
  printf("SA\n");
  for(i=0; i<n; i++)
    printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif

  getSAlms(SA, s, s1, n, n1, level, cs);

  #if DEBUG
  printf("getSAlms:\n");
  printf("SA\n");
  for(i=0; i<n; i++)
    printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif

  uint_t *RA=s1;
  int_t *PLCP=LCP+m-n1;//PHI is stored in PLCP array

  #if TIME
    t_start(&t_time, &c_time);
  #endif

  //compute the LCP of consecutive LMS-suffixes
  compute_lcp_phi_sparse(s, SA1, RA, LCP, PLCP, n1, cs);

  #if TIME
  printf("PHI-algorithm (lms):\n");
  t_stop(t_time, c_time);
  #endif

  #if DEBUG
  printf("--\nSA1\n");
  for(i=0; i<n1; i++)//SA1
        printf("%" PRIdN "\t", SA1[i]+1);
  printf("| ");
  for(i=0; i<n1; i++)//s1
        printf("%" PRIdN "\t", RA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n1; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("| ");
  for(i=0; i<n1; i++)
        printf("%" PRIdN "\t", PLCP[i]);
  printf("\n");
  printf("\nstage 3:\n\n");
  #endif

  for(i=0; i<n1; i++) SA[i]=s1[SA[i]];
  for(i=n1; i<n; i++) SA[i]=level?EMPTY_k:U_MAX;
  for(i=n1;i<n;i++) LCP[i]=0;
    

  #if DEBUG
  printf("SA\n");
  for(i=0; i<n; i++)
    printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif

  #if TIME
    t_start(&t_time, &c_time);
  #endif

  putSuffix0_LCP(SA, LCP, s, bkt, n, K, n1, cs);

  #if TIME
    printf("SA (mapped):\n");
    t_stop(t_time, c_time);
  #endif

  #if DEBUG
  printf("SA (mapped)\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif

  #if TIME
    t_start(&t_time, &c_time);
  #endif
  induceSAl0_LCP(SA, LCP, s, bkt, n, K, cs);
  #if TIME
    printf("L-type:\n");
    t_stop(t_time, c_time);
  #endif
  #if DEBUG
  printf("L-type\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif
    

  #if TIME
    t_start(&t_time, &c_time);
  #endif

  induceSAs0_LCP(SA, LCP, s, bkt, n, K, cs);

  #if TIME
    printf("S-type:\n");
    t_stop(t_time, c_time);
  #endif

  #if DEBUG
  printf("S-type\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", SA[i]+1);
  printf("\n");
  printf("LCP\n");
  for(i=0; i<n; i++)
        printf("%" PRIdN "\t", LCP[i]);
  printf("\n\n");
  #endif
  free(bkt);

  #if DEPTH
  printf("depth %" PRIdN ":\nname_ctr = %" PRIdN ", n1 =%" PRIdN ", n = %" PRIdN "\n", depth, name_ctr, n1, n);
  t_stop(t_start, c_start);
  #endif

return depth;
}

/*****************************************************************************/

int sacak(unsigned char *s, uint_t *SA, uint_t n){
  if((s == NULL) || (SA == NULL) || (n < 0)) return -1;
  return SACA_K((int_t*)s, (uint_t*)SA, n, 256, n, sizeof(char), 0);
}

int sacak_int(int_t *s, uint_t *SA, uint_t n, uint_t k){
  if((s == NULL) || (SA == NULL) || (n < 0)) return -1;
  return SACA_K((int_t*)s, (uint_t*)SA, n, k, n, sizeof(int_t), 0);
}

int sacak_lcp(unsigned char *s, uint_t *SA, int_t* LCP, uint_t n){
  if((s == NULL) || (SA == NULL) || (LCP == NULL) || (n < 0)) return -1;
  return SACA_K_LCP((int_t*)s, (uint_t*)SA, (int_t*)LCP, n, 256, n, sizeof(char), 0);
}

int sacak_lcp_int(int_t *s, uint_t *SA, int_t* LCP, uint_t n, uint_t k){
  if((s == NULL) || (SA == NULL) || (LCP == NULL) || (n < 0)) return -1;
  return SACA_K_LCP((int_t*)s, (uint_t*)SA, (int_t*)LCP, n, k, n, sizeof(int_t), 0);
}

/*****************************************************************************/


void reformatSA(uint_t* SA, int n) {
    uint_t last_sa = SA[0];
    for(int i = 0; i < n-1; i++) {
        SA[i] = SA[i+1];
    }
    SA[n-1] = last_sa;
}

void reformatLCP(int* LCP, int n) {
    uint_t last_lcp = LCP[0];
    for(int i = 0; i < n-1; i++) {
        LCP[i] = LCP[i+1];
    }
    LCP[n-1] = last_lcp;
}

void setBWT(uint_t* SA, char* BWT, const char* Text, int n) {
    for(int i = 0; i < n; ++i) {
        char j = (SA[i])? Text[SA[i]-1]:'?';
        BWT[i] = j;
    }
}

void reformatSuffixArrays(const char* seq, uint_t *SA, int_t* LCP, char* BWT, uint_t n) {
    sacak_lcp((unsigned char *)seq, (uint_t*)SA, LCP, n);
    reformatSA(SA, n);
    reformatLCP(LCP, n);
    setBWT(SA, BWT, seq, n);
}