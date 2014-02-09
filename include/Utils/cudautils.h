#pragma once
// CUDA runtime
#include <cuda_runtime.h>

// helper functions and utilities to work with CUDA
#include <helper_functions.h>
#include <helper_cuda.h>

#include "fileutils.h"

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
__host__ inline void writeback(T* ptr, int size, const string& filename) {
	vector<T> v(size);
	checkCudaErrors(cudaMemcpy(&(v[0]), ptr, sizeof(T)*size, cudaMemcpyDeviceToHost));
	PhGUtils::write2file(v, filename);
}

template <typename T> 
__host__ inline void writeback(T* ptr, int rows, int cols, const string& filename) {
	vector<T> v(rows*cols);
	checkCudaErrors(cudaMemcpy(&(v[0]), ptr, sizeof(T)*rows*cols, cudaMemcpyDeviceToHost));
	ofstream fout(filename);
	if( !fout ) {
		PhGUtils::error("failed to write file " + filename);
		return;
	}
	PhGUtils::print2DArray(&v[0], rows, cols, fout);
	fout.close();
}