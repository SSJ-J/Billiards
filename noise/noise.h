#pragma once

#include <vector>
using namespace std;

class PerlinNoiseCreator {
public:
	vector<int> generateNoise(int rows, int cols);
	vector<vector<float>> MapGray(vector<vector<float>> perlinNoise);
	vector<vector<float>> MapGradient(vector<vector<float>> perlinNoise);

private:
	vector<vector<float>> GenerateWhiteNoise(int width, int height);
	vector<vector<float>> GenerateSmoothNoise(vector<vector<float>> baseNoise, int octave);
	inline float Interpolate(float x0, float x1, float alpha);
	vector<vector<float>> GeneratePerlinNoise(vector<vector<float>> baseNoise, int octaveCount);

};
