#include <iostream>
#include <cmath>
#include <vector>

template <class T, class Comparator>
void merge(std::vector<T> &input, Comparator comp, int l, int r, int m)
{   

    int s1 = l, e1=m, s2=m, e2=r;
   // std::cout << "s1:" << s1 << " e1:" << e1 <<" " << " s2:" << s2 <<" " << " e2:" << e2 << "\n";

    std::vector<T> output;
   while(s1<e1 && s2<e2)
   {
    //    std:: cout << "\t" << input[s1] << " " << input[s2] << "\n\n";
        if(comp(input[s1],input[s2]))
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
      
   while(s1<e1)
   {
      output.push_back(input[s1]);
      s1++;
   }
    while(s2<e2)
    {
        output.push_back(input[s2]);  
        s2++;
    }

/*
     for (int i = 0; i < input.size(); i++)
  {
    std::cout << input[i] << " ";
  }
    std::cout << "\t";

    for (int i = 0; i < output.size(); i++)
  {
    std::cout << output[i] << " ";
  }
    std::cout << "\n";

*/

    for(int i = 0; i<(int)output.size(); i++)
    {
         input[l] = output[i];
         l++;
} 

   // std::cout<< "\n\n";


}

template <class T, class Comparator>
void mSort(std::vector<T> &myArray, Comparator comp, int start, int end )
{
    if(start < end-1)
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
