#ifndef TAMA_TAG_H
#define TAMA_TAG_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "tama_file.h"

using namespace std;

using time_secs = double;
using time_mins = uint16_t;

using block = vector<byte>;
using header = vector<byte>;
using body = vector<byte>;
using length = size_t;

using sample_rate = uint32_t;
using channel_count = uint16_t;
using bits_per_sample = uint16_t;
using samples_count = uint64_t;

const size_t HEADER_SIZE = 4;
const size_t SIGNATURE_SIZE = 4;
const header FLAC_SIGNATURE = {byte{0x66}, byte{0x4c}, byte{0x61}, byte{0x43}};

enum block_type 
{
    STREAMINFO = 0x00,
    PADDING = 0x01,
    APPLICATION = 0x02,
    SEEKTABLE = 0x03,
    VORBIUS_COMMENT = 0x04,
    CUESHEET = 0x05,
    PICTURE = 0x06,
};

struct user_comment
{
	string _field;
	string _content;
	bool _isModified;
};

struct metadata 
{
	block_type _type;
	length _length;
	body _body;
	bool _isExist = false;
	bool _isLast = false;
};

struct metadata_streaminfo : metadata
{
	metadata_streaminfo() {
		_type = STREAMINFO;
		_length = 34;
	}
};

struct metadata_padding : metadata
{
	metadata_padding() {_type = PADDING;}
};

struct metadata_application : metadata
{
	metadata_application() {_type = APPLICATION;}
};

struct metadata_seektable : metadata
{
	metadata_seektable() {_type = SEEKTABLE;}
};

struct metadata_vorbius : metadata
{
	metadata_vorbius() {_type = VORBIUS_COMMENT;}
	
	string _vendor;
	vector<user_comment> _userComments;
};

struct metadata_cuesheet : metadata
{
	metadata_cuesheet() {_type = CUESHEET;}
};

struct metadata_picture : metadata
{
	metadata_picture() {_type = PICTURE;}
};

class song : public file 
{
	private:
	metadata_streaminfo _streaminfo;
	metadata_padding _padding;
	metadata_application _application;
	metadata_seektable _seektable;
	metadata_vorbius _vorbiusComment;
	metadata_cuesheet _cuesheet;
	metadata_picture _picture;

	public:
	void loadMetadata();
	void setStreaminfo(const body);
	void setVorbiusComment(const body);
	void setPadding(const length);

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

	metadata& metadataBlock(const block_type);

	void listMetadata();
	void listStreaminfo();
	void listVorbiusComment();

	void editUserComment(user_comment);

	block encodeMetadata();

	block encodeStreaminfo(const bool);
	block encodePadding(const bool);
	block encodeVorbiusComment(const bool);
};

void printBytes(const vector<byte>);

header encodeLEHeader(const length);
header encodeBEHeader(const length, const block_type, const bool);

length decodeBEHeader(const header);
length decodeLEHeader(const header);

void scanSongs(vector<song>&, vector<file>&);
void listSongs(const vector<song>&);

#endif // TAMA_TAG_H