/**
* volImage.cpp
* Author: Alan Berman
* 07/03/2015
*/

#include "volImage.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>

namespace BRMALA003{
using namespace std;
VolImage::VolImage() 
{
		width=0;
		height=0;
		vector<unsigned char**> slices;
		number_images=0;
}

VolImage::~VolImage()
{
	
}
	
bool VolImage::readImages(string baseName)
{
	string mriFile = baseName;
	mriFile+=".data";
	string info,count,filename;
	string base_name=baseName;
	streampos size;
	stringstream out;
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
		ss >> this->number_images;
	}
	slices.resize(this->number_images);
	for (int i=0;i<this->number_images;++i)
	{		
			unsigned char ** images = new unsigned char *[height];
			//https://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
			out.str("");
			out << i;	
			count=out.str();
			//Adapted from http://www.cplusplus.com/doc/tutorial/files/
			ifstream img(string(base_name + count+ ".raw").c_str(), ios::in|ios::binary);
			for (int j = 0;j<height;++j)
			{
				unsigned char * image_row = new unsigned char[width];
				img.read ((	char*)image_row, width);
				images[j]=image_row;
			}
			slices[i]=images;
			img.close();    			 
	}
	cout << slices.size()<< endl;	
	return true;
}

void VolImage::diffmap(int sliceI, int sliceJ, string output_prefix)
{
	ofstream out;
	output_prefix+=".raw";
	out.open(output_prefix.c_str(), ios::out | ios::binary);
	unsigned char ** scans = new unsigned char *[height];
	for (int r=0;r<height;r++)
	{
		unsigned char * row = new unsigned char[width];
		for (int z=0;z<width;z++)
		{

			row[z]=(unsigned char)(abs(slices[sliceI][r][z] -slices[sliceJ][r][z])/2);
			scans[r]=row;
			//out.write((char *)(unsigned char)(abs((float)slices[sliceI][y][z] - (float)slices[sliceJ][y][z])/2),width);
		}
		out.write((char*)scans[r],width);

	}

	out.close();
}

void VolImage::extract(int sliceId, string output_prefix)	
{
	ofstream outHeader;
	outHeader.open("output.dat");
	if (outHeader.is_open())
	{
		outHeader  << width << " " << height << " " << 1 << endl;
	}
	outHeader.close();
	ofstream output;
	output_prefix+=".raw";
	output.open(output_prefix.c_str(), ios::out | ios::binary);
	for (int y=0;y<height;y++)
	{
		output.write((char *)slices	[sliceId][y],width); 
	}

	output.close();
}


int VolImage::volImageSize(void)
{
	int size_of_chars = width*height;
	int size_of_pointers= height *sizeof(unsigned char* );
	int size_of_image=size_of_chars+size_of_pointers;
	int total_size=size_of_image*this->number_images;//*sizeof(unsigned char *);
	return total_size;
}	
}
