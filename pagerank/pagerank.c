#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <immintrin.h>

#include "pagerank.h"
#define float double
#define MAX 8192

float result[MAX], tr[MAX];
size_t size;
size_t nt;
float d;
float matrix[MAX * MAX];
float eps;

void output(){
    for(int i = 0;i < size;++ i, printf("\n"))
        for(int j = 0;j < size;++ j)
            printf("%f ",matrix[i * size + j]);
}

float *getMatrix(node *list, size_t npages){
    //float *matrix = (float *)calloc(npages * npages, sizeof(float));
    //result = (float *)calloc(npages, sizeof(float));
    //tr = (float *)malloc(npages * sizeof(float));

    for(int i = 0;i < npages;++ i)
        tr[i] = 1.0 / (float)npages;


    node *now = list, *toThis;
    while(now){
        toThis = now->page->inlinks;
        while(toThis){
            matrix[now->page->index * npages + toThis->page->index] = 1.0 / (float)toThis->page->noutlinks;
            toThis = toThis->next;
        }

        if(now->page->noutlinks == 0)
            for(int i = 0;i < npages;++ i)
                matrix[i * npages + now->page->index] = 1.0 / (float)npages;
        now = now->next;
    }

    __m256d aa, dd, rr;
    dd = _mm256_set_pd(d,d,d,d);
    //tt = _mm256_set_pd(tmp,tmp,tmp,tmp);
    npages = npages * npages;
    const size_t chunk_size = 4;
    const size_t chunks = npages / chunk_size;

    for(int i = 0;i + 3 < npages;i += 4){
        aa = _mm256_loadu_pd(&matrix[i]);
        rr = _mm256_mul_pd(aa,dd);
     //   rr = _mm256_add_pd(rr,tt);
        _mm256_store_pd(&matrix[i], rr);
    }

    for(size_t i = chunk_size * chunks; i < npages;++ i)
        matrix[i] = matrix[i] * d; 

    return matrix;
}

pthread_barrier_t barr;



void *runPage(void *arg){
    int id = (int) arg;
    const size_t block = size / nt; 
    const float tmp = (1.0 - d) / size;
    size_t from = block * id, to = from + block;
    size_t base = 0;

    const size_t chunk_size = 4;
    const size_t chunks = size / chunk_size;

    if(id == nt - 1) {
        from = block * (nt - 1);
        to = size;
    }

    /*
       for(size_t i = from;i < to;++ i){
       base = i * size;
       for(size_t j = 0;j < size;++ j){
       tr[i] += matrix[base + j] * result[j];
       }
       }
     */

    double tmp_double;
    __m256d aa, bb, rr;
    for(size_t i = from;i < to;++ i){
        tr[i] += tmp;
        base = i * size;
        for(size_t j = 0;j < chunks;++ j){
            aa = _mm256_loadu_pd(&matrix[base + j * chunk_size]);
            bb = _mm256_loadu_pd(&result[j * chunk_size]);
            rr = _mm256_mul_pd(aa,bb);
            __m256d hsum = _mm256_add_pd(rr, _mm256_permute2f128_pd(rr, rr, 0x1));
            _mm_store_sd(&tmp_double, _mm_hadd_pd( _mm256_castpd256_pd128(hsum), _mm256_castpd256_pd128(hsum) ) );
            tr[i] += tmp_double;
        }
        for(size_t j = chunk_size * chunks; j < size;++ j)
            tr[i] += matrix[base + j] * result[j];
    }
    pthread_barrier_wait(&barr);
    return NULL;
}

int check(){
    float res = 0;
    for(int i = 0;i < size;++ i)
        res += (tr[i] - result[i]) * (tr[i] - result[i]);
    return res <= eps;
}

void pagerank(node* list, size_t npages, size_t nedges, size_t nthreads, double dampener) {
    d = (float)dampener;
    nthreads = 8;

    size = npages;
    nt = nthreads;
    eps = 0.005 * 0.005;
    pthread_barrier_init(&barr, NULL, nthreads);
    getMatrix(list, npages);


    pthread_t threads[16];
    while(!check()){
        memcpy(result, tr, sizeof(float) * size);
        memset(tr,0,sizeof(float) * size);
        for(long i = 0;i < nthreads - 1;++ i)
            pthread_create(&threads[i], NULL, runPage, (void *)i);
        runPage((void *)(nthreads - 1));
        for(int i = 0;i < nthreads - 1;++ i)
            pthread_join(threads[i], NULL);
    }
    node * now = list;
    while(now){
        printf("%s %.8lf\n", now->page->name, tr[now->page->index]);
        now = now->next;
    }
}

/*
######################################
### DO NOT MODIFY BELOW THIS POINT ###
######################################
 */

int main(int argc, char** argv) {

	/*
	######################################################
	### DO NOT MODIFY THE MAIN FUNCTION OR HEADER FILE ###
	######################################################
	*/

	config conf;

	init(&conf, argc, argv);

	node* list = conf.list;
	size_t npages = conf.npages;
	size_t nedges = conf.nedges;
	size_t nthreads = conf.nthreads;
	double dampener = conf.dampener;

	pagerank(list, npages, nedges, nthreads, dampener);

	release(list);

	return 0;
}
