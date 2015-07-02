/*----------------------------------------------------------------------------------------------------------------
Code for Cross Interleaver
Pratik Kapoor
----------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------
Example

a = [1 -1 1 1]  
b = cross_interleaver(2,a)
b = [1 0 1 -1 0 1 0 0]
----------------------------------------------------------------------------------------------------------------*/

#include <numeric>
#include <itpp/comm/modulator.h>
#include <itpp/base/vec.h>
#include <itpp/itcomm.h>
#include <itpp/itbase.h>
#include <itpp/comm/pulse_shape.h>

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

	int itpp_cross_interleaver(char *fname, unsigned long fname_len)
	{
		//Error management variable
		SciErr sciErr;

		//Variable declaration
		int m=0, n=0, m1=0, n1=0;
		int i=0;
		
		int *in_order = NULL;
		int *piAddressVar = NULL;
		
		double *inputMatrix = NULL;
		double *inorderMatrix = NULL;
		
		double *outputMatrix = NULL;

		/*=================================================================*/
		// C function which checks the number of input/output arguments present in the calling Scilab function.
		// CheckInputArgument(pvApiCtx, minArgs, maxArgs)
		// CheckOutputArgument(pvApiCtx, minArgs, maxArgs)
		// pvApiCtx --> Scilab environment pointer
		/*=================================================================*/
		
		CheckInputArgument(pvApiCtx, 2, 2);
	    	CheckOutputArgument(pvApiCtx, 1, 1);

		/*=================================================================*/
		// This function retrieves the address of an argument in a gateway.
		// sciErr getVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress)
		// iVAr --> Position of the argument in the function call.
		// piAddress --> Return address of the Scilab variable.
		/*=================================================================*/
		
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &in_order);
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVar);
		
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}
	    	
	    	/*=================================================================*/
	    	// Getting Input Signal
		// SciErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
		/*=================================================================*/
	    	sciErr = getMatrixOfDouble(pvApiCtx, in_order, &m, &n, &inorderMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar, &m1, &n1, &inputMatrix);
	    	
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}
	    	
	    	
	    	/*=================================================================*/
	    	// Input Error Checks
		/*=================================================================*/
		
		//----------------------------------WORKING OF FUNCTION-------------------------//
		
		vec inputVector;
		vec outputVector;
		inputVector = vec(inputMatrix, (m1*n1));
		
		/*=================================================================*/
		// Cross_Interleaver (int in_order)
		// Vec< T > interleave (const Vec< T > &input)
		// Calling the required IT++ function
		
		outputVector = Cross_Interleaver<double>((double)*inorderMatrix).interleave(inputVector);
		
		/*=================================================================*/
		
		int outLen = outputVector.length();
		
		outputMatrix = (double *) malloc( sizeof(double)*outLen);
		
		for(i=0; i < outLen; i++)
		{
			outputMatrix[i] = outputVector(i);
		}
		
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1, outLen, outputMatrix);
 
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

		
		
		
		
		
		
		
		
		
		
		
		
