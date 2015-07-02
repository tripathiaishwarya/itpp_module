/*----------------------------------------------------------------------------------------------------------------
Code for OFDM De-Modulation
Pratik Kapoor
----------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------
input = [1.1547005  1.1547005i  1.1547005  1.1547005  -1.1547005i  1.1547005]
output = ofdm_demod(2,1,1,a)
output = [1+i  -1+i  1-i  -1-i]
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

	int sci_cpp_ofdm_demod(char *fname, unsigned long fname_len)
	{	
		//Error management variable
		SciErr sciErr;
		
		//Variable declaration
		int m=0,n=0;
		int m1=0,n1=0;
		int m2=0,n2=0;
		int m3=0,n3=0;
		int row=1,col=1;  			//Matrix Dimensions
		int i=0;
		
		int *piAddressVar = NULL;
		int *inNfft = NULL;
		int *inNcp = NULL;
		int *inNupsample = NULL;
		
		double *inputMatrixReal = NULL;
		double *inputMatrixImg = NULL; 	//Input-Output Matrices
		double *outputMatrixReal = NULL;
		double *outputMatrixImg = NULL;
		
		double *inNfftMatrix = NULL;
		double *inNcpMatrix = NULL;
		double *inNupsampleMatrix = NULL;

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
		
		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &inNfft);
		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &inNcp);
		sciErr = getVarAddressFromPosition(pvApiCtx, 3, &inNupsample);
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
	    	sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddressVar, &m, &n, &inputMatrixReal, &inputMatrixImg);
	    	sciErr = getMatrixOfDouble(pvApiCtx, inNfft, &m1, &n1, &inNfftMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, inNcp, &m2, &n2, &inNcpMatrix);
	    	sciErr = getMatrixOfDouble(pvApiCtx, inNupsample, &m3, &n3, &inNupsampleMatrix);
	    	
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
		
		//Declaring input and output vectors for modulation
		vec inputReal, inputImg;
		cvec outputVector;
		cvec inputVector;

		//Converting input Real and imaginary matrices into vectors
		inputReal = vec(inputMatrixReal, m * n);
		inputImg = vec(inputMatrixImg, m * n);
		inputVector = itpp::to_cvec(inputReal, inputImg);
		
		/*=================================================================*/
		// OFDM: Ncp must be >=0 and <=Nfft
		// Nfft >= 2
		// cvec demodulate (const cvec &input)
		// OFDM (int inNfft, int inNcp, int inNupsample=1)
		// Calling the required IT++ function
		// Length of input must be an integer multiple of Nfft+Ncp.
		
		outputVector = OFDM((int)*inNfftMatrix,(int)*inNcpMatrix,(int)*inNupsampleMatrix).demodulate(inputVector);
		
		/*=================================================================*/
	
		//Storing the output in a double array to be sent to scilab
		
		// Output Length = input.length() / (Nfft + Ncp) / Nupsample * Nfft
		
		int outLen = (inputVector.length()/((int)*inNfftMatrix+(int)*inNcpMatrix)/((int)*inNupsampleMatrix))*(int)*inNfftMatrix;
		
		sciErr = allocComplexMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m, outLen, &outputMatrixReal, &outputMatrixImg);
		
		for(i=0; i < outLen; i++)
		{
			outputMatrixReal[i] = real(outputVector(i));
			outputMatrixImg[i] = imag(outputVector(i));
		}
 
	    	if (sciErr.iErr)
	    	{
			printError(&sciErr, 0);
			return 0;
	    	}

		//free(outputMatrix); 


		//Returning output arguements to Scilab

		AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

		ReturnArguments(pvApiCtx);
	
		return 0;
	}
	
}
