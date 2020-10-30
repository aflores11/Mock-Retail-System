#include <iostream>
#include <cmath>
#include <vector>

template <class T, class Comparator>
void merge(std::vector<T> &input, Comparator comp, int l, int r, int m)
{   

    int s1 = l, e1=m, s2=m, e2=r;

    std::vector<T> output;
   while(s1<e1 && s2<e2) 
   {
        if(comp(input[s1],input[s2])) //compares values using the functor provided to provide different sortings
        {
        output.push_back(input[s1]);
        s1++;
        }
        else
        {
        output.push_back( input[s2]);
        s2++;
        } 
   }   
      
   while(s1<e1) //pushes whatever is left from the smaller sorted section
   {
      output.push_back(input[s1]);
      s1++;
   }
    while(s2<e2) //pushes whatever is left from the other smaller sorted section if not done; both do not run
    {
        output.push_back(input[s2]);  
        s2++;
    }

    for(int i = 0; i<(int)output.size(); i++) //updates the vector starting from the index given
    {
         input[l] = output[i];
         l++;
} 


}

template <class T, class Comparator>
void mSort(std::vector<T> &myArray, Comparator comp, int start, int end )
{
    if(start < end-1) // takes care of the infinite loop when start is 0, end is 1 and mid is 0; 
    {
        int middle = (start+end)/2;
        mSort(myArray, comp, start, middle);
        mSort(myArray, comp,  middle, end);
        merge(myArray, comp, start, end, middle);
    }
}


template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, Comparator comp)
{
    if(myArray.size() == 1 ) return;
    mSort(myArray, comp, 0, myArray.size());
}
