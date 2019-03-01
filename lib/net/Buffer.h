#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H
#include <list>
#include <vector>
#include <set>
#include <map>
#include <assert.h>
#include <stdint.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <algorithm>
#include <memory>

class  Buffer {
public:
	class error {
	};

	const static size_t DEFAULT_SIZE = 4096; //4096
	Buffer() : _rpos(0), _wpos(0) {
		_storage.resize(DEFAULT_SIZE);
	}
	Buffer(size_t res) : _rpos(0), _wpos(0) {
		_storage.resize(res);
	}
	Buffer(const Buffer &buf) : _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage) { }
	virtual ~Buffer() {}

	void Reset() {
		_rpos = _wpos = 0;
	}

	template <typename T> void Append(T value) {
		Append((uint8_t *)&value, sizeof(value));
	}

	Buffer &operator<<(bool value) {
		Append<char>((char)value);
		return *this;
	}
	// unsigned
	Buffer &operator<<(uint8_t value) {
		Append<uint8_t>(value);
		return *this;
	}
	Buffer &operator<<(uint16_t value) {

		Append<uint16_t>(value);

		return *this;
	}
	Buffer &operator<<(uint32_t value) {

		Append<uint32_t>(value);
		return *this;
	}
	Buffer &operator<<(uint64_t value) {

		Append<uint64_t>(value);

		return *this;
	}
	// signed as in 2e complement
	Buffer &operator<<(int8_t value) {
		Append<int8_t>(value);
		return *this;
	}
	Buffer &operator<<(int16_t value) {

		Append<int16_t>(value);

		return *this;
	}
	Buffer &operator<<(int32_t value) {

		Append<int32_t>(value);

		return *this;
	}
	Buffer &operator<<(int64_t value) {

		Append<int64_t>(value);

		return *this;
	}
	Buffer &operator<<(float value) {

		Append<float>(value);

		return *this;
	}
	Buffer &operator<<(double value) {

		Append<double>(value);

		return *this;
	}

	Buffer &operator<<(const std::string &value) {
		//printf("Buffer << str len = %d \n",value.length());
		Append((uint32_t)value.length());
		Append((uint8_t *)value.c_str(), value.length());
		return *this;
	}
	Buffer &operator<<(const char *str) {
		Append((uint8_t *)str, strlen(str));
		Append((uint8_t)0);
		return *this;
	}

	// stream like operators for reading data
	Buffer &operator>>(bool &value) {
		value = Read<char>() > 0 ? true : false;
		return *this;
	}
	//unsigned
	Buffer &operator>>(uint8_t &value) {
		value = Read<uint8_t>();
		return *this;
	}
	Buffer &operator>>(uint16_t &value) {

		value = Read<uint16_t>();

		return *this;
	}
	Buffer &operator>>(uint32_t &value) {

		value = Read<uint32_t>();
		return *this;
	}
	Buffer &operator>>(uint64_t &value) {

		value = Read<uint64_t>();

		return *this;
	}
	//signed as in 2e complement
	Buffer &operator>>(int8_t &value) {
		value = Read<int8_t>();
		return *this;
	}
	Buffer &operator>>(int16_t &value) {

		value = Read<int16_t>();

		return *this;
	}
	Buffer &operator>>(int32_t &value) {

		value = Read<int32_t>();

		return *this;
	}
	Buffer &operator>>(int64_t &value) {

		value = Read<int64_t>();
		return *this;
	}
	Buffer &operator>>(float &value) {

		value = Read<float>();

		return *this;
	}
	Buffer &operator>>(double &value) {

		value = Read<double>();

		return *this;
	}
	Buffer &operator>>(std::string& value) {
		uint32_t len = Read<uint32_t>();
		size_t old_rpos = _rpos;
		if (Read(len))
		{
			value.clear();
			value.reserve(len);
			value.append((char*)(&_storage[old_rpos]), len);
		}
		return *this;
	}

	//获取下一个完整的包，如果大小不够则获取不成功
	bool CheckAndGetNextMsg(std::string& value)
	{
		if (ReadableSize()< sizeof(uint32_t))
			return false;
		uint32_t length = *((uint32_t*)&_storage[_rpos]);
		if (ReadableSize() < sizeof(uint32_t) + length)
			return false;
		(*this) >> value;
		return true;
	}

	template <typename T> T Read() {
		size_t old_rpos = _rpos;
		if (Read(sizeof(T)))
		{
			return *((T*)&_storage[old_rpos]);
		}
		else
		{
			return (T)0;
		}
	};

	//读后读read_size个字节，返回能否读取成功
	bool Read(size_t read_size)
	{
		if (ReadableSize() < read_size)
		{
			Reset();
			return false;
		}
		else
		{
			_rpos += read_size;
			//数据全部读完重置_wpos, _rpos
			if (ReadableSize() == 0)
			{
				Reset();
			}
		}
		return true;
	}

	size_t ReadableSize()
	{
		return _wpos - _rpos;
	}

	size_t WritableSize()
	{
		return GetTotalSize() - _wpos;
	}

	size_t ReadPos(){return _rpos;}
	void SetRreadPos(size_t pos){_rpos = pos;}
	size_t WritePos(){return _wpos;}
	void SetWritePos(size_t pos){_wpos = pos;}

	char *WriteBegin() const {return (char*)&_storage[_wpos];}
	const char *ReadBegin() const { return (char *)&_storage[_rpos]; }

	inline size_t GetTotalSize() const { return _storage.size(); };


	// appending to the end of buffer
	void Append(const char *src, size_t cnt) {
		return Append((const uint8_t *)src, cnt);
	}
	void Append(const uint8_t *src, size_t cnt) {
		if (!cnt) return;
		assert(GetTotalSize() < 10000000);

		CheckResize(cnt);
		memcpy(&_storage[_wpos], src, cnt);
		_wpos += cnt;
	}
private:
	void CheckResize(size_t cnt)
	{
		if (WritableSize() < cnt)
		{
			//前面+后面剩余的空间不能放下cnt个数据时才会选择扩容
			if (_rpos + WritableSize() < cnt)
			{
				_storage.resize(_wpos + cnt);
			}
			//现存的数据前移
			memcpy(&_storage[0],&_storage[_rpos],_wpos - _rpos);
			_rpos = 0;
			_wpos = _wpos - _rpos;
		}
	}

protected:
	// read and write positions
	size_t _rpos, _wpos;
	std::vector<uint8_t> _storage;
};


#endif
