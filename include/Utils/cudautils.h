#pragma once
// CUDA runtime
#include <cuda_runtime.h>

// helper functions and utilities to work with CUDA
#include <helper_functions.h>
#include <helper_cuda.h>

#include <cublas.h>

#include "utility.hpp"
#include "fileutils.h"

//#define CHECKCUDASTATE_DEBUG

__host__ inline void checkCudaState_impl(const char* file = __FILE__ , int line = __LINE__ ) {
	cudaThreadSynchronize();
	cudaError_t err = cudaGetLastError();
	if (err != cudaSuccess)  {
		fprintf(stderr, "CUDA error at %s: line# %d\t%s\n",file, line, cudaGetErrorString(err));
		::system("pause");
		//exit(-1);
	}
}

#ifdef CHECKCUDASTATE_DEBUG
#define checkCudaState() checkCudaState_impl(__FILE__, __LINE__)
#else
#define checkCudaState()
#endif

static const char *cublasGetErrorString(cublasStatus_t error)
{
    switch (error)
    {
        case CUBLAS_STATUS_SUCCESS:
            return "CUBLAS_STATUS_SUCCESS";

        case CUBLAS_STATUS_NOT_INITIALIZED:
            return "CUBLAS_STATUS_NOT_INITIALIZED";

        case CUBLAS_STATUS_ALLOC_FAILED:
            return "CUBLAS_STATUS_ALLOC_FAILED";

        case CUBLAS_STATUS_INVALID_VALUE:
            return "CUBLAS_STATUS_INVALID_VALUE";

        case CUBLAS_STATUS_ARCH_MISMATCH:
            return "CUBLAS_STATUS_ARCH_MISMATCH";

        case CUBLAS_STATUS_MAPPING_ERROR:
            return "CUBLAS_STATUS_MAPPING_ERROR";

        case CUBLAS_STATUS_EXECUTION_FAILED:
            return "CUBLAS_STATUS_EXECUTION_FAILED";

        case CUBLAS_STATUS_INTERNAL_ERROR:
            return "CUBLAS_STATUS_INTERNAL_ERROR";
    }

    return "<unknown>";
}

__host__ inline void showCUDAMemoryUsage(const char* str = NULL)
{
	size_t free_byte ;
	size_t total_byte ;
	cudaError_t cuda_status = cudaMemGetInfo( &free_byte, &total_byte ) ;
	if ( cudaSuccess != cuda_status ){
		printf("Error: cudaMemGetInfo fails, %s \n", cudaGetErrorString(cuda_status) );
		exit(1);
	}
	
	double free_db = (double)free_byte ;
	double total_db = (double)total_byte ;
	double used_db = total_db - free_db ;
	if(str != NULL)
	{
		printf("%s GPU memory usage: \n\tused = %f \n\tfree = %f MB \n\ttotal = %f MB\n", str,
			used_db/1024.0/1024.0, free_db/1024.0/1024.0, total_db/1024.0/1024.0);
	} else
	{
		printf("GPU memory usage: \n\tused = %f \n\tfree = %f MB \n\ttotal = %f MB\n",
			used_db/1024.0/1024.0, free_db/1024.0/1024.0, total_db/1024.0/1024.0);
	}
}

template <typename T>
__host__ inline void cudaAllocate(T** ptr, size_t count) {
	cudaMalloc(ptr, sizeof(T)*count);
	checkCudaState();
}

template <typename T> 
__host__ inline void writeback(T* ptr, int size, const string& filename) {
	vector<T> v(size);
	cudaMemcpy(&(v[0]), ptr, sizeof(T)*size, cudaMemcpyDeviceToHost);
	PhGUtils::write2file(v, filename);
}

template <typename T> 
__host__ inline void writeback(T* ptr, int rows, int cols, const string& filename) {
	vector<T> v(rows*cols);
	cudaMemcpy(&(v[0]), ptr, sizeof(T)*rows*cols, cudaMemcpyDeviceToHost);
	ofstream fout(filename);
	if( !fout ) {
		PhGUtils::error("failed to write file " + filename);
		return;
	}
	PhGUtils::print2DArray(&v[0], rows, cols, fout);
	fout.close();
}

__host__ __inline__ ostream& operator<<( ostream& os, const float3& v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

__host__ __inline__ ostream& operator<<( ostream& os, const float4& v) {
	os << v.x << " " << v.y << " " << v.z << " " << v.w << " ";
	return os;
}