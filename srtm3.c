#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main (int argc, char **argv)
{
	if (argc < 3)
	{
		printf("Usage: srtm3 [lat] [lon]\n");
		return -1;
	}
	float lat = atof(argv[1]);
	float lon = atof(argv[2]);

  	// hgt file consists of a 1201x1201 grid of 16 bit elevation values
  	// the name coresponds to the lower left coordinate of the grid
  	// e.g. N46E014 means [0,0] = N47E014, [0,1201] = N47E015, [1201,0] = N46E014, [1201,1201] = N46E015
	FILE *hgt_file;
	
	// find the correct file
	float lat_name = floorf(lat);
	float lon_name = floorf(lon);

	char hgt_name[12];
	sprintf(hgt_name, "N%02.0fE%03.0f.hgt", lat_name, lon_name);
	
	printf("I'll open the file: %s\n", hgt_name);
	hgt_file = fopen(hgt_name, "r"); // open the hgt file

	if (!hgt_file)
	{
		printf("The needed hgt file could not be found.\n");
		return -1;
	}

	// calculate the correct file offset
	int value_offset = 2; // 16 bit short integers, 2 bytes
	
	float lat_delta = lat - lat_name;
	float lon_delta = lon - lon_name;
	
	int strm_size = 1201;
	int lat_row = (int)(lat_delta*strm_size);
	int lon_col = (int)(lon_delta*strm_size);

	int file_offset = (lon_col + strm_size * (strm_size - lat_row - 1)) * value_offset;
	printf("The correct elevation is at the row %d and column %d file offset in bytes: %d\n", lat_row, lon_col, file_offset);

	// seek to the file offset
	fseek(hgt_file, file_offset, SEEK_SET);
	
	// read 16 byte elevation
	char buffer[2];
	fread(buffer, 1, value_offset, hgt_file);
	
	unsigned int elevation = (buffer[0] & 0xff);
	elevation = elevation << 8;
	elevation |= (buffer[1] & 0xff);
	elevation &= 0xffff;
	
	printf("Elevation at %.4f %.4f is %u m\n", lat, lon, elevation);
	
	// close file
  	fclose(hgt_file) ;

	return 0;
}
