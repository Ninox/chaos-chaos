--[[
		because the qbase' lua binding is created by swig, so Lua cannot access
		the 2d array in qbase_matrix2, this test cannot provide the matrix test.
		if you want to get the test result about the qbase_matrix2, please see		
		"gtest-qbase.cpp"
--]]

local q = require("qbase")
local PI = 3.141592631
local qtest = {}
-------------------  test math module  -------------------
local gv = function(x,y)
    local v = q.qbase_vector()
    v.posX = x
    v.posY = y
    return v
end

qtest.test_vplus = function(v1,v2)
    local vec1 = q.qbase_vector()
    local vec2 = q.qbase_vector()
    vec1.posX = v1.x
    vec1.posY = v1.y
    vec2.posX = v2.x
    vec2.posY = v2.y
    local rs = q.qbase_vector_plus(vec1,vec2)
    return {rs.posX, rs.posY}
end

qtest.randomPoint = function()
	local tp = {}
	for i = 1, 4 do
		tp[i] = math.random() + ((math.random(100) % 2 == 0 and 0) or 1) * math.random(1,100)
		tp[i] = math.pow(-1,(math.random(100) % 2 == 0 and 0) or 1) * tp[i]
	end
	return tp
end

--{{gv(0,1), gv(1,0)}, {gv(1.414,1.414),gv(-1.414,1.414)}, {gv(1.732,1),gv(5,0)}, {gv(3,4),gv(1,0)}, {gv(1,0),gv(-1,1.732)}, {gv(1,0),gv(0,0)} }
qtest.test_getdotcossin = function()
	local fix_case = {{0,1, 1,0}, {1.414,1.414,-1.414,1.414}, {1.732,1,5,0}, {3,4,1,0}, {1,0,-1,1.732}, {1,0,0,0}}
	for i = 1, 10 do
		table.insert(fix_case, qtest.randomPoint())
	end
	table.insert(fix_case, {1,5,0,0})
	local len = table.maxn(fix_case)
	--- dot
	local retcase = {}
	local dot ={}
	local vcos = {}
	local vsin = {}
	for i, v in ipairs(fix_case) do
		retcase[i] = {gv(v[1],v[2]), gv(v[3],v[4])}
		dot[i] = v[1] * v[3]  + v[2] * v[4]
		vcos[i] = dot[i] / (math.sqrt(v[1]*v[1]+v[2]*v[2]) * math.sqrt(v[3]*v[3]+v[4]*v[4]))
		vsin[i] = 1 - vcos[i] * vcos[i]
	end	
	return retcase, dot, vcos, vsin
end

qtest.test_parallelvertical = function()
	local pcase = {}
	local vcase = {}
	local result = {}
	for i = 1, 15 do
		result[i] = (math.random(1000) % 2 == 0 and 1) or 0
		local x = math.random(1, 30)
		local y = math.random(1, 30)
		if result[i] == 1 then
			local k = math.random(0,10) * math.random()	 * math.pow(-1,(math.random(3000) % 2 == 0 and 0) or 1)			
			pcase[i] = {gv(x,y), gv(k*x, k*y)}
			k = math.random() + math.random(0,10) * math.pow(-1,(math.random(3000) % 2 == 0 and 0) or 1)
			vcase[i] = {gv(x,y), gv(k*x, -k*x*x/y)}
		else
			local xx = math.random(1,30) * math.random()
			local yy = math.random(1,30) * math.random()
			while x*xx+y*yy == 0 or x/xx - y/yy > 0.0005 do
				xx = math.random(1,30) * math.random()
				yy = math.random(1,30) * math.random()
			end
			pcase[i] = {gv(x,y),gv(xx,yy)}
			pcase[i] = {gv(x,y),gv(xx,yy)}
		end
	end
	result[16] = 1
	pcase[16] = {gv(0,0), gv(1,2)}
	vcase[16] = {gv(0,0), gv(1,2)}
	return pcase, vcase, result
end

qtest.test_createplusmul = function()
	local pluscase = {}
	local mulcase = {}
	local pexpect = {}
	local mexpcet = {}
	
	pluscase[i] = {0 , 0,math.random(0, 100),math.random(0, 100)}
	mulcase[i] = {0, 0, math.random() + math.random(0,10)}
	for i = 2, 15 do
		pluscase[i] = {math.random(0, 100),math.random(0, 100),math.random(0, 100),math.random(0, 100)}
		mulcase[i] = {math.random(0, 100), math.random(0, 100), math.random() + math.random(0,10)}
		pexpect[i] = {x = pluscase[i][1] + pluscase[i][3], y=pluscase[i][2]+pluscase[i][4]}
		mexpcet[i] = {x = mulcase[i][1]*mulcase[i][3], y=mulcase[i][2]*mulcase[i][3]}
	end
end

qtest.test_length = function()
	local lcase = {}
	local lexpect = {}
	for i = 1, 15 do
		local x = math.random(0, 100) * math.random()
		local y = math.random(0, 100) * math.random()
		lcase[i] = {x,y}
		lexpect[i] = math.sqrt(x*x+y*y)
	end
	lcase[16] = {0, 0}
	lexpect[16] = 0
	return lcase, lexpect
end

--- get test case and expect
local dcs_case, dcsr_dot, dcsr_cos, dcsr_sin = qtest.test_getdotcossin()
local pcase, vcase, pvresult = qtest.test_parallelvertical()
local lcase, lexpect = qtest.test_length()

local testcases = {
    ------    math case -------
    {
        name = "todegree", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "equal", f = q.qbase_math_equal, mode="EQ", case = {{34, 34},{3.141, PI},{3.141592,PI},{1,2},{1,100},{-5, 5},{-1,1},{-1,0},{-5,-3},{-0.000001,0},{-0.000001,0.000002}},
        expect = {1,0,1,0,0,0,0,0,0,1,1}, cycle = 10000
    },
    {
        name = "vplus", f = qtest.test_vplus, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
	{
        name = "vmul", f = q.qbase_vector_mul, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    { name = "vdot", f = q.qbase_vector_dot, mode="EQ", case = dcs_case, expect = dcsr_dot, cycle = 10000 },
    { name = "vcos", f = q.qbase_vector_cos, mode="EQ", case = dcs_case, expect = dcsr_cos, cycle = 10000 },
    { name = "vsin", f = q.qbase_vector_sin, mode="EQ", case = dcs_case, expect = dcsr_sin, cycle = 10000 },
    { name = "vparallel", f = q.qbase_vector_isparallel, mode="EQ", case = pcase, expect = pvresult, cycle = 10000 },
    { name = "vvertical", f = q.qbase_vector_isvertical, mode="EQ", case = vcase, expect = pvresult, cycle = 10000 },
    { name = "vlength", f = q.qbase_vector_length, mode="EQ", case = lcase, expect = lexpect, cycle = 10000 },    
    --------- loader test -------------
    {
        name = "linit", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "lfree", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "lgetf", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "ldes", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    ---------- script test ------------
    {
        name = "screate", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "sfree", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "sget", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "sfreetb", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "sgetfield", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "sreg", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "sload", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "scall", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "sexec", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    ---------- packer test ---------------
    {
        name = "pcreate", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "pload", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "psave", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "pfree", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "psecur", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "ppwd", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "pshow", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "pget", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "padd", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "premove", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "pupdate", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "prename", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    }
}

require("xaloy")
local obj = xaloy.create("qbase")
xaloy.bind(obj, testcases)
xaloy.assert(obj)
