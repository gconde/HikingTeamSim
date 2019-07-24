#ifndef _BARCODETEST_H_
#define _BARCODETEST_H_

#include "Barcode.h"
#include "PropertyFile.h"
#include "gtest/gtest.h"

extern std::string keyG;
extern PropertyFile propertiesG;
extern std::string resourceDirG;
extern std::string bczFileG;

class BarcodeTest : public ::testing::Test {
protected:
	static void SetUpTestCase();
	static void TearDownTestCase();
};

#endif //_BARCODETEST_H_
