#include <stdio.h>
#include <math.h>

#define B		1000 	// adjust accuracy there 
#define BARR 	(B+1)

float calculateLocation(unsigned int* x, int n)
{
  // Compute the mean and standard deviation
  float sum = 0;
  int i;
  for (i = 0; i < n; i++) {
    sum += x[i];
  }
  float mu = sum/n;

  sum = 0;
  for (i = 0; i < n; i++) {
    sum += (x[i]-mu)*(x[i]-mu);
  }
  float sigma = sqrt(sum/n);

  // Bin x across the interval [mu-sigma, mu+sigma]
  int bottomcount = 0;
  int bincounts[BARR];
  for (i = 0; i < BARR; i++) {
    bincounts[i] = 0;
  }
  float scalefactor = B/(2*sigma);
  float leftend =  mu-sigma;
  float rightend = mu+sigma;
  int bin;

  for (i = 0; i < n; i++) {
    if (x[i] < leftend) {
      bottomcount++;
    }
    else if (x[i] < rightend) {
      bin = (int)((x[i]-leftend) * scalefactor);
      bincounts[bin]++;
    }
  }

  // If n is odd
  if (n & 1) {
    // Find the bin that contains the median
    int k = (n+1)/2;
    int count = bottomcount;

    for (i = 0; i < BARR; i++) {
      count += bincounts[i];

      if (count >= k) {
        return (i+0.5)/scalefactor + leftend;
      }
    }
  }
  
  // If n is even
  else {
    // Find the bins that contains the medians
    int k = n/2;
    int count = bottomcount;
    
    for (i = 0; i < BARR; i++) {
      count += bincounts[i];
      
      if (count >= k) {
        int j = i;
        while (count == k) {
          j++;
          count += bincounts[j];
        }
        return (i+j+1)/(2*scalefactor) + leftend;
      }
    }
  } 
} 

int main()
{
	unsigned int houses[] = { 12, 3, 5, 19, 4, 7, 26};
	size_t n = sizeof(houses) / sizeof(houses[0]);
	float res = calculateLocation(houses, n);
	printf("Result: %f\n", res);

	return 0;
}
