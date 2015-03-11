/**
* volImage.cpp
* Author: Alan Berman
* 07/03/2015
*/
#include "vector.h"
#include "volImage.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace BRMALA003{
using namespace std;
VolImage::VolImage() 
{
		width=0;
		height=0;
		vector<unsigned char**> slices;
}

VolImage::~VolImage()
{
	
}
	
bool VolImage::readImages(string baseName)
{
	string mriFile = "MRI.data";
	string info,count,filename;
	string base_name="MRI";
	streampos size;
	stringstream out;
	int width,height,number_images;
    ifstream i(mriFile.c_str());
   
    if(!i)
		{ cout << "Couldn't open file."  << endl; 
		  return false;
		}
	
	while (!i.eof())
	{
		getline(i,info);
		istringstream ss(info);
		ss >> width;
		ss >> height;
		ss >> number_images; 
	}
	
	for (int i=0;i<number_images;++i)
	{		
			unsigned char ** images = new unsigned char *[height];
			//https://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
			out << i;	
			count=out.str();	
			for (int j = 0;j<height;++j)
			{
				//Adapted from http://www.cplusplus.com/doc/tutorial/files/
				ifstream img(string("brain_mri_raws" + base_name + ".raw").c_str(), ios::in|ios::binary|ios::ate	);
				if (img.is_open())	
				{
					images[j] = new unsigned char[width];			
                    img.read ((char*)images[j], width);
				}		
			}
			img.close();    
			slices.push_back(images); //NOT SURE IF THIS IS OK/RIGHT
	}	
	return true;
}

void VolImage::diffmap(int sliceI, int sliceJ, string output_prefix)
{
	
}

void VolImage::extract(int sliceId, string output_prefix)	
{
	
}

int VolImage::volImageSize(void)
{
	return 0;
}	
}
