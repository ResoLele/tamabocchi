#ifndef TAMA_TAG_H
#define TAMA_TAG_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>

#include "tama_file.h"

using namespace std;

using time_secs = double;
using time_mins = uint16_t;

using header = vector<byte>;
using body = vector<byte>;
using length = uint32_t;

using metadata_streaminfo = vector<byte>;

using sample_rate = uint32_t;
using channel_count = uint16_t;
using bits_per_sample = uint16_t;
using samples_count = uint64_t;

using user_comment = pair<string, string>;

const size_t HEADER_SIZE = 4;
const size_t SIGNATURE_SIZE = 4;

enum block_type 
{
    STREAMINFO = 0x00,
    PADDING = 0x01,
    APPLICATION = 0x02,
    SEEKTABLE = 0x03,
    VORBIUS_COMMENT = 0x04,
    CUESHEET = 0x05,
    PICTURE = 0x06,
    INVALID = 0x07,
};


struct metadata 
{
	block_type _type;
	length _length;
};

struct metadata_vorbius : metadata
{
	header _vendorHeader;
	string _vendor;

	vector<user_comment> _userComments;
};

class song : public file 
{
	private:
	metadata_streaminfo _streaminfo;
	metadata_vorbius _vorbiusComment;

	public:
	void loadMetadata();
	void setStreaminfo(const body);
	void setVorbiusComment(const body);

	sample_rate sampleRate();
	channel_count channel();
	bits_per_sample bps();
	samples_count totalSamples();
	
	time_secs totalSecs();
	time_mins minutes();
	time_secs seconds();
	
	size_t userCommentCount();
	string userCommentField(const uint32_t);
	string userCommentContent(const string);

	void listMetadata();
};

length readBEHeader(const header);
length readLEHeader(const header);

void scanSongs(vector<song>&, vector<file>&);
void listSongs(const vector<song>&);

#endif // TAMA_TAG_H