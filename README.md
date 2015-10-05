# performanceGap

A minimal example extracted from my libsampling driver, that illustrates a performance gap of **up to 2x** between CentOS and SLES (on our compute nodes with equivalent hardware).
The additional overhead in SLES is proportional to the number of instrumented function calls (= dynamic calls to __cyg_profile_func_XXX).

The behavior only occurs if all of the following factors are present
* A dynamic library with __cyg_profile - functions
* An global array accessed via thread local variable

### Build
```
$ make
```
Note that this does also create a soft link to the "parallel" version of the library (were the performance gap happens). 

### Run
```
$ make run
```
... to run the latest build behind the soft link. Or just choose a target with "parallel" flavor and run it yourself.

### Requirements
* GCC (I used 4.9.3 on both machines) 

