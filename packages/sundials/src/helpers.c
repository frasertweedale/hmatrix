#include <stdio.h>
#include <math.h>
#include <arkode/arkode.h>                 /* prototypes for ARKODE fcts., consts. */
#include <nvector/nvector_serial.h>        /* serial N_Vector types, fcts., macros */
#include <sunmatrix/sunmatrix_dense.h>     /* access to dense SUNMatrix            */
#include <sunlinsol/sunlinsol_dense.h>     /* access to dense SUNLinearSolver      */
#include <arkode/arkode_direct.h>          /* access to ARKDls interface           */
#include <sundials/sundials_types.h>       /* definition of type realtype          */
#include <sundials/sundials_math.h>

/* Check function return value...
    opt == 0 means SUNDIALS function allocates memory so check if
             returned NULL pointer
    opt == 1 means SUNDIALS function returns a flag so check if
             flag >= 0
    opt == 2 means function allocates memory so check if returned
             NULL pointer  
*/
int check_flag(void *flagvalue, const char *funcname, int opt)
{
  int *errflag;

  /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
  if (opt == 0 && flagvalue == NULL) {
    fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n",
	    funcname);
    return 1; }

  /* Check if flag < 0 */
  else if (opt == 1) {
    errflag = (int *) flagvalue;
    if (*errflag < 0) {
      fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n",
	      funcname, *errflag);
      return 1; }}

  /* Check if function returned NULL pointer - no memory allocated */
  else if (opt == 2 && flagvalue == NULL) {
    fprintf(stderr, "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n",
	    funcname);
    return 1; }

  return 0;
}

/* check the computed solution */
int check_ans(N_Vector y, realtype t, realtype rtol, realtype atol)
{
  int      passfail=0;     /* answer pass (0) or fail (1) flag     */  
  realtype ans, err, ewt;  /* answer data, error, and error weight */
  realtype ONE=RCONST(1.0);

  /* compute solution error */
  ans  = atan(t);
  ewt = ONE / (rtol * SUNRabs(ans) + atol);
  err = ewt * SUNRabs(NV_Ith_S(y,0) - ans);

  /* is the solution within the tolerances? */
  passfail = (err < ONE) ? 0 : 1; 

  if (passfail) {
    fprintf(stdout, "\nSUNDIALS_WARNING: check_ans error=%g \n\n", err);
  }

  return(passfail);
}
