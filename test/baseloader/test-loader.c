#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "baseloader.h"

//	function pointer declaration
typedef int (*normal_add)(int,int);
typedef int (*normal_minus)(int,float);
typedef int (*mtrx_dot)();
typedef int (*mtrx_sum)();
typedef void (*renderfunc)();
typedef void (*sayfunc)(const char*);

int main()	{
	normal_add nadd = NULL;
	normal_minus nmin = NULL;
	mtrx_dot mdot = NULL;
	mtrx_sum msum = NULL;
	renderfunc render = NULL;
	sayfunc saying = NULL;
    printf("press for start...\n");
    getchar();
    printf("==================  initalize start ==================\n");
	// init the plugin
	// memory waste here, 32KB per structure, but first initalize cost more than 32KB, in LUA
	qbase_loader_init("OGL_Render");
	qbase_loader_init("math_a");
	qbase_loader_init("math_b");
	qbase_loader_init("strhelper");

    getchar();
	printf("==================  get func start ==================\n");
	// get function
	nadd = (normal_add)qbase_loader_getf("math_a", "add");
	nmin = (normal_minus)qbase_loader_getf("math_a", "min");
	mdot = (mtrx_dot)qbase_loader_getf("math_b", "dot");
	msum = (mtrx_sum)qbase_loader_getf("math_b", "sum");
	render = (renderfunc)qbase_loader_getf("OGL_Render", "renderit");
	saying = (sayfunc)qbase_loader_getf("strhelper", "say_somthing");

	printf("press for continue...\n");
    getchar();
	// test function
	assert(nadd != NULL);					// checked whether the function can load successfully
	assert(nmin != NULL);
	assert(mdot != NULL);
	assert(msum != NULL);
	assert(render != NULL);
	assert(saying != NULL);

	assert(nadd(3,9) == 12);	// checked whether the function execute successfully
	assert(nmin(-3,-0.12f));
	assert(mdot() == -2.0f);
	assert(msum() == 10.0f);
	render();
	saying("yogi");

    printf("press for continue (release)...\n");
    getchar();
	// release on plugin
	qbase_loader_free("math_a")	;
	// destory the plugin list
	qbase_loader_destory();

    printf("press for ending \n");
    getchar();
	return 0;
}
