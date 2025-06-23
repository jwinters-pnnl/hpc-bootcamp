#include "system_includes.h"

#define __USE_FILE_OFFSET64

int main(int argc, char **argv) {
    int ierr;
    int success;
    int myrank;
    int nranks;
    MPI_Comm comm;
    char *log_filename;
    char log_filename_b[256];
    FILE *lfp;
#include "mpi_init2.h"
#include "open_log_file.h"
    ierr = MPI_Init(&argc,&argv);
    /*
        Check on success of mpi_init
    */
   success = 1;
   if (ierr) {
        /*MPI_Error_string(ierr,error_string,err_len);*/
        /* if MPI_Init failed, it is likely the MPI_Error_string will also fail */
        fprintf(stderr, "MPI_Init call failed, ierr = %d\n", ierr);
        fflush(stderr);
        success = 0;
   }

   if (success == 1) {
        comm = MPI_COMM_WORLD;
        /*
            Discover my rank, number of mpi_ranks
            and set the error handler to MPI_ERRORS_RETURN.
        */
        success = mpi_init2(comm,&myrank,&nranks);
    }

    if (success == 1) {
        lfp = NULL;
        log_filename = (char*)&log_filename_b[0];
        strcpy(log_filename, "fdist.log");
    
        success = open_log_file(log_filename,myrank,&lfp);
        if (success) {
            fprintf(lfp, "Hello from rank %d of %d ranks\n",myrank,nranks);
            fflush(lfp);
        }
    }
    
    fprintf(lfp, "Hello from rank %d of %d ranks\n",myrank,nranks);
    ierr = MPI_Finalize();
    fclose(lfp);
    if (ierr == MPI_SUCCESS) {
        return 0;
    } else {
        fprintf(stderr, "MPI_Finalize call failed\n");
        fflush(stderr);
        return 1;
    }
}