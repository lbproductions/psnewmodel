//----------------------------------------------------------------------------------------------------
//
//
//
//
//
//----------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
// Unit tests
//----------------------------------------------------------------------------------------------------
#include <QtTest/QtTest>

//----------------------------------------------------------------------------------------------------
// Libraries
//----------------------------------------------------------------------------------------------------
#include "Test.hpp"

//----------------------------------------------------------------------------------------------------
// Apple Aperture 2.1 - TIFF - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_aperture_tiff_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("pixelXDimension");
  QTest::addColumn<unsigned int>("pixelYDimension");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<double>("xResolution");
  QTest::addColumn<double>("yResolution");

  QTest::addColumn<double>("exposureBias");

  QTest::addColumn<QString>("dateAndTime");
  QTest::addColumn<QString>("dateAndTimeOriginal");
  QTest::addColumn<QString>("dateAndTimeDigitized");

  QTest::addColumn<QString>("subseconds");
  QTest::addColumn<QString>("subsecondsOriginal");
  QTest::addColumn<QString>("subsecondsDigitized");

  QTest::addColumn<double>("digitalZoomRatio");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<double>("fNumber");
  QTest::addColumn<double>("focalLength");
  QTest::addColumn<unsigned int>("focalLength35mm");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  QTest::addColumn<double>("maxAperture");

  QTest::addColumn<QString>("software");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // Aperture 2.1
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / Aperture 2.1 / 8bit TIFF")
      << "../../../testimages/test_1_aperture_2_1_8bit.tiff"
      << static_cast<unsigned int>(4272)
      << static_cast<unsigned int>(2828)
      << static_cast<unsigned int>(4272)
      << static_cast<unsigned int>(2828)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(0.0)
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00 ";

  QTest::newRow("Test 1 / Aperture 2.1 / 16bit TIFF")
      << "../../../testimages/test_1_aperture_2_1_16bit.tiff"
      << static_cast<unsigned int>(4272)
      << static_cast<unsigned int>(2828)
      << static_cast<unsigned int>(4272)
      << static_cast<unsigned int>(2828)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(16)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(0.0)
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00 ";

  QTest::newRow("Test 2 / Aperture 2.1 / 8bit TIFF")
      << "../../../testimages/test_2_aperture_2_1_8bit.tiff"
      << static_cast<unsigned int>(2828)
      << static_cast<unsigned int>(4272)
      << static_cast<unsigned int>(2828)
      << static_cast<unsigned int>(4272)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(-1.0)
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00 ";

  QTest::newRow("Test 2 / Aperture 2.1 / 16bit TIFF")
      << "../../../testimages/test_2_aperture_2_1_16bit.tiff"
      << static_cast<unsigned int>(2828)
      << static_cast<unsigned int>(4272)
      << static_cast<unsigned int>(2828)
      << static_cast<unsigned int>(4272)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(16)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(-1.0)
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00 ";
}

