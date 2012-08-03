#include <stdlib>
#include <time.h>
#include <xaloy.h>
#include <Quab/qbase/qbase.h>
using namespace xaloy;

#define PI 3.1415f
#define XEXPECT XaloyTester::Expect
#define XASSERT XaloyTester::Assert
#define XASSERT_N XaloyTester::Assert_null

XALOY_TEST_MODULE(baseloader){
	// initalize the test module
	qbase_loader_init("module");
	qbase_loader_init("module_inner");
	qbase_loader_init("module_inner_inner");
	qbase_loader_init("module_outside");
	qbase_loader_init("module_outside_outside");
	
	// getfunc in modules
	testfoo1 cal = (testfoo1)qbase_loader_getf("module", "calculate");
	testfoo1 fab = (testfoo1)qbase_loader_getf("module", "fab");
	testfoo2 summ = (testfoo2)qbase_loader_getf("module_inner", "summ");
	testfoo3 minus = (testfoo3)qbase_loader_getf("module_inner_inner", "minus");
	testfoo3 isnull_1 = (testfoo3)qbase_loader_getf("module_inner_inner", "isnull");
	testfoo4 foo = (testfoo4)qbase_loader_getf("module_inner_inner", "foo");
	testfoo5 big_foo = (testfoo5)qbase_loader_getf("module_outside", "big_foo");
	testfoo5 search = (testfoo5)qbase_loader_getf("module_outside_outside", "search");
	testfoo6 isnull_2 = (testfoo6)qbase_loader_getf("module_outside", "isnull");
	testfoo7 cannot = (testfoo7)qbase_loader_getf("wrong_module", "wrong");
	
	//test start
	XASSERT_N(XL_NOTNULL, cal);
	XEXPECT(XL_EQUAL, cal(15,20,5), 305);
	XASSERT_N(XL_NOTNULL, fab);
	XEXPECT(XL_EQUAL, fab(5, 1, 1), 5);
	XASSERT_N(XL_NOTNULL, summ);
	XEXPECT(XL_EQUAL, summ(2,20), 41);
	XASSERT_N(XL_NOTNULL, minus);
	XEXPECT(XL_EQUAL, minus(15,20), -5);
	XASSERT_N(XL_ISNULL, isnull_1);
	XASSERT_N(XL_NOTNULL, foo);
	XEXPECT(XL_EQUAL, foo("Hello world"), 22);
	XASSERT_N(XL_NOTNULL, big_foo);
	XEXPECT(XL_EQUAL, big_foo("Hello world", 'w'), 6);
	XASSERT_N(XL_NOTNULL, search);
	XEXPECT(XL_EQUAL, search("asdasdasdasd",'a'), 18);
	XASSERT_N(XL_ISNULL, isnull_2);
	XASSERT_N(XL_ISNULL, cannot);
	
	// destory
	qbase_loader_free("module_inner");
	XASSERT_N(XL_ISNULL, summ);
	qbase_loader_destory();
	XASSERT_N(XL_ISNULL, cal);
	XASSERT_N(XL_ISNULL, fab);
	XASSERT_N(XL_ISNULL, summ);
	XASSERT_N(XL_ISNULL, minus);
	XASSERT_N(XL_ISNULL, isnull_1);
	XASSERT_N(XL_ISNULL, foo);
	XASSERT_N(XL_ISNULL, big_foo);
	XASSERT_N(XL_ISNULL, search);
	XASSERT_N(XL_ISNULL, isnull_2);
	XASSERT_N(XL_ISNULL, cannot);
}

