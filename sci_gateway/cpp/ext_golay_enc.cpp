/* 
Extended Golay Encoder (24,12)
coded_bits=ext_golay_enc(uncoded_bits)

uncoded_bits=[1 0 0 0 1 1 0 1 1 1 1 0]
coded_bits=[1 0 0 0 1 1 0 1 1 1 1 0 0 0 0 0 0 1 1 1 1 0 0 1]

Will terminate the uncoded string to length that is a multiple of 12.
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

int sci_ext_golay_enc(char *fname, unsigned long fname_len)
	{
    	// Error management variable
    	SciErr sciErr;

    	//-------------------------------------Variable Declaration----------------------------------------------------------
    	int m1 = 0, n1 = 0;						//input variable lengths
    	int *piAddressVarOne = NULL;

	double *matrixOfBits = NULL;					//The parameter uncoded_bits: the uncoded bit stream
 
	double *newMatrixOfBits = NULL;					//The output parameter: encoded_bits

   	//----------------------------------Check the number of input and output arguments-------------------------------------
	CheckInputArgument(pvApiCtx, 1, 1) ;   				//One input: uncoded_bits
    	CheckOutputArgument(pvApiCtx, 1, 1) ;  				//One output: coded_bits

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
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &matrixOfBits);
    	if (sciErr.iErr)
    	{
        	printError(&sciErr, 0);
        	return 0;
    	}

	//Check if the input consists of binary inputs:0/1
	for (int i = 0; i < m1 * n1; i++)
		{
		if (matrixOfBits[i]!= 0 && matrixOfBits[i]!= 1)
			{
			Scierror(999, "%s: Value of matrix of double.\n", fname, 1);
        		return 0;
			}
		}
	//-----------------------------------------Declaration of ITPP objects--------------------------------------------------------
	itpp::vec v(matrixOfBits, m1*n1);    						//uncoded_bits

	//-----------------------------------------Extended Golay encoding-----------------------------------------------------------
    	Extended_Golay golay;
	bvec coded_bits = golay.encode(itpp::to_bvec(v));

	//----------------------------------------Output Variable: coded_bits---------------------------------------------------
	newMatrixOfBits = (double*)malloc(sizeof(double) * 1* coded_bits.length());
    
	for (int i = 0; i < 1*coded_bits.length(); i++) 
		{
		newMatrixOfBits[i] = coded_bits[i];              
		}

	// Assign to output variable
    	if (sciErr.iErr)
    		{
		printError(&sciErr, 0);
		return 0;
    		}

   	//-----------------------------------------Matrix for the BCH decoded sequence----------------------------------------
    	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, coded_bits.length(), newMatrixOfBits);
    	free(newMatrixOfBits); 							// Data have been copied into Scilab memory
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
