/*
Sequence Spreading:

transmitted_bits=spread(uncoded_bits, spreading_code)
uncoded_bits should be 0s and 1s
spreading_code should be 1s and -1s

"spread" replaces each 0 of the input bitstream by the spreading_code and each 1 of the input bitstream by negative of the spreading_code
and vice-versa for "despread".
The length of the output of spread=length(input)*length(spreading_code)
*/

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

using namespace itpp;
using std::cout;
using std::endl;

int sci_spread(char *fname, unsigned long fname_len)
	{
    	// Error management variable
    	SciErr sciErr;

    	//-------------------------------------Variable Declaration----------------------------------------------------------
    	int m1 = 0, n1 = 0, m2=0, n2=0;					 //Input variable lengths
    	int *piAddressVarOne = NULL;
	int *piAddressVarTwo = NULL;

    	double *matrixOfBitStream = NULL;				//Input stream of uncoded bits
	double *matrixOfSpreadCode = NULL;				//Input stream of spreading/despreading code

    	double *newMatrixOfDouble = NULL;				//Output stream of spread code

   	//----------------------------------Check the number of input and output arguments-------------------------------------
    	CheckInputArgument(pvApiCtx, 2, 2) ;				//Two inputs: uncoded_bits, spreading_code
    	CheckOutputArgument(pvApiCtx, 1, 1) ;				//One output: transmitted_bits


     	//--------------------------------- Input Variable 1:uncoded_bits-------------------------------------------------------
    	// Get Address of inputs
    	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

    	// Check that the first input argument is a real matrix (and not complex) 
    	if ( !isDoubleType(pvApiCtx, piAddressVarOne) ||  isVarComplex(pvApiCtx, piAddressVarOne) )
    		{
        	Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 1);
        	return 0;
    		}

    	//Get the matrix
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &matrixOfBitStream);
    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

	//Check if the input has a single row/coloumn
     	if(m1!=1 && n1 !=1)
		{
		Scierror(999, "%s: Single row or column expected.\n", fname, 1);
        	return 0;
		}

	//Check if the input consists of binary inputs:0/1
      	for (int i = 0; i < m1 * n1; i++)
		{
		if (matrixOfBitStream[i]!= 0 && matrixOfBitStream[i]!= 1)
			{
			Scierror(999, "%s: Value of matrix of doule.\n", fname, 1);
        		return 0;
			}
		}
     	//--------------------------------- Input Variable 2:spreading_code-------------------------------------------------------
    	// Get Address of input
    	sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

    	// Check that the first input argument is a real matrix (and not complex) 
    	if ( !isDoubleType(pvApiCtx, piAddressVarTwo) ||  isVarComplex(pvApiCtx, piAddressVarTwo) )
    		{
        	Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 2);
        	return 0;
    		}

	// Get the matrix
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &matrixOfSpreadCode);
    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

	//Check if the input has a single row/coloumn
     	if(m2!=1 && n2!=1)
		{
		Scierror(999, "%s: Single row and column expected.\n", fname, 2);
        	return 0;
		}

	// Input should be a stream of 1s and -1s 
      	for (int i = 0; i < m2 * n2; i++)
		{
		if (matrixOfSpreadCode[i]!= -1 && matrixOfSpreadCode[i]!= 1)
			{
			Scierror(999, "%s: Value of matrix of doule.\n", fname, 2);
        		return 0;
			}
		}

        //-----------------------------------------Dimension Check---------------------------------------------------------------
	//Check if the two are 1-D arrays
        if((m1!=m2 && m1==1) || (n1!=n2 && n1==1))
		{
		Scierror(999, "%s: Same type of arrays expected.\n", fname, 2);
        	return 0;
		}

	//----------------------------------------Output Variable: transmitted_bits---------------------------------------------------
	newMatrixOfDouble = (double*)malloc(sizeof(double) * (m1*m2)* (n1*n2));
        
	//-----------------------------------------Declaration of ITPP objects--------------------------------------------------------
	itpp::vec v(matrixOfBitStream, m1*n1);
	itpp::vec spreading_code(matrixOfSpreadCode, m2*n2);
    	itpp::BPSK bpsk;
    	itpp::Spread_1d spread_1d(spreading_code);
   	itpp::vec transmitted_bits;
  
	//-----------------------------------------Modulation and spreading operation -------------------------------------------------
    	transmitted_bits = bpsk.modulate_bits(itpp::to_bvec(v));		//Modulation
        vec transmitted_signal = spread_1d.spread(transmitted_bits);		//Spreading

	//Assign to output variable
	for (int i = 0; i < m1*m2*n1*n2; i++) 
		{
		newMatrixOfDouble[i]=transmitted_signal[i];         
		}

    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

   	//-----------------------------------------Matrix for the spread signal---------------------------------------------------
    	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1*m2, n1*n2, newMatrixOfDouble);
    	free(newMatrixOfDouble); 						// Data have been copied into Scilab memory
    	if (sciErr.iErr)
    		{ 
        	printError(&sciErr, 0);
        	return 0;
    		}

    	//-----------------------------------------Return the output arguments to the Scilab engine ------------------------------------
    	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;	
	ReturnArguments(pvApiCtx);
	 
	return 0;


}
	
}
