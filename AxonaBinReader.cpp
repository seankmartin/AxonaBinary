#include "AxonaBinReader.h"
#include "Utils.h"
#include <fstream>
#include <iostream>

int16_t AxonaBinReader::ConvertBytes(char b1, char b2)
{
	uint16_t us = (uint8_t)b1 * 256 + (uint8_t)b2;
	int16_t out = (int16_t) us;
	return out;
}

AxonaBinReader::AxonaBinReader()
{
}

AxonaBinReader::AxonaBinReader(std::string name)
{
	Init(name);
}

void AxonaBinReader::Init(std::string name)
{
	SetSetFname(name);
	std::string bin_name = name.substr(0, name.length() - 4);
	bin_name.append(".bin");
	SetBinFname(bin_name);
}

bool const AxonaBinReader::Read()
{
	long fsize = GetFileSize(GetBinFname());
	long total_samples = fsize / _chunksize;
	total_samples *= _samples_per_chunk;
	std::cout << total_samples << std::endl;

	const int buff_size = _chunksize;
	std::vector<char> buffer(buff_size, 0);
	std::vector<std::vector<int16_t>> channel_data(
		_num_channels, std::vector<int16_t>(total_samples, 0));

	std::ifstream infile;
	infile.open(_bin_fname, std::ios::binary | std::ios::in);
	int sample_count = 0;
	while (infile.read(buffer.data(), buffer.size())) {
		for (int i = _header_bytes; i < _chunksize - _trailer_bytes; i = i + _sample_bytes) {
			int compare_val = (i - _header_bytes) / 2;
			int row_sample = compare_val % _num_channels;
			int col_sample = sample_count + (compare_val / _num_channels);
			int16_t val = ConvertBytes(buffer[i + 1], buffer[i]);
			channel_data[_reverse_map_channels[row_sample]][col_sample] = val;
		}
		sample_count += 3;
	}
	// std::ostream outfile;
	return true;
}

int main() {
	AxonaBinReader axbr{
		"C:\\Users\\smartin5\\Recordings\\Raw\\Raw_160819\\LCA7_34_35_36.set"};
	axbr.Read();
}