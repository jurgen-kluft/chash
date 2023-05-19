#include "ccore/c_target.h"
#include "cbase/c_memory.h"
#include "cbase/c_runes.h"

#include "chash/c_hash.h"
#include "chash/private/c_internal_hash.h"

#include "cunittest/cunittest.h"

using namespace ncore;

extern alloc_t *gTestAllocator;

namespace SkeinTestVectors
{
    // Original test-vectors
    struct Vector
    {
        u32 Len;
        char const *Msg;
        char const *Digest;
    };

    static inline u8 CharToByte(char ch)
    {
        u8 ich = 0;
        if ((ch >= '0') && (ch <= '9'))
            ich = (ch - '0');
        else if ((ch >= 'A') && (ch <= 'F'))
            ich = (ch - 'A') + 10;
        else if ((ch >= 'a') && (ch <= 'f'))
            ich = (ch - 'a') + 10;
        return ich;
    }

    static inline u8 TwoCharsToByte(char h, char l)
    {
        u8 ich = (CharToByte(h) << 4) | (CharToByte(l));
        return ich;
    }

    static u32 TextMsgToByteMsg(char const *_textmsg, u32 _length, u8 *_bytemsg)
    {
        for (u32 j = 0; j < _length; ++j)
            _bytemsg[j] = 0;

        u32 i = 0;
        u32 d = 0;
        while (true)
        {
            char const ch = _textmsg[i];
            if (ch == '\0')
                break;

            u8 const b = CharToByte(ch);
            for (u32 j = 0; j < _length - 1; j++)
                _bytemsg[j] = (_bytemsg[j] << 4) | (_bytemsg[j + 1] >> 4);
            _bytemsg[_length - 1] = (_bytemsg[_length - 1] << 4) | b;
            d++;
            i += 1;
        }
        return (d + 1) / 2;
    }

    static u8 ByteMsg[4096];

    static Vector Tests_256_256[] = {
        {8, "FF", "0B98DCD198EA0E50A7A244C444E25C23DA30C10FC9A1F270A6637F1F34E67ED2"}, {0, nullptr}};

    static Vector Tests_512_256[] = {
        {8, "CC", "A018268ED814E0AD0F2D0304E8FE3F4118FCEFC07454D07123CC2C3E40E06A4F"}, {16, "41FB", "F91902DDCC9688462E48F0BCDFCA031637F0D8DA577C1E2AA316B5C022450BF2"}, {0, nullptr}};

    static Vector Tests_512_512[] = {
        {2048, "724627916C50338643E6996F07877EAFD96BDF01DA7E991D4155B9BE1295EA7D21C9391F4C4A41C75F77E5D27389253393725F1427F57914B273AB862B9E31DABCE506E558720520D33352D119F699E784F9E548FF91BC35CA147042128709820D69A8287EA3257857615EB0321270E94B84F446942765CE882B191FAEE7E1C87E0F0BD4E0CD8A927703524B559B769CA4ECE1F6DBF313FDCF67C572EC4185C1A88E86EC11B6454B371980020F19633B6B95BD280E4FBCB0161E1A82470320CEC6ECFA25AC73D09F1536F286D3F9DACAFB2CD1D0CE72D64D197F5C7520B3CCB2FD74EB72664BA93853EF41EABF52F015DD591500D018DD162815CC993595B195", "B2A35CF130E39CF82D85B5E4205934C0550293326354A0F9473890048F05AD76369E17E86D5D3F841C211312155F0B46266D8FB0FB515F044BCEB32FFEBA2871"}, {4064, "4FBDC596508D24A2A0010E140980B809FB9C6D55EC75125891DD985D37665BD80F9BEB6A50207588ABF3CEEE8C77CD8A5AD48A9E0AA074ED388738362496D2FB2C87543BB3349EA64997CE3E7B424EA92D122F57DBB0855A803058437FE08AFB0C8B5E7179B9044BBF4D81A7163B3139E30888B536B0F957EFF99A7162F4CA5AA756A4A982DFADBF31EF255083C4B5C6C1B99A107D7D3AFFFDB89147C2CC4C9A2643F478E5E2D393AEA37B4C7CB4B5E97DADCF16B6B50AAE0F3B549ECE47746DB6CE6F67DD4406CD4E75595D5103D13F9DFA79372924D328F8DD1FCBEB5A8E2E8BF4C76DE08E3FC46AA021F989C49329C7ACAC5A688556D7BCBCB2A5D4BE69D3284E9C40EC4838EE8592120CE20A0B635ECADAA84FD5690509F54F77E35A417C584648BC9839B974E07BFAB0038E90295D0B13902530A830D1C2BDD53F1F9C9FAED43CA4EED0A8DD761BC7EDBDDA28A287C60CD42AF5F9C758E5C7250231C09A582563689AFC65E2B79A7A2B68200667752E9101746F03184E2399E4ED8835CB8E9AE90E296AF220AE234259FE0BD0BCC60F7A4A5FF3F70C5ED4DE9C8C519A10E962F673C82C5E9351786A8A3BFD570031857BD4C87F4FCA31ED4D50E14F2107DA02CB5058700B74EA241A8B41D78461658F1B2B90BFD84A4C2C9D6543861AB3C56451757DCFB9BA60333488DBDD02D601B41AAE317CA7474EB6E6DD", "961A63783683371125E7D4FD6455E60678AFCD354CE2C0A4FB299DAFB3C4EC46F45F48A63FF8EC29D44B3B033C931122924D3C2C9683D5C576E2F0453653CEA7"}, {2552, "3139840B8AD4BCD39092916FD9D01798FF5AA1E48F34702C72DFE74B12E98A114E318CDD2D47A9C320FFF908A8DBC2A5B1D87267C8E983829861A567558B37B292D4575E200DE9F1DE45755FAFF9EFAE34964E4336C259F1E66599A7C904EC02539F1A8EAB8706E0B4F48F72FEC2794909EE4A7B092D6061C74481C9E21B9332DC7C6E482D7F9CC3210B38A6F88F7918C2D8C55E64A428CE2B68FD07AB572A8B0A2388664F99489F04EB54DF1376271810E0E7BCE396F52807710E0DEA94EB49F4B367271260C3456B9818FC7A72234E6BF2205FF6A36546205015EBD7D8C2527AA430F58E0E8AC97A7B6B793CD403D517D66295F37A34D0B7D2FA7BC345AC04CA1E266480DEEC39F5C88641C9DC0BD1358158FDECDD96685BBBB5C1FE5EA89D2CB4A9D5D12BB8C893281FF38E87D6B4841F0650092D447E013F20EA934E18", "DC0CDEDBA4F46C081F84E8DB765CF6DA2570A0D5C638ABB74774FC6F8C9A2708F0AB027B0CAAEF047A2FEB08DB43DBA5D802F5541D58956F998013AC4E5C5897"}, {0, nullptr}};

}; // namespace SkeinTestVectors

