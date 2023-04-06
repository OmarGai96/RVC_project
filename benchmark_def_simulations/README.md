# SIMULATIONS 
## Description
In this folder 3 different files are provided in order to show: <br>
1) general behavior of the system <br>
2) how threads communicate each others <br>
3) task timing information (when the system pass from one task to another) <br>

The 3 files report the output provided by our program with different parameters. <br>
The desired outuput can be obtained modifing (comment or uncomment) the following <br>
defines, available in the file [rtconfig.h](/RVC_Oses/rtconfig.h):

```c
/* user define */
#define BENCHMARK_TIME  //uncomment to print timing information
#define DEB_DISPLAY     //uncomment to print SIMULATION info
#define DEB_INTERNAL   //uncomment to print INTERNAL STATUS information
#define DEBUG_SCH      //uncomment to print scheduler info
```