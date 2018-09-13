#include "xbase/x_target.h"
#include "xbase/x_allocator.h"

#include "xunittest/xunittest.h"
#include "xbase/x_console.h"

UNITTEST_SUITE_LIST(xHashUnitTest);
UNITTEST_SUITE_DECLARE(xHashUnitTest, xcrc);
UNITTEST_SUITE_DECLARE(xHashUnitTest, xmd5);
UNITTEST_SUITE_DECLARE(xHashUnitTest, xsha1);
UNITTEST_SUITE_DECLARE(xHashUnitTest, xskein);
UNITTEST_SUITE_DECLARE(xHashUnitTest, xuuid);
UNITTEST_SUITE_DECLARE(xHashUnitTest, xhash32);
UNITTEST_SUITE_DECLARE(xHashUnitTest, xhash64);

class UnitTestAllocator : public UnitTest::Allocator
{
public:
	xcore::xalloc*	mAllocator;
	int						mNumAllocations;

	UnitTestAllocator(xcore::xalloc* allocator)
		: mNumAllocations(0)
	{
		mAllocator = allocator;
	}

	void*	Allocate(size_t size)
	{
		++mNumAllocations;
		return mAllocator->allocate(size, 4);
	}
	void	Deallocate(void* ptr)
	{
		--mNumAllocations;
		mAllocator->deallocate(ptr);
	}
	void	Release()
	{
		
	}
};

bool gRunUnitTest(UnitTest::TestReporter& reporter)
{
	UnitTestAllocator unittestAllocator( xcore::gCreateSystemAllocator() );
	UnitTest::SetAllocator(&unittestAllocator);

	int r = UNITTEST_SUITE_RUN(reporter, xHashUnitTest);
	if (unittestAllocator.mNumAllocations!=0)
	{
		reporter.reportFailure(__FILE__, __LINE__, "xunittest", "memory leaks detected!");
		r = -1;
	}

	unittestAllocator.Release();
	return r==0;
}
