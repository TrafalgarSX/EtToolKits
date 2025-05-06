#ifndef _HEXUTIL_H_
#define _HEXUTIL_H_

#include <cstdint>
#include <memory>
#include <string>
#include <vector>


std::string bytesToHexString(const std::vector<uint8_t> buf, char sep = 0);
std::string bytesToHexString(const uint8_t* buf, int len, char sep = 0);
std::vector<uint8_t> hexStringToBytes(const std::string& hexString, char sep = 0);

std::shared_ptr<uint8_t> hexStringToBytes(const std::string& hexString, int& outLen, char sep = 0);

#endif
