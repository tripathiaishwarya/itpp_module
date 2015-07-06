/*----------------------------------------------------------------------------------------------------------------
Code for Raised Cosine get_pulse_shape
Pratik Kapoor
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

	int rc_get_pulse_shape(char *fname, unsigned long fname_len)
	{
		//Error management variable
		SciErr sciErr;

		//Variable declaration
		int m=0, n=0, m1=0, n1=0, m2=0, n2=0;
		
		int *roll_off= NULL;
		int *filter_length = NULL;
		int *upsampling_factor = NULL;
		
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
		
		CheckInputArgument(pvApiCtx, 3, 3);
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
	    	
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}
	    	
	    	/*=================================================================*/
	    	// Input Error Checks
	    	// roll_off_factor between 0 and 1, filter_length even
		/*=================================================================*/
	    	
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
		
		vec v;
		
		/*=================================================================*/
		// Raised_Cosine (double roll_off, int filter_length=6, int upsampling_factor=8)
		// Vec< double > get_pulse_shape (void) const
		// Calling the required IT++ function
		
		v = Raised_Cosine<double>((double)*roll_offMatrix,(int)*filter_lengthMatrix,(int)*upsampling_factorMatrix).get_pulse_shape();
		
		/*=================================================================*/
		
		//Storing the output in a double array to be sent to scilab
		outputMatrix = (double *) malloc( sizeof(double)*v.length());
		
		for(int i=0 ; i<v.length() ; i++)
		{
			outputMatrix[i] = v(i);
		}
		
		//Creating Output Modulated Signal
		sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, n*v.length(), outputMatrix);
 
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

		
		
		
		
		
		
		
		
		
		
		
		
