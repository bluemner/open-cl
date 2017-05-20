// Description :: Simple kernel takes the size and time A[ element i]
__kernel void test(const int size, __constant  int *A, __global int *B){
	int id = get_global_id(0);
	B[id] =  size *  A[id];
}