//----------------------------------------------------------------------------------------------------
// Apple Aperture 1.6 - TIFF - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_aperture_tiff()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(QString,make);
  QFETCH(QString,model);

  QFETCH(unsigned int,pixelXDimension);
  QFETCH(unsigned int,pixelYDimension);

  QFETCH(unsigned int,depth);

  QFETCH(double,xResolution);
  QFETCH(double,yResolution);

  QFETCH(double,exposureBias);

  QFETCH(QString,dateAndTime);
  QFETCH(QString,dateAndTimeOriginal);
  QFETCH(QString,dateAndTimeDigitized);

  QFETCH(QString,subseconds);
  QFETCH(QString,subsecondsOriginal);
  QFETCH(QString,subsecondsDigitized);

  QFETCH(double,digitalZoomRatio);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(double,fNumber);
  QFETCH(double,focalLength);
  QFETCH(unsigned int,focalLength35mm);

  QFETCH(unsigned int,ISOSpeedRating);

  QFETCH(double,maxAperture);

  QFETCH(QString,software);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVERIFY(metaData->make().compare(make)   == 0);
  QVERIFY(metaData->model().compare(model) == 0);

  QCOMPARE(metaData->pixelXDimension(),pixelXDimension);
  QCOMPARE(metaData->pixelYDimension(),pixelYDimension);

  QVector<quint32> bitDepth = metaData->depth();

  QCOMPARE(bitDepth.size(),3);

  QCOMPARE(bitDepth[0],depth);
  QCOMPARE(bitDepth[1],depth);
  QCOMPARE(bitDepth[2],depth);

  QCOMPARE(metaData->xResolution(),xResolution);
  QCOMPARE(metaData->yResolution(),yResolution);

  QCOMPARE(metaData->exposureBias(),exposureBias);

  QVERIFY(metaData->dateAndTime().compare(dateAndTime) == 0);
  QVERIFY(metaData->dateAndTimeOriginal().compare(dateAndTimeOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitized().compare(dateAndTimeDigitized) == 0);

  QVERIFY(metaData->dateAndTimeSubseconds().compare(subseconds) == 0);
  QVERIFY(metaData->dateAndTimeOriginalSubseconds().compare(subsecondsOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitizedSubseconds().compare(subsecondsDigitized) == 0);

  QCOMPARE(metaData->digitalZoomRatio(),digitalZoomRatio);

  QCOMPARE(metaData->exposureTime().nominator(),exposureTimeNom);
  QCOMPARE(metaData->exposureTime().denominator(),exposureTimeDen);

  QCOMPARE(metaData->exposureTime().value(),static_cast<qreal>(exposureTimeNom)/static_cast<qreal>(exposureTimeDen));

  QCOMPARE(metaData->fNumber().value(),fNumber);
  QCOMPARE(metaData->focalLength().value(),focalLength);
  QCOMPARE(metaData->focalLength35mm(),focalLength35mm);

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(metaData->maxAperture(),maxAperture);

  QVERIFY(metaData->software().compare(software) == 0);

  QVERIFY(metaData->resolutionUnit()            == QImageMetaData::ResolutionInch);
  QVERIFY(metaData->compression()               == QImageMetaData::CompressionUncompressed);
  QVERIFY(metaData->exposureMode()              == QImageMetaData::ExposureModeAuto);
  QVERIFY(metaData->exposureProgram()           == QImageMetaData::ExposureProgramAperturePriority);
  QVERIFY(metaData->contrast()                  == QImageMetaData::ContrastNormal);
  QVERIFY(metaData->customRendered()            == QImageMetaData::CustomRenderedNormal);
  QVERIFY(metaData->flashStatus()               == QImageMetaData::FlashStatusNotFired);
  QVERIFY(metaData->gainControl()               == QImageMetaData::GainControlNone);
  QVERIFY(metaData->lightSource()               == QImageMetaData::LightSourceUnknown);
  QVERIFY(metaData->meteringMode()              == QImageMetaData::MeteringModeCenterWeightedAverage);
  QVERIFY(metaData->saturation()                == QImageMetaData::SaturationNormal);
  QVERIFY(metaData->sceneCaptureType()          == QImageMetaData::SceneCaptureTypeStandard);
  QVERIFY(metaData->sensingMethod()             == QImageMetaData::SensingMethodOneChipColorAreaSensor);
  QVERIFY(metaData->sharpness()                 == QImageMetaData::SharpnessNormal);
  QVERIFY(metaData->subjectDistanceRange()      == QImageMetaData::SubjectDistanceRangeUnknown);
  QVERIFY(metaData->whiteBalance()              == QImageMetaData::WhiteBalanceAuto);
  QVERIFY(metaData->photometricInterpretation() == QImageMetaData::PhotometricRGB);
  QVERIFY(metaData->orientation()               == QImageMetaData::OrientationTopLeft);

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Apple Aperture 1.6 - JPEG - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_aperture_jpeg_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("pixelXDimension");
  QTest::addColumn<unsigned int>("pixelYDimension");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<double>("xResolution");
  QTest::addColumn<double>("yResolution");

  QTest::addColumn<double>("exposureBias");

  QTest::addColumn<QString>("dateAndTime");
  QTest::addColumn<QString>("dateAndTimeOriginal");
  QTest::addColumn<QString>("dateAndTimeDigitized");

  QTest::addColumn<QString>("subseconds");
  QTest::addColumn<QString>("subsecondsOriginal");
  QTest::addColumn<QString>("subsecondsDigitized");

  QTest::addColumn<double>("digitalZoomRatio");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<double>("fNumber");
  QTest::addColumn<double>("focalLength");
  QTest::addColumn<unsigned int>("focalLength35mm");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  QTest::addColumn<double>("maxAperture");

  QTest::addColumn<QString>("software");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // Aperture 2.1
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / Aperture 2.1 / JPEG")
      << "../../../testimages/test_1_aperture_2_1.jpg"
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(640)
      << static_cast<unsigned int>(424)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(0.0)
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00 ";

  QTest::newRow("Test 2 / Aperture 2.1 / JPEG")
      << "../../../testimages/test_2_aperture_2_1.jpg"
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(424)
      << static_cast<unsigned int>(640)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(-1.0)
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00 ";
}

//----------------------------------------------------------------------------------------------------
// Apple Aperture 1.6 - JPEG - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_aperture_jpeg()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(QString,make);
  QFETCH(QString,model);

  QFETCH(unsigned int,pixelXDimension);
  QFETCH(unsigned int,pixelYDimension);

  QFETCH(double,xResolution);
  QFETCH(double,yResolution);

  QFETCH(double,exposureBias);

  QFETCH(QString,dateAndTime);
  QFETCH(QString,dateAndTimeOriginal);
  QFETCH(QString,dateAndTimeDigitized);

  QFETCH(QString,subseconds);
  QFETCH(QString,subsecondsOriginal);
  QFETCH(QString,subsecondsDigitized);

  QFETCH(double,digitalZoomRatio);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(double,fNumber);
  QFETCH(double,focalLength);
  QFETCH(unsigned int,focalLength35mm);

  QFETCH(unsigned int,ISOSpeedRating);

  QFETCH(double,maxAperture);

  QFETCH(QString,software);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVERIFY(metaData->make().compare(make)   == 0);
  QVERIFY(metaData->model().compare(model) == 0);

  QCOMPARE(metaData->pixelXDimension(),pixelXDimension);
  QCOMPARE(metaData->pixelYDimension(),pixelYDimension);

  QCOMPARE(metaData->xResolution(),xResolution);
  QCOMPARE(metaData->yResolution(),yResolution);

  QCOMPARE(metaData->exposureBias(),exposureBias);

  QVERIFY(metaData->dateAndTime().compare(dateAndTime) == 0);
  QVERIFY(metaData->dateAndTimeOriginal().compare(dateAndTimeOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitized().compare(dateAndTimeDigitized) == 0);

  QVERIFY(metaData->dateAndTimeSubseconds().compare(subseconds) == 0);
  QVERIFY(metaData->dateAndTimeOriginalSubseconds().compare(subsecondsOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitizedSubseconds().compare(subsecondsDigitized) == 0);

  QCOMPARE(metaData->digitalZoomRatio(),digitalZoomRatio);

  QCOMPARE(metaData->exposureTime().nominator(),exposureTimeNom);
  QCOMPARE(metaData->exposureTime().denominator(),exposureTimeDen);

  QCOMPARE(metaData->exposureTime().value(),static_cast<qreal>(exposureTimeNom)/static_cast<qreal>(exposureTimeDen));

  QCOMPARE(metaData->fNumber().value(),fNumber);
  QCOMPARE(metaData->focalLength().value(),focalLength);
  QCOMPARE(metaData->focalLength35mm(),focalLength35mm);

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(metaData->maxAperture(),maxAperture);

  QVERIFY(metaData->software().compare(software) == 0);

  QVERIFY(metaData->resolutionUnit()            == QImageMetaData::ResolutionInch);
  QVERIFY(metaData->compression()               == QImageMetaData::CompressionUndefined);
  QVERIFY(metaData->exposureMode()              == QImageMetaData::ExposureModeAuto);
  QVERIFY(metaData->exposureProgram()           == QImageMetaData::ExposureProgramAperturePriority);
  QVERIFY(metaData->contrast()                  == QImageMetaData::ContrastNormal);
  QVERIFY(metaData->customRendered()            == QImageMetaData::CustomRenderedNormal);
  QVERIFY(metaData->flashStatus()               == QImageMetaData::FlashStatusNotFired);
  QVERIFY(metaData->gainControl()               == QImageMetaData::GainControlNone);
  QVERIFY(metaData->lightSource()               == QImageMetaData::LightSourceUnknown);
  QVERIFY(metaData->meteringMode()              == QImageMetaData::MeteringModeCenterWeightedAverage);
  QVERIFY(metaData->saturation()                == QImageMetaData::SaturationNormal);
  QVERIFY(metaData->sceneCaptureType()          == QImageMetaData::SceneCaptureTypeStandard);
  QVERIFY(metaData->sensingMethod()             == QImageMetaData::SensingMethodOneChipColorAreaSensor);
  QVERIFY(metaData->sharpness()                 == QImageMetaData::SharpnessNormal);
  QVERIFY(metaData->subjectDistanceRange()      == QImageMetaData::SubjectDistanceRangeUnknown);
  QVERIFY(metaData->whiteBalance()              == QImageMetaData::WhiteBalanceAuto);
  QVERIFY(metaData->photometricInterpretation() == QImageMetaData::PhotometricUnknown);
  QVERIFY(metaData->orientation()               == QImageMetaData::OrientationTopLeft);

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Apple Aperture 3.0 - PNG - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_aperture_png_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");
  QTest::addColumn<QString>("software");
  QTest::addColumn<QString>("lens");

  QTest::addColumn<unsigned int>("fNumberNom");
  QTest::addColumn<unsigned int>("fNumberDen");

  QTest::addColumn<unsigned int>("focalLengthNom");
  QTest::addColumn<unsigned int>("focalLengthDen");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // Aperture 2.1
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / Aperture 3.0 / PNG")
      << "../../../testimages/test_1_aperture_3_0.png"
      << static_cast<unsigned int>(2828)
      << static_cast<unsigned int>(4272)
      << static_cast<unsigned int>(8)
      << "Nikon Corporation"
      << "Nikon D3"
      << "ExifTool"
      << "Zeiss 50mm f1.4"
      << static_cast<unsigned int>(56)
      << static_cast<unsigned int>(10)
      << static_cast<unsigned int>(50)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(3200)
      << static_cast<unsigned int>(200);

  QTest::newRow("Test 2 / Aperture 3.0 / PNG")
      << "../../../testimages/test_2_aperture_3_0.png"
      << static_cast<unsigned int>(2766)
      << static_cast<unsigned int>(1832)
      << static_cast<unsigned int>(8)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << "ExifTool 8.49"
      << "Nikkor 35mm f2.0"
      << static_cast<unsigned int>(8)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(35)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1600)
      << static_cast<unsigned int>(400);
}

//----------------------------------------------------------------------------------------------------
// Apple Aperture 3.0 - PNG - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_aperture_png()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(unsigned int,depth);

  QFETCH(QString,make);
  QFETCH(QString,model);
  QFETCH(QString,software);
  QFETCH(QString,lens);

  QFETCH(unsigned int,fNumberNom);
  QFETCH(unsigned int,fNumberDen);

  QFETCH(unsigned int,focalLengthNom);
  QFETCH(unsigned int,focalLengthDen);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(unsigned int,ISOSpeedRating);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVector<quint32> bitDepth = metaData->depth();

  QCOMPARE(bitDepth.size(),4);

  QCOMPARE(bitDepth[0],depth);
  QCOMPARE(bitDepth[1],depth);
  QCOMPARE(bitDepth[2],depth);
  QCOMPARE(bitDepth[3],depth);

  QCOMPARE(metaData->make(),make);
  QCOMPARE(metaData->model(),model);
  QCOMPARE(metaData->software(),software);
  QCOMPARE(metaData->lens(),lens);

  QImageMetaDataRational<quint32> fNumber = metaData->fNumber();

  QCOMPARE(fNumber.nominator(),fNumberNom);
  QCOMPARE(fNumber.denominator(),fNumberDen);

  QCOMPARE(fNumber.value(),static_cast<double>(fNumberNom)/static_cast<double>(fNumberDen));

  QImageMetaDataRational<quint32> focalLength = metaData->focalLength();

  QCOMPARE(focalLength.nominator(),focalLengthNom);
  QCOMPARE(focalLength.denominator(),focalLengthDen);

  QCOMPARE(focalLength.value(),static_cast<double>(focalLengthNom)/static_cast<double>(focalLengthDen));

  QImageMetaDataRational<quint32> exposureTime = metaData->exposureTime();

  QCOMPARE(exposureTime.nominator(),exposureTimeNom);
  QCOMPARE(exposureTime.denominator(),exposureTimeDen);

  QCOMPARE(exposureTime.value(),static_cast<double>(exposureTimeNom)/static_cast<double>(exposureTimeDen));

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(fNumber.value(),static_cast<double>(fNumberNom)/static_cast<double>(fNumberDen));

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Adobe Lightroom 3.2 - TIFF - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_lightroom_tiff_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("pixelXDimension");
  QTest::addColumn<unsigned int>("pixelYDimension");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<double>("xResolution");
  QTest::addColumn<double>("yResolution");

  QTest::addColumn<double>("exposureBias");

  QTest::addColumn<QString>("dateAndTime");
  QTest::addColumn<QString>("dateAndTimeOriginal");
  QTest::addColumn<QString>("dateAndTimeDigitized");

  QTest::addColumn<QString>("subseconds");
  QTest::addColumn<QString>("subsecondsOriginal");
  QTest::addColumn<QString>("subsecondsDigitized");

  QTest::addColumn<double>("digitalZoomRatio");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<double>("fNumber");
  QTest::addColumn<double>("focalLength");
  QTest::addColumn<unsigned int>("focalLength35mm");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  QTest::addColumn<double>("maxAperture");

  QTest::addColumn<QString>("software");

  QTest::addColumn<QString>("exifVersion");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // Lightroom 3.2
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / Lightroom 3.2 / 8bit TIFF")
      << "../../../testimages/test_1_lightroom_3_2_8bit.tif"
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(240)
      << static_cast<double>(240)
      << static_cast<double>(0.0)
      << "2010:11:26 22:53:38"
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "Not defined"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00"
      << "0221";

  QTest::newRow("Test 1 / Lightroom 3.2 / 16bit TIFF")
      << "../../../testimages/test_1_lightroom_3_2_16bit.tif"
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(16)
      << static_cast<double>(240)
      << static_cast<double>(240)
      << static_cast<double>(0.0)
      << "2010:11:26 22:54:01"
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "Not defined"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00"
      << "0221";

  QTest::newRow("Test 2 / Lightroom 3.2 / 8bit TIFF")
      << "../../../testimages/test_2_lightroom_3_2_8bit.tif"
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(240)
      << static_cast<double>(240)
      << static_cast<double>(-1.0)
      << "2010:11:26 22:54:33"
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "Not defined"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00"
      << "0221";

  QTest::newRow("Test 2 / Lightroom 3.2 / 16bit TIFF")
      << "../../../testimages/test_2_lightroom_3_2_16bit.tif"
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(16)
      << static_cast<double>(240)
      << static_cast<double>(240)
      << static_cast<double>(-1.0)
      << "2010:11:26 22:55:05"
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "Not defined"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00"
      << "0221";
}

