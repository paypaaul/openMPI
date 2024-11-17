#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <mpi.h>


#define ERR_SIZE 100

typedef enum err_t 
{
    NONE                = 0,
    IO_ERROR            = 1,
    HASH_COLLISION      = 2,
    NOT_FOUND           = 3,
    UNCOMPATIBLE_SIZES  = 4,
    MPI                 = 5,
    GENERIC_ERR         = 6,
    CORE_IDLE           = 7
    
} err_t;

typedef struct status_t 
{   
    uint32_t *match_indexes;
    int freq;
    err_t current_err;

} status_t;


