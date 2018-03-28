/*=============================================================================

  CMAKECATCHMPI: A software package to demo MPI with CMake, CTest and Catch.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "catch.hpp"
#include "ccmpiCatchMain.h"
#include "ccmpiMyFunctions.h"
using namespace std;

TEST_CASE( "2. FillSeries Test", "[CW2]" ) {
	unsigned long int *test_array = new unsigned long int[10]();
	ccmpi::FillSeries(test_array,10);
	REQUIRE( test_array[0] == 1 );
	REQUIRE( test_array[1] == 2 );
	REQUIRE( test_array[9] == 10 );
	//REQUIRE_THROWS_AS(test_array[20], std::logic_error);
}

TEST_CASE( "2. SumSeries Test", "[CW2]" ) {
	// Define empty long int array
	unsigned long int *test_array = new unsigned long int[10]();
	// Populate array using FillSeries
	ccmpi::FillSeries(test_array,10);
	REQUIRE(ccmpi::SumSeries(test_array,1) == 1);
	REQUIRE(ccmpi::SumSeries(test_array,3) == 6);
	REQUIRE(ccmpi::SumSeries(test_array,10) == 55);
}

TEST_CASE( "3. EvaluateGregoryLeibnizSeries Test", "[CW2]" ) {
	REQUIRE(ccmpi::EvaluateGregoryLeibnizSeries(1, 1) == Approx(1).epsilon(0.0001));
	REQUIRE(ccmpi::EvaluateGregoryLeibnizSeries(1, 2) == Approx(0.66666667).epsilon(0.0001));
	REQUIRE(ccmpi::EvaluateGregoryLeibnizSeries(1, 3) == Approx(0.86666667).epsilon(0.0001));
	REQUIRE(ccmpi::EvaluateGregoryLeibnizSeries(2, 5) == Approx(-0.165079365).epsilon(0.0001));
	REQUIRE(ccmpi::EvaluateGregoryLeibnizSeries(5, 5) == Approx(0.11111111).epsilon(0.0001));
	REQUIRE(ccmpi::EvaluateGregoryLeibnizSeries(10, 20) == Approx(-0.04018532).epsilon(0.0001));
}
