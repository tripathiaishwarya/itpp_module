/*----------------------------------------------------------------------------------------------------------------
Code for PAM De-Modulation
Pratik Kapoor
May 28, 2015
----------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------
Example

a = [1 -1 1 1]
b = pam_demod(2,a);
b = [1 0 1 1]
----------------------------------------------------------------------------------------------------------------*/

#include <numeric>
#include <itpp/comm/modulator.h>
#include <itpp/base/vec.h>
#include <itpp/itcomm.h>
#include <itpp/itbase.h>
#include <itpp/comm/modulator.h>

using namespace itpp;
using namespace std;

extern "C"
{
	#include "api_scilab.h"
	#include "Scierror.h"
	#include "BOOL.h"
	#include <localization.h>
	#include "MALLOC.h"
	#include <itpp/base/vec.h>

	/*=================================================================*/

	int sci_cpp_pam_demod(char *fname, unsigned long fname_len)
	{
		//Error management variable
		SciErr sciErr;

		//Variable declaration
		int m=0,n=0,m1=0,n1=0;  			//Matrix Dimensions
		int i=0;
		int *piAddressVar = NULL;
		int *setm = NULL;
		
		double *inputMatrix = NULL; 	//Input-Output Matrices
		double *outputMatrix = NULL;
		double *setmMatrix = NULL;

		/*=================================================================*/
		// C function which checks the number of input/output arguments present in the calling Scilab function.
		// CheckInputArgument(pvApiCtx, minArgs, maxArgs)
		// CheckOutputArgument(pvApiCtx, minArgs, maxArgs)
		// pvApiCtx --> Scilab environment pointer
		/*=================================================================*/
		
		CheckInputArgument(pvApiCtx, 1, 2);
	    	CheckOutputArgument(pvApiCtx, 1, 1);

		/*=================================================================*/
		// This function retrieves the address of an argument in a gateway.
		// sciErr getVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress)
		// iVAr --> Position of the argument in the function call.
		// piAddress --> Return address of the Scilab variable.
		/*=================================================================*/
		
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVar);
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &setm);
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}
	    	
	    	// Checking if input signal is of boolean type
		if ( !isDoubleType(pvApiCtx, piAddressVar) ||  isVarComplex(pvApiCtx, piAddressVar) )
	    	{
			Scierror(1, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
			return 0;
	    	}
	    	
	    	/*=================================================================*/
	    	// Getting Input Signal
		// SciErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
		/*=================================================================*/
	    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar, &m, &n, &inputMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, setm, &m1, &n1, &setmMatrix);
	    	
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}
	    	
	    	//Checking if input signal is a vector
		if((m!=1) && (n!=1)) 
		{
			Scierror(2, _("%s: Single row or column vector expected.\n"), fname, 1);
			return 0;
		}
		
		
		//----------------------------------WORKING OF FUNCTION-------------------------//
		
		vec v;
		bvec v0;
		v = vec(inputMatrix, m * n);
		//v1 = itpp::to_bvec(v);
		
		/*=================================================================*/
		// virtual bvec demodulate_bits (const Vec< double > &signal) const
		// Calling the required IT++ function
		
		v0 = PAM((int)*setmMatrix).demodulate_bits(v);
		
		/*=================================================================*/
		
		int relative = log((int)*setmMatrix)/log(2);
		
		//Storing the output in a double array to be sent to scilab
		outputMatrix = (double *) malloc( sizeof(double)*m*n*relative);
		
		for(i=0 ; i<m*n*relative ; i++)
		{
			outputMatrix[i] = v0(i);
		}
		
		//Creating Output Modulated Signal
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, n*relative, outputMatrix);
 
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}

		free(outputMatrix); 


		//Returning output arguements to Scilab

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

		ReturnArguments(pvApiCtx);
	
		return 0;
	}
	
}

		
		
		
		
		
		
		
		
		
		
		
		
