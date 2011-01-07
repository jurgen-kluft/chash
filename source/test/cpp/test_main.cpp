#include "xbase\x_target.h"
#include "xbase\x_types.h"

#ifdef TARGET_360
	#include <Xtl.h>
#endif
#ifdef TARGET_WII
	#include <string.h>
#endif

#include "xunittest\xunittest.h"

using namespace xcore;

UNITTEST_SUITE_LIST(xCoreUnitTest);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xdouble);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xendian);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xfloat);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xinteger);
UNITTEST_SUITE_DECLARE(xCoreUnitTest, xstring_std);

#ifndef X_NO_CUSTOM_INT64
UNITTEST_SUITE_DECLARE(xCoreUnitTest, __xint64);
#endif // X_NO_CUSTOM_INT64

int main(int argc, char** argv)
{
	UnitTest::TestReporterStdout reporter;
	int r = UNITTEST_SUITE_RUN(reporter, xCoreUnitTest);

	return r;
}