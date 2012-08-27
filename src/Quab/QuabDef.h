#ifndef QUAB_PREDEFINATION_H
#define QUAB_PREDEFINATION_H

#define interface class

#ifdef QUAB_USE_DOUBLE
typedef double Real;
#else
#ifdef QUAB_USE_INTEGER
typedef int Real;
#else
typedef float Real
#endif
#endif

#endif