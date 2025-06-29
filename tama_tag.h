#ifndef TAMA_TAG_H
#define TAMA_TAG_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <unordered_map>

#include "tama_file.h"

using TimeSecs = double;
using TimeMins = uint16_t;

using Block       = std::vector<std::byte>;
using Header      = std::vector<std::byte>;
using Body        = std::vector<std::byte>;
using Size        = size_t;
using HashMapTag  = std::unordered_map<std::string, std::string>;

using SampleRate    = uint32_t;
using ChannelCount  = uint16_t;
using BitsPerSample = uint16_t;
using SampleCount   = uint64_t;

const size_t HEADER_SIZE    = 4;
const size_t SIGNATURE_SIZE = 4;
const Header FLAC_SIGNATURE = {std::byte{0x66}, std::byte{0x4c}, std::byte{0x61}, std::byte{0x43}};

enum MetadataType {
    STREAMINFO      = 0x00,
    PADDING         = 0x01,
    APPLICATION     = 0x02,
    SEEKTABLE       = 0x03,
    VORBIUS_COMMENT = 0x04,
    CUESHEET        = 0x05,
    PICTURE         = 0x06,
};

// struct UserComment {
// 	std::string _field;
// 	std::string _content;
// 	bool   _isModified;
// };

struct Metadata {
	MetadataType _type;
	Size         _length;
	bool         _isExist = false;
	bool         _isLast = false;
};

const Size STREAMINFO_SIZE = 34;

struct StreaminfoBlock : Metadata {
	Body _body;
	StreaminfoBlock() {
		_type = STREAMINFO;
		_length = STREAMINFO_SIZE;
	}
};

struct PaddingBlock : Metadata {
	PaddingBlock() {_type = PADDING;}
};

struct ApplicationBlock : Metadata {
	ApplicationBlock() {_type = APPLICATION;}
};

struct SeektableBlock : Metadata {
	SeektableBlock() {_type = SEEKTABLE;}
};

struct VorbiusCommentBlock : Metadata {
	
	HashMapTag _tags;
	
	VorbiusCommentBlock() {_type = VORBIUS_COMMENT;}
	size_t size();

	std::string find(const std::string);
	// std::string              _vendor;
	// std::vector<UserComment> _userComments;
};

struct Cuesheet : Metadata {
	Cuesheet() {_type = CUESHEET;}
};

struct Picture : Metadata {
	Picture() {_type = PICTURE;}
};

class Song : public File {
	private:
	StreaminfoBlock     _streaminfo;
	PaddingBlock        _padding;
	ApplicationBlock    _application;
	SeektableBlock      _seektable;
	VorbiusCommentBlock _vorbiusComment;
	Cuesheet            _cuesheet;
	Picture             _picture;

	public:
	Song(const FileEntry&);
	void lazyLoad();
	
	void loadMetadata();
	void setStreaminfo(const Body);
	void setVorbiusComment(const Body);
	void setPadding(const Size);

	SampleRate sampleRate();
	ChannelCount channel();
	BitsPerSample bps();
	SampleCount totalSamples();
	
	TimeSecs totalSecs();
	TimeMins minutes();
	TimeSecs seconds();
	
	// size_t userCommentCount();
	// std::string userCommentField(const uint32_t);
	// std::string userCommentContent(const std::string);

	Metadata& metadataBlock(const MetadataType);

	void listMetadata();
	void listStreaminfo();
	void listVorbiusComment();

	std::string getTag(std::string);
	void setTag(std::string, const std::string);

	Block encodeMetadata();

	Block encodeStreaminfo(const bool);
	Block encodePadding(const bool);
	Block encodeVorbiusComment(const bool);
};

void printBytes(const std::vector<std::byte>);

Header encodeLEHeader(const Size);
Header encodeBEHeader(const Size, const MetadataType, const bool);

Size decodeBEHeader(const Header);
Size decodeLEHeader(const Header);

void scanSongs(std::vector<Song>&, std::vector<File>&);
void listSongs(const std::vector<Song>&);

#endif // TAMA_TAG_H