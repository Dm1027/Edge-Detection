//John Merino
//Edge Detection

#include<iostream>
#include<fstream>
#include <string>
#include <cmath>
using namespace std;

//Sobel Kernels
const int Hx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
const int Hy[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};

//Laplacian Kernel
const int LapKernel[3][3] = {{0,1,0},{1,-4,1},{0,1,0}};

int main(int argc, char *argv[]) {

	ifstream input_1;
	ofstream output_1, output_2,output_3,output_4;
	string header;

	if (argc > 1) {
		// file name 
		input_1.open(argv[1], ios::in | ios::binary);

		

	}
	else {
		fprintf(stderr, "argv[1] for reading");
		exit(0);
	}
	getline(input_1, header);

	//header w,h extract
	string s_w, s_h;
	s_w = header.substr(3, 3);
	s_h = header.substr(7, 3);
	int w = stoi(s_w), h = stoi(s_h);

	//matrix for first channel 
	int** in_matrix_ch1 = new int*[h];
	for (int i = 0; i < h; ++i)
		in_matrix_ch1[i] = new int[w];

	//matrix for second channel
	int** in_matrix_ch2 = new int*[h];
	for (int i = 0; i < h; ++i)
		in_matrix_ch2[i] = new int[w];

	//matrix for thrid channel
	int** in_matrix_ch3 = new int*[h];
	for (int i = 0; i < h; ++i)
		in_matrix_ch3[i] = new int[w];


	//reading 
	for (int i_h = 0; i_h<h; i_h++) {
		for (int i_w = 0; i_w<w; i_w++) {
			char r, g, b;
			input_1.get(r);
			input_1.get(g);
			input_1.get(b);
			in_matrix_ch1[i_h][i_w] = r;
			in_matrix_ch2[i_h][i_w] = g;
			in_matrix_ch3[i_h][i_w] = b;
		}

	}

	int grayMatrix[h][w];
	for (int i_h = 0; i_h<h; i_h++) {
		for (int i_w = 0; i_w<w; i_w++) {
			grayMatrix[i_h][i_w] = (in_matrix_ch1[i_h][i_w] + in_matrix_ch2[i_h][i_w] + in_matrix_ch3[i_h][i_w]) / 3;
		}
	}

	int choice;
	cout << "Choose whether you would like to see Sobel or Laplacian edge detection on the input image" <<endl;
	cout << "1: Sobel" << endl << "2: Laplacian" << endl;
	cin >> choice;
	int threshold;
	cout << "Enter an integer to use as the threshold, 1 - 255" << endl;
	cin >> threshold;

	if (choice == 1)
	{

		output_1.open("SobelEdgesX.ppm", ios::out | ios::binary);
		output_2.open("SobelEdgesY.ppm", ios::out | ios::binary);
		output_3.open("SobelEdgesCombined.ppm", ios::out | ios::binary);

		cout << "Sobel edge output to 3 files:" << endl;
		cout << "\t SobelEdgesX.ppm" << endl;
		cout << "\t SobelEdgesY.ppm" << endl;
		cout << "\t SobelEdgesCombined.ppm" << endl;

		int SobelMatrixX[h][w];
			for (int i = 1; i<h; i++) {
				for (int j = 1; j<w; j++) {
					int TL = grayMatrix[i-1][j-1] * Hx[0][0];
					int ML = grayMatrix[i-1][j] * Hx[1][0];
					int BL = grayMatrix[i-1][j+1] * Hx[2][0];
					int TR = grayMatrix[i+1][j+1] * Hx[0][2];
					int MR = grayMatrix[i+1][j] * Hx[1][2];
					int BR = grayMatrix[i+1][j-1] * Hx[2][2];

					SobelMatrixX[i][j] = TL+ML+BL+TR+MR+BR;
					
				}
			}

		int SobelMatrixY[h][w];
			for (int i = 1; i<h; i++) {
				for (int j = 1; j<w; j++) {
					int TL = grayMatrix[i-1][j-1] * Hy[0][0];
					int TM = grayMatrix[i-1][j] * Hy[0][1];
					int TR = grayMatrix[i-1][j+1] * Hy[0][2];
					int BL = grayMatrix[i+1][j+1] * Hy[2][0];
					int BM = grayMatrix[i+1][j] * Hy[2][1];
					int BR = grayMatrix[i+1][j-1] * Hy[2][2];

					SobelMatrixY[i][j] = TL+TM+TR+BL+BM+BR;
					
				}
			}

		int SobelMatrixOutput[h][w];
			for (int i = 1; i<h; i++) {
				for (int j = 1; j<w; j++) {
					SobelMatrixOutput[i][j] = sqrt((SobelMatrixX[i][j]*SobelMatrixX[i][j]) + (SobelMatrixY[i][j]*SobelMatrixY[i][j]));
					
					if (SobelMatrixOutput[i][j] > threshold)
					{
						SobelMatrixOutput[i][j] = 255;
					}
					else SobelMatrixOutput[i][j] = 0;

				}
			}




			//output matrix 
			output_1 << header << endl;
			for (int i_h = 0; i_h<h; i_h++) {
				for (int i_w = 0; i_w<w; i_w++) {
					output_1 << (char)SobelMatrixX[i_h][i_w];
					output_1 << (char)SobelMatrixX[i_h][i_w];
					output_1 << (char)SobelMatrixX[i_h][i_w];

				}

			}

			output_2 << header << endl;
			for (int i_h = 0; i_h<h; i_h++) {
				for (int i_w = 0; i_w<w; i_w++) {
					output_2 << (char)SobelMatrixY[i_h][i_w];
					output_2 << (char)SobelMatrixY[i_h][i_w];
					output_2 << (char)SobelMatrixY[i_h][i_w];

				}

			}
			output_3 << header << endl;
			for (int i_h = 0; i_h<h; i_h++) {
				for (int i_w = 0; i_w<w; i_w++) {
					output_3 << (char)SobelMatrixOutput[i_h][i_w];
					output_3 << (char)SobelMatrixOutput[i_h][i_w];
					output_3 << (char)SobelMatrixOutput[i_h][i_w];

				}

			}
	}	
	else if (choice == 2)
	{

		output_4.open("LaplacianEdges.ppm", ios::out | ios::binary);

		cout << "Laplacian edge output to 1 file:" << endl;
		cout << "\t LaplacianEdges.ppm" << endl;

		int LaplacianMatrix[h][w];
			for (int i = 1; i<h; i++) {
				for (int j = 1; j<w; j++) {
					int TL = grayMatrix[i-1][j-1] * LapKernel[0][0];
					int TM = grayMatrix[i-1][j] * LapKernel[0][1];
					int TR = grayMatrix[i-1][j+1] * LapKernel[0][2];
					int ML = grayMatrix[i-1][j] * LapKernel[0][1];
					int MM = grayMatrix[i][j] * LapKernel[1][1];
					int MR = grayMatrix[i][j+1] * LapKernel[1][2];
					int BL = grayMatrix[i+1][j+1] * LapKernel[2][0];
					int BM = grayMatrix[i+1][j] * LapKernel[2][1];
					int BR = grayMatrix[i+1][j-1] * LapKernel[2][2];

					LaplacianMatrix[i][j] = TL+TM+TR+ML+MM+MR+BL+BM+BR;

					if (LaplacianMatrix[i][j] > threshold)
					{
						LaplacianMatrix[i][j] = 255;
					}
					else LaplacianMatrix[i][j] = 0;
					
				}
			}

			output_4 << header << endl;
			for (int i_h = 0; i_h<h; i_h++) {
				for (int i_w = 0; i_w<w; i_w++) {
					output_4 << (char)LaplacianMatrix[i_h][i_w];
					output_4 << (char)LaplacianMatrix[i_h][i_w];
					output_4 << (char)LaplacianMatrix[i_h][i_w];

				}

			}
	}

	else
	{
		cout << "Error, must select 1 or 2" << endl;
	}
	return 0;
}