UNITTEST_SUITE_BEGIN(skein)
{
    UNITTEST_FIXTURE(type)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}
    }

    UNITTEST_FIXTURE(generator)
    {
        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(Empty)
        {
        }

        UNITTEST_TEST(test_256_256)
        {
            skein256_t ctx;

            u8 *bytemsg = SkeinTestVectors::ByteMsg;
            SkeinTestVectors::Vector *test = SkeinTestVectors::Tests_256_256;
            while (test->Msg != nullptr)
            {
                nmem::memset(bytemsg, 0, sizeof(SkeinTestVectors::ByteMsg));

                u32 const test_bytelen = (test->Len + 7) / 8;
                u32 len = SkeinTestVectors::TextMsgToByteMsg(test->Msg, test_bytelen, bytemsg);
                CHECK_EQUAL(test_bytelen, len);

                nhash::skein256 hash;
                ctx.reset();
                ctx.hash(bytemsg, test_bytelen);
                ctx.end(hash.m_data);

                u32 const verify_len = SkeinTestVectors::TextMsgToByteMsg(test->Digest, 32, bytemsg);
                CHECK_EQUAL(hash.size(), verify_len);
                CHECK_TRUE(nmem::memcmp(bytemsg, hash.m_data, 32) == 0);
                test = test + 1;
            }
        }

        UNITTEST_TEST(test_512_512)
        {
            skein512_t ctx;

            u8 *bytemsg = SkeinTestVectors::ByteMsg;
            SkeinTestVectors::Vector *test = SkeinTestVectors::Tests_512_512;
            while (test->Msg != nullptr)
            {
                nmem::memset(bytemsg, 0, sizeof(SkeinTestVectors::ByteMsg));

                u32 const test_bytelen = (test->Len + 7) / 8;
                u32 len = SkeinTestVectors::TextMsgToByteMsg(test->Msg, test_bytelen, bytemsg);
                CHECK_EQUAL(test_bytelen, len);

                nhash::skein512 hash;
                ctx.reset();
                ctx.hash(bytemsg, test_bytelen);
                ctx.end(hash.m_data);

                u32 const verify_len = SkeinTestVectors::TextMsgToByteMsg(test->Digest, 64, bytemsg);
                CHECK_EQUAL(hash.size(), verify_len);
                CHECK_TRUE(nmem::memcmp(bytemsg, hash.m_data, 64) == 0);
                test = test + 1;
            }
        }
    }
}
UNITTEST_SUITE_END
