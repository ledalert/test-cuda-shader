#define BLOCK_SIZE 16     // block size

#define v3_v3_dot(a, b) (a.x * b.x + a.y * b.y + a.z * b.z)


__global__ void TestKernel( uchar4* dst,float3* normal_map, float3 cam_vec, unsigned int imgWidth, unsigned int imgHeight )
{
    unsigned int tx = threadIdx.x;
    unsigned int ty = threadIdx.y;
    unsigned int bw = blockDim.x;
    unsigned int bh = blockDim.y;
    // Non-normalized U, V coordinates of input texture for current thread.
    unsigned int u = ( bw * blockIdx.x ) + tx;
    unsigned int v = ( bh * blockIdx.y ) + ty;
    unsigned int index = ( v * imgWidth ) + u;



    float G = v3_v3_dot(normal_map[index], cam_vec);

    int g = G*127+127;

    dst[index] = make_uchar4(g, g, g, 255);

}



extern "C" void first_test(uchar4* g_dstBuffer, float3* normal_map,float3 cam_vec, int width, int height) {

	size_t blocksW = (size_t)ceilf( width / (float)BLOCK_SIZE );
	size_t blocksH = (size_t)ceilf( height / (float)BLOCK_SIZE );
	dim3 gridDim( blocksW, blocksH, 1 );
	dim3 blockDim( BLOCK_SIZE, BLOCK_SIZE, 1 );
	TestKernel<<< gridDim, blockDim >>>( g_dstBuffer, normal_map, cam_vec, width, height );


}