//----------------------------------------------------------------------------------------------------
// Adobe Lightroom 3.2 - TIFF - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_lightroom_tiff()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(QString,make);
  QFETCH(QString,model);

  QFETCH(unsigned int,pixelXDimension);
  QFETCH(unsigned int,pixelYDimension);

  QFETCH(unsigned int,depth);

  QFETCH(double,xResolution);
  QFETCH(double,yResolution);

  QFETCH(double,exposureBias);

  QFETCH(QString,dateAndTime);
  QFETCH(QString,dateAndTimeOriginal);
  QFETCH(QString,dateAndTimeDigitized);

  QFETCH(QString,subseconds);
  QFETCH(QString,subsecondsOriginal);
  QFETCH(QString,subsecondsDigitized);

  QFETCH(double,digitalZoomRatio);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(double,fNumber);
  QFETCH(double,focalLength);
  QFETCH(unsigned int,focalLength35mm);

  QFETCH(unsigned int,ISOSpeedRating);

  QFETCH(double,maxAperture);

  QFETCH(QString,software);

  QFETCH(QString,exifVersion);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVERIFY(metaData->make().compare(make)   == 0);
  QVERIFY(metaData->model().compare(model) == 0);

  QCOMPARE(metaData->pixelXDimension(),pixelXDimension);
  QCOMPARE(metaData->pixelYDimension(),pixelYDimension);

  QVector<quint32> bitDepth = metaData->depth();

  QCOMPARE(bitDepth.size(),3);

  QCOMPARE(bitDepth[0],depth);
  QCOMPARE(bitDepth[1],depth);
  QCOMPARE(bitDepth[2],depth);

  QCOMPARE(metaData->xResolution(),xResolution);
  QCOMPARE(metaData->yResolution(),yResolution);

  QCOMPARE(metaData->exposureBias(),exposureBias);

  QVERIFY(metaData->dateAndTime().compare(dateAndTime) == 0);
  QVERIFY(metaData->dateAndTimeOriginal().compare(dateAndTimeOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitized().compare(dateAndTimeDigitized) == 0);

  QVERIFY(metaData->dateAndTimeSubseconds().compare(subseconds) == 0);
  QVERIFY(metaData->dateAndTimeOriginalSubseconds().compare(subsecondsOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitizedSubseconds().compare(subsecondsDigitized) == 0);

  QCOMPARE(metaData->digitalZoomRatio(),digitalZoomRatio);

  QCOMPARE(metaData->exposureTime().nominator(),exposureTimeNom);
  QCOMPARE(metaData->exposureTime().denominator(),exposureTimeDen);

  QCOMPARE(metaData->exposureTime().value(),static_cast<qreal>(exposureTimeNom)/static_cast<qreal>(exposureTimeDen));

  QCOMPARE(metaData->fNumber().value(),fNumber);
  QCOMPARE(metaData->focalLength().value(),focalLength);
  QCOMPARE(metaData->focalLength35mm(),focalLength35mm);

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(metaData->maxAperture(),maxAperture);

  QVERIFY(metaData->software().compare(software) == 0);

  QVERIFY(metaData->exifVersion().compare(exifVersion) == 0);

  QVERIFY(metaData->resolutionUnit()            == QImageMetaData::ResolutionInch);
  QVERIFY(metaData->compression()               == QImageMetaData::CompressionUncompressed);
  QVERIFY(metaData->exposureMode()              == QImageMetaData::ExposureModeAuto);
  QVERIFY(metaData->exposureProgram()           == QImageMetaData::ExposureProgramAperturePriority);
  QVERIFY(metaData->contrast()                  == QImageMetaData::ContrastNormal);
  QVERIFY(metaData->customRendered()            == QImageMetaData::CustomRenderedNormal);
  QVERIFY(metaData->flashStatus()               == QImageMetaData::FlashStatusNotFired);
  QVERIFY(metaData->gainControl()               == QImageMetaData::GainControlNone);
  QVERIFY(metaData->lightSource()               == QImageMetaData::LightSourceUnknown);
  QVERIFY(metaData->meteringMode()              == QImageMetaData::MeteringModeCenterWeightedAverage);
  QVERIFY(metaData->saturation()                == QImageMetaData::SaturationNormal);
  QVERIFY(metaData->sceneCaptureType()          == QImageMetaData::SceneCaptureTypeStandard);
  QVERIFY(metaData->sensingMethod()             == QImageMetaData::SensingMethodOneChipColorAreaSensor);
  QVERIFY(metaData->sharpness()                 == QImageMetaData::SharpnessNormal);
  QVERIFY(metaData->subjectDistanceRange()      == QImageMetaData::SubjectDistanceRangeUnknown);
  QVERIFY(metaData->whiteBalance()              == QImageMetaData::WhiteBalanceAuto);
  QVERIFY(metaData->photometricInterpretation() == QImageMetaData::PhotometricRGB);
  QVERIFY(metaData->orientation()               == QImageMetaData::OrientationUndefined);

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Adobe Lightroom 3.2 - JPEG - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_lightroom_jpeg_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("pixelXDimension");
  QTest::addColumn<unsigned int>("pixelYDimension");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<double>("xResolution");
  QTest::addColumn<double>("yResolution");

  QTest::addColumn<double>("exposureBias");

  QTest::addColumn<QString>("dateAndTime");
  QTest::addColumn<QString>("dateAndTimeOriginal");
  QTest::addColumn<QString>("dateAndTimeDigitized");

  QTest::addColumn<QString>("subseconds");
  QTest::addColumn<QString>("subsecondsOriginal");
  QTest::addColumn<QString>("subsecondsDigitized");

  QTest::addColumn<double>("digitalZoomRatio");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<double>("fNumber");
  QTest::addColumn<double>("focalLength");
  QTest::addColumn<unsigned int>("focalLength35mm");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  QTest::addColumn<double>("maxAperture");

  QTest::addColumn<QString>("software");

  QTest::addColumn<QString>("exifVersion");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // Lightroom 3.2
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / Lightroom 3.2 / JPEG")
      << "../../../testimages/test_1_lightroom_3_2.jpg"
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(240)
      << static_cast<double>(240)
      << static_cast<double>(0.0)
      << "2010:11:26 22:52:58"
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "Not defined"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00"
      << "0221";

  QTest::newRow("Test 2 / Lightroom 3.2 / JPEG")
      << "../../../testimages/test_2_lightroom_3_2.jpg"
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(240)
      << static_cast<double>(240)
      << static_cast<double>(-1.0)
      << "2010:11:26 22:54:25"
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "Not defined"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Ver.2.00"
      << "0221";
}

//----------------------------------------------------------------------------------------------------
// Adobe Lightroom 3.2 - JPEG - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_lightroom_jpeg()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(QString,make);
  QFETCH(QString,model);

  QFETCH(unsigned int,pixelXDimension);
  QFETCH(unsigned int,pixelYDimension);

  QFETCH(double,xResolution);
  QFETCH(double,yResolution);

  QFETCH(double,exposureBias);

  QFETCH(QString,dateAndTime);
  QFETCH(QString,dateAndTimeOriginal);
  QFETCH(QString,dateAndTimeDigitized);

  QFETCH(QString,subseconds);
  QFETCH(QString,subsecondsOriginal);
  QFETCH(QString,subsecondsDigitized);

  QFETCH(double,digitalZoomRatio);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(double,fNumber);
  QFETCH(double,focalLength);
  QFETCH(unsigned int,focalLength35mm);

  QFETCH(unsigned int,ISOSpeedRating);

  QFETCH(double,maxAperture);

  QFETCH(QString,software);

  QFETCH(QString,exifVersion);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVERIFY(metaData->make().compare(make)   == 0);
  QVERIFY(metaData->model().compare(model) == 0);

  QCOMPARE(metaData->pixelXDimension(),pixelXDimension);
  QCOMPARE(metaData->pixelYDimension(),pixelYDimension);

  QCOMPARE(metaData->xResolution(),xResolution);
  QCOMPARE(metaData->yResolution(),yResolution);

  QCOMPARE(metaData->exposureBias(),exposureBias);

  QVERIFY(metaData->dateAndTime().compare(dateAndTime) == 0);
  QVERIFY(metaData->dateAndTimeOriginal().compare(dateAndTimeOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitized().compare(dateAndTimeDigitized) == 0);

  QVERIFY(metaData->dateAndTimeSubseconds().compare(subseconds) == 0);
  QVERIFY(metaData->dateAndTimeOriginalSubseconds().compare(subsecondsOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitizedSubseconds().compare(subsecondsDigitized) == 0);

  QCOMPARE(metaData->digitalZoomRatio(),digitalZoomRatio);

  QCOMPARE(metaData->exposureTime().nominator(),exposureTimeNom);
  QCOMPARE(metaData->exposureTime().denominator(),exposureTimeDen);

  QCOMPARE(metaData->exposureTime().value(),static_cast<qreal>(exposureTimeNom)/static_cast<qreal>(exposureTimeDen));

  QCOMPARE(metaData->fNumber().value(),fNumber);
  QCOMPARE(metaData->focalLength().value(),focalLength);
  QCOMPARE(metaData->focalLength35mm(),focalLength35mm);

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(metaData->maxAperture(),maxAperture);

  QVERIFY(metaData->software().compare(software) == 0);

  QVERIFY(metaData->exifVersion().compare(exifVersion) == 0);

  QVERIFY(metaData->resolutionUnit()            == QImageMetaData::ResolutionInch);
  QVERIFY(metaData->compression()               == QImageMetaData::CompressionUndefined);
  QVERIFY(metaData->exposureMode()              == QImageMetaData::ExposureModeAuto);
  QVERIFY(metaData->exposureProgram()           == QImageMetaData::ExposureProgramAperturePriority);
  QVERIFY(metaData->contrast()                  == QImageMetaData::ContrastNormal);
  QVERIFY(metaData->customRendered()            == QImageMetaData::CustomRenderedNormal);
  QVERIFY(metaData->flashStatus()               == QImageMetaData::FlashStatusNotFired);
  QVERIFY(metaData->gainControl()               == QImageMetaData::GainControlNone);
  QVERIFY(metaData->lightSource()               == QImageMetaData::LightSourceUnknown);
  QVERIFY(metaData->meteringMode()              == QImageMetaData::MeteringModeCenterWeightedAverage);
  QVERIFY(metaData->saturation()                == QImageMetaData::SaturationNormal);
  QVERIFY(metaData->sceneCaptureType()          == QImageMetaData::SceneCaptureTypeStandard);
  QVERIFY(metaData->sensingMethod()             == QImageMetaData::SensingMethodOneChipColorAreaSensor);
  QVERIFY(metaData->sharpness()                 == QImageMetaData::SharpnessNormal);
  QVERIFY(metaData->subjectDistanceRange()      == QImageMetaData::SubjectDistanceRangeUnknown);
  QVERIFY(metaData->whiteBalance()              == QImageMetaData::WhiteBalanceAuto);
  QVERIFY(metaData->photometricInterpretation() == QImageMetaData::PhotometricUndefined);
  QVERIFY(metaData->orientation()               == QImageMetaData::OrientationUndefined);

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Pixelmator 1.6 - TIFF - Data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_pixelmator_tiff_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("pixelXDimension");
  QTest::addColumn<unsigned int>("pixelYDimension");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<double>("xResolution");
  QTest::addColumn<double>("yResolution");

  QTest::addColumn<double>("exposureBias");

  QTest::addColumn<QString>("dateAndTime");
  QTest::addColumn<QString>("dateAndTimeOriginal");
  QTest::addColumn<QString>("dateAndTimeDigitized");

  QTest::addColumn<QString>("subseconds");
  QTest::addColumn<QString>("subsecondsOriginal");
  QTest::addColumn<QString>("subsecondsDigitized");

  QTest::addColumn<double>("digitalZoomRatio");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<double>("fNumber");
  QTest::addColumn<double>("focalLength");
  QTest::addColumn<unsigned int>("focalLength35mm");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  QTest::addColumn<double>("maxAperture");

  QTest::addColumn<QString>("software");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // Pixelmator 1.6
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / Pixelmator 1.6 / Uncompressed TIFF")
      << "../../../testimages/test_1_pixelmator_1_6_uncompressed.tiff"
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(0.0)
      << "2010-11-26 22:42:21 "
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Pixelmator 1.6.2";

  QTest::newRow("Test 2 / Pixelmator 1.6 / Uncompressed TIFF")
      << "../../../testimages/test_2_pixelmator_1_6_uncompressed.tiff"
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(-1.0)
      << "2010-11-26 22:43:10 "
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Pixelmator 1.6.2";
}

//----------------------------------------------------------------------------------------------------
// Pixelmator 1.6 - TIFF - Data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_pixelmator_tiff()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(QString,make);
  QFETCH(QString,model);

  QFETCH(unsigned int,pixelXDimension);
  QFETCH(unsigned int,pixelYDimension);

  QFETCH(unsigned int,depth);

  QFETCH(double,xResolution);
  QFETCH(double,yResolution);

  QFETCH(double,exposureBias);

  QFETCH(QString,dateAndTime);
  QFETCH(QString,dateAndTimeOriginal);
  QFETCH(QString,dateAndTimeDigitized);

  QFETCH(QString,subseconds);
  QFETCH(QString,subsecondsOriginal);
  QFETCH(QString,subsecondsDigitized);

  QFETCH(double,digitalZoomRatio);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(double,fNumber);
  QFETCH(double,focalLength);
  QFETCH(unsigned int,focalLength35mm);

  QFETCH(unsigned int,ISOSpeedRating);

  QFETCH(double,maxAperture);

  QFETCH(QString,software);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVERIFY(metaData->make().compare(make)   == 0);
  QVERIFY(metaData->model().compare(model) == 0);

  QCOMPARE(metaData->pixelXDimension(),pixelXDimension);
  QCOMPARE(metaData->pixelYDimension(),pixelYDimension);

  QVector<quint32> bitDepth = metaData->depth();

  QCOMPARE(bitDepth.size(),4);

  QCOMPARE(bitDepth[0],depth);
  QCOMPARE(bitDepth[1],depth);
  QCOMPARE(bitDepth[2],depth);
  QCOMPARE(bitDepth[3],depth);

  QCOMPARE(metaData->xResolution(),xResolution);
  QCOMPARE(metaData->yResolution(),yResolution);

  QCOMPARE(metaData->exposureBias(),exposureBias);

  QVERIFY(metaData->dateAndTime().compare(dateAndTime) == 0);
  QVERIFY(metaData->dateAndTimeOriginal().compare(dateAndTimeOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitized().compare(dateAndTimeDigitized) == 0);

  QVERIFY(metaData->dateAndTimeSubseconds().compare(subseconds) == 0);
  QVERIFY(metaData->dateAndTimeOriginalSubseconds().compare(subsecondsOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitizedSubseconds().compare(subsecondsDigitized) == 0);

  QCOMPARE(metaData->digitalZoomRatio(),digitalZoomRatio);

  QCOMPARE(metaData->exposureTime().nominator(),exposureTimeNom);
  QCOMPARE(metaData->exposureTime().denominator(),exposureTimeDen);

  QCOMPARE(metaData->exposureTime().value(),static_cast<qreal>(exposureTimeNom)/static_cast<qreal>(exposureTimeDen));

  QCOMPARE(metaData->fNumber().value(),fNumber);
  QCOMPARE(metaData->focalLength().value(),focalLength);
  QCOMPARE(metaData->focalLength35mm(),focalLength35mm);

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(metaData->maxAperture(),maxAperture);

  QVERIFY(metaData->software().compare(software) == 0);

  QVERIFY(metaData->resolutionUnit()            == QImageMetaData::ResolutionInch);
  QVERIFY(metaData->compression()               == QImageMetaData::CompressionUncompressed);
  QVERIFY(metaData->exposureMode()              == QImageMetaData::ExposureModeAuto);
  QVERIFY(metaData->exposureProgram()           == QImageMetaData::ExposureProgramAperturePriority);
  QVERIFY(metaData->contrast()                  == QImageMetaData::ContrastNormal);
  QVERIFY(metaData->customRendered()            == QImageMetaData::CustomRenderedNormal);
  QVERIFY(metaData->flashStatus()               == QImageMetaData::FlashStatusNotFired);
  QVERIFY(metaData->gainControl()               == QImageMetaData::GainControlNone);
  QVERIFY(metaData->lightSource()               == QImageMetaData::LightSourceUnknown);
  QVERIFY(metaData->meteringMode()              == QImageMetaData::MeteringModeCenterWeightedAverage);
  QVERIFY(metaData->saturation()                == QImageMetaData::SaturationNormal);
  QVERIFY(metaData->sceneCaptureType()          == QImageMetaData::SceneCaptureTypeStandard);
  QVERIFY(metaData->sensingMethod()             == QImageMetaData::SensingMethodOneChipColorAreaSensor);
  QVERIFY(metaData->sharpness()                 == QImageMetaData::SharpnessNormal);
  QVERIFY(metaData->subjectDistanceRange()      == QImageMetaData::SubjectDistanceRangeUnknown);
  QVERIFY(metaData->whiteBalance()              == QImageMetaData::WhiteBalanceAuto);
  QVERIFY(metaData->photometricInterpretation() == QImageMetaData::PhotometricRGB);
  QVERIFY(metaData->orientation()               == QImageMetaData::OrientationTopLeft);

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Pixelmator 1.6 - JPEG - Data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_pixelmator_jpeg_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("pixelXDimension");
  QTest::addColumn<unsigned int>("pixelYDimension");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<double>("xResolution");
  QTest::addColumn<double>("yResolution");

  QTest::addColumn<double>("exposureBias");

  QTest::addColumn<QString>("dateAndTime");
  QTest::addColumn<QString>("dateAndTimeOriginal");
  QTest::addColumn<QString>("dateAndTimeDigitized");

  QTest::addColumn<QString>("subseconds");
  QTest::addColumn<QString>("subsecondsOriginal");
  QTest::addColumn<QString>("subsecondsDigitized");

  QTest::addColumn<double>("digitalZoomRatio");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<double>("fNumber");
  QTest::addColumn<double>("focalLength");
  QTest::addColumn<unsigned int>("focalLength35mm");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  QTest::addColumn<double>("maxAperture");

  QTest::addColumn<QString>("software");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // Pixelmator 1.6
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / Pixelmator 1.6 / JPEG")
      << "../../../testimages/test_1_pixelmator_1_6.jpg"
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(0.0)
      << "2010-11-26 22:41:57 +0200"
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Pixelmator 1.6.2";

  QTest::newRow("Test 2 / Pixelmator 1.6 / JPEG")
      << "../../../testimages/test_2_pixelmator_1_6.jpg"
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(-1.0)
      << "2010-11-26 22:43:06 +0200"
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "25"
      << "25"
      << "25"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "Pixelmator 1.6.2";
}

//----------------------------------------------------------------------------------------------------
// Pixelmator 1.6 - JPEG - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_pixelmator_jpeg()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(QString,make);
  QFETCH(QString,model);

  QFETCH(unsigned int,pixelXDimension);
  QFETCH(unsigned int,pixelYDimension);

  QFETCH(double,xResolution);
  QFETCH(double,yResolution);

  QFETCH(double,exposureBias);

  QFETCH(QString,dateAndTime);
  QFETCH(QString,dateAndTimeOriginal);
  QFETCH(QString,dateAndTimeDigitized);

  QFETCH(QString,subseconds);
  QFETCH(QString,subsecondsOriginal);
  QFETCH(QString,subsecondsDigitized);

  QFETCH(double,digitalZoomRatio);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(double,fNumber);
  QFETCH(double,focalLength);
  QFETCH(unsigned int,focalLength35mm);

  QFETCH(unsigned int,ISOSpeedRating);

  QFETCH(double,maxAperture);

  QFETCH(QString,software);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVERIFY(metaData->make().compare(make)   == 0);
  QVERIFY(metaData->model().compare(model) == 0);

  QCOMPARE(metaData->pixelXDimension(),pixelXDimension);
  QCOMPARE(metaData->pixelYDimension(),pixelYDimension);

  QCOMPARE(metaData->xResolution(),xResolution);
  QCOMPARE(metaData->yResolution(),yResolution);

  QCOMPARE(metaData->exposureBias(),exposureBias);

  QVERIFY(metaData->dateAndTime().compare(dateAndTime) == 0);
  QVERIFY(metaData->dateAndTimeOriginal().compare(dateAndTimeOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitized().compare(dateAndTimeDigitized) == 0);

  QVERIFY(metaData->dateAndTimeSubseconds().compare(subseconds) == 0);
  QVERIFY(metaData->dateAndTimeOriginalSubseconds().compare(subsecondsOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitizedSubseconds().compare(subsecondsDigitized) == 0);

  QCOMPARE(metaData->digitalZoomRatio(),digitalZoomRatio);

  QCOMPARE(metaData->exposureTime().nominator(),exposureTimeNom);
  QCOMPARE(metaData->exposureTime().denominator(),exposureTimeDen);

  QCOMPARE(metaData->exposureTime().value(),static_cast<qreal>(exposureTimeNom)/static_cast<qreal>(exposureTimeDen));

  QCOMPARE(metaData->fNumber().value(),fNumber);
  QCOMPARE(metaData->focalLength().value(),focalLength);
  QCOMPARE(metaData->focalLength35mm(),focalLength35mm);

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(metaData->maxAperture(),maxAperture);

  QVERIFY(metaData->software().compare(software) == 0);

  QVERIFY(metaData->resolutionUnit()            == QImageMetaData::ResolutionInch);
  QVERIFY(metaData->compression()               == QImageMetaData::CompressionUndefined);
  QVERIFY(metaData->exposureMode()              == QImageMetaData::ExposureModeAuto);
  QVERIFY(metaData->exposureProgram()           == QImageMetaData::ExposureProgramAperturePriority);
  QVERIFY(metaData->contrast()                  == QImageMetaData::ContrastNormal);
  QVERIFY(metaData->customRendered()            == QImageMetaData::CustomRenderedNormal);
  QVERIFY(metaData->flashStatus()               == QImageMetaData::FlashStatusNotFired);
  QVERIFY(metaData->gainControl()               == QImageMetaData::GainControlNone);
  QVERIFY(metaData->lightSource()               == QImageMetaData::LightSourceUnknown);
  QVERIFY(metaData->meteringMode()              == QImageMetaData::MeteringModeCenterWeightedAverage);
  QVERIFY(metaData->saturation()                == QImageMetaData::SaturationNormal);
  QVERIFY(metaData->sceneCaptureType()          == QImageMetaData::SceneCaptureTypeStandard);
  QVERIFY(metaData->sensingMethod()             == QImageMetaData::SensingMethodOneChipColorAreaSensor);
  QVERIFY(metaData->sharpness()                 == QImageMetaData::SharpnessNormal);
  QVERIFY(metaData->subjectDistanceRange()      == QImageMetaData::SubjectDistanceRangeUnknown);
  QVERIFY(metaData->whiteBalance()              == QImageMetaData::WhiteBalanceAuto);
  QVERIFY(metaData->photometricInterpretation() == QImageMetaData::PhotometricUnknown);
  QVERIFY(metaData->orientation()               == QImageMetaData::OrientationTopLeft);

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Pixelmator 1.6 - PNG - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_pixelmator_png_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("samplesPerPixel");

  QTest::addColumn<unsigned int>("depth");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // Pixelmator 1.6
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / Pixelmator 1.6 / PNG")
      << "../../../testimages/test_1_pixelmator_1_6.png"
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4)
      << static_cast<unsigned int>(8);

  QTest::newRow("Test 2 / Pixelmator 1.6 / PNG")
      << "../../../testimages/test_2_pixelmator_1_6.png"
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(3)
      << static_cast<unsigned int>(8);
}

//----------------------------------------------------------------------------------------------------
// Pixelmator 1.6 - PNG - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_pixelmator_png()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(unsigned int,samplesPerPixel);

  QFETCH(unsigned int,depth);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVector<quint32> bitDepth = metaData->depth();

  QCOMPARE(bitDepth.size(),static_cast<int>(samplesPerPixel));

  for (int i = 0;i < bitDepth.size();++i) {
    QCOMPARE(bitDepth[i],depth);
  }

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Apple iPhoto 11 - TIFF - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_iphoto_tiff_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("pixelXDimension");
  QTest::addColumn<unsigned int>("pixelYDimension");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<double>("xResolution");
  QTest::addColumn<double>("yResolution");

  QTest::addColumn<double>("exposureBias");

  QTest::addColumn<QString>("dateAndTime");
  QTest::addColumn<QString>("dateAndTimeOriginal");
  QTest::addColumn<QString>("dateAndTimeDigitized");

  QTest::addColumn<QString>("subseconds");
  QTest::addColumn<QString>("subsecondsOriginal");
  QTest::addColumn<QString>("subsecondsDigitized");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<double>("fNumber");
  QTest::addColumn<double>("focalLength");
  QTest::addColumn<unsigned int>("focalLength35mm");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  QTest::addColumn<double>("maxAperture");

  QTest::addColumn<QString>("software");

  QTest::addColumn<QString>("exifVersion");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // iPhoto 11
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / iPhoto 11 / TIFF")
      << "../../../testimages/test_1_iphoto_11.tif"
      << static_cast<unsigned int>(1280)
      << static_cast<unsigned int>(852)
      << static_cast<unsigned int>(1280)
      << static_cast<unsigned int>(852)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(0.0)
      << "2010:12:18 21:41:30"
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "Not defined"
      << "Not defined"
      << "Not defined"
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "QuickTime 7.6.6"
      << "0220";

  QTest::newRow("Test 2 / iPhoto 11 / TIFF")
      << "../../../testimages/test_2_iphoto_11.tif"
      << static_cast<unsigned int>(852)
      << static_cast<unsigned int>(1280)
      << static_cast<unsigned int>(852)
      << static_cast<unsigned int>(1280)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(-1.0)
      << "2010:12:18 21:41:31"
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "Not defined"
      << "Not defined"
      << "Not defined"
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "QuickTime 7.6.6"
      << "0220";
}

//----------------------------------------------------------------------------------------------------
// Apple iPhoto 11 - TIFF - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_iphoto_tiff()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(QString,make);
  QFETCH(QString,model);

  QFETCH(unsigned int,pixelXDimension);
  QFETCH(unsigned int,pixelYDimension);

  QFETCH(unsigned int,depth);

  QFETCH(double,xResolution);
  QFETCH(double,yResolution);

  QFETCH(double,exposureBias);

  QFETCH(QString,dateAndTime);
  QFETCH(QString,dateAndTimeOriginal);
  QFETCH(QString,dateAndTimeDigitized);

  QFETCH(QString,subseconds);
  QFETCH(QString,subsecondsOriginal);
  QFETCH(QString,subsecondsDigitized);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(double,fNumber);
  QFETCH(double,focalLength);
  QFETCH(unsigned int,focalLength35mm);

  QFETCH(unsigned int,ISOSpeedRating);

  QFETCH(double,maxAperture);

  QFETCH(QString,software);

  QFETCH(QString,exifVersion);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVERIFY(metaData->make().compare(make)   == 0);
  QVERIFY(metaData->model().compare(model) == 0);

  QCOMPARE(metaData->pixelXDimension(),pixelXDimension);
  QCOMPARE(metaData->pixelYDimension(),pixelYDimension);

  QVector<quint32> bitDepth = metaData->depth();

  QCOMPARE(bitDepth.size(),4);

  QCOMPARE(bitDepth[0],depth);
  QCOMPARE(bitDepth[1],depth);
  QCOMPARE(bitDepth[2],depth);
  QCOMPARE(bitDepth[3],depth);

  QCOMPARE(metaData->xResolution(),xResolution);
  QCOMPARE(metaData->yResolution(),yResolution);

  QCOMPARE(metaData->exposureBias(),exposureBias);

  QVERIFY(metaData->dateAndTime().compare(dateAndTime) == 0);
  QVERIFY(metaData->dateAndTimeOriginal().compare(dateAndTimeOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitized().compare(dateAndTimeDigitized) == 0);

  QVERIFY(metaData->dateAndTimeSubseconds().compare(subseconds) == 0);
  QVERIFY(metaData->dateAndTimeOriginalSubseconds().compare(subsecondsOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitizedSubseconds().compare(subsecondsDigitized) == 0);

  QCOMPARE(metaData->exposureTime().nominator(),exposureTimeNom);
  QCOMPARE(metaData->exposureTime().denominator(),exposureTimeDen);

  QCOMPARE(metaData->exposureTime().value(),static_cast<qreal>(exposureTimeNom)/static_cast<qreal>(exposureTimeDen));

  QCOMPARE(metaData->fNumber().value(),fNumber);
  QCOMPARE(metaData->focalLength().value(),focalLength);
  QCOMPARE(metaData->focalLength35mm(),focalLength35mm);

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(metaData->maxAperture(),maxAperture);

  QVERIFY(metaData->software().compare(software) == 0);

  QVERIFY(metaData->exifVersion().compare(exifVersion) == 0);

  QVERIFY(metaData->resolutionUnit()            == QImageMetaData::ResolutionInch);
  QVERIFY(metaData->compression()               == QImageMetaData::CompressionUncompressed);
  QVERIFY(metaData->exposureMode()              == QImageMetaData::ExposureModeUndefined);
  QVERIFY(metaData->exposureProgram()           == QImageMetaData::ExposureProgramAperturePriority);
  QVERIFY(metaData->contrast()                  == QImageMetaData::ContrastUndefined);
  QVERIFY(metaData->customRendered()            == QImageMetaData::CustomRenderedUndefined);
  QVERIFY(metaData->flashStatus()               == QImageMetaData::FlashStatusNotFired);
  QVERIFY(metaData->gainControl()               == QImageMetaData::GainControlUndefined);
  QVERIFY(metaData->lightSource()               == QImageMetaData::LightSourceUnknown);
  QVERIFY(metaData->meteringMode()              == QImageMetaData::MeteringModeCenterWeightedAverage);
  QVERIFY(metaData->saturation()                == QImageMetaData::SaturationUndefined);
  QVERIFY(metaData->sceneCaptureType()          == QImageMetaData::SceneCaptureTypeUndefined);
  QVERIFY(metaData->sensingMethod()             == QImageMetaData::SensingMethodOneChipColorAreaSensor);
  QVERIFY(metaData->sharpness()                 == QImageMetaData::SharpnessUndefined);
  QVERIFY(metaData->subjectDistanceRange()      == QImageMetaData::SubjectDistanceRangeUndefined);
  QVERIFY(metaData->whiteBalance()              == QImageMetaData::WhiteBalanceUndefined);
  QVERIFY(metaData->photometricInterpretation() == QImageMetaData::PhotometricRGB);
  QVERIFY(metaData->orientation()               == QImageMetaData::OrientationTopLeft);

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Apple iPhoto 11 - JPEG - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_iphoto_jpeg_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("pixelXDimension");
  QTest::addColumn<unsigned int>("pixelYDimension");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<double>("xResolution");
  QTest::addColumn<double>("yResolution");

  QTest::addColumn<double>("exposureBias");

  QTest::addColumn<QString>("dateAndTime");
  QTest::addColumn<QString>("dateAndTimeOriginal");
  QTest::addColumn<QString>("dateAndTimeDigitized");

  QTest::addColumn<QString>("subseconds");
  QTest::addColumn<QString>("subsecondsOriginal");
  QTest::addColumn<QString>("subsecondsDigitized");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<double>("fNumber");
  QTest::addColumn<double>("focalLength");
  QTest::addColumn<unsigned int>("focalLength35mm");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  QTest::addColumn<double>("maxAperture");

  QTest::addColumn<QString>("software");

  QTest::addColumn<QString>("exifVersion");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // iPhoto 11
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / iPhoto 11 / JPEG")
      << "../../../testimages/test_1_iphoto_11.jpg"
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(1280)
      << static_cast<unsigned int>(852)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(0.0)
      << "2010:12:18 21:41:30"
      << "2010:07:13 17:02:32"
      << "2010:07:13 17:02:32"
      << "Not defined"
      << "Not defined"
      << "Not defined"
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1250)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "QuickTime 7.6.6"
      << "0220";

  QTest::newRow("Test 2 / iPhoto 11 / JPEG")
      << "../../../testimages/test_2_iphoto_11.jpg"
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(852)
      << static_cast<unsigned int>(1280)
      << "NIKON CORPORATION"
      << "NIKON D3"
      << static_cast<unsigned int>(8)
      << static_cast<double>(72)
      << static_cast<double>(72)
      << static_cast<double>(-1.0)
      << "2010:12:18 21:41:31"
      << "2010:07:14 14:50:50"
      << "2010:07:14 14:50:50"
      << "Not defined"
      << "Not defined"
      << "Not defined"
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(2500)
      << static_cast<double>(0.0)
      << static_cast<double>(0.0)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(200)
      << static_cast<double>(0.0)
      << "QuickTime 7.6.6"
      << "0220";
}

//----------------------------------------------------------------------------------------------------
// Apple iPhoto 11 - JPEG - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_iphoto_jpeg()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(QString,make);
  QFETCH(QString,model);

  QFETCH(unsigned int,pixelXDimension);
  QFETCH(unsigned int,pixelYDimension);

  QFETCH(double,xResolution);
  QFETCH(double,yResolution);

  QFETCH(double,exposureBias);

  QFETCH(QString,dateAndTime);
  QFETCH(QString,dateAndTimeOriginal);
  QFETCH(QString,dateAndTimeDigitized);

  QFETCH(QString,subseconds);
  QFETCH(QString,subsecondsOriginal);
  QFETCH(QString,subsecondsDigitized);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(double,fNumber);
  QFETCH(double,focalLength);
  QFETCH(unsigned int,focalLength35mm);

  QFETCH(unsigned int,ISOSpeedRating);

  QFETCH(double,maxAperture);

  QFETCH(QString,software);

  QFETCH(QString,exifVersion);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVERIFY(metaData->make().compare(make)   == 0);
  QVERIFY(metaData->model().compare(model) == 0);

  QCOMPARE(metaData->pixelXDimension(),pixelXDimension);
  QCOMPARE(metaData->pixelYDimension(),pixelYDimension);

  QCOMPARE(metaData->xResolution(),xResolution);
  QCOMPARE(metaData->yResolution(),yResolution);

  QCOMPARE(metaData->exposureBias(),exposureBias);

  QVERIFY(metaData->dateAndTime().compare(dateAndTime) == 0);
  QVERIFY(metaData->dateAndTimeOriginal().compare(dateAndTimeOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitized().compare(dateAndTimeDigitized) == 0);

  QVERIFY(metaData->dateAndTimeSubseconds().compare(subseconds) == 0);
  QVERIFY(metaData->dateAndTimeOriginalSubseconds().compare(subsecondsOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitizedSubseconds().compare(subsecondsDigitized) == 0);

  QCOMPARE(metaData->exposureTime().nominator(),exposureTimeNom);
  QCOMPARE(metaData->exposureTime().denominator(),exposureTimeDen);

  QCOMPARE(metaData->exposureTime().value(),static_cast<qreal>(exposureTimeNom)/static_cast<qreal>(exposureTimeDen));

  QCOMPARE(metaData->fNumber().value(),fNumber);
  QCOMPARE(metaData->focalLength().value(),focalLength);
  QCOMPARE(metaData->focalLength35mm(),focalLength35mm);

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(metaData->maxAperture(),maxAperture);

  QVERIFY(metaData->software().compare(software) == 0);

  QVERIFY(metaData->exifVersion().compare(exifVersion) == 0);

  QVERIFY(metaData->resolutionUnit()            == QImageMetaData::ResolutionInch);
  QVERIFY(metaData->compression()               == QImageMetaData::CompressionUndefined);
  QVERIFY(metaData->exposureMode()              == QImageMetaData::ExposureModeUndefined);
  QVERIFY(metaData->exposureProgram()           == QImageMetaData::ExposureProgramAperturePriority);
  QVERIFY(metaData->contrast()                  == QImageMetaData::ContrastUndefined);
  QVERIFY(metaData->customRendered()            == QImageMetaData::CustomRenderedUndefined);
  QVERIFY(metaData->flashStatus()               == QImageMetaData::FlashStatusNotFired);
  QVERIFY(metaData->gainControl()               == QImageMetaData::GainControlUndefined);
  QVERIFY(metaData->lightSource()               == QImageMetaData::LightSourceUnknown);
  QVERIFY(metaData->meteringMode()              == QImageMetaData::MeteringModeCenterWeightedAverage);
  QVERIFY(metaData->saturation()                == QImageMetaData::SaturationUndefined);
  QVERIFY(metaData->sceneCaptureType()          == QImageMetaData::SceneCaptureTypeUndefined);
  QVERIFY(metaData->sensingMethod()             == QImageMetaData::SensingMethodOneChipColorAreaSensor);
  QVERIFY(metaData->sharpness()                 == QImageMetaData::SharpnessUndefined);
  QVERIFY(metaData->subjectDistanceRange()      == QImageMetaData::SubjectDistanceRangeUndefined);
  QVERIFY(metaData->whiteBalance()              == QImageMetaData::WhiteBalanceUndefined);
  QVERIFY(metaData->photometricInterpretation() == QImageMetaData::PhotometricUndefined);
  QVERIFY(metaData->orientation()               == QImageMetaData::OrientationTopLeft);

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Apple iPhoto 11 - PNG - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_iphoto_png_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("depth");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // iPhoto 11
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test 1 / iPhoto 11 / PNG")
      << "../../../testimages/test_1_iphoto_11.png"
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(8);

  QTest::newRow("Test 2 / iPhoto 11 / PNG")
      << "../../../testimages/test_2_iphoto_11.png"
      << static_cast<unsigned int>(2832)
      << static_cast<unsigned int>(4256)
      << static_cast<unsigned int>(8);
}

//----------------------------------------------------------------------------------------------------
// Apple iPhoto 11 - PNG - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_iphoto_png()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(unsigned int,depth);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVector<quint32> bitDepth = metaData->depth();

  QCOMPARE(bitDepth.size(),3);

  QCOMPARE(bitDepth[0],depth);
  QCOMPARE(bitDepth[1],depth);
  QCOMPARE(bitDepth[2],depth);

  delete(metaData);
}

//--------------------------------------------------------------------------------------------------
// GPS - Test data
//--------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_gps_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // TIFF
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test / GPS / TIFF")
      << "../../../testimages/test_3_aperture_3_0_gps.tiff";

  //--------------------------------------------------------------------------------------------------
  // JPEG
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test / GPS / JPEG")
      << "../../../testimages/test_3_aperture_3_0_gps.jpg";
}

//----------------------------------------------------------------------------------------------------
// GPS - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_gps()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QImageMetaData::GPSLatitude latitudeRef = metaData->gpsLatitudeRef();

  QVERIFY(latitudeRef == QImageMetaData::GPSLatitudeNorth);

  QImageMetaData::GPSLongitude longitudeRef = metaData->gpsLongitudeRef();

  QVERIFY(longitudeRef == QImageMetaData::GPSLongitudeEast);

  QVERIFY(metaData->gpsLatitudeDegrees() == 60.0);
  QVERIFY(metaData->gpsLatitudeMinutes() ==  9.0);
  QVERIFY(metaData->gpsLatitudeSeconds() ==  5.4);

  QVERIFY(metaData->gpsLongitudeDegrees() == 24.0);
  QVERIFY(metaData->gpsLongitudeMinutes() == 57.0);
  QVERIFY(metaData->gpsLongitudeSeconds() ==  8.4);

  delete(metaData);
}


