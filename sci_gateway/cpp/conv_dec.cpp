
/*
Convolutional Decoder:

decoded_bits=conv_dec(coded_bits, Generator,uncon_length)
where,
coded_bits is a stream of binary bits
Generator is the array of generator polyomials in the octal form
uncon_length is the unconstrained length of the system

Example: 
coded_bits=[0 0 0 0 0 0 0 0 0 0 0 1 0 1 0 0 0 0 1 1 1 0 0 0 0 1 1 0 1 1 1 1 1 1 1 1 0 0 0 0 1 0 0 0 1 1 1 1 0 0 1 0 1 0 0 0 1 1 0 1 0 0 1 1 1 0 0 1 1 0 1 0 1 1 1 1 1 0 0 0 1 0 0 1 0 0 1 0 0 0 0 1 1 0 0 1 1 1 0 0 0 0 0 0 1 0 0 1 0 1 0 0 1 1 1 0 1 1 0 1 1 0 1 1 1 0 1 0 0 0 0 0 1 0 0 0 0 1 1 0 0 0 0 0 1 0 0 0 0 0 1 1 1 0 1 1 0 0 1 1 1 0 0 0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 0 0 1 1 0 1 0 1 0 0 1 0 0 1 0 1 1 0 1 1 0 0 1 0 1 1 1 1 1 0 1 0 1 1 1 0 0 1 1 0 1 1 1 0 0 1 0 1 1 0 1 0 1 1 1 1 1 1 0 1 0 0 1 1 1 1 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 1 1 0 1 0 1 1 0 1 0 0 0 0 1 1 1 1 0 1 1 0 1 0 1 0 1 1 1 0 1 0 0 0 0 1 1 1 1 0 1 0 0 0 1 1 1 1 0 0 0]
Generator=[0133 0145 0175], uncon_length=7,
decoded_bits=[0 0 1 0 1 0 0 1 0 1 0 0 0 0 0 1 0 0 0 0 1 1 1 1 1 1 1 1 0 1 0 1 0 1 1 0 0 1 1 1 0 1 1 1 0 1 1 1 1 0 1 0 1 0 1 0 1 0 0 1 1 0 0 1 1 0 1 1 0 0 0 0 0 1 1 1 1 0 0 0 0 0 0 0 0 0 0 1 1 1 0 1 0 0 1 1 1 0 1 0]
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

int sci_conv_dec(char *fname, unsigned long fname_len)
	{
	// Error management variable    	
    	SciErr sciErr;

    	//-------------------------------------Variable Declaration----------------------------------------------------------
    	int m1 = 0, n1 = 0, m2=0, n2=0, m3=0, n3=0; 			//input variable lengths
    	int *piAddressVarOne = NULL;
	int *piAddressVarTwo = NULL;
	int *piAddressVarThree = NULL;

    	double *matrixOfBits = NULL;					//The coded bit stream
	double *Generator= NULL;					//The generator polynomial in octal form
	double *uncon_length= NULL;					//The unconstrained length

	double *newMatrixOfBits = NULL;

   	//----------------------------------Check the number of input and output arguments-------------------------------------
	CheckInputArgument(pvApiCtx, 3, 3) ;   				//Three inputs: coded_bits, generator, uncon_length
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

     	//--------------------------------- Input Variable 2: Generator-------------------------------------------------------
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
 
	// Get the input
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &Generator);
    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

     	//--------------------------------- Input Variable 3: uncon_length-------------------------------------------------------
 	// Get Address of inputs 
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
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarThree, &m3, &n3, &uncon_length);
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
	itpp::vec v(matrixOfBits, m1*n1);  
	itpp::vec gen(Generator, m2*n2) ;
	itpp::vec modulated_bits;
  	itpp::Convolutional_Code conv_code;
	itpp::BPSK bpsk;	

	//-----------------------------------------Convolutional decoding--------------------------------------------------------
	conv_code.set_generator_polynomials(itpp::to_ivec(gen),*uncon_length);		//Set the generator polynomials

	modulated_bits=bpsk.modulate_bits(itpp::to_bvec(v));				//BPSK Modulate the bits
	bvec decoded_bits = conv_code.decode(modulated_bits);				//Decode the input stream
	
	//----------------------------------------Output Variable: decoded_bits---------------------------------------------------
	newMatrixOfBits = (double*)malloc(sizeof(double) * 1* decoded_bits.length());	

    	//Assign to output variable
	for (int i = 0; i < 1*decoded_bits.length(); i++) 
		{
		newMatrixOfBits[i] = decoded_bits[i];              
		}


    	if (sciErr.iErr)
    		{
		printError(&sciErr, 0);
		return 0;
    		}

   	//-----------------------------------------Matrix for the convolutional decoded sequence----------------------------------------
    	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, decoded_bits.length(), newMatrixOfBits);
    	free(newMatrixOfBits); 						// Data have been copied into Scilab memory
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
