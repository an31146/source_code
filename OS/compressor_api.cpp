/*
 * g++ -Wall -O2 -std=c++11 -o compressor_api.exe compressor_api.cpp -lcabinet
 */
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#define NTDDI_VERSION 0x06020000
#define BUFFER_SIZE 32768
#include <compressapi.h>
#include <windows.h>

int main(int argc, char *argv[])
{
	using namespace std;
	
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <filename>" << endl;
		exit(-1);
	}
	
	string strFilename = argv[1];
	string strZipFilename = strFilename + ".zip";
	
	COMPRESSOR_HANDLE c_hnd;
	CreateCompressor(COMPRESS_ALGORITHM_MSZIP, NULL, &c_hnd);
	if (c_hnd == NULL)
	{
		cout << "CreateCompressor failed." << endl << "Error: " <<  GetLastError() << endl;
		exit(-1);
	}
	
	vector<char> buffer(BUFFER_SIZE);
	vector<char> compressed_buffer(BUFFER_SIZE+52);
	size_t uncompressed_size;
	size_t compressed_data_size;
	
	FILE *fs1 = fopen(strFilename.c_str(), "rb");
	if (fs1 == NULL)
	{
		cout << "File open failed." << endl << "Error: " <<  GetLastError() << endl;
		exit(-1);
	}
	FILE *fs2 = fopen(strZipFilename.c_str(), "wb");
	if (fs2 == NULL)
	{
		cout << "File create failed." << endl << "Error: " <<  GetLastError() << endl;
		exit(-1);
	}
		
/*
BOOL Compress(
  COMPRESSOR_HANDLE CompressorHandle,
  LPCVOID           UncompressedData,
  SIZE_T            UncompressedDataSize,
  PVOID             CompressedBuffer,
  SIZE_T            CompressedBufferSize,
  PSIZE_T           CompressedDataSize
);
*/
		
	do
	{
		uncompressed_size = fread((LPVOID)buffer.data(), sizeof buffer[0], buffer.size(), fs1);
		cout << uncompressed_size << " bytes uncompressed data read." << endl;

		if (uncompressed_size > 0)
		{
			int result = Compress(c_hnd, (LPVOID)buffer.data(), uncompressed_size, (LPVOID)compressed_buffer.data(), compressed_buffer.size(), &compressed_data_size);
			if (result == 0)
			{
				cout << "Compress failed." << endl << "Error: " <<  GetLastError() << endl << compressed_data_size << endl;
				exit(-1);
			}
			else
			{	
				__attribute__((unused)) size_t bytes_written = fwrite((LPVOID)compressed_buffer.data(), sizeof compressed_buffer[0], compressed_data_size, fs2);
				cout << setw(5) << bytes_written << " bytes compressed data written." << endl;
			}
		}
	} while (uncompressed_size > 0);
	/*
	for (auto c : buffer)
		cout << c;
	cout << endl;
	*/
	
	if (c_hnd != 0)
		CloseCompressor(c_hnd);
	if (fs1 != NULL)		
		fclose(fs1);
	if (fs2 != NULL)		
		fclose(fs2);
	
	return 0;
}
