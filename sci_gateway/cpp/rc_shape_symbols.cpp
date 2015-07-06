/*----------------------------------------------------------------------------------------------------------------
Code for Raised Cosine shape_symbols
Pratik Kapoor
June 4, 2015
----------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------
Example 

a = [1 -1 1 1]
b = itpp_shape_symbols(0.5,6,8,a)
b = 8.951D-34    0.0011377    0.0047728    0.0105661    0.0171489    0.0221856    0.0227496  
 
    0.0159920  - 1.299D-17  - 0.0264666  - 0.0623855  - 0.1022829  - 0.1371911  - 0.1556013  
 
  - 0.1452514  - 0.0955170    4.361D-17    0.1423460    0.3248893    0.5312656    0.7374019  
 
    0.9143499    1.0324875    1.0664587    1.    0.8308710    0.5718924    0.2486151  - 0.1028933  
     
  - 0.4409959  - 0.7244844  - 0.9185954
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

	int rc_shape_symbols(char *fname, unsigned long fname_len)
	{
		//Error management variable
		SciErr sciErr;

		//Variable declaration
		int m=0, n=0, m1=0, n1=0, m2=0, n2=0, m3=0, n3=0;
		int i=0;
		
		int *roll_off= NULL;
		int *filter_length = NULL;
		int *upsampling_factor = NULL;
		int *piAddressVar = NULL;
		
		double *inputMatrix = NULL;
		double *roll_offMatrix = NULL; 		//Input-Output Matrices
		double *filter_lengthMatrix = NULL;
		double *upsampling_factorMatrix = NULL;
		
		double *outputMatrix = NULL;

		/*=================================================================*/
		// C function which checks the number of input/output arguments present in the calling Scilab function.
		// CheckInputArgument(pvApiCtx, minArgs, maxArgs)
		// CheckOutputArgument(pvApiCtx, minArgs, maxArgs)
		// pvApiCtx --> Scilab environment pointer
		/*=================================================================*/
		
		CheckInputArgument(pvApiCtx, 4, 4);
	    	CheckOutputArgument(pvApiCtx, 1, 1);

		/*=================================================================*/
		// This function retrieves the address of an argument in a gateway.
		// sciErr getVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress)
		// iVAr --> Position of the argument in the function call.
		// piAddress --> Return address of the Scilab variable.
		/*=================================================================*/
		
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &roll_off);
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &filter_length);
		sciErr = getVarAddressFromPosition(pvApiCtx, 3, &upsampling_factor);
		sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVar);
		
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}
	    	
	    	/*=================================================================*/
	    	// Getting Input Signal
		// SciErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
		/*=================================================================*/
	    	sciErr = getMatrixOfDouble(pvApiCtx, roll_off, &m, &n, &roll_offMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, filter_length, &m1, &n1, &filter_lengthMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, upsampling_factor, &m2, &n2, &upsampling_factorMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVar, &m3, &n3, &inputMatrix);
	    	
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}
	    	
	    	
	    	/*=================================================================*/
	    	// Input Error Checks
	    	// roll_off_factor between 0 and 1, filter_length even
		/*=================================================================*/
		
	    	//Checking if input signal is a vector
		if((m3!=1) && (n3!=1)) 
		{
			Scierror(2, _("%s: Single row or column vector expected.\n"), fname, 1);
			return 0;
		}
	    	
		if(*roll_offMatrix<0 || *roll_offMatrix>1) 
		{
			Scierror(3, _("%s: roll_off_factor must be between 0 and 1\n"), fname, 1);
			return 0;
		}
		
		if((*filter_lengthMatrix - floor(*filter_lengthMatrix)) != 0)
		{
			Scierror(999, _("%s: Integer input expected for filter length.\n"), fname, 1);
			return 0;
		}
		
		if((*upsampling_factorMatrix - floor(*upsampling_factorMatrix)) != 0)
		{
			Scierror(999, _("%s: Integer input expected for upsampling factor.\n"), fname, 1);
			return 0;
		}
		
		if(((int)*filter_lengthMatrix)%2!=0) 
		{
			Scierror(3, _("%s: filter_length must be even\n"), fname, 1);
			return 0;
		}
		
		//----------------------------------WORKING OF FUNCTION-------------------------//
		
		vec inputVector;
		vec outputVector;
		inputVector = vec(inputMatrix, (m3*n3));
		
		/*=================================================================*/
		// Raised_Cosine (double roll_off, int filter_length=6, int upsampling_factor=8)
		// Vec< T1 > shape_symbols (const Vec< T1 > &input)
		// Calling the required IT++ function
		
		outputVector = Raised_Cosine<double>((double)*roll_offMatrix,(int)*filter_lengthMatrix,(int)*upsampling_factorMatrix).shape_symbols(inputVector);
		
		/*=================================================================*/
		
		int outLen = outputVector.length();
		
		outputMatrix = (double *) malloc( sizeof(double)*m3*n3*outLen);
		
		for(i=0; i < outLen; i++)
		{
			outputMatrix[i] = outputVector(i);
		}
		
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, n*outLen, outputMatrix);
 
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

		
		
		
		
		
		
		
		
		
		
		
		
