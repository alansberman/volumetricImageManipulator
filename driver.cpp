/**
* driver.cpp
* Author: Alan Berman
* 07/03/2015
*/
#include <sstream>
#include <iostream>
#include "volImage.h"
//Driver that creates a VolImage object, and allows the user to either 
//view a difference map/extract a particular slice from the vector
//If these optional flags are not set, the default is to 
//The total number of slices, as well as the total bytes needed
// to represent the images are outputted to the user.
using namespace std;
int main(int argc, char * argv[])
{
	BRMALA003::VolImage v;
	//Flag used to prevent the user from creating a difference map
	//AND extracting a particular slice from the vector
	int flag = 0;
	cout << "Number of images: ";
	//the prefix for the MRI scan sequence
	string base = string(argv[1]);
	v.readImages(base);
	for (int i=2;i<argc;i++)
	{
		//To check for if the user has opted to create a difference map
		string s = string(argv[i]);
		if 	(s=="-d")
		{
			i++;
			s=string(argv[i]);
			//Extract the j [first slice] from the args
			int j = atoi(s.c_str());
			i++;
			s=string(argv[i]);
			//Extract the k [second slice] from the args
			int k = atoi(s.c_str());
			i++;
			//Extract the name of the output file
			s=string(argv[i]);
			v.diffmap(j,k,s);
			//Change the flag to prevent user from also extracting a slice
			flag =1;
		}
		if (s=="-x")
		{
			if (flag==1)
			{
				break;
			}
			else
			{
				i++;
				s=string(argv[i]);
				//Extract the slice from the args
				int l = atoi(s.c_str());
				i++;
				//Extract the name of the output file
				s=string(argv[i]);		
				v.extract(l,s);
			}
		}	
	}	
	cout << "Number of bytes required: " << v.volImageSize() << endl;

	return 0;
}	
