
#include "xbase/x_target.h"

#include "xhash/x_hash.h"

namespace xcore
{
    xhash::xmd5 xhash::md5;
    xhash::xsha1 xhash::sha1;

    xhash::xskein256 xhash::skein256;
    xhash::xskein512 xhash::skein512;
    xhash::xskein1024 xhash::skein1024;

    xhash::xmurmur32 xhash::murmur32;
    xhash::xmurmur64 xhash::murmur64;

	xhash::xxhash64 xhash::xxhash;
}
