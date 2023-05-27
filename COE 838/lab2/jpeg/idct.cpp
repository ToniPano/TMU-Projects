#include "idct.h"

//inverse discrete cosine transform
void idct :: calculate_idct(void){
	unsigned char 	u, v, x, y;
	double 	temp;

	// do forward discrete cosine transform
	for (x = 0; x < 8; x++)
		for (y = 0; y < 8; y++) {
			temp = 0.0;
			for (u = 0; u < 8; u++)
				for (v = 0; v < 8; v++)
					temp += in64[u][v].read() * fcosine[x][u].read() * fcosine[y][v].read();
					
					if ((u == 0) && (v == 0))
						temp /= 8.0;
					else if (((u == 0) && (v != 0)) || ((u != 0) && (v == 0)))
						temp /= (4.0*sqrt(2.0));
					else
						temp /= 4.0;
			
			output_data[x][y] = temp;
		}
	
	printf(".");
}

//read 8x8 block and shift signed integers
void idct :: write_data(void){
	// shift the signed integers to the unsigned integer range of [0, 2^8 - 1]
	// of range [2^(8-1), 2^(8-1) - 1]
	for (unsigned char uv = 0; uv < 64; uv++)
		output_data[uv/8][uv%8] += (char) (pow(2,8-1));
	// read the 8x8 block as a continuous 64 values and store them in
	// input_data as an 8x8 block
	fwrite(output_data, 1, 64, sc_output.read());
}
