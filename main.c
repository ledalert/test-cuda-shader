#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <png.h>
#include <string.h>

#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>
//#include <driver_types.h>






void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void read_png_file(char* file_name, png_bytep** row_pointers, int* width, int* height);


static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}


static void cursor_callback (GLFWwindow* window, double x, double y) {
	printf("Cursor: %f, %f\n", x, y);
}


int init() {


	glfwSetErrorCallback(error_callback);
	return glfwInit();

}




int window_loop() {

	GLFWwindow* window;
	
	window = glfwCreateWindow(640, 480, "Shader test", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetKeyCallback(window, key_callback);

	//glfwSetCursorPosCallback(window, cursor_callback);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHT0);
	//glEnable(GL_DEPTH_TEST);
//	glEnable(GL_LIGHTING);

//	glEnable(GL_BLEND);
	// glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	// glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
//	glEnable(GL_NORMALIZE);  
	//glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	// glPolygonMode( GL_FRONT, GL_LINE );
	// glPolygonMode( GL_BACK, GL_POINT );

//	glEnable(GL_COLOR_MATERIAL);

	cudaGLSetGLDevice(0);


	double ot, nt = glfwGetTime();

    GLuint textureID[6];
    glGenTextures(1, textureID);
    
    png_bytep* tex1;
    int lw, lh; 
   	printf("Laddar PNG\n");
    read_png_file("/srv/texturer/Slate Tiles - (Normal Map).png", &tex1, &lw, &lh);

    printf("Laddade textur som är %i x %i pixelitaz stor.\n", lw, lh);

	float3* normal_map = NULL;

	size_t normal_map_bufferSize = 1024 * 1024 * sizeof(float3);
	cudaMalloc( &normal_map, normal_map_bufferSize );
    float3* host_normal_map = calloc(1024*1024, sizeof(float3));



    glBindTexture(GL_TEXTURE_2D, textureID[0]);
    
    for (int y=0; y<1024; y++) {
    	for (int x=0; x<1024; x++) {
    		host_normal_map[y*1024+x].x = (float)(tex1[y][x*3+0]-127) / 127;
    		host_normal_map[y*1024+x].y = (float)(tex1[y][x*3+1]-127) / 127;
    		host_normal_map[y*1024+x].z = (float)(tex1[y][x*3+2]-127) / 127;
    	}
    }

    cudaMemcpy(normal_map, host_normal_map, normal_map_bufferSize, cudaMemcpyHostToDevice);

    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double cx, cy;
	glfwGetCursorPos(window, &cx, &cy);


	struct cudaGraphicsResource *test1;
	int r1=cudaGraphicsGLRegisterImage(&test1, textureID[0], GL_TEXTURE_2D, cudaGraphicsMapFlagsWriteDiscard);
	printf("r1=%i\n");

	uchar4* g_dstBuffer = NULL;

	size_t bufferSize = 1024 * 1024 * sizeof(uchar4);
	cudaMalloc( &g_dstBuffer, bufferSize );

	cudaMemset(g_dstBuffer, 0x7F, bufferSize);	//Make texture gray to start with

	printf("cuda alloc: %p\n", g_dstBuffer);

	double fps_time =0 ;
	int fps_count=0;

	while (!glfwWindowShouldClose(window))
	{

		ot=nt;
		nt =glfwGetTime();
		float dt = nt - ot;


		fps_time += dt;
		fps_count++;

		if (fps_time > 1) {
			printf("FPS: %f\n", fps_count/fps_time);
			fps_time=0;
			fps_count =0;

		}




		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		

		glClearColor(0.0, 0.0, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glViewport(0, 0, width-1, height-1);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, width-1, height-1, 0,0,1);

		glMatrixMode(GL_MODELVIEW);


		for (int testa_flera=0; testa_flera<16; testa_flera++) {

			glLoadIdentity();			
			glTranslatef(testa_flera*150, testa_flera*50+100, 0);
			glRotatef(testa_flera*10, 0,0,1);
			glTranslatef(0, testa_flera*50, 0);
			glScalef(0.5, 0.5, 0.5);


			float ta = fmod(nt+testa_flera*0.2, M_PI*2.0);
			float tb = fmod(nt*0.7+testa_flera*0.4, M_PI*2.0);
			float tc = fmod(nt*0.3+testa_flera*0.1, M_PI*2.0);
			float3 cam_vec = {sin(ta), sin(tb), sin(tc)};

			int res=cudaGraphicsMapResources(1, &test1, 0);
			//printf("res: %i (succ=%i)\n", res, cudaSuccess);
			struct cudaArray* dstArray = 0;
			int r2 = cudaGraphicsSubResourceGetMappedArray( &dstArray, test1, 0, 0 );
			//printf("r2: %i array: %p\n", r2, dstArray);

			first_test(g_dstBuffer, normal_map, cam_vec, 1024, 1024);

			cudaMemcpyToArray( dstArray, 0, 0, g_dstBuffer, bufferSize, cudaMemcpyDeviceToDevice );

			cudaGraphicsUnmapResources(1, &test1, 0);

			glColor3f(1,1,1);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex3f(0,0,0);

				glTexCoord2f(1,0);
				glVertex3f(511,0,0);

				glTexCoord2f(1,1);
				glVertex3f(511,511,0);

				glTexCoord2f(0,1);
				glVertex3f(0,511,0);
			glEnd();



		}






		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	

	glfwDestroyWindow(window);
	
	glfwTerminate();
	return(EXIT_SUCCESS);
}


int main(void) {
	init();
	window_loop();
}