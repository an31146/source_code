/*
 * g++ -Wall -O2 -std=c++11 -o decompressor_api.exe decompressor_api.cpp -lcabinet
 */
#include <iomanip>
#include <iostream>
#include <vector>

#define NTDDI_VERSION 0x06020000
#define BUFFER_SIZE 0x8000
#define ZIP_HEADER_LEN 0X1c
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
	
	string strZipFilename = argv[1];
	string strFilename = strZipFilename;
	
	if (strFilename.rfind(".zip") > 0)
	{
		// assumes original extension is preserved.
		strFilename.erase(strFilename.rfind(".zip"));
	}
	
	COMPRESSOR_HANDLE c_hnd;
	CreateDecompressor(COMPRESS_ALGORITHM_MSZIP, NULL, &c_hnd);
	if (c_hnd == NULL)
	{
		cout << "CreateDecompressor failed." << endl << "Error: " <<  GetLastError() << endl;
		exit(-1);
	}
	
	vector<char> buffer(BUFFER_SIZE);
	vector<char> zipHeader(ZIP_HEADER_LEN);
	vector<char> compressed_buffer(BUFFER_SIZE+52);
	size_t uncompressed_size = 0;
	//size_t compressed_data_size;
	uint32_t compressed_data_size = 0;
	size_t compressed_data_size_read;
	size_t zipHeader_size;
	
	FILE *fs1 = fopen(strZipFilename.c_str(), "rb");
	if (fs1 == NULL)
	{
		cout << "File open failed." << endl << "Error: " <<  GetLastError() << endl;
		exit(-1);
	}
	FILE *fs2 = fopen(strFilename.c_str(), "wb");
	if (fs2 == NULL)
	{
		cout << "File create failed." << endl << "Error: " <<  GetLastError() << endl;
		exit(-1);
	}
		
/*
BOOL Decompress(
  DECOMPRESSOR_HANDLE DecompressorHandle,
  LPCVOID             CompressedData,
  SIZE_T              CompressedDataSize,
  PVOID               UncompressedBuffer,
  SIZE_T              UncompressedBufferSize,
  PSIZE_T             UncompressedDataSize
);*/
		
	do
	{
		zipHeader_size = fread((LPVOID)zipHeader.data(), sizeof zipHeader[0], ZIP_HEADER_LEN, fs1);
		cout << zipHeader_size << " header bytes read:" << endl;
		
		for (auto b : zipHeader)
			printf("%02X ", b & 0xff);
		cout << endl;

		//sscanf(zipHeader.data()+8, "%p", (void **)&compressed_data_size);
		memcpy(&uncompressed_size, zipHeader.data()+0x00000010UL, sizeof(uint64_t));
		memcpy(&compressed_data_size, zipHeader.data()+0x00000018UL, sizeof(uint32_t));
		printf("%llu bytes in uncompressed data block.\n", uncompressed_size);
		printf("%5u bytes in compressed data block.\n", compressed_data_size);
		//printf("sizeof(size_t): %llu bytes\n", sizeof(size_t));
		//cout << compressed_data_size << " bytes." << endl;
		
		memcpy(compressed_buffer.data(), zipHeader.data(), ZIP_HEADER_LEN);
		compressed_data_size_read = fread((LPVOID)&compressed_buffer.data()[ZIP_HEADER_LEN], sizeof compressed_buffer[0], compressed_data_size, fs1);
		
		// for (auto b : compressed_buffer)
			// printf("%0X ", b & 0xff);
		
		cout << setw(5) << compressed_data_size_read << " bytes of compressed data read." << endl;

		if (compressed_data_size > 0 && compressed_data_size_read > 0)
		{
			int result = Decompress(c_hnd, (LPVOID)compressed_buffer.data(), compressed_data_size + ZIP_HEADER_LEN, (LPVOID)buffer.data(), buffer.size(), &uncompressed_size);
			if (result == 0)
			{
				cout << "Decompress failed." << endl << "Error: " <<  GetLastError() << endl;
				exit(-1);
			}
			else
			{	
				__attribute__((unused)) size_t bytes_written = fwrite((LPVOID)buffer.data(), sizeof buffer[0], uncompressed_size, fs2);
				cout << bytes_written << " bytes of decompressed data written." << endl;
			}
			//getchar();
		}
	} while (compressed_data_size_read > 0);
	/*
	for (auto c : buffer)
		cout << c;
	cout << endl;
	*/
	
	if (c_hnd != 0)
		CloseDecompressor(c_hnd);
	if (fs1 != NULL)		
		fclose(fs1);
	if (fs2 != NULL)		
		fclose(fs2);
	
	return 0;
}