//----------------------------------------------------------------------------------------------------
// Unknown ColorChecker - TIFF - Test data
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_colorchecker_tiff_data()
{
  //--------------------------------------------------------------------------------------------------
  // Columns
  //--------------------------------------------------------------------------------------------------
  QTest::addColumn<QString>("file");

  QTest::addColumn<unsigned int>("width");
  QTest::addColumn<unsigned int>("length");

  QTest::addColumn<unsigned int>("pixelXDimension");
  QTest::addColumn<unsigned int>("pixelYDimension");

  QTest::addColumn<QString>("make");
  QTest::addColumn<QString>("model");

  QTest::addColumn<unsigned int>("depth");

  QTest::addColumn<double>("xResolution");
  QTest::addColumn<double>("yResolution");

  QTest::addColumn<double>("exposureBias");

  QTest::addColumn<QString>("dateAndTime");
  QTest::addColumn<QString>("dateAndTimeOriginal");
  QTest::addColumn<QString>("dateAndTimeDigitized");

  QTest::addColumn<QString>("subseconds");
  QTest::addColumn<QString>("subsecondsOriginal");
  QTest::addColumn<QString>("subsecondsDigitized");

  QTest::addColumn<double>("digitalZoomRatio");

  QTest::addColumn<unsigned int>("exposureTimeNom");
  QTest::addColumn<unsigned int>("exposureTimeDen");

  QTest::addColumn<double>("fNumber");
  QTest::addColumn<double>("focalLength");
  QTest::addColumn<unsigned int>("focalLength35mm");

  QTest::addColumn<unsigned int>("ISOSpeedRating");

  QTest::addColumn<double>("maxAperture");

  QTest::addColumn<QString>("software");

  QTest::addColumn<QString>("exifVersion");

  //--------------------------------------------------------------------------------------------------
  // Rows
  //--------------------------------------------------------------------------------------------------

  //--------------------------------------------------------------------------------------------------
  // Unknown ColorChecker
  //--------------------------------------------------------------------------------------------------
  QTest::newRow("Test / Unknown ColorChecker / TIFF")
      << "../../../testimages/ColorChecker.tiff"
      << static_cast<unsigned int>(3187)
      << static_cast<unsigned int>(2095)
      << static_cast<unsigned int>(0)
      << static_cast<unsigned int>(0)
      << "NIKON CORPORATION"
      << "NIKON D2Xs"
      << static_cast<unsigned int>(16)
      << static_cast<double>(240)
      << static_cast<double>(240)
      << static_cast<double>(0.0)
      << "2008:01:19 01:06:14"
      << "2008:01:19 01:58:25"
      << "2008:01:19 01:58:25"
      << "Not defined"
      << "24"
      << "24"
      << static_cast<double>(1.0)
      << static_cast<unsigned int>(1)
      << static_cast<unsigned int>(1)
      << static_cast<double>(5.0)
      << static_cast<double>(46.0)
      << static_cast<unsigned int>(69)
      << static_cast<unsigned int>(200)
      << static_cast<double>(4.6)
      << "Ver.1.00"
      << "0221";
}

