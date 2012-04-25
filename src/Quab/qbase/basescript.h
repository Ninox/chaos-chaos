#ifndef BASESCRIPT_H
#define BASESCRIPT_H

typedef void* (*qbase_function)();
typedef char ret_status;
union qbase_return  {
    int iVal;
    double dVal;
    char* strVal;
    bool bVal;
};

ret_status qbase_script_init();
ret_status qbase_script_execute(char* src, int retcount, void** ret);
ret_status qbase_script_load(char* path);
void qbase_script_reg(char* name, qbase_function f);
qbase_return qbase_script_get(char* valname);
qbase_return qbase_script_getfield(char* table, char* field);
void qbase_script_close();


#endif  //basescript.h