XALOY_TEST_MODULE(basemath){
	/*base math API*/
	XEXPECT(XL_EQUAL, qbase_math_todegree(-PI/6), -30);
	XEXPECT(XL_EQUAL, qbase_math_todegree(0), 0);
	XEXPECT(XL_EQUAL, qbase_math_todegree(PI/6), 30);
	XEXPECT(XL_EQUAL, qbase_math_todegree(PI/4), 45);
	XEXPECT(XL_EQUAL, qbase_math_todegree(PI/3), 60);
	XEXPECT(XL_EQUAL, qbase_math_todegree(PI/2), 90);
	XEXPECT(XL_EQUAL, qbase_math_todegree(2*PI/3), 120)
	XEXPECT(XL_EQUAL, qbase_math_todegree(PI), 180);
	XEXPECT(XL_EQUAL, qbase_math_todegree(7*PI/6), 210);
	XEXPECT(XL_EQUAL, qbase_math_todegree(5*PI/4), 225);
	XEXPECT(XL_EQUAL, qbase_math_todegree(4*PI/3), 240);
	XEXPECT(XL_EQUAL, qbase_math_todegree(5*PI/3), 300);
	XEXPECT(XL_EQUAL, qbase_math_todegree(2*PI), 360);
	
		
	XEXPECT(XL_EQUAL, qbase_math_equal(0, 0), 1);
	XEXPECT(XL_EQUAL, qbase_math_equal(1, -1), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(0, 1), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(0, 2), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(0, 0.00005), 1);
	XEXPECT(XL_EQUAL, qbase_math_equal(0, 0.005), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(0, -2), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(0, -1), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(0, -0.005), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(0, -0.000005), 1);
	XEXPECT(XL_EQUAL, qbase_math_equal(-10, -10), 1);
	XEXPECT(XL_EQUAL, qbase_math_equal(-10, -11), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(-10, -10.005), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(-10, -10.000005), 1);
	XEXPECT(XL_EQUAL, qbase_math_equal(10, 10), 1);
	XEXPECT(XL_EQUAL, qbase_math_equal(10, 11), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(10, 10.005), 0);
	XEXPECT(XL_EQUAL, qbase_math_equal(10, 10.000005), 1);
	
	/*vector API*/
	qbase_vector v_actual, v_expect, v1, v2;
	Real case_v[] = {0, 0.0000005, 0.005, -0.004, -0.000004, 20, 30, -15.3, -25.7}
	Real act_val = 0.0f, exp_val = 0.0f;
	
	// this scope is true
	for(int i = 0; i < 10; i++)	{
		srand((unsigned)time(0));
		v1.posX = case_v[rand()%9]; v1.posY = case_v[rand()%9];
		v2.posX = case_v[rand()%9]; v2.posY = case_v[rand()%9];
		v_expect.posX = v1.posX+v2.posX; v_expect.posY = v1.posY+v2.posY;
		v_actual = qbase_vector_plus(&v1, &v2);
		XEXPECT(XL_EQUAL, qbase_math_equalV(&v_actual, &v_expect), 1);
	}
	// this scope is false
	for(int i = 0; i < 10; i++)	{
		srand((unsigned)time(0));
		v1.posX = case_v[rand()%9]; v1.posY = case_v[rand()%9];
		v2.posX = case_v[rand()%9]; v2.posY = case_v[rand()%9];
		v_expect.posX = rand();
		v_expect.posY = rand();
		v_actual = qbase_vector_plus(&v1, &v2);
		while(v_expect.posX == v_actual.posX || v_expect.posY == v_actual.posY)	{
			v_expect.posX = rand();
			v_expect.posY = rand();
		}			
		XEXPECT(XL_EQUAL, qbase_math_equalV(&v_actual, &v_expect), 0);
	}
	
	v1.posX = 0; v1.posY = 0;
	v2.posX = 3; v2.posY = -5;
	act_val = qbase_vector_dot(&v1, &v2);
	XEXPECT(XL_EQUAL, act_val, 0);
	for(int i = 0; i < 10; i++)	{
		v1.posX = case_v[rand()%9]; v1.posY = case_v[rand()%9];
		v2.posX = case_v[rand()%9]; v2.posY = case_v[rand()%9];
		exp_val = v1.posX * v2.posX + v1.posY * v2.posY;
		act_val = qbase_vector_dot(&v1, &v2);
		XEXPECT(XL_EQUAL, qbase_math_equal(act_val, exp_val), 1);
	}
	for(int i = 0; i < 10; i++)	{
		v1.posX = case_v[rand()%9]; v1.posY = case_v[rand()%9];
		v2.posX = case_v[rand()%9]; v2.posY = case_v[rand()%9];		
		act_val = qbase_vector_dot(&v1, &v2);
		do	{
			exp_val = rand() % 1024;
			exp_val *= (float)rand()/65535;
		}while(act_val == exp_val)
		XEXPECT(XL_EQUAL, qbase_math_equal(act_val, exp_val), 0);
	}
	
	v1.posX = 0; v1.posY = 0;
	v2.posX = 3; v2.posY = -5;
	Real act_val = qbase_vector_cos(&v1, &v2);
	XEXPECT(XL_EQUAL, qbase_math_equal(act_val, 0), 1);
	
	/*matrix API*/
}

XALOY_TEST_MODULE(basepacker){
	XEXPECT(XL_EQUAL, 0, 0);
}

XALOY_TEST_MODULE(basescript){
	XEXPECT(XL_EQUAL, 0, 0);
}
int main()
{
	return GLOBAL_TEST->execute();
}
