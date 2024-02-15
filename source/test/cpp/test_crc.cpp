#include "ccore/c_target.h"
#include "chash/c_crc.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(crc)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(CRC_CRC32)
        {
            u8  buffer[512] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            u8  buff2[512]  = {1, 1, 2, 3, 5, 8, 13, 21};
            s64 len         = 128;
            CHECK_NOT_EQUAL(xcrc::crc32(cbuffer_t(buffer, buffer + len), 1321), xcrc::crc32(cbuffer_t(buffer, buffer + len), 654321));
            CHECK_NOT_EQUAL(xcrc::crc32(cbuffer_t(buffer, buffer+254), 110), xcrc::crc32(cbuffer_t(buffer, buffer+25), 110));
            CHECK_EQUAL(xcrc::crc32(cbuffer_t(buffer, buffer + len), 0), xcrc::crc32(cbuffer_t(buffer, buffer + len)));
            CHECK_NOT_EQUAL(xcrc::crc32(cbuffer_t(buffer, buffer + len), 1321), xcrc::crc32(cbuffer_t(buff2, buff2 + len), 654321));
        }
        UNITTEST_TEST(CRC_Adler16)
        {
            u8  buffer[512] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            u8  buff2[512]  = {1, 1, 2, 3, 5, 8, 13, 21};
            s64 len         = 100;
            CHECK_EQUAL(xcrc::adler16(cbuffer_t(buffer, buffer + len), 1), xcrc::adler16(cbuffer_t(buffer, buffer + len)));
            CHECK_NOT_EQUAL(xcrc::adler16(cbuffer_t(buffer, buffer + len), 110), xcrc::adler16(cbuffer_t(buffer, buffer + len), 5));
            CHECK_NOT_EQUAL(xcrc::adler16(cbuffer_t(buffer, buffer + len), 110), xcrc::adler16(cbuffer_t(buff2, buff2 + len), 110));
            CHECK_NOT_EQUAL(xcrc::adler16(cbuffer_t(buffer, buffer + len), 110), xcrc::adler16(cbuffer_t(buffer, buffer + 50), 110));
            CHECK_EQUAL(xcrc::adler16(cbuffer_t(buffer, buffer), 358), ((358 & 0xFF) | ((358 >> 8) << 8)));
        }
        UNITTEST_TEST(CRC_Adler32)
        {
            u8  buffer[512] = {1, 2, 3, 51, 54, 65, 98};
            u8  buff2[512]  = {1, 1, 2, 3, 5, 8, 13, 21};
            s64 len         = 100;
            CHECK_EQUAL(xcrc::adler32(cbuffer_t(buffer, buffer + len), 1), xcrc::adler32(cbuffer_t(buffer, buffer + len)));
            CHECK_NOT_EQUAL(xcrc::adler32(cbuffer_t(buffer, buffer + len), 110), xcrc::adler32(cbuffer_t(buffer, buffer + len), 5));
            CHECK_NOT_EQUAL(xcrc::adler32(cbuffer_t(buffer, buffer + len), 123456), xcrc::adler32(cbuffer_t(buff2, buff2 + len), 123456));
            CHECK_NOT_EQUAL(xcrc::adler32(cbuffer_t(buffer, buffer + len), 110), xcrc::adler32(cbuffer_t(buffer, buffer + 50), 110));
            CHECK_EQUAL(xcrc::adler32(cbuffer_t(buffer, buffer), 112358), ((112358 & 0xFFFF) | ((112358 >> 16) << 16)));
        }
    }
}
UNITTEST_SUITE_END