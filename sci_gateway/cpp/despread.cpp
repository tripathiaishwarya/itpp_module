/*
Sequence De-spreading:

received_bits=despread(received_signal, spreading_code)
received_signal should be -1/2s and 1/2s
spreading_code should be 1s and -1s
received_bits will be 0s and 1s

"spread" replaces each 0 of the input bitstream by the spreading_code and each 1 of the input bitstream by negative of the spreading_code
and vice-versa for "despread".
The length of the output of despread=length(received_signal)/length(spreading_code)
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

int sci_despread(char *fname, unsigned long fname_len)
	{
    	// Error management variable
    	SciErr sciErr;

    	//-------------------------------------Variable Declaration----------------------------------------------------------
    	int m1 = 0, n1 = 0, m2=0, n2=0;			 		//Input variable lengths

    	int *piAddressVarOne = NULL;
	int *piAddressVarTwo = NULL;

    	double *matrixOfBitStream = NULL;				//Input stream of already spread bits
	double *matrixOfSpreadCode = NULL;				//Input stream of spreading/despreading code

    	double *newMatrixOfDouble = NULL;				//Output stream of despread code

   	//----------------------------------Check the number of input and output arguments-------------------------------------
    	CheckInputArgument(pvApiCtx, 2, 2) ;				//Two inputs: received_symbol, spreading_code
    	CheckOutputArgument(pvApiCtx, 1, 1) ;				//One output: received_bits

     	//--------------------------------- Input Variable 1:received_symbol-------------------------------------------------------
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

	//Check if the input is a stream of 1/2 and -(1/2)
      	for (int i = 0; i < m1 * n1; i++)
		{
		if (matrixOfBitStream[i]!= -0.5 && matrixOfBitStream[i]!= 0.5)
			{
			Scierror(999, "%s: Value of matrix of double should be -1/2 or 1/2.\n", fname, 1);
        		return 0;
			}
		}

     	//--------------------------------- Input Variable 2:spreading_code-------------------------------------------------------
    	// Get Address of inputs 
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
    	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &matrixOfSpreadCode);
    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}
   	
	//Check if the input has a single row/coloumn
     	if(m2!=1 && n2!=1)
		{
		Scierror(999, "%s: Single row or column expected.\n", fname, 2);
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

        //-----------------------------------------Dimension Check----------------------------------------------------------------
	//Check if the two are 1-D arrays
        if((m1!=m2 && m1==1) || (n1!=n2 && n1==1))
		{
		Scierror(999, "%s: Same type of arrays expected.\n", fname, 2);
        	return 0;
		}

	//----------------------------------------Output Variable: transmitted_bits---------------------------------------------------
	newMatrixOfDouble = (double*)malloc(sizeof(double) * (m1/m2)*(n1/n2));
        
	//-----------------------------------------Declaration of ITPP objects--------------------------------------------------------
	itpp::vec received_signal(matrixOfBitStream, m1*n1);
	itpp::vec spreading_code(matrixOfSpreadCode, m2*n2);
    	itpp::BPSK bpsk;
    	itpp::Spread_1d spread_1d(spreading_code);
  
	//-----------------------------------------De-spreading and demodulation operation --------------------------------------------
	vec received_symbols  = spread_1d.despread(received_signal,0);		//Despreading
        bvec received_bits = bpsk.demodulate_bits(received_symbols);		//Demodulation
		
	//Assign 1 to boolean TRUE and 0 to boolean FALSE
	for (int i = 0; i < (m1/m2)*(n1/n2); i++) 
		{
		if(received_bits[i]==TRUE)
			newMatrixOfDouble[i]=1;
		else if (received_bits[i]==FALSE)
			newMatrixOfDouble[i]=0;        
		}

    	if (sciErr.iErr)
    		{
        	printError(&sciErr, 0);
        	return 0;
    		}

   	//-----------------------------------------Matrix for the despread signal---------------------------------------------------
    	sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m1/m2, n1/n2, newMatrixOfDouble);
    	free(newMatrixOfDouble); 					// Data have been copied into Scilab memory
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
