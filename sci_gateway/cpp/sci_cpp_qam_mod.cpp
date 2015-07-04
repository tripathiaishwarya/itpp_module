/*----------------------------------------------------------------------------------------------------------------
Pratik Kapoor
Code for QAM De-Modulation
----------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------
Example

a = [1 0 1 1]
b = qam_mod(4,a);
b = [0.7071068 -0.7071068i  -0.7071068 -0.7071068i]
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

	int sci_cpp_qam_mod(char *fname, unsigned long fname_len)
	{
		//Error management variable
		SciErr sciErr;

		//Variable declaration
		int m=0,n=0,m1=0,n1=0,row=1,col=1;  			//Matrix Dimensions
		int i=0;
		int *piAddressVar = NULL;
		int *setm = NULL;
		
		double *inputMatrix = NULL; 	//Input-Output Matrices
		double *outputMatrixReal = NULL;
		double *outputMatrixImg = NULL;
		double *setmMatrix = NULL;

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
		
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVar);
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &setm);
		
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
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
		
		//To check if the input signal is digital(binary)
		for( i = 0 ; i < m*n ; i++ )
		{
			if( (inputMatrix[i] != 0) && (inputMatrix[i] != 1) )
			{
				Scierror(3, _("%s: Digital Input Signal Expected.\n"), fname, 1);
				return 0;
			}
		}
		
		//Checking if m is not an integer
		if((*setmMatrix - floor(*setmMatrix)) != 0)
		{
			Scierror(999, _("%s: Integer input expected for M.\n"), fname, 1);
			return 0;
		}
		
		//To check if M is even power of 2
		i=1;
		while(1)
		{
			i *= 4;
			if( (int)*setmMatrix == i)
				break;
			if( (int)*setmMatrix < i)
			{
				Scierror(999, _("%s: M should be an even power of 2.\n"), fname, 1);
				return 0;
			}
		}
		
		//----------------------------------WORKING OF FUNCTION-------------------------//
		
		vec v, v0;
		bvec v1;
		v = vec(inputMatrix, (m*n));
		v1 = itpp::to_bvec(v);
		cvec outputVector;
		
		/*=================================================================*/
		// virtual cvec modulate_bits (const bVec< double > &signal) const
		// Calling the required IT++ function
		
		outputVector = QAM((int)*setmMatrix).modulate_bits(v1);
		
		/*=================================================================*/
		
		int relative = log((int)*setmMatrix)/log(2);
	
		//Storing the output in a double array to be sent to scilab
		sciErr = allocComplexMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, n/relative, &outputMatrixReal, &outputMatrixImg);
		
		for(i=0; i < m*n/relative; i++)
		{
			outputMatrixReal[i] = real(outputVector(i));
			outputMatrixImg[i] = imag(outputVector(i));
		}
		
		//Creating Output Modulated Signal
		//sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, n/relative, outputMatrix);
 
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}

		//Returning output arguements to Scilab

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

		ReturnArguments(pvApiCtx);
	
		return 0;
	}
	
}
