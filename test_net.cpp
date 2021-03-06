/*
 * This code was made to forward a image and get the car predictions using the DetectNet
 * it receive as input one image and save result in a kitti's format txt file.
 * Author: Luan
 */


#include "detect_net.hpp"

#include <stdio.h>

#define FNAME_LENGTH 255

void removeFileExtension(const char *name_src, char *name_dest)
{
	int i, index = 0;	

	for(i = 0; i < strlen(name_src); i++)
		if(name_src[i] == '.')
			index = i;

	if(index != 0)
	{
		memset(name_dest, '\0', strlen(name_dest));
		memcpy(name_dest, name_src, index);
	}
		
}

int main(int argc, char **argv)
{
	if (argc != 4)
		exit(printf("Usage %s <file filenames> <file prototxt> <file caffemodel>\n", argv[0]));

	FILE *input_file = fopen(argv[1], "rt");
	std::string model_file = argv[2];
	std::string trained_file = argv[3];
	
	if(input_file == NULL)
		exit(printf("Failed to open the file %s\n", argv[1]));

	// use the gpu and first device
	int gpu = 1;
	int device_id = 0;

	// loads the detect net
	// the network layers weights need to be loaded only once.
	DetectNet detectNet(model_file, trained_file, gpu, device_id);

	while(!feof(input_file))
	{
		
		char input_file_name[FNAME_LENGTH];

		// Read file name from input_file
		fscanf(input_file, "%[^\n]\n", input_file_name);
		printf("%s\n", input_file_name);
		
		cv::Mat img = cv::imread(input_file_name, -1);
	
		// forward the image through the network
		std::vector<float> result = detectNet.Predict(img);
	
		// fix the scale of image
		float correction_x = img.cols / 1250.0;
		float correction_y = img.rows / 380.0;
	
		char output_file_name[FNAME_LENGTH];
		memset(output_file_name, '\0', FNAME_LENGTH);

		removeFileExtension(input_file_name, output_file_name);
		sprintf(output_file_name, "%s.txt", output_file_name);
	
		FILE *output_file = fopen(output_file_name, "w");
	
		for (int i = 0; i < 10; i++)
		{
			// top left
			float xt = result[5*i] * correction_x;
			float yt = result[5*i + 1] * correction_y;
	
			// botton right
			float xb = result[5*i + 2] * correction_x;
			float yb = result[5*i + 3] * correction_y;
	
			float confidence = result[5*i + 4];
			if (confidence > 0.0)
				fprintf(output_file, "Car 0.00 0 0.00 %.2f %.2f %.2f %.2f 0.00 0.00 0.00 0.00 0.00 0.00 0 %.2f\n", xt, yt, xb, yb, confidence);
		}
	}

	return 0;
}
