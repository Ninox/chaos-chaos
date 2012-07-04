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
        name = "vdot", f = q.qbase_vector_dot, mode="EQ", case = {{gv(0,1), gv(1,0)}, {gv(1.414,1.414),gv(-1.414,1.414)}, {gv(1.732,1),gv(5,0)}, {gv(3,4),gv(1,0)}, {gv(1,0),gc(-1,1.732)}, {gv(1,0),gv(0,0)} },
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "vcos", f = q.qbase_vector_cos, mode="EQ", case = {{gv(0,1), gv(1,0)}, {gv(1.414,1.414),gv(-1.414,1.414)}, {gv(1.732,1),gv(5,0)}, {gv(3,4),gv(1,0)}, {gv(1,0),gc(-1,1.732)}, {gv(1,0),gv(0,0)} },
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "vsin", f = q.qbase_vector_sin, mode="EQ", case =  {{gv(0,1), gv(1,0)}, {gv(1.414,1.414),gv(-1.414,1.414)}, {gv(1.732,1),gv(5,0)}, {gv(3,4),gv(1,0)}, {gv(1,0),gc(-1,1.732)}, {gv(1,0),gv(0,0)} },
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "vparallel", f = q.qbase_vector_isparallel, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "vvertical", f = q.qbase_vector_isvertical, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "vlength", f = q.qbase_vector_length, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "vmul", f = q.qbase_vector_mul, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "mzero", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "minit", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "mplus", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "mtrans", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "madjoint", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "minverse", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "mdel", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "mextend", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "mmul", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
    {
        name = "mvmul", f = q.qbase_math_todegree, mode="EQ", case = {{0},{PI/6},{PI/3},{PI/2},{PI*2/3},{PI},{7*PI/6},{4*PI/3},{3*PI/2},{2*PI}},
        expect = {0,30,60,90,120,180,210,240,270,360}, cycle = 10000
    },
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


