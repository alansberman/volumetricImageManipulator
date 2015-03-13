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
	cout << "Number of images: ";
	string base = string(argv[1]);
	v.readImages(base);
	for (int i=2;i<argc;i++)
	{
		string s = string(argv[i]);
		if 	(s=="-d")
		{
			i++;
			s=string(argv[i]);
			//cout << s << endl;
			int j = atoi(s.c_str());
			i++;
			//cout << j << endl;
			s=string(argv[i]);
			int k = atoi(s.c_str());
			//cout << k << endl;
			i++;
			s=string(argv[i]);
			//cout << s << endl;
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


//	string g = "output_prefix";
//	string j = "uit";
//	v.extract(15,g); //
//	v.diffmap(0,1,j);
	cout << "Number of bytes required: " << v.volImageSize() << endl;
	return 0;
}	
