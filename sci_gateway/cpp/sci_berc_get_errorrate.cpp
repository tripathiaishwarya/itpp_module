/*----------------------------------------------------------------------------------------------------------------
Code for BERC get_error_rate
Pratik Kapoor
----------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------
Example

in1 = [1 0 1 1]
in2 = [1 1 1 1]
result = berc_get_errorrate(0, 0, 0, in1, in2)
result = 0.25
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

	int sci_berc_get_errorrate(char *fname, unsigned long fname_len)
	{
		//Error management variable
		SciErr sciErr;

		//Variable declaration
		int m=0,n=0,m1=0,n1=0,m2=0,n2=0,m3=0,n3=0,m4=0,n4=0;  			//Matrix Dimensions
		int i=0;
		int *indelay = NULL;
		int *inignorefirst=NULL;
		int *inignorelast=NULL;
		int *input1 = NULL;
		int *input2 = NULL;
		
		double *indelayMatrix = NULL; 			//Input-Output Matrices
		double *inignorefirstMatrix = NULL;
		double *inignorelastMatrix = NULL;
		double *input1Matrix = NULL;
		double *input2Matrix = NULL;
		
		double *outputMatrix = NULL;

		/*=================================================================*/
		// C function which checks the number of input/output arguments present in the calling Scilab function.
		// CheckInputArgument(pvApiCtx, minArgs, maxArgs)
		// CheckOutputArgument(pvApiCtx, minArgs, maxArgs)
		// pvApiCtx --> Scilab environment pointer
		/*=================================================================*/
		
		CheckInputArgument(pvApiCtx, 5, 5);
	    	CheckOutputArgument(pvApiCtx, 1, 1);

		/*=================================================================*/
		// This function retrieves the address of an argument in a gateway.
		// sciErr getVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress)
		// iVAr --> Position of the argument in the function call.
		// piAddress --> Return address of the Scilab variable.
		/*=================================================================*/
		
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &indelay);
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &inignorefirst);
		sciErr = getVarAddressFromPosition(pvApiCtx, 3, &inignorelast);
		sciErr = getVarAddressFromPosition(pvApiCtx, 4, &input1);
		sciErr = getVarAddressFromPosition(pvApiCtx, 5, &input2);
		
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}
	    	
	    	/*// Checking if input signal is of boolean type
		if ( !isDoubleType(pvApiCtx, piAddressVar) ||  isVarComplex(pvApiCtx, piAddressVar) )
	    	{
			Scierror(1, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), fname, 1);
			return 0;
	    	}*/
	    	
	    	/*=================================================================*/
	    	// Getting Input Signal
		// SciErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
		/*=================================================================*/
	    	sciErr = getMatrixOfDouble(pvApiCtx, indelay, &m, &n, &indelayMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, inignorefirst, &m1, &n1, &inignorefirstMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, inignorelast, &m2, &n2, &inignorelastMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, input1, &m3, &n3, &input1Matrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, input2, &m4, &n4, &input2Matrix);
	    	
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
		
		double result = 0.0;
		vec vi1, vi2;
		bvec v1, v2;
		vi1 = vec(input1Matrix, (m3*n3));
		vi2 = vec(input2Matrix, (m4*n4));
		
		v1 = itpp::to_bvec(vi1);
		v2 = itpp::to_bvec(vi2);
		
		/*=================================================================*/
		// BERC (int indelay=0, int inignorefirst=0, int inignorelast=0)
		// void count (const bvec &in1, const bvec &in2)
		// double get_errorrate () const 
		// Calling the required IT++ function
		
		BERC berc((int)*indelayMatrix,(int)*inignorefirstMatrix,(int)*inignorelastMatrix);
		berc.count(v1,v2);
		result = berc.get_errorrate();
		
		/*=================================================================*/
		
		//Storing the output in a double array to be sent to scilab
		//outputMatrix = (double *) malloc((sizeof(double)));
		
		//Creating Output Modulated Signal
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, &result);
 
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}

		free(outputMatrix); 


		//Returning output arguments to Scilab

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

		ReturnArguments(pvApiCtx);
	
		return 0;
	}
	
}						
