/*=============================================================================

  CMAKECATCHMPI: A software package to demo MPI with CMake, CTest and Catch.

  Copyright (c) University College London (UCL). All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

  See LICENSE.txt in the top level directory for details.

=============================================================================*/

#include "ccmpiMyFunctions.h"
#include <iostream>
#include <omp.h>

namespace ccmpi {

//-----------------------------------------------------------------------------
void GetFirstAndLastIndex(const int& numThreads,
                          const unsigned long int& i,
                          const double& elementsPerThread,
                          const unsigned long int& numberElements,
                          unsigned long int& first,
                          unsigned long int& last)
{
  first = i * elementsPerThread + 1;
  last = ((i + 1) * elementsPerThread + 1) - 1;
  if (last < first) last = first;
  if (last > numberElements) last = numberElements;
  if (i + 1 == numThreads) last = numberElements;
}


//-----------------------------------------------------------------------------
std::vector<std::pair<unsigned long int, unsigned long int> >
GetVectorOfSeriesIndexPairs(const unsigned int& numberOfThreads,
                            const unsigned long int& numberOfElements
                           )
{

  double elementsPerThread = static_cast<double>(numberOfElements)
                           / static_cast<double>(numberOfThreads);

  std::vector<std::pair<unsigned long int, unsigned long int> > v;
  for (int i = 0; i < numberOfThreads; i++)
  {
    unsigned long int first = 0;
    unsigned long int last = 0;
    GetFirstAndLastIndex(numberOfThreads, i, elementsPerThread, numberOfElements, first, last);
    v.push_back(std::pair<unsigned long int, unsigned long int>(first, last));

    std::cout << "For " << numberOfElements << " elements, "
              << " spread over " << numberOfThreads << " processes/threads, "
              << " the i=" << i << " block "
              << " started at " << first
              << " and finished at " << last << std::endl;
  }
  return v;
}


//-----------------------------------------------------------------------------
void FillSeries(unsigned long int* const values, unsigned long int const& numberOfValues)
{
  for(unsigned long int i=0;i<numberOfValues;i++)
  {
    values[i] = i+1;
  }
}


//-----------------------------------------------------------------------------
unsigned long int SumSeries(const unsigned long int* const values,
                            unsigned long int const & numberOfValues)
{
  unsigned long int sum = 0;
  for(int i=0;i<numberOfValues;i++)
  {
    sum += values[i];
  }
  return sum;
}


//-----------------------------------------------------------------------------
double EvaluateGregoryLeibnizSeries(unsigned long int startingTermNumber,
                                    unsigned long int finishingTermNumber
                                   )
{
  double sum = 0;
  double *odd_even = new double[2](); /*Using ints here limits precision, 
  and using pow(-1,n) will give worse precision for large n */
  odd_even[0] = -1;
  odd_even[1] = 1;

  for (unsigned long i = startingTermNumber; i <=finishingTermNumber; i++)
  {
    double n = i*2 - 1;
    double term = odd_even[i%2] * (1/n);
    sum += term;
  }
  return sum;
}


//-----------------------------------------------------------------------------
double EvaluatePiUsingOpenMP(unsigned long int numberElements)
{

  // Use OpenMP to find number of threads and initialise vector to apportion series
  unsigned int numThreads = omp_get_max_threads();
  std::vector<std::pair<unsigned long int, unsigned long int> >  // we have 2 cpu cores available on the VM
  vector = GetVectorOfSeriesIndexPairs(numThreads,numberElements);

  
  // Split vector into chunks and reduce sum across threads 
  double sum = 0;

  #pragma omp parallel reduction(+:sum)
    {
      unsigned long int this_thread_num = omp_get_thread_num();
      sum = EvaluateGregoryLeibnizSeries(vector[this_thread_num].first,vector[this_thread_num].second);
    }

  // Leibniz formula returns pi/4
  return 4*sum;
}

} // end namespace