//----------------------------------------------------------------------------------------------------
// Unknown ColorChecker - TIFF - Test
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_metadata_colorchecker_tiff()
{
  //--------------------------------------------------------------------------------------------------
  // Fetch
  //--------------------------------------------------------------------------------------------------
  QFETCH(QString,file);

  QFETCH(unsigned int,width);
  QFETCH(unsigned int,length);

  QFETCH(QString,make);
  QFETCH(QString,model);

  QFETCH(unsigned int,pixelXDimension);
  QFETCH(unsigned int,pixelYDimension);

  QFETCH(unsigned int,depth);

  QFETCH(double,xResolution);
  QFETCH(double,yResolution);

  QFETCH(double,exposureBias);

  QFETCH(QString,dateAndTime);
  QFETCH(QString,dateAndTimeOriginal);
  QFETCH(QString,dateAndTimeDigitized);

  QFETCH(QString,subseconds);
  QFETCH(QString,subsecondsOriginal);
  QFETCH(QString,subsecondsDigitized);

  QFETCH(double,digitalZoomRatio);

  QFETCH(unsigned int,exposureTimeNom);
  QFETCH(unsigned int,exposureTimeDen);

  QFETCH(double,fNumber);
  QFETCH(double,focalLength);
  QFETCH(unsigned int,focalLength35mm);

  QFETCH(unsigned int,ISOSpeedRating);

  QFETCH(double,maxAperture);

  QFETCH(QString,software);

  QFETCH(QString,exifVersion);

  //--------------------------------------------------------------------------------------------------
  // Verify
  //--------------------------------------------------------------------------------------------------
  QImageMetaData* metaData = new QImageMetaData();

  QVERIFY(metaData->read(file) == true);

  QCOMPARE(metaData->width(),width);
  QCOMPARE(metaData->height(),length);

  QVERIFY(metaData->make().compare(make)   == 0);
  QVERIFY(metaData->model().compare(model) == 0);

  QCOMPARE(metaData->pixelXDimension(),pixelXDimension);
  QCOMPARE(metaData->pixelYDimension(),pixelYDimension);

  QVector<quint32> bitDepth = metaData->depth();

  QCOMPARE(bitDepth.size(),3);

  QCOMPARE(bitDepth[0],depth);
  QCOMPARE(bitDepth[1],depth);
  QCOMPARE(bitDepth[2],depth);

  QCOMPARE(metaData->xResolution(),xResolution);
  QCOMPARE(metaData->yResolution(),yResolution);

  QCOMPARE(metaData->exposureBias(),exposureBias);

  QVERIFY(metaData->dateAndTime().compare(dateAndTime) == 0);
  QVERIFY(metaData->dateAndTimeOriginal().compare(dateAndTimeOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitized().compare(dateAndTimeDigitized) == 0);

  QVERIFY(metaData->dateAndTimeSubseconds().compare(subseconds) == 0);
  QVERIFY(metaData->dateAndTimeOriginalSubseconds().compare(subsecondsOriginal) == 0);
  QVERIFY(metaData->dateAndTimeDigitizedSubseconds().compare(subsecondsDigitized) == 0);

  QCOMPARE(metaData->digitalZoomRatio(),digitalZoomRatio);

  QCOMPARE(metaData->exposureTime().nominator(),exposureTimeNom);
  QCOMPARE(metaData->exposureTime().denominator(),exposureTimeDen);

  QCOMPARE(metaData->exposureTime().value(),static_cast<qreal>(exposureTimeNom)/static_cast<qreal>(exposureTimeDen));

  QCOMPARE(metaData->fNumber().value(),fNumber);
  QCOMPARE(metaData->focalLength().value(),focalLength);
  QCOMPARE(metaData->focalLength35mm(),focalLength35mm);

  QVector<quint32> ISOSpeedRatings = metaData->ISOSpeedRatings();

  QCOMPARE(ISOSpeedRatings.size(),1);
  QCOMPARE(ISOSpeedRatings[0],ISOSpeedRating);

  QCOMPARE(metaData->maxAperture(),maxAperture);

  QVERIFY(metaData->software().compare(software) == 0);

  QVERIFY(metaData->exifVersion().compare(exifVersion) == 0);

  QVERIFY(metaData->resolutionUnit()            == QImageMetaData::ResolutionInch);
  QVERIFY(metaData->compression()               == QImageMetaData::CompressionUncompressed);
  QVERIFY(metaData->exposureMode()              == QImageMetaData::ExposureModeAuto);
  QVERIFY(metaData->exposureProgram()           == QImageMetaData::ExposureProgramAperturePriority);
  QVERIFY(metaData->contrast()                  == QImageMetaData::ContrastNormal);
  QVERIFY(metaData->customRendered()            == QImageMetaData::CustomRenderedNormal);
  QVERIFY(metaData->flashStatus()               == QImageMetaData::FlashStatusNotFired);
  QVERIFY(metaData->gainControl()               == QImageMetaData::GainControlNone);
  QVERIFY(metaData->lightSource()               == QImageMetaData::LightSourceUnknown);
  QVERIFY(metaData->meteringMode()              == QImageMetaData::MeteringModeCenterWeightedAverage);
  QVERIFY(metaData->saturation()                == QImageMetaData::SaturationNormal);
  QVERIFY(metaData->sceneCaptureType()          == QImageMetaData::SceneCaptureTypeStandard);
  QVERIFY(metaData->sensingMethod()             == QImageMetaData::SensingMethodOneChipColorAreaSensor);
  QVERIFY(metaData->sharpness()                 == QImageMetaData::SharpnessNormal);
  QVERIFY(metaData->subjectDistanceRange()      == QImageMetaData::SubjectDistanceRangeUnknown);
  QVERIFY(metaData->whiteBalance()              == QImageMetaData::WhiteBalanceAuto);
  QVERIFY(metaData->photometricInterpretation() == QImageMetaData::PhotometricRGB);
  QVERIFY(metaData->orientation()               == QImageMetaData::OrientationUndefined);

  delete(metaData);
}

//----------------------------------------------------------------------------------------------------
// Empty metadata
//----------------------------------------------------------------------------------------------------
void TestQImageMetaData::test_empty_metadata_jpeg()
{
  QImageMetaData* metaData = new QImageMetaData();

  qDebug() << "------------------------------------------------------------------------------------------------------------------------------------------------------------";

  QVERIFY(metaData->read("../../../testimages/empty_metadata.jpg") == false);

  delete(metaData);
}
