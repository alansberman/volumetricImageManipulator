/**
* driver.cpp
* Author: Alan Berman
* 07/03/2015
*/
#include <sstream>
#include <iostream>
#include "volImage.h"

using namespace std;
int main(int argc, char * argv[])
{
	BRMALA003::VolImage v;
	int flag = 0;
	for (int i=1;i<argc;i++)
	{
		string s = string(argv[i]);
		if 	(s=="-d")
		{
			i++;
			s=string(argv[i]);
			int j = atoi(s.c_str());
			i++;
			s=string(argv[i]);
			int k = atoi(s.c_str());
			i++;
			s=string(argv[i]);
			v.diffmap(j,k,s);
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
				int l = atoi(s.c_str());
				i++;
				s=string(argv[i]);		
				v.extract(l,s);
			}
		}	
	}	
	string base = string(argv[1]);
	cout << "Number of images: ";
	v.readImages(base);
	string g = "output_prefix";
	v.extract(15,g); //
	v.diffmap(1,2,g);
	cout << "Number of bytes required: " << v.volImageSize() << endl;
	return 0;
}	
