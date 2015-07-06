/* 
BCH Decoder:
decoded_bits=bch_dec(coded_bits,n,t)
where,
coded_bits is the  coded stream 1s and 0s
n is the length of the code
t is the bits to correct
decoded_bits is the stream of decoded message consisting of 1s and 0s 

Examples:
1. 	coded_bits=[0 1 0 0 1 1 1], n=7, t=1, (k=4)
	decoded_bits=[0 1 0 1]
2.      coded_bits=[0 1 1 1 1 0 1 0 1 0 0 0 1 0 0 1 1 1 0 0 0 0 0 1 1 0 0 1 0 1 1], n=31, t=7, (k=6) 
	decoded_bits=  [0 1 0 1 0 1]
*/

#include <itpp/itcomm.h>
#include <numeric>
extern "C" {
#include </usr/include/scilab/api_scilab.h>
#include </usr/include/scilab/Scierror.h>
#include </usr/include/scilab/BOOL.h>
#include </usr/include/scilab/localization.h>

using namespace itpp;
using std::cout;
using std::endl;

int sci_bch_dec(char *fname, unsigned long fname_len)
	{
    	// Error management variable
    	SciErr sciErr;

    	//-------------------------------------Variable Declaration----------------------------------------------------------
    	int m1 = 0, n1 = 0, m2=0, n2=0, m3=0, n3=0; 			//input variable lengths
    	int *piAddressVarOne = NULL;
	int *piAddressVarTwo = NULL;
	int *piAddressVarThree = NULL;

	double *matrixOfBits = NULL;					//The parameter coded_bits: the coded bit stream
 	double *CodeLength = NULL;     					//The parameter 'n': Code length
	double *BitsToCorrect = NULL;   				//The parameter 't': number of errors to correct

	double *newMatrixOfBits = NULL;					//The output parameter: decoded_bits

   	//----------------------------------Check the number of input and output arguments-------------------------------------
	CheckInputArgument(pvApiCtx, 3, 3) ;   				//Three inputs:coded_bits, n,t
    	CheckOutputArgument(pvApiCtx, 1, 1) ;  				//One output: decoded_bits

     	//--------------------------------- Input Variable 1:coded_bits-------------------------------------------------------
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
     	//--------------------------------- Input Variable 2: n-------------------------------------------------------
    	// Get the address of input
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
    	
	//Get the matrix
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &CodeLength);
    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

   	// Check that the input is single valued, i.e. 1*1 dimension
     	if(m2!=1 && n2!=1)
		{
		Scierror(999, "%s: Single row or column expected.\n", fname, 2);
        	return 0;
		}

     	//--------------------------------- Input Variable 3: t-------------------------------------------------------
     	// Get the address of input
    	sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

    	// Check that the first input argument is a real matrix (and not complex) 
    	if ( !isDoubleType(pvApiCtx, piAddressVarThree) ||  isVarComplex(pvApiCtx, piAddressVarThree) )
    		{
        	Scierror(999, "%s: Wrong type for input argument #%d: A real matrix expected.\n", fname, 3);
        	return 0;
    		}

	//Get the matrix
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarThree, &m3, &n3, &BitsToCorrect);
    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

   	// Check that the input is single valued, i.e. 1*1 dimension
     	if(m3!=1 && n3!=1)
		{
		Scierror(999, "%s: Single row or column expected.\n", fname, 3);
        	return 0;
		}

	//-----------------------------------------Declaration of ITPP objects--------------------------------------------------------
	itpp::vec v(matrixOfBits, m1*n1);    						//uncoded_bits

	//-----------------------------------------BCH decoding----------------------------------------------------------------------
    	itpp::BCH bch(*CodeLength, *BitsToCorrect, false);
	bvec decoded_bits = bch.decode(itpp::to_bvec(v));

	//----------------------------------------Output Variable: decoded_bits---------------------------------------------------
	newMatrixOfBits = (double*)malloc(sizeof(double) * 1* decoded_bits.length());
    
	for (int i = 0; i < 1*decoded_bits.length(); i++) 
		{
		newMatrixOfBits[i] = decoded_bits[i];              
		}

	// Assign to output variable
    	if (sciErr.iErr)
    		{
		printError(&sciErr, 0);
		return 0;
    		}

   	//-----------------------------------------Matrix for the BCH decoded sequence----------------------------------------
    	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, decoded_bits.length(), newMatrixOfBits);
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
