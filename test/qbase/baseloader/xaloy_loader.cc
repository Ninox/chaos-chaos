#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "baseloader.h"
#include <xaloy.h>
using namespace xaloy;

//	function pointer declaration
typedef int (*normal_add)(int,int);
typedef int (*normal_minus)(int,float);
typedef int (*mtrx_dot)();
typedef int (*mtrx_sum)();
typedef void (*renderfunc)();
typedef void (*sayfunc)(const char*);

XALOY_TEST_MODULE(baseloader)
{
	normal_add nadd = NULL;
	normal_minus nmin = NULL;
	mtrx_dot mdot = NULL;
	mtrx_sum msum = NULL;
	renderfunc render = NULL;
	sayfunc saying = NULL;
	sayfunc helloer = NULL;
	// init the plugin
	// memory waste here, 32KB per structure, but first initalize cost more than 32KB, in LUA
	qbase_loader_init("OGL_Render");
	qbase_loader_init("math_a");
	qbase_loader_init("math_b");
	qbase_loader_init("strhelper");

	// get function
	nadd = (normal_add)qbase_loader_getf("math_a", "add");
	XALOY_ASSERT_NULL(XL_NOTNULL, nadd);
	
	nmin = (normal_minus)qbase_loader_getf("math_a", "min");
	XALOY_ASSERT_NULL(XL_NOTNULL, nmin);
	
	mdot = (mtrx_dot)qbase_loader_getf("math_b", "dot");
	XALOY_ASSERT_NULL(XL_NOTNULL, mdot);
	
	msum = (mtrx_sum)qbase_loader_getf("math_b", "sum");
	XALOY_ASSERT_NULL(XL_NOTNULL, msum);
	
	render = (renderfunc)qbase_loader_getf("OGL_Render", "renderit");
	XALOY_ASSERT_NULL(XL_NOTNULL, render);
	
	saying = (sayfunc)qbase_loader_getf("strhelper", "say_somthing");
	XALOY_ASSERT_NULL(XL_NOTNULL, saying);
	
	helloer = (sayfunc)qbase_loader_getf("strhelper", "helloer");
	XALOY_ASSERT_NULL(XL_ISNULL, helloer);	// is null
	
	helloer = (sayfunc)qbase_loader_getf("str2helper", "helloer");
	XALOY_ASSERT_NULL(XL_ISNULL, helloer); // is null
	

	XALOY_EXPECT(XL_EQUAL, nadd(3,9), 12);
	XALOY_EXPECT(XL_EQUAL, nmin(-3,-0.12f), -2.88f);
	XALOY_EXPECT(XL_EQUAL, mdot(), -2.0f);
	XALOY_EXPECT(XL_EQUAL, msum(), 10.0f);

	// release on plugin
	qbase_loader_free("math_a");
	XALOY_EXPECT_NULL(XL_ISNULL, nadd);
	XALOY_EXPECT_NULL(XL_ISNULL, nmin);
	// destory the plugin list
	qbase_loader_destory();
	XALOY_EXPECT_NULL(XL_ISNULL, render);

    getchar();
}

int main()	{	
	return GLOBAL_TEST->execute(true);
}
