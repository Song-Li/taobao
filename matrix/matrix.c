#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "matrix.h"

static int g_seed = 0;

static ssize_t g_width = 0;
static ssize_t g_height = 0;
static ssize_t g_elements = 0;

static ssize_t g_nthreads = 4;

struct arg_struct {
    float * matrix_a, *matrix_b, *result;
    float arg1, arg2;
    int from, to;
};

pthread_t threads[50];

////////////////////////////////
///     UTILITY FUNCTIONS    ///
////////////////////////////////

/**
 * Returns pseudorandom number determined by the seed.
 */
int fast_rand(void) {

	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

/**
 * Sets the seed used when generating pseudorandom numbers.
 */
void set_seed(int seed) {

	g_seed = seed;
}

/**
 * Sets the number of threads available.
 */
void set_nthreads(ssize_t count) {

	g_nthreads = count;
}

/**
 * Sets the dimensions of the matrix.
 */
void set_dimensions(ssize_t order) {

	g_width = order;
	g_height = order;

	g_elements = g_width * g_height;
}

/**
 * Displays given matrix.
 */
void display(const float* matrix) {

	for (ssize_t y = 0; y < g_height; y++) {
		for (ssize_t x = 0; x < g_width; x++) {
			if (x > 0) printf(" ");
			printf("%.2f", matrix[y * g_width + x]);
		}

		printf("\n");
	}
}

/**
 * Displays given matrix row.
 */
void display_row(const float* matrix, ssize_t row) {

	for (ssize_t x = 0; x < g_width; x++) {
		if (x > 0) printf(" ");
		printf("%.2f", matrix[row * g_width + x]);
	}

	printf("\n");
}

/**
 * Displays given matrix column.
 */
void display_column(const float* matrix, ssize_t column) {

	for (ssize_t i = 0; i < g_height; i++) {
		printf("%.2f\n", matrix[i * g_width + column]);
	}
}

/**
 * Displays the value stored at the given element index.
 */
void display_element(const float* matrix, ssize_t row, ssize_t column) {

	printf("%.2f\n", matrix[row * g_width + column]);
}

////////////////////////////////
///   MATRIX INITALISATIONS  ///
////////////////////////////////

/**
 * Returns new matrix with all elements set to zero.
 */
float* new_matrix(void) {

	return calloc(g_elements, sizeof(float));
}

/**
 * Returns new identity matrix.
 */
float* identity_matrix(void) {

	float* result = new_matrix();
	/*
		TODO

		1 0
		0 1
	*/

    for(ssize_t i = 0; i < g_height; i++)
    {
        result[i * g_width + i] = 1.0f;
    }

	return result;
}

/**
 * Returns new matrix with elements generated at random using given seed.
 */
float* random_matrix(int seed) {

	float* result = new_matrix();

	set_seed(seed);

	for (ssize_t i = 0; i < g_elements; i++) {
		result[i] = fast_rand();
	}

	return result;
}

/**
 * Returns new matrix with all elements set to given value.
 */
float* uniform_matrix(float value) {

	float* result = new_matrix();

    for (ssize_t i = 0; i < g_elements; i++)
    {
        result[i] = value;
    }

	return result;
}

/**
 * Returns new matrix with elements in sequence from given start and step
 */
float* sequence_matrix(float start, float step) {

	float* result = new_matrix();

	/*
		TODO

		       1 2
		1 1 => 3 4
	*/

    float val = start;
    for (ssize_t i = 0; i < g_elements; ++i)
    {
        result[i] = val;
        val += step;
    }

	return result;
}

////////////////////////////////
///     MATRIX OPERATIONS    ///
////////////////////////////////

/**
 * Returns new matrix with elements cloned from given matrix.
 */
float* cloned(const float* matrix) {

	float* result = new_matrix();

    memcpy(result, matrix, sizeof(float) * g_elements);

	return result;
}

/**
 * Returns new matrix with elements in ascending order.
 */
int compare_float(const void *a, const void *b)
{
    return (*(float*)a >= *(float*)b) ? 1 : -1;
}


float* sorted(const float* matrix) {

	float* result = new_matrix();

	/*
		TODO

		3 4    1 2
		2 1 => 3 4

	*/
    memcpy(result, matrix, sizeof(float) * g_elements);
    qsort(result, g_elements, sizeof(float), compare_float);

	return result;
}

/**
 * Returns new matrix with elements rotated 90 degrees clockwise.
 */
float* rotated(const float* matrix) {

	float* result = new_matrix();

	/*
		TODO

		1 2    3 1
		3 4 => 4 2
	*/
    
    for(ssize_t i = 0; i < g_height; i++)
    {   
        for(ssize_t j = 0; j < g_width; j++)
        {
            result[i * g_width + j] = matrix[(g_height - 1 - j) * g_width + i];
        }
    }

	return result;
}

/**
 * Returns new matrix with elements ordered in reverse.
 */
float* reversed(const float* matrix) {

	float* result = new_matrix();

	/*
		TODO

		1 2    4 3
		3 4 => 2 1
	*/
    for(ssize_t i = 0; i < g_elements; i++)
    {
        result[i] = matrix[g_elements - i - 1];
    }
	return result;
}

/**
 * Returns new transposed matrix.
 */
float* transposed(const float* matrix) {

	float* result = new_matrix();

	/*
		TODO

		1 2    1 3
		3 4 => 2 4
    ssize_t count = 0.0f;
	*/

    for(ssize_t i = 0; i < g_height; i++)
    {
        for(ssize_t j = 0; j < g_width; j++)
        {
            result[i * g_width + j] = matrix[j * g_width + i];
        }
    }

	return result;
}

/**
 * Returns new matrix with scalar added to each element.
 */

float* scalar_add(const float* matrix, float scalar) {

	float* result = new_matrix();

	/*
		TODO

		1 0        2 1
		0 1 + 1 => 1 2

		1 2        5 6
		3 4 + 4 => 7 8
	*/

    for(ssize_t i = 0; i < g_elements; i++)
        result[i] = matrix[i] + scalar;

	return result;
}

/**
 * Returns new matrix with scalar multiplied to each element.
 */


float* scalar_mul(const float* matrix, float scalar) {

	float* result = new_matrix();

    for (ssize_t i = 0;i < g_elements;++ i)
        result[i] = matrix[i] * scalar;

	return result;
}

void* do_add(void *arguments)
{
    struct arg_struct *args = (struct arg_struct *)arguments;
    float * result = args->result,* matrix_a = args->matrix_a, *matrix_b = args->matrix_b;
    ssize_t to = args->to;
    for(size_t i = args->from;i < to;++ i)
        result[i] = matrix_a[i] + matrix_b[i];

    pthread_exit(NULL);
    return NULL;
}

/**
 * Returns new matrix that is the result of
 * adding the two given matrices together.
 */
float* matrix_add(const float* matrix_a, const float* matrix_b) {

	float* result = new_matrix();

    for (ssize_t i = 0;i < g_elements;++ i)
        result[i] = matrix_a[i] + matrix_b[i];

    return result;

    struct arg_struct args[32];

    for (size_t i = 0; i < g_nthreads;++ i){
        args[i].matrix_a = (float *)matrix_a;
        args[i].matrix_b = (float *)matrix_b;
        args[i].result = result;
        args[i].from = i * g_elements / g_nthreads;
        if(i != g_nthreads - 1) args[i].to = g_elements / g_nthreads * (i + 1);
        else args[i].to = g_elements;
        pthread_create(&threads[i], NULL, do_add, &args[i]);
    }
    for (size_t i = 0;i < g_nthreads;++ i) pthread_join(threads[i], NULL);

	return result;
}

/**
 * Returns new matrix that is the result of
 * multiplying the two matrices together.
 */
float* matrix_mul(const float* matrix_a, const float* matrix_b) {

	float* result = new_matrix();
    int base, inbase;

    for (ssize_t i = 0; i < g_height; i++)
    {
        base = i * g_width;
        for (ssize_t k = 0; k < g_width; k++)
        {
            float c = matrix_a[i * g_width + k];
            inbase = k * g_width;
            for (ssize_t j = 0; j < g_width; j++)
            {
                result[i * g_width + j] += c * matrix_b[k * g_width + j];
            }
        }
    }

	return result;
}

/**
 * Returns new matrix that is the result of
 * powering the given matrix to the exponent.
 */
float* matrix_pow(const float* matrix, int exponent) {

	float* result = new_matrix();

    if (!exponent)
    {
        for(ssize_t i = 0; i < g_height; i++)
        {
            result[i * g_width + i] = 1.0f;
        }
    }
    else if (exponent == 1)
    {
        memcpy(result, matrix, sizeof(float) * g_elements);
    }
    else
    {
        float* half = matrix_pow(matrix, exponent >> 1);
        float* half_2 = matrix_mul(half, half);
        if (!(exponent & 1))
            memcpy(result, half_2, g_elements * sizeof(float));
        else
        {
            float* final = matrix_mul(half_2, matrix);
            memcpy(result, final, g_elements * sizeof(float));
            free(final);
        }
        free(half);
        free(half_2);
    }

	return result;
}

/**
 * Returns new matrix that is the result of
 * convolving given matrix with a 3x3 kernel matrix.
 */

#define CONV_L(i) ((i == 0u) ? i : i - 1)
#define CONV_R(i) ((i == g_width - 1) ? i : i + 1)
#define CONV_U(i) ((i == 0u) ? i : i - 1)
#define CONV_D(i) ((i == g_height - 1) ? i : i + 1)

float conv_element(const float* matrix, const float* kernel, ssize_t r, ssize_t c)
{
    double sum = 0.0;

    sum += kernel[0] * matrix[CONV_U(r) * g_width + CONV_L(c)];
    sum += kernel[1] * matrix[CONV_U(r) * g_width + c];
    sum += kernel[2] * matrix[CONV_U(r) * g_width + CONV_R(c)];

    sum += kernel[3] * matrix[r * g_width + CONV_L(c)];
    sum += kernel[4] * matrix[r * g_width + c];
    sum += kernel[5] * matrix[r * g_width + CONV_R(c)];

    sum += kernel[6] * matrix[CONV_D(r) * g_width + CONV_L(c)];
    sum += kernel[7] * matrix[CONV_D(r) * g_width + c];
    sum += kernel[8] * matrix[CONV_D(r) * g_width + CONV_R(c)];

    return sum;
}


float* matrix_conv(const float* matrix, const float* kernel) {

	float* result = new_matrix();

	/*
		TODO

		Convolution is the process in which the values of a matrix are
		computed according to the weighted sum of each value and it's
		neighbours, where the weights are given by the kernel matrix.
	*/
    for(ssize_t i = 0; i < g_height; i++)
    {
        for(ssize_t j = 0; j < g_width; j++)
        {
            result[i * g_width + j] = conv_element(matrix, kernel, i, j);
        }
    }

	return result;
}

////////////////////////////////
///       COMPUTATIONS       ///
////////////////////////////////

/**
 * Returns the sum of all elements.
 */
float get_sum(const float* matrix) {

	/*
		TODO

		2 1
		1 2 => 6

		1 1
		1 1 => 4
	*/

    float sum = 0.0f;
    for(ssize_t i = 0; i < g_elements; i++)
    {
        sum += matrix[i];
    }

	return sum;
}

/**
 * Returns the trace of the matrix.
 */
float get_trace(const float* matrix) {

    float trace = 0.0f;

    for(ssize_t i = 0; i < g_height; i++)
    {
        trace += matrix[i * g_width + i];
    }

	return trace;
}

#define min(a,b) (a) < (b) ? (a) : (b)
#define max(a,b) (a) > (b) ? (a) : (b)

/**
 * Returns the smallest value in the matrix.
 */
float get_minimum(const float* matrix) {

    float minimum = matrix[0];
    for(ssize_t i = 1; i < g_elements; i++)
    {
        minimum = min(minimum, matrix[i]);
    }
	return minimum;
}

/**
 * Returns the largest value in the matrix.
 */
float get_maximum(const float* matrix) {

    float maximum = matrix[0];
    for(ssize_t i = 1; i < g_elements; i++)
    {
        maximum = max(maximum, matrix[i]);
    }
	return maximum;
}

/**
 * Returns the determinant of the matrix.
 */

float calculate_determinant(const float** d_matrix, ssize_t order)
{
    if (order == 1)
    {
        return d_matrix[0][0];
    }
    else if (order == 2)
        return d_matrix[0][0] * d_matrix[1][1] - d_matrix[0][1] * d_matrix[1][0];
    else
    {
        double sum = 0.0f;
        const float** n_matrix = (const float**)(malloc(sizeof(float*) * order));
        const float* tmp = NULL;
        for(ssize_t i = 0; i < order; i++)
        {
            n_matrix[i] = d_matrix[i];
        }

        for(ssize_t i = 0; i < order; i++)
        {
            float sign = ((order - 1 + i) % 2) ? -1.0f : 1.0f;
            /* To swap */
            tmp = n_matrix[0];
            n_matrix[0] = n_matrix[i];
            n_matrix[i] = tmp;

            if (d_matrix[i][order - 1] == 0)
                ;
            else
                sum += sign * calculate_determinant(&n_matrix[1], order - 1) * d_matrix[i][order - 1];
        }
        
        free(n_matrix);
        return sum;
    }
}


float get_determinant(const float* matrix) {

	/*
		TODO

		1 0
		0 1 => 1

		1 2
		3 4 => -2

		8 0 2
		0 4 0
		2 0 8 => 240
	*/
    const float** d_matrix = (const float**)(malloc(sizeof(float*) * g_height));
    for(ssize_t i = 0; i < g_height;  i++)
    {
        d_matrix[i] = &matrix[i * g_width];
    }
    
    float determinant = calculate_determinant(d_matrix, g_height);
    free(d_matrix);

	return determinant;
}

/**
 * Returns the frequency of the given value in the matrix.
 */
ssize_t get_frequency(const float* matrix, float value) {

    ssize_t count = 0;
    for(ssize_t i = 0; i < g_elements; i++)
    {
        if (matrix[i] == value) count++;
    }

    return count;
}

