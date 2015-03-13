/**
* volImage.cpp
* Author: Alan Berman
* 07/03/2015
*/

//Using the VolImage class defined in volImage.h, volImage.cpp reads in .raw image files
//which represent various MRI scans into a vector, which stores pointers to a 2D array representing
//the various scans. The user may view a specific image (or slice) of the vector (written to a file) or compute a
// 'difference map' using two slices of the vector (again written to a file), but not both.
#include "volImage.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace BRMALA003{
using namespace std;
//Constructor
VolImage::VolImage() 
{
		width=0;
		height=0;
		vector<unsigned char**> slices;
		//Extra field to represent total number of scans read in
		number_images=0;
}
//Destructor
VolImage::~VolImage()
{
	//For each scan in the vector
	for (unsigned char **image:slices)
	{
		for (int i = 0; i < this->number_images; ++i)
		{
			//Delete each row of the scan
			delete [] image[i];
		}
		//delete the scan itself
		delete [] image;
	}
}
//Populates the vector of MRI scans
bool VolImage::readImages(string baseName)
{
	string mriFile = baseName;
	mriFile+=".data";
	string info,count,filename;
	string base_name=baseName;
	streampos size;
	stringstream out;
	//Read in the header file
    ifstream i(mriFile.c_str());
    if(!i)
		{ cout << "Couldn't open file."  << endl; 
		  return false;
		}
	//Extract the width, height and number of images (contained in the header file)
	while (!i.eof())
	{
		getline(i,info);
		istringstream ss(info);
		ss >> width;
		ss >> height;
		ss >> this->number_images;
	}
	//Allocate sufficient memory for the vector of slices
	slices.resize(this->number_images);
	for (int i=0;i<this->number_images;++i)
	{
			//Create memory for each MRI scan
			unsigned char ** images = new unsigned char *[height];
			//Adapted from https://notfaq.wordpress.com/2006/08/30/c-convert-int-to-string/
			//To use i as a way of opening the ith scan in the folder [as both indexed from 0]
			out.str("");
			out << i;	
			count=out.str();
			//Adapted from http://www.cplusplus.com/doc/tutorial/files/
			//Open a MRI scan
			ifstream img(string(base_name + count+ ".raw").c_str(), ios::in|ios::binary);
			for (int j = 0;j<height;++j)
			{
				//Create memory for each row of bytes in a scan, then read the particular bytes
				//into the row
				unsigned char * image_row = new unsigned char[width];
				img.read ((	char*)image_row, width);
				images[j]=image_row;
			}
			//Add the MRI scan to the vector
			slices[i]=images;
			img.close();    			 
	}
	//Used to inform the user of how many images/scans were added to the vector
	cout << slices.size()<< endl;	
	return true;
}
//Computes a difference map given 2 slices and writes the result to a file
void VolImage::diffmap(int sliceI, int sliceJ, string output_prefix)
{
	ofstream out;
	output_prefix+=".raw";
	out.open(output_prefix.c_str(), ios::out | ios::binary);
	//Create memory for a new scan
	unsigned char ** scans = new unsigned char *[height];
	for (int r=0;r<height;r++)
	{
		unsigned char * row = new unsigned char[width];
		for (int z=0;z<width;z++)
		{
			//Create the bytes for each row of the scan, then add the resulting row to the difference map scan
			row[z]=(unsigned char)(abs(slices[sliceI][r][z] -slices[sliceJ][r][z])/2);
			scans[r]=row;
			//out.write((char *)(unsigned char)(abs((float)slices[sliceI][y][z] - (float)slices[sliceJ][y][z])/2),width);
		}
		//Write out each row of the scan
		out.write((char*)scans[r],width);
	}
	out.close();
}
//Extracts a particular MRI scan from the vector and writes the content to a file
void VolImage::extract(int sliceId, string output_prefix)	
{
	ofstream outHeader;
	outHeader.open("output.dat");
	//Write the number of images and dimensions of the scan to a header file
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
		//Writes each line of the particular scan to the output file
		output.write((char *)slices	[sliceId][y],width); 
	}
	output.close();
}

//Calculates the total size (in bytes) needed to represent the images/scans
int VolImage::volImageSize(void)
{
	//Total size of the chars in each scan
	int size_of_chars = width*height;
	//Size of the pointers to each line of the scan
	int size_of_pointers= height *sizeof(unsigned char* );
	int size_of_image=size_of_chars+size_of_pointers;
	int total_size=size_of_image*this->number_images;
	return total_size;
}	
}